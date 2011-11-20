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
#include <knewstuff3/downloaddialog.h>
#include <knewstuff3/uploaddialog.h>
#include <KEMailSettings>
#include <KMessageBox>
#include <KProcess>
#include <KTempDir>

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
#include "settings/documentproperties.h"

namespace DocumentHelper {
void fetchGrammar(KEduVocDocument* doc, int languageIndex)
{
    QString locale = doc->identifier(languageIndex).locale();
    
    KUrl location(QString("http://edu.kde.org/parley/locale/") + locale + QString(".kvtml"));
    
    KEduVocDocument grammarDoc;
    if (grammarDoc.open(location) == KEduVocDocument::NoError) {
        doc->identifier(languageIndex).setArticle(grammarDoc.identifier(0).article());
        doc->identifier(languageIndex).setPersonalPronouns(grammarDoc.identifier(0).personalPronouns());
        // @todo        m_doc->identifier(index).setDeclension(grammarDoc.identifier(0).declension());
        doc->identifier(languageIndex).setTenseList(grammarDoc.identifier(0).tenseList());
    } else {
        kDebug() << "Download of " << location.url() << " failed.";
    }
}
} // namespace DocumentHelper

ParleyDocument::ParleyDocument(ParleyMainWindow* parleyMainWindow)
    :QObject(parleyMainWindow), m_parleyApp(parleyMainWindow), m_doc(new KEduVocDocument(this)), m_backupTimer(0)
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
    m_parleyApp->slotUpdateWindowCaption();
    m_doc->setModified(true);
}

