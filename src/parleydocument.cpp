/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "parleydocument.h"

#include "../config-parley.h"
#include "parleymainwindow.h"
#include "editor/editor.h"
#include "version.h"
#include "prefs.h"

#include "vocabulary/vocabularyview.h"
#include "settings/documentproperties.h"
#include "welcomescreen/welcomescreen.h"

#include <keduvoclesson.h>
#include <keduvocleitnerbox.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>

#include <KFileDialog>
#include <KRecentFilesAction>
#include <KStandardDirs>
#include <knewstuff2/engine.h>
#include <KUser>
#include <KMessageBox>
#include <KProcess>

#include <QTimer>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>

#ifdef HAVE_LIBXSLT
#include "export/exportdialog.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#endif
#include "settings/languageproperties.h"

ParleyDocument* ParleyDocument::s_instance = 0;

ParleyDocument* ParleyDocument::instance()
{
    if (!s_instance) {
        s_instance = new ParleyDocument;
    }
    return s_instance;
}

void ParleyDocument::destroy()
{
    if (s_instance) {
        delete s_instance;
        s_instance = 0;
    }
}

ParleyDocument::ParleyDocument()
    :QObject(ParleyMainWindow::instance()), m_doc(new KEduVocDocument(this)), m_backupTimer(0)
{
}

ParleyDocument::~ParleyDocument()
{
    delete m_backupTimer;
    delete m_doc;
}

KEduVocDocument * ParleyDocument::document()
{
    return m_doc;
}

void ParleyDocument::setTitle(const QString& title)
{
    m_doc->setTitle(title);
    ParleyMainWindow::instance()->slotUpdateWindowCaption();
    m_doc->setModified(true);
}

void ParleyDocument::slotFileNew()
{
    if (ParleyMainWindow::instance()->queryExit()) {
        newDocument(true);
    }
}

void ParleyDocument::newDocument(bool wizard)
{
    KEduVocDocument *newDoc = new KEduVocDocument();
    
    initializeDefaultGrammar(newDoc);
    setDefaultDocumentProperties(newDoc);
    bool showGrammarDialog = false;
    bool fetchGrammarOnline = false;
    if (wizard) {
        DocumentProperties* titleAuthorWidget = new DocumentProperties(newDoc, true, ParleyMainWindow::instance());
        KDialog* titleAuthorDialog;
        titleAuthorDialog = new KDialog(ParleyMainWindow::instance());
        titleAuthorDialog->setMainWidget( titleAuthorWidget );
        titleAuthorDialog->setCaption(i18nc("@title:window document properties", "Properties for %1", newDoc->url().url()));
        connect(titleAuthorDialog, SIGNAL(accepted()), titleAuthorWidget, SLOT(accept()));
        if(titleAuthorDialog->exec()) {
            showGrammarDialog = titleAuthorWidget->grammarCheckBox->isChecked();
            fetchGrammarOnline = titleAuthorWidget->downloadGrammarCheckBox->isChecked();
            delete titleAuthorDialog;
        } else {
            delete titleAuthorDialog;
            delete newDoc;
            return;
        }
    }

    close();
    m_doc = newDoc;
    
    //ParleyMainWindow::instance()->editor()->updateDocument();
    
    emit documentChanged(m_doc);
    enableAutoBackup(Prefs::autoBackup());
    

    if(fetchGrammarOnline) {
        fetchGrammar(0);
        fetchGrammar(1);
    }
    if(showGrammarDialog) {
        languageProperties();
    }

    ParleyMainWindow::instance()->showEditor();
}

void ParleyDocument::fetchGrammar(int languageIndex)
{
    QString locale = m_doc->identifier(languageIndex).locale();

    KUrl location(QString("http://edu.kde.org/parley/locale/") + locale + QString(".kvtml"));

    KEduVocDocument grammarDoc;
    if (grammarDoc.open(location) == KEduVocDocument::NoError) {
        m_doc->identifier(languageIndex).setArticle(grammarDoc.identifier(0).article());
        m_doc->identifier(languageIndex).setPersonalPronouns(grammarDoc.identifier(0).personalPronouns());
// @todo        m_doc->identifier(index).setDeclension(grammarDoc.identifier(0).declension());
        m_doc->identifier(languageIndex).setTenseList(grammarDoc.identifier(0).tenseList());
    } else {
        kDebug() << "Download of " << location.url() << " failed.";
    }
}

