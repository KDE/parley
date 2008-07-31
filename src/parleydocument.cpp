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
#include "parley.h"
#include "version.h"
#include "prefs.h"

#include "newdocument-wizard/kvtnewdocumentwizard.h"
#include "vocabulary/vocabularyview.h"

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
#include "export/export.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#endif


ParleyDocument::ParleyDocument(ParleyApp *parent)
 : QObject(parent)
{
    m_parleyApp = parent;
    m_doc = new KEduVocDocument(this);
    m_backupTimer = 0;
    enableAutoBackup(Prefs::autoBackup());
}


ParleyDocument::~ParleyDocument()
{
    m_parleyApp->m_vocabularyView->saveColumnVisibility(m_doc->url());
    delete m_doc;
}


KEduVocDocument * ParleyDocument::document()
{
    return m_doc;
}


void ParleyDocument::slotFileNew()
{
    if (m_parleyApp->queryExit()) {
        newDocumentWizard();
    }
}


void ParleyDocument::newDocument()
{
    disconnect(m_doc);
    delete m_doc;
    m_doc = new KEduVocDocument(this);
    m_parleyApp->updateDocument();

    initializeDefaultGrammar();
    createExampleEntries();

    m_doc->setModified(false);
    m_parleyApp->updateDocument();

    emit documentChanged(m_doc);
}

void ParleyDocument::slotFileOpen()
{
    if (m_parleyApp->queryExit()) {
        KUrl url = KFileDialog::getOpenUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Reading), m_parleyApp, i18n("Open Vocabulary Document"));
        open(url, true);
    }
}

void ParleyDocument::slotFileOpenRecent(const KUrl& url)
{
    if (m_parleyApp->queryExit()) {
        open(url);
    }
}


void ParleyDocument::open(const KUrl & url, bool addRecent)
{
    if (!url.path().isEmpty()) {
        emit documentChanged(0);
        disconnect(m_doc);
        delete m_doc;
        m_doc = new KEduVocDocument(this);
        m_doc->setCsvDelimiter(Prefs::separator());
        m_doc->open(url);

        m_parleyApp->updateDocument();

        if (addRecent) { // open sample does not go into recent
            m_parleyApp->m_recentFilesAction->addUrl(url);
        }
        emit documentChanged(m_doc);
    }
}


void ParleyDocument::openExample()
{
    if (m_parleyApp->queryExit()) {
        QString s = KStandardDirs::locate("data", "parley/examples/");
        KUrl url = KFileDialog::getOpenUrl(s, KEduVocDocument::pattern(KEduVocDocument::Reading), m_parleyApp, i18n("Open Example Vocabulary Document"));
        open(url, false);
    }
}


void ParleyDocument::save()
{
//     if (m_parleyApp->m_entryDlg != 0) {
//         m_parleyApp->m_entryDlg->commitData(false);
//     }

    if (m_doc->url().fileName() == i18n("Untitled")) {
        saveAs();
        return;
    }

    // remove previous backup
    QFile::remove(QFile::encodeName(m_doc->url().path()+'~'));
    ::rename(QFile::encodeName(m_doc->url().path()), QFile::encodeName(m_doc->url().path()+'~'));

    m_doc->setCsvDelimiter(Prefs::separator());

    m_parleyApp->m_vocabularyView->saveColumnVisibility(m_doc->url());

    int result = m_doc->saveAs(m_doc->url(), KEduVocDocument::Automatic, QString::fromLatin1("Parley ") + PARLEY_VERSION_STRING);
    if ( result != 0 ) {
        KMessageBox::error(m_parleyApp, i18n("Writing file \"%1\" resulted in an error: %2", m_doc->url().url(), m_doc->errorDescription(result)), i18n("Save File"));
        saveAs();
        return;
    }
    m_parleyApp->m_recentFilesAction->addUrl(m_doc->url());
}


void ParleyDocument::saveAs(KUrl url)
{
    if (url.isEmpty()) {
        url = KFileDialog::getSaveUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Writing), m_parleyApp->parentWidget(), i18n("Save Vocabulary As"));
    }

    if (!url.isEmpty()) {
        QFileInfo fileinfo(url.path());
        if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
            i18n("<qt>The file<p><b>%1</b></p>already exists. Do you want to overwrite it?</qt>",
                 url.path()),QString(),KStandardGuiItem::overwrite()) == KMessageBox::Cancel) {
            // do nothing
                 } else

                     if (m_doc) {
                         QString msg = i18nc("@info:status saving a file", "Saving %1", url.path());
//                 slotStatusMsg(msg);

                         QFile::remove(QFile::encodeName(url.path()+'~')); // remove previous backup
                             ::rename(QFile::encodeName(url.path()), QFile::encodeName(QString(url.path()+'~')));

                             m_doc->setCsvDelimiter(Prefs::separator());

                             if ( !url.fileName().contains('.') ) {
                                 url.setFileName(url.fileName() + QString::fromLatin1(".kvtml"));
                             }

                            m_parleyApp->m_vocabularyView->saveColumnVisibility(url);

                             int result = m_doc->saveAs(url, KEduVocDocument::Automatic, "Parley");
                             if (result != 0) {
                                 KMessageBox::error(m_parleyApp, i18n("Writing file \"%1\" resulted in an error: %2", m_doc->url().url(), m_doc->errorDescription(result)), i18n("Save File"));
                             } else {
                                 m_parleyApp->m_recentFilesAction->addUrl(m_doc->url());
                             }
                     }
    }
}