void ParleyDocument::slotFileNew()
{
    if (m_parleyApp->queryExit()) {
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
        DocumentProperties* titleAuthorWidget = new DocumentProperties(newDoc, true, m_parleyApp);
        KDialog* titleAuthorDialog;
        titleAuthorDialog = new KDialog(m_parleyApp);
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
    
    emit documentChanged(m_doc);
    enableAutoBackup(Prefs::autoBackup());
    

    if(fetchGrammarOnline) {
        DocumentHelper::fetchGrammar(m_doc, 0);
        DocumentHelper::fetchGrammar(m_doc, 1);
    }
    if(showGrammarDialog) {
        languageProperties();
    }

    m_parleyApp->showEditor();
}

void ParleyDocument::slotFileOpen()
{
    if (m_parleyApp->queryExit()) {
        QCheckBox *practiceCheckBox = new QCheckBox(i18n("Open in practice &mode"));
        practiceCheckBox->setChecked(m_parleyApp->currentComponent() != ParleyMainWindow::EditorComponent);
        KFileDialog dialog(QString(), KEduVocDocument::pattern(KEduVocDocument::Reading), m_parleyApp, practiceCheckBox);
        dialog.setCaption(i18n("Open Vocabulary Collection"));
        if(dialog.exec() && !dialog.selectedUrl().isEmpty()) {
            open(dialog.selectedUrl());
            if(practiceCheckBox->isChecked()) {
                m_parleyApp->showPracticeConfiguration();
            } else {
                m_parleyApp->showEditor();
            }
        }
    }
}

void ParleyDocument::slotFileOpenRecent(const KUrl& url)
{
    if (m_parleyApp->queryExit()) {
        open(url);
        m_parleyApp->showEditor(); ///@todo: start practice directly depending on current component
    }
}

void ParleyDocument::open(const KUrl & url)
{
    if (!url.path().isEmpty()) {
        close();
        m_doc = new KEduVocDocument(this);
        m_doc->setCsvDelimiter(Prefs::separator());
        m_doc->open(url);

        //m_parleyApp->editor()->updateDocument();
        m_parleyApp->addRecentFile(url, m_doc->title());
        
        emit documentChanged(m_doc);
        enableAutoBackup(Prefs::autoBackup());
    }
}

void ParleyDocument::close() {
    kDebug() << "Close Document";
    enableAutoBackup(false);
    emit documentChanged(0);
    disconnect(m_doc);
    delete m_doc;
    m_doc = 0;
    m_parleyApp->slotUpdateWindowCaption();
}

void ParleyDocument::openGHNS()
{
    if (m_parleyApp->queryExit()) {
        QString downloadDir = KStandardDirs::locateLocal("data", "kvtml/");
        KUrl url = KFileDialog::getOpenUrl(
                downloadDir,
                KEduVocDocument::pattern(KEduVocDocument::Reading),
                m_parleyApp,
                i18n("Open Downloaded Vocabulary Collection"));
        if (!url.isEmpty()) {
            open(url);
            m_parleyApp->showPracticeConfiguration();
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
        KMessageBox::error(m_parleyApp,
                i18n("Writing file \"%1\" resulted in an error: %2", m_doc->url().url(),
                        m_doc->errorDescription(result)), i18n("Save File"));
        saveAs();
        return;
    }
    m_parleyApp->addRecentFile(m_doc->url(), m_doc->title());
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
            m_parleyApp->parentWidget(),
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
        m_parleyApp->addRecentFile(m_doc->url(), m_doc->title());
        emit statesNeedSaving();
    } else {
        KMessageBox::error(m_parleyApp, i18n("Writing file \"%1\" resulted in an error: %2",
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

    KEduVocWordType *conjunction = new KEduVocWordType(i18n("Conjunction"), root);
    conjunction->setWordType(KEduVocWordFlag::Conjunction);
    root->appendChildContainer(conjunction);
}


void ParleyDocument::setDefaultDocumentProperties(KEduVocDocument *doc)
{
    KEMailSettings emailSettings;
    emailSettings.setProfile(emailSettings.defaultProfileName());
    doc->setAuthor(emailSettings.getSetting(KEMailSettings::RealName));
    doc->setAuthorContact(emailSettings.getSetting(KEMailSettings::EmailAddress));

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

    // add some entries
    for ( int i = 0; i < 15 ; i++ ) {
        lesson->appendEntry(new KEduVocExpression());
    }

    doc->setModified(false);
}

void ParleyDocument::slotGHNS()
{
    QString fileName;
    KNS3::DownloadDialog newStuffDialog(ParleyMainWindow::instance());
    newStuffDialog.exec();
    KNS3::Entry::List entries = newStuffDialog.installedEntries();
    int numberInstalled = entries.size();
    foreach(const KNS3::Entry& entry, entries) {
        // check mime type and if kvtml, open it
        foreach(const QString &file, entry.installedFiles()) {
            KMimeType::Ptr mimeType = KMimeType::findByPath(file);
            kDebug() << "KNS2 file of mime type:" << KMimeType::findByPath(file)->name();
            if (mimeType->is("application/x-kvtml")) {
                ParleyMainWindow::instance()->addRecentFile(file, QString()); ///@todo: title!
                fileName = file;
            }
        }
    }
    
    // to enable the display in the welcome screen
    Prefs::self()->writeConfig();
    m_parleyApp->updateRecentFilesModel();
    if (numberInstalled > 1) {
        openGHNS();
    } else if (numberInstalled == 1) {
        open(KUrl(fileName));
        m_parleyApp->showPracticeConfiguration();
    }
}

void ParleyDocument::documentProperties()
{
    DocumentProperties* titleAuthorWidget = new DocumentProperties(m_doc, false, m_parleyApp);
    KDialog* titleAuthorDialog;
    titleAuthorDialog = new KDialog(m_parleyApp);
    titleAuthorDialog->setMainWidget( titleAuthorWidget );
    
    // the language options are only shown, when this is used to create a new document.
    titleAuthorWidget->languageGroupBox->setVisible(false);
    titleAuthorDialog->setCaption(i18nc("@title:window document properties", "Properties for %1", m_doc->url().url()));
    connect(titleAuthorDialog, SIGNAL(accepted()), titleAuthorWidget, SLOT(accept()));
    titleAuthorDialog->exec();
    delete titleAuthorDialog;
}

void ParleyDocument::languageProperties()
{
    LanguageProperties properties(m_doc, m_parleyApp);
    if ( properties.exec() == KDialog::Accepted ) {
        emit languagesChanged();
    }
}

void ParleyDocument::uploadFile()
{
    // save file to temp location
    KTempDir dir;
    KUrl url(dir.name() + m_doc->url().fileName());
    kDebug() << "save in " << url.url();
    m_doc->saveAs(url, KEduVocDocument::Automatic, "Parley");

    KEduVocDocument tempDoc(this);
    tempDoc.open(url);
    // remove grades
    tempDoc.lesson()->resetGrades(-1, KEduVocContainer::Recursive);
    tempDoc.saveAs(url, KEduVocDocument::Automatic, "Parley");

    // upload
    KNS3::UploadDialog dialog(ParleyMainWindow::instance());
    dialog.setUploadFile(url);
    dialog.exec();
}

void ParleyDocument::exportDialog()
{
#ifdef HAVE_LIBXSLT
    ExportDialog dialog(this, m_parleyApp);
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