void ParleyDocument::slotFileOpen()
{
    if (ParleyMainWindow::instance()->queryExit()) {
        QCheckBox *practiceCheckBox = new QCheckBox(i18n("Open in practice &mode"));
        KFileDialog dialog(QString(), KEduVocDocument::pattern(KEduVocDocument::Reading), ParleyMainWindow::instance(), practiceCheckBox);
        dialog.setCaption(i18n("Open Vocabulary Collection"));
        if(dialog.exec() && !dialog.selectedUrl().isEmpty()) {
            open(dialog.selectedUrl());
            if(practiceCheckBox->isChecked()) {
                ParleyMainWindow::instance()->startPractice();
            } else {
                ParleyMainWindow::instance()->showEditor();
            }
        }
    }
}

void ParleyDocument::slotFileOpenRecent(const KUrl& url)
{
    if (ParleyMainWindow::instance()->queryExit()) {
        open(url);
        ParleyMainWindow::instance()->showEditor(); ///@todo: start practice directly depending on current component
    }
}

void ParleyDocument::open(const KUrl & url)
{
    if (!url.path().isEmpty()) {
        close();
        m_doc = new KEduVocDocument(this);
        m_doc->setCsvDelimiter(Prefs::separator());
        m_doc->open(url);

        //ParleyMainWindow::instance()->editor()->updateDocument();
        ParleyMainWindow::instance()->addRecentFile(url, m_doc->title());
        
        emit documentChanged(m_doc);
        enableAutoBackup(Prefs::autoBackup());
    }
}

void ParleyDocument::close() {
    enableAutoBackup(false);
    emit documentChanged(0);
    disconnect(m_doc);
    delete m_doc;
    m_doc = 0;
    ParleyMainWindow::instance()->slotUpdateWindowCaption();
}

void ParleyDocument::openGHNS()
{
    if (ParleyMainWindow::instance()->queryExit()) {
        QString downloadDir = KStandardDirs::locateLocal("data", "kvtml/");
        KUrl url = KFileDialog::getOpenUrl(
                downloadDir,
                KEduVocDocument::pattern(KEduVocDocument::Reading),
                ParleyMainWindow::instance(),
                i18n("Open Downloaded Vocabulary Collection"));
        if (!url.isEmpty()) {
            open(url);
            ParleyMainWindow::instance()->showEditor();
        }
    }
}

void ParleyDocument::save()
{
    if (m_doc->url().fileName() == i18n("Untitled")) {
        saveAs();
        return;
    }

    // remove previous backup
    QFile::remove(m_doc->url().toLocalFile()+'~');
    ::rename(QFile::encodeName(m_doc->url().toLocalFile()), QFile::encodeName(m_doc->url().toLocalFile()+'~'));

    m_doc->setCsvDelimiter(Prefs::separator());

    emit statesNeedSaving();
    

    int result = m_doc->saveAs(m_doc->url(), KEduVocDocument::Automatic, QString::fromLatin1("Parley ") + PARLEY_VERSION_STRING);
    if ( result != 0 ) {
        KMessageBox::error(ParleyMainWindow::instance(),
                i18n("Writing file \"%1\" resulted in an error: %2", m_doc->url().url(),
                        m_doc->errorDescription(result)), i18n("Save File"));
        saveAs();
        return;
    }
    ParleyMainWindow::instance()->addRecentFile(m_doc->url(), m_doc->title());
    enableAutoBackup(Prefs::autoBackup());
}

