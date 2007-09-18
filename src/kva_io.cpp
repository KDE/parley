/***************************************************************************

                         i/o part of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kvoctrain.h"
#include "newdocument-wizard/kvtnewdocumentwizard.h"
#include "entry-dialogs/EntryDlg.h"
#include "prefs.h"

#include "kvttablemodel.h"
#include "kvtsortfiltermodel.h"
#include "kvttableview.h"
#include "kvtlessonmodel.h"
#include "kvtlessonview.h"

#include <KFileDialog>
#include <KStatusBar>
#include <KRecentFilesAction>
#include <KStandardDirs>
#include <knewstuff2/engine.h>
#include <KUser>
#include <KMessageBox>

#include <QTimer>
#include <QFrame>
#include <QWizard>
#include <QApplication>
#include <QProgressBar>

void KVocTrainApp::slotTimeOutBackup()
{
    if (Prefs::autoBackup() && m_doc && m_doc->isModified()) {
        slotStatusMsg(i18n("Autobackup in progress"));
        slotFileSave();
    }

    if (Prefs::autoBackup())
        QTimer::singleShot(Prefs::backupTime() * 60 * 1000, this, SLOT(slotTimeOutBackup()));
    slotStatusMsg(IDS_DEFAULT);
}


bool KVocTrainApp::queryClose()
{
    bool erg = queryExit();
    if (erg)
        m_doc->setModified(false);  // avoid double query on exit via system menu
    return erg;
}


bool KVocTrainApp::queryExit()
{
    saveOptions();
    if (!m_doc || !m_doc->isModified())
        return true;

    bool save = Prefs::autoSave(); //save without asking

    if (!save) {
        int exit = KMessageBox::warningYesNoCancel(this, i18n("Vocabulary is modified.\n\nSave file before exit?\n"),
                   "", KStandardGuiItem::save(), KStandardGuiItem::discard());
        if (exit == KMessageBox::Yes) {
            save = true;   // save and exit
        } else if (exit == KMessageBox::No) {
            save = false;  // don't save but exit
        } else {
            return false;  // continue work
        }
    }

    if (save) {
        if (!m_doc->url().isEmpty())
            slotFileSave();       // save and exit
        if (m_doc->isModified()) {
            // Error while saving or no name
            slotFileSaveAs();
        }
    }
    return true;
}


void KVocTrainApp::slotFileQuit()
{
    close();
}


void KVocTrainApp::slotProgress(KEduVocDocument *curr_doc, int percent)
{
    Q_UNUSED(curr_doc);
    if (pbar != 0)
        pbar->setValue(percent);
    qApp->processEvents();
}

void KVocTrainApp::slotFileNew()
{
    slotStatusMsg(i18n("Creating new file..."));
    if (queryExit()) {
        newDocumentWizard();
    }
    slotStatusMsg(IDS_DEFAULT);
}

void KVocTrainApp::slotFileOpen()
{
    slotStatusMsg(i18n("Opening file..."));

    if (queryExit()) {
        KUrl url = KFileDialog::getOpenUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Reading), this, i18n("Open Vocabulary Document"));
        loadFileFromPath(url, true);
    }

    slotStatusMsg(IDS_DEFAULT);
}

void KVocTrainApp::slotFileOpenRecent(const KUrl& url)
{
    slotStatusMsg(i18n("Opening file..."));
    if (queryExit()) {
        loadFileFromPath(url);
    }
    slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::loadFileFromPath(const KUrl & url, bool addRecent)
{
    if (!url.path().isEmpty()) {
        delete m_doc;
        m_doc = 0;

        slotStatusMsg(i18n("Loading %1", url.path()));
        //prepareProgressBar();
        m_doc = new KEduVocDocument(this);
        m_doc->setCsvDelimiter(Prefs::separator());
        m_doc->open(url);

        m_tableModel->setDocument(m_doc);
        m_tableModel->reset();

        removeProgressBar();
        if (addRecent) { // open sample does not go into recent
            fileOpenRecent->addUrl(url);
        }
        connect(m_doc, SIGNAL(docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
        m_doc->setModified(false);
        m_sortFilterModel->restoreNativeOrder();
        if (m_tableView) {
            m_tableView->adjustContent();
        }
        m_lessonModel->setDocument(m_doc);
    }
}


void KVocTrainApp::slotFileOpenExample()
{
    slotStatusMsg(i18n("Opening example file..."));

    if (queryExit()) {
        QString s = KStandardDirs::locate("data", "parley/examples/");
        KUrl url = KFileDialog::getOpenUrl(s, KEduVocDocument::pattern(KEduVocDocument::Reading), this, i18n("Open Example Vocabulary Document"));
        loadFileFromPath(url, false);
        if (m_doc) {
            m_doc->url().setFileName(QString());
        }
    }

    slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotGHNS()
{
  ///Make sure the installation directory exists
  KConfig conf("parley.knsrc");
  KConfigGroup confGroup = conf.group("KNewStuff2");
  QString installDir = confGroup.readEntry("InstallPath", "Vocabularies");
  KStandardDirs::makeDir(QDir::home().path() + '/' + installDir);

  KNS::Entry::List entries = KNS::Engine::download();
}


void KVocTrainApp::slotFileMerge()
{
//     slotStatusMsg(i18n("Merging file..."));
//
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
//         KEduVocConjugation::setTenseNames(m_doc->tenseDescriptions());
//         KVTUsage::setUsageNames(m_doc->usageDescriptions());
//
//         delete(new_doc);
//         fileOpenRecent->addUrl(url);
//         m_tableModel->reset();
//         m_lessonModel->setDocument(m_doc);
//         m_tableView->adjustContent();
//     }
//
//     slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotFileSave()
{
    if (entryDlg != 0) {
        entryDlg->commitData(false);
    }

    if (m_doc->url().fileName() == i18n("Untitled")) {
        slotFileSaveAs();
        return;
    }

    QString msg = i18nc("@info:status saving a file", "Saving %1", m_doc->url().path());
    slotStatusMsg(msg);

    // remove previous backup
    QFile::remove(QFile::encodeName(m_doc->url().path()+'~'));
    ::rename(QFile::encodeName(m_doc->url().path()), QFile::encodeName(m_doc->url().path()+'~'));

    prepareProgressBar();
    m_doc->setCsvDelimiter(Prefs::separator());
    if (!m_doc->saveAs(m_doc->url(), KEduVocDocument::Automatic, "Parley")) {
        slotFileSaveAs();
        return;
    }
    fileOpenRecent->addUrl(m_doc->url());
    removeProgressBar();

    slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotFileSaveAs()
{
    slotStatusMsg(i18n("Saving file under new filename..."));

    if (entryDlg != 0) {
        entryDlg->commitData(false);
    }

    KUrl url = KFileDialog::getSaveUrl(QString(), KEduVocDocument::pattern(KEduVocDocument::Writing), parentWidget(), i18n("Save Vocabulary As"));

    if (!url.isEmpty()) {
        QFileInfo fileinfo(url.path());
        if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
                i18n("<qt>The file<br><b>%1</b><br />already exists. Do you want to overwrite it?</qt>",
                     url.path()),QString(),KStandardGuiItem::overwrite()) == KMessageBox::Cancel) {
            // do nothing
        } else

            if (m_doc) {
                QString msg = i18nc("@info:status saving a file", "Saving %1", url.path());
                slotStatusMsg(msg);

                QFile::remove(QFile::encodeName(url.path()+'~')); // remove previous backup
                ::rename(QFile::encodeName(url.path()), QFile::encodeName(QString(url.path()+'~')));

                prepareProgressBar();
                m_doc->setCsvDelimiter(Prefs::separator());
                m_doc->saveAs(url, KEduVocDocument::Automatic, "Parley");
                fileOpenRecent->addUrl(m_doc->url());
                removeProgressBar();
            }
    }
    slotStatusMsg(IDS_DEFAULT);
}


void KVocTrainApp::slotSaveSelection()
{
///@todo I doubt this words. If it's not checked, better not enable it.

    /*
    if (entryDlg != 0) {
        entryDlg->commitData(false);
    }
    slotStatusMsg(i18n("Saving selected area under new filename..."));
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
            slotStatusMsg(msg);

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
    slotStatusMsg(IDS_DEFAULT);
    */
}