void ParleyDocument::slotSaveSelection()
{
///@todo I doubt this works. If it's not checked, better not enable it.

    /*
    if (m_entryDlg != 0) {
    m_entryDlg->commitData(false);
}
//     slotStatusMsg(i18n("Saving selected area under new filename..."));
    QString save_separator = Prefs::separator();
    Prefs::setSeparator("\t");
    KEduVocDocument seldoc(this);
    // transfer most important parts
    for (int i = 0; i < m_doc->identifierCount(); i++) {
    seldoc.appendIdentifier(m_doc->identifier(i));
}
    seldoc.setAuthor(m_doc->author());
    //seldoc.setLessons(m_doc->lessons());
    //seldoc.setLessonsInQuery(m_doc->lessonsInQuery());
    //seldoc.setTypeDescriptions(m_doc->typeDescriptions());

//     for (int i = m_doc->entryCount()-1; i >= 0; i--)
//         if (m_doc->entry(i)->isInQuery())
//             seldoc.appendEntry(m_doc->entry(i));

    KUrl url = KFileDialog::getSaveUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Writing), parentWidget(), i18n("Save Vocabulary As"));

    if (!url.isEmpty()) {
    QFileInfo fileinfo(url.path());
    if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
    i18n("<qt>The file<br><b>%1</b><br />already exists. Do you want to overwrite it?</qt>",
    url.path()),QString(),KStandardGuiItem::overwrite()) == KMessageBox::Cancel) {
            // do nothing
} else {
    QString msg = i18nc("@info:status saving a file", "Saving %1", url.path());
//             slotStatusMsg(msg);

    QFile::remove(url.path()+'~'); // remove previous backup
            // FIXME: check error
    ::rename(QFile::encodeName(url.path()), QFile::encodeName(url.path()+'~'));

    prepareProgressBar();
    seldoc.setCsvDelimiter(Prefs::separator());
    seldoc.saveAs(url, KEduVocDocument::Automatic, "Parley");
    removeProgressBar();
}
}
    Prefs::setSeparator(save_separator);
//     slotStatusMsg(IDS_DEFAULT);
    */
}






void ParleyDocument::newDocumentWizard()
{
    newDocument();

    KVTNewDocumentWizard *wizard;

    wizard = new KVTNewDocumentWizard(m_doc, m_parleyApp);
    if( !wizard->exec() == KDialog::Accepted ){
        delete wizard;
        return;
    }
    delete wizard;
}

void ParleyDocument::initializeDefaultGrammar()
{
    KEduVocWordType *root = m_doc->wordTypeContainer();
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

void ParleyDocument::createExampleEntries()
{
    // some default values
    KUser user;
    QString userName = user.property(KUser::FullName).toString();
    if ( userName.isEmpty() ) {
        userName = user.loginName();
    }
    m_doc->setAuthor( userName );
    m_doc->setTitle( i18n("Welcome") );
    m_doc->setLicense( i18n("Public Domain") );
    m_doc->setCategory( i18n("Languages") );

    QString locale = KGlobal::locale()->language();

    m_doc->appendIdentifier();
    m_doc->appendIdentifier();
    m_doc->identifier(0).setName( KGlobal::locale()->languageCodeToName( locale) );
    m_doc->identifier(0).setLocale( locale );
    m_doc->identifier(1).setName( i18n("A Second Language") );
    m_doc->identifier(1).setLocale( locale );

    KEduVocLesson* lesson = new KEduVocLesson(i18n("Lesson 1"), m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);

    KEduVocLeitnerBox *box = m_doc->leitnerContainer();
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

    m_doc->setModified(false);
}


void ParleyDocument::slotGHNS()
{
    KNS::Entry::List entries = KNS::Engine::download();
    // list of changed entries
    foreach(KNS::Entry* entry, entries) {
        // care only about installed ones
        if (entry->status() == KNS::Entry::Installed) {
            // check mime type and if kvtml, open it
            foreach(const QString &file, entry->installedFiles()) {
                KMimeType::Ptr mimeType = KMimeType::findByPath(file);
                kDebug() << "KNS2 file of mime type:" << KMimeType::findByPath(file)->name();
                if (mimeType->name() == "application/x-kvtml") {
                    KProcess newParley;
                    newParley.setProgram("parley", QStringList() << file);
                    newParley.startDetached();
                    m_parleyApp->m_downloadedFilesAction->addUrl(file);
                }
            }
        }
    }
    qDeleteAll(entries);
}

void ParleyDocument::exportHtmlDialog()
{
#ifdef HAVE_LIBXSLT

    ParleyExport::exportDocument(this, m_parleyApp);
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
            m_backupTimer = new QTimer;
            connect(m_backupTimer, SIGNAL(timeout()), this, SLOT(save()));
        }
        m_backupTimer->start(Prefs::backupTime() * 60 * 1000);
    }
}

void ParleyDocument::slotDocumentChanged(bool modified) {
    kDebug() << modified;
    if (modified)
        emit documentChanged(m_doc);
}


#include "parleydocument.moc"