void ParleyDocument::saveAs(KUrl url)
{
    if (!m_doc) {
        return;
    }

    if (url.isEmpty()) {
        url = KFileDialog::getSaveUrl(QString(),
            KEduVocDocument::pattern(KEduVocDocument::Writing),
            ParleyMainWindow::instance()->parentWidget(),
            i18n("Save Vocabulary As"));
    }
    if (url.isEmpty()) {
        return;
    }
    
    QFileInfo fileinfo(url.toLocalFile());
    if (fileinfo.exists()) {
        if(KMessageBox::warningContinueCancel(0,
                i18n("<qt>The file<p><b>%1</b></p>already exists. Do you want to overwrite it?</qt>",
                url.toLocalFile()),QString(),KStandardGuiItem::overwrite()) == KMessageBox::Cancel) {
            return;
        }
    }

    QString msg = i18nc("@info:status saving a file", "Saving %1", url.toLocalFile());

    QFile::remove(url.toLocalFile()+'~'); // remove previous backup
    QFile::rename(QFile::encodeName(url.toLocalFile()), QFile::encodeName(QString(url.toLocalFile()+'~')));

    m_doc->setCsvDelimiter(Prefs::separator());

    if ( !url.fileName().contains('.') ) {
        url.setFileName(url.fileName() + QString::fromLatin1(".kvtml"));
    }

    int result = m_doc->saveAs(url, KEduVocDocument::Automatic, "Parley");
    if (result == 0) {
        ParleyMainWindow::instance()->addRecentFile(m_doc->url(), m_doc->title());
        emit statesNeedSaving();
    } else {
        KMessageBox::error(ParleyMainWindow::instance(), i18n("Writing file \"%1\" resulted in an error: %2",
            m_doc->url().url(), m_doc->errorDescription(result)), i18n("Save File"));
    }
}