void KVocTrainApp::prepareProgressBar()
{
    statusBar()->clearMessage();
    pbar = new QProgressBar(statusBar());
    statusBar()->addPermanentWidget(pbar, 150);
    pbar->show();
}


void KVocTrainApp::removeProgressBar()
{
    statusBar()->clearMessage();
    statusBar()->removeWidget(pbar);
    delete pbar;
    pbar = 0;
}


void KVocTrainApp::newDocumentWizard()
{
    KVTNewDocumentWizard *wizard;
    KEduVocDocument* newDoc = new KEduVocDocument(this);

    wizard = new KVTNewDocumentWizard(newDoc, this);
    if( !wizard->exec() == QDialog::Accepted ){
            delete wizard;
            return;
    }
    delete wizard;

    delete m_doc;
    m_doc = 0;
    m_doc = newDoc;

    m_tableModel->setDocument(m_doc);
    connect(m_doc, SIGNAL(docModified(bool)), this, SLOT(slotModifiedDoc(bool)));

    m_lessonModel->setDocument(m_doc);
    if (m_lessonView) {
        m_lessonView->setModel(m_lessonModel);
        m_lessonView->initializeSelection();
    }

    if (m_tableView) {
        m_tableView->adjustContent();
        m_tableView->setColumnHidden(KV_COL_LESS, !Prefs::tableLessonColumnVisible());
    }

    m_tableModel->reset(); // clear old entries otherwise we get crashes

    initializeDefaultGrammar();

    // Set the language headers of the table.
    for (int i=0; i<m_doc->identifierCount(); i++){
        m_tableModel->setHeaderData(i+KV_COL_TRANS, Qt::Horizontal, m_doc->identifier(i).name(), Qt::EditRole);
    }

    int lessonIndex = m_lessonModel->addLesson();

    if (m_lessonView) {
        m_lessonView->slotSelectLesson(lessonIndex);
    }

    // add some entries
    for ( int i = 0; i < 15 ; i++ ) {
        m_tableModel->appendEntry();
    }

    m_doc->setModified(false);
}