void ParleyDocument::initializeDefaultGrammar(KEduVocDocument *doc)
{
    KEduVocWordType *root = doc->wordTypeContainer();
    KEduVocWordType *noun = new KEduVocWordType(i18n("Noun"), root);
    noun->setWordType(KEduVocWordFlag::Noun);
    root->appendChildContainer(noun);

    KEduVocWordType *nounChild = new KEduVocWordType(i18n("Masculine"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Masculine);
    noun->appendChildContainer(nounChild);
    nounChild = new KEduVocWordType(i18n("Feminine"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Feminine);
    noun->appendChildContainer(nounChild);
    nounChild = new KEduVocWordType(i18n("Neuter"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Neuter);
    noun->appendChildContainer(nounChild);

    KEduVocWordType *verb = new KEduVocWordType(i18n("Verb"), root);
    verb->setWordType(KEduVocWordFlag::Verb);
    root->appendChildContainer(verb);

    KEduVocWordType *adjective = new KEduVocWordType(i18n("Adjective"), root);
    adjective->setWordType(KEduVocWordFlag::Adjective);
    root->appendChildContainer(adjective);

    KEduVocWordType *adverb = new KEduVocWordType(i18n("Adverb"), root);
    adverb->setWordType(KEduVocWordFlag::Adverb);
    root->appendChildContainer(adverb);
}

void ParleyDocument::setDefaultDocumentProperties(KEduVocDocument *doc)
{
    // some default values
    KUser user;
    QString userName = user.property(KUser::FullName).toString();
    if ( userName.isEmpty() ) {
        userName = user.loginName();
    }
    doc->setAuthor( userName );
    doc->setLicense( i18n("Public Domain") );
    doc->setCategory( i18n("Languages") );

    QString locale = KGlobal::locale()->language();

    doc->appendIdentifier();
    doc->appendIdentifier();
    doc->identifier(0).setName( KGlobal::locale()->languageCodeToName( locale) );
    doc->identifier(0).setLocale( locale );
    doc->identifier(1).setName( i18n("A Second Language") );
    doc->identifier(1).setLocale( locale );

    KEduVocLesson* lesson = new KEduVocLesson(i18n("Lesson 1"), doc->lesson());
    doc->lesson()->appendChildContainer(lesson);

    KEduVocLeitnerBox *box = doc->leitnerContainer();
    box->appendChildContainer(new KEduVocLesson(i18n("Box 7 (best)"), box));
    box->appendChildContainer(new KEduVocLesson(i18n("Box 6"), box));
    box->appendChildContainer(new KEduVocLesson(i18n("Box 5"), box));
    box->appendChildContainer(new KEduVocLesson(i18n("Box 4"), box));
    box->appendChildContainer(new KEduVocLesson(i18n("Box 3"), box));
    box->appendChildContainer(new KEduVocLesson(i18n("Box 2"), box));
    box->appendChildContainer(new KEduVocLesson(i18n("Box 1 (lowest)"), box));

    // add some entries
    for ( int i = 0; i < 15 ; i++ ) {
        lesson->appendEntry(new KEduVocExpression());
    }

    doc->setModified(false);
}

void ParleyDocument::slotGHNS()
{
    KNS::Entry::List entries = KNS::Engine::download();
    int numberInstalled = 0;
    QString fileName;
    // list of changed entries
    foreach(KNS::Entry* entry, entries) {
        // care only about installed ones
        if (entry->status() == KNS::Entry::Installed) {
            // check mime type and if kvtml, open it
            foreach(const QString &file, entry->installedFiles()) {
                KMimeType::Ptr mimeType = KMimeType::findByPath(file);
                kDebug() << "KNS2 file of mime type:" << KMimeType::findByPath(file)->name();
                if (mimeType->name() == "application/x-kvtml") {
                    ParleyMainWindow::instance()->addRecentFile(file, QString()); ///@todo: title!
		    fileName = file;
                    ++numberInstalled;
                }         
            }
        }
    }
    qDeleteAll(entries);
    // to enable the display in the welcome screen
    Prefs::self()->writeConfig();
    ParleyMainWindow::instance()->updateRecentFilesModel();
    if (numberInstalled > 1) {
        openGHNS();
    } else if (numberInstalled == 1) {
        open(KUrl(fileName));
        ParleyMainWindow::instance()->showEditor();
    }
}


void ParleyDocument::languageProperties()
{
    LanguageProperties properties(m_parleyApp);
    if ( properties.exec() == KDialog::Accepted ) {
        emit languagesChanged();
    }
}


void ParleyDocument::exportDialog()
{
#ifdef HAVE_LIBXSLT
    ExportDialog dialog(this, ParleyMainWindow::instance());
    dialog.exec();
#endif
}

void ParleyDocument::slotFileMerge()
{
    ///@todo as soon as some brave soul descends into the lib and implements merging this should be enabled
//     KUrl url = KFileDialog::getOpenUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Reading), parentWidget(), i18n("Merge Vocabulary File"));
    //
//     if (!url.isEmpty()) {
//         QString msg = i18n("Loading %1", url.path());
//         slotStatusMsg(msg);
    //
//         KEduVocDocument *new_doc = new KEduVocDocument(this);
//         new_doc->setCsvDelimiter(Prefs::separator());
//         new_doc->open(url);
    //
//         m_doc->merge(new_doc, true);
    //
//         KEduVocWordFlag::setTenseNames(m_doc->tenseDescriptions());
//         KVTUsage::setUsageNames(m_doc->usageDescriptions());
    //
//         delete(new_doc);
//         m_recentFilesAction->addUrl(url);
//         m_tableModel->reset();
//         m_lessonModel->setDocument(m_doc);
//         m_tableView->adjustContent();
//     }
}

void ParleyDocument::enableAutoBackup(bool enable)
{
    if ( !enable ) {
        if ( m_backupTimer ) {
            m_backupTimer->stop();
        }
    } else {
        if ( !m_backupTimer ) {
            m_backupTimer = new QTimer(this);
            connect(m_backupTimer, SIGNAL(timeout()), this, SLOT(save()));
        }
        m_backupTimer->start(Prefs::backupTime() * 60 * 1000);
    }
}

#include "parleydocument.moc"