void KVocTrainApp::initializeDefaultGrammar()
{
    m_doc->wordTypes()->createDefaultWordTypes();

    // Preset some usages
    m_doc->addUsage( i18n("abbreviation") );
    m_doc->addUsage( i18n("anatomy") );
    m_doc->addUsage( i18n("biology") );
    m_doc->addUsage( i18n("figuratively") );
    m_doc->addUsage( i18n("geology") );
    m_doc->addUsage( i18n("historical") );
    m_doc->addUsage( i18n("informal") );
    m_doc->addUsage( i18n("ironic") );
    m_doc->addUsage( i18n("literary") );
    m_doc->addUsage( i18n("mythology") );
    m_doc->addUsage( i18n("proper name") );
    m_doc->addUsage( i18n("pharmacy") );
    m_doc->addUsage( i18n("philosophy") );
    m_doc->addUsage( i18n("physics") );
    m_doc->addUsage( i18n("physiology") );
    m_doc->addUsage( i18n("rhetoric") );
    m_doc->addUsage( i18n("zoology") );
}


void KVocTrainApp::createExampleEntries()
{
    m_tableModel->reset(); // clear old entries otherwise we get crashes

    // some default values
    KUser user;
    QString userName = user.fullName();
    if ( userName.isEmpty() ) {
        userName = user.loginName();
    }
    m_doc->setAuthor( userName );
    m_doc->setTitle( i18n("Welcome") );
    m_doc->setLicense( i18n("GPL (GNU General Public License)") );
    m_doc->setCategory( i18n("Example document") );

    QString locale = KGlobal::locale()->language();

    m_doc->appendIdentifier();
    m_doc->appendIdentifier();
    m_doc->identifier(0).setName( KGlobal::locale()->languageCodeToName( locale) );
    m_doc->identifier(0).setLocale( locale );
    m_doc->identifier(1).setName( i18n("A Second Language") );
    m_doc->identifier(1).setLocale( locale );

    // Set the language headers of the table.
    for (int i=0; i < m_doc->identifierCount(); i++){
        m_tableModel->setHeaderData(i+KV_COL_TRANS, Qt::Horizontal, m_doc->identifier(i).name(), Qt::EditRole);
    }

    int lessonIndex = m_lessonModel->addLesson();

    if (m_lessonView) {
        m_lessonView->slotSelectLesson(lessonIndex);
    }

    // add some entries
    for ( int i = 0; i < 15 ; i++ ) {
        m_tableModel->appendEntry();
    }

    // select the empty row
    Prefs::setCurrentCol(KV_COL_TRANS);
    Prefs::setCurrentRow(0);

    m_doc->setModified(false);
}


