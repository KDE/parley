/***************************************************************************

                      main part of parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                    (C) 2008 Daniel Laidig <d.laidig@gmx.de>
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

#include "parleymainwindow.h"

#include "../config-parley.h"

#include "editor/editor.h"

#include "practiceold/testentrymanager.h"
#include "practice/parleypracticemainwindow.h"

#include "vocabulary/vocabularymodel.h"
#include "vocabulary/vocabularyview.h"
#include "vocabulary/vocabularyfilter.h"
#include "vocabulary/containerview.h"
#include "vocabulary/lessonview.h"
#include "vocabulary/wordtypeview.h"
#include "vocabulary/containermodel.h"
#include "vocabulary/lessonmodel.h"
#include "vocabulary/wordtypemodel.h"
#include "vocabulary/leitnerview.h"
#include "vocabulary/leitnermodel.h"

#include "entry-dialogs/multiplechoicewidget.h"
#include "entry-dialogs/comparisonwidget.h"
#include "entry-dialogs/conjugationwidget.h"
#include "entry-dialogs/declensionwidget.h"
#include "entry-dialogs/imagechooserwidget.h"
#include "entry-dialogs/audiowidget.h"
#include "entry-dialogs/browserwidget.h"
#include "entry-dialogs/synonymwidget.h"
#include "entry-dialogs/summarywordwidget.h"

#include "statistics-dialogs/StatisticsDialog.h"
#include "settings/parleyprefs.h"
#include "settings/TitlePage.h"
#include "settings/languageproperties.h"
#include "configure-practice/configurepracticedialog.h"
#include "prefs.h"

#include "scripts/scriptdialog.h"
#include "scripts/scripting/parley.h"
#include "scripts/scripting/expression.h"
#include "scripts/translator.h"

#include "welcomescreen/welcomescreen.h"

#include <keduvoclesson.h>
#include <keduvocexpression.h>

#include <KLineEdit>
#include <KStatusBar>

#include <KActionCollection>
#include <KRecentFilesAction>
#include <KToggleAction>
#include <KActionMenu>
#include <KMenuBar>
#include <KMessageBox>
#include <KTipDialog>
#include <KCharSelect>
#include <knewstuff2/ui/knewstuffaction.h>

#include <QtCore/QTimer>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QStackedWidget>

#include "modeltest/modeltest.h"
///@todo remove unneccessary includes

ParleyMainWindow::ParleyMainWindow(const QString& appName, const KUrl & filename) : KXmlGuiWindow(0)
{
    m_appName = appName;
    m_document = new ParleyDocument(this);

    m_editor = new Editor(this);

    setCentralWidget(new QStackedWidget());
    initWelcomeScreen();

    initActions();

    bool showWelcomeScreen = false;

    if ( !filename.url().isEmpty() ) {
        kDebug() << "open doc" << filename.url();
        m_document->open(filename);
        kDebug() << "open done";
    } else {
        bool openLastFile = Prefs::autoOpenLast();
        if (openLastFile && m_recentFilesAction->actions().count() > 0
            && m_recentFilesAction->action(m_recentFilesAction->actions().count()-1)->isEnabled() ) {
            m_recentFilesAction->action(m_recentFilesAction->actions().count()-1)->trigger();
        } else {
            kDebug() << "new doc";
            m_document->newDocument();
            emit documentChanged();
            if (!openLastFile) {
                showWelcomeScreen = true;
            }
        }
    }

    if (!initialGeometrySet()) {
        resize(QSize(800, 600).expandedTo(minimumSizeHint()));
    }
    setupGUI(ToolBar | Keys | StatusBar | Create);

    // save position of dock windows etc
    setAutoSaveSettings();

    if (showWelcomeScreen) {
        setShowWelcomeScreen(true);
    }

    // finally show tip-of-day ( if the user wants it :) )
    QTimer::singleShot( 0, this, SLOT( startupTipOfDay() ) );
}

void ParleyMainWindow::saveOptions()
{
    m_recentFilesAction->saveEntries(KGlobal::config()->group("Recent Files"));
    m_downloadedFilesAction->saveEntries(KGlobal::config()->group("Downloaded Files"));

///@todo save selection per document
//     if (m_tableView) {
//         // map to original entry numbers:
//         QModelIndex sourceIndex = m_sortFilterModel->mapToSource(m_tableView->currentIndex());
//         Prefs::setCurrentRow(sourceIndex.row());
//         Prefs::setCurrentCol(sourceIndex.column());
//     }
    Prefs::self()->writeConfig();
}

void ParleyMainWindow::slotUpdateWindowCaption()
{
    setCaption(m_document->document()->title(), m_document->document()->isModified());
}

void ParleyMainWindow::slotShowStatistics()
{
    StatisticsDialog statisticsDialog(m_document->document(), this);
    statisticsDialog.exec();
}

void ParleyMainWindow::slotGeneralOptions()
{
    ParleyPrefs* dialog = new ParleyPrefs(m_document->document(), this, "settings",  Prefs::self());
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotApplyPreferences()));
    dialog->show();
}

void ParleyMainWindow::slotApplyPreferences()
{
    m_document->enableAutoBackup(Prefs::autoBackup());

//     if (m_pronunciationStatusBarLabel) {
//         m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
//     }
    m_editor->setTableFont(Prefs::tableFont());
}

void ParleyMainWindow::slotCloseDocument()
{
    if (!queryClose()) {
        return;
    }
    m_document->newDocument();
    emit documentChanged();
    setShowWelcomeScreen(true);
}

void ParleyMainWindow::slotDocumentProperties()
{
    TitlePage* titleAuthorWidget = new TitlePage(m_document->document(), false, this);
    KDialog* titleAuthorDialog;
    titleAuthorDialog = new KDialog(this);
    titleAuthorDialog->setMainWidget( titleAuthorWidget );

    // the language options are only shown, when this is used to create a new document.
    titleAuthorWidget->languageGroupBox->setVisible(false);
    titleAuthorDialog->setCaption(i18nc("@title:window document properties", "Properties for %1", m_document->document()->url().url()));
    connect(titleAuthorDialog, SIGNAL(accepted()), titleAuthorWidget, SLOT(accept()));
    titleAuthorDialog->exec();
    slotUpdateWindowCaption();
    delete titleAuthorDialog;
}

void ParleyMainWindow::configurePractice()
{
    ConfigurePracticeDialog* configurePracticeDialog;
    configurePracticeDialog = new ConfigurePracticeDialog(m_document->document(), this, "practice settings",  Prefs::self());

    if ( configurePracticeDialog->exec() == ConfigurePracticeDialog::StartPractice ) {
//         startPractice();
    ///@todo: implement start practice
    }
}

void ParleyMainWindow::closeEvent(QCloseEvent *event)
{
    // hide the welcome screen to prevent the dock widgets to be hidden when saving the window state
    setShowWelcomeScreen(false);
    KXmlGuiWindow::closeEvent(event);
}

bool ParleyMainWindow::queryClose()
{
    bool erg = queryExit();
    if (erg)
        m_document->document()->setModified(false);  // avoid double query on exit via system menu
    return erg;
}


bool ParleyMainWindow::queryExit()
{
    saveOptions();
    if (!m_document->document()->isModified()) {
        return true;
    }

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
        m_document->save();       // save and exit
    }
    return true;
}

void ParleyMainWindow::tipOfDay() {
  KTipDialog::showTip(this, "parley/tips", true);
}

void ParleyMainWindow::startupTipOfDay() {
  KTipDialog::showTip(this, "parley/tips");
}

///@todo: split between editor and mainwindow
void ParleyMainWindow::initActions()
{
// -- FILE --------------------------------------------------
    KAction* fileNew = KStandardAction::openNew(m_document, SLOT(slotFileNew()), actionCollection());
    fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
    fileNew->setToolTip(fileNew->whatsThis());
    fileNew->setStatusTip(fileNew->whatsThis());

    KAction* fileOpen = KStandardAction::open(m_document, SLOT(slotFileOpen()), actionCollection());
    fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
    fileOpen->setToolTip(fileOpen->whatsThis());
    fileOpen->setStatusTip(fileOpen->whatsThis());

    KAction* fileOpenExample = new KAction(this);
    actionCollection()->addAction("file_open_example", fileOpenExample);
    fileOpenExample->setIcon(KIcon("document-open"));
    fileOpenExample->setText(i18n("Open &Example..."));
    connect(fileOpenExample, SIGNAL(triggered(bool)), m_document, SLOT(openExample()));
    fileOpenExample->setWhatsThis(i18n("Open an example vocabulary document"));
    fileOpenExample->setToolTip(fileOpenExample->whatsThis());
    fileOpenExample->setStatusTip(fileOpenExample->whatsThis());

    KAction* fileGHNS = KNS::standardAction(i18n("Download New Vocabularies..."), m_document, SLOT(slotGHNS()), actionCollection(), "file_ghns");
    fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
    fileGHNS->setToolTip(fileGHNS->whatsThis());
    fileGHNS->setStatusTip(fileGHNS->whatsThis());

    m_recentFilesAction = KStandardAction::openRecent(m_document, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());
    m_recentFilesAction->loadEntries(KGlobal::config()->group("Recent Files"));

    m_downloadedFilesAction = new KRecentFilesAction(KIcon("get-hot-new-stuff"), "file_open_downloaded", this);
    actionCollection()->addAction("file_open_downloaded", m_downloadedFilesAction);
    m_downloadedFilesAction->setText(i18n("Open Downloaded Vocabularies"));
    m_downloadedFilesAction->loadEntries(KGlobal::config()->group("Downloaded Files"));
    connect(m_downloadedFilesAction, SIGNAL(urlSelected(const KUrl &)), m_document, SLOT(open(const KUrl&)));
    m_downloadedFilesAction->loadEntries(KGlobal::config()->group("Downloaded Files"));
    m_downloadedFilesAction->setMaxItems(30);

    /*
    KAction* fileMerge = new KAction(this);
    actionCollection()->addAction("file_merge", fileMerge);
    fileMerge->setText(i18n("&Merge..."));
    connect(fileMerge, SIGNAL(triggered(bool)), m_document, SLOT(slotFileMerge()));
    fileMerge->setWhatsThis(i18n("Merge an existing vocabulary document with the current one"));
    fileMerge->setToolTip(fileMerge->whatsThis());
    fileMerge->setStatusTip(fileMerge->whatsThis());
    fileMerge->setEnabled(false); ///@todo merging files is horribly broken
    */

    KAction* fileSave = KStandardAction::save(m_document, SLOT(save()), actionCollection());
    fileSave->setWhatsThis(i18n("Save the active vocabulary document"));
    fileSave->setToolTip(fileSave->whatsThis());
    fileSave->setStatusTip(fileSave->whatsThis());

    KAction* fileSaveAs = KStandardAction::saveAs(m_document, SLOT(saveAs()), actionCollection());
    fileSaveAs->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    fileSaveAs->setWhatsThis(i18n("Save the active vocabulary document with a different name"));
    fileSaveAs->setToolTip(fileSaveAs->whatsThis());
    fileSaveAs->setStatusTip(fileSaveAs->whatsThis());

#ifdef HAVE_LIBXSLT

// Printing would be nice, but for now html export has to suffice
//     KAction* filePrint = KStandardAction::print(m_document, SLOT(print()), actionCollection());
//     filePrint->setWhatsThis(i18n("Print the active vocabulary document"));
//     filePrint->setToolTip(filePrint->whatsThis());
//     filePrint->setStatusTip(filePrint->whatsThis());

    KAction* fileExport = new KAction(this);
    actionCollection()->addAction("file_export", fileExport);
    fileExport->setText(i18n("&Export..."));
    connect(fileExport, SIGNAL(triggered(bool)), m_document, SLOT(exportHtmlDialog()));
    fileExport->setIcon(KIcon("document-export"));
    fileExport->setWhatsThis(i18n("Export to HTML"));
    fileExport->setToolTip(fileExport->whatsThis());
    fileExport->setStatusTip(fileExport->whatsThis());
#endif

    KAction* fileProperties = new KAction(this);
    actionCollection()->addAction("file_properties", fileProperties);
    fileProperties->setText(i18n("&Properties..."));
    connect(fileProperties, SIGNAL(triggered(bool)), SLOT(slotDocumentProperties()));
    fileProperties->setIcon(KIcon("document-properties"));
    fileProperties->setWhatsThis(i18n("Edit document properties"));
    fileProperties->setToolTip(fileProperties->whatsThis());
    fileProperties->setStatusTip(fileProperties->whatsThis());

    KAction* fileClose = KStandardAction::close(this, SLOT(slotCloseDocument()), actionCollection());
    fileClose->setWhatsThis(i18n("Close the current collection"));
    fileClose->setToolTip(fileClose->whatsThis());
    fileClose->setStatusTip(fileClose->whatsThis());

    KAction* fileQuit = KStandardAction::quit(this, SLOT(close()), actionCollection());
    fileQuit->setWhatsThis(i18n("Quit Parley"));
    fileQuit->setToolTip(fileQuit->whatsThis());
    fileQuit->setStatusTip(fileQuit->whatsThis());


    KAction* editLanguages =new KAction(this);
    actionCollection()->addAction("edit_languages", editLanguages);
    editLanguages->setIcon(KIcon("set-language"));
    editLanguages->setText(i18n("&Languages..."));
    editLanguages->setWhatsThis(i18n("Edit which languages are in the collection and their grammar properties."));
    editLanguages->setToolTip(editLanguages->whatsThis());
    editLanguages->setStatusTip(editLanguages->whatsThis());
    connect(editLanguages, SIGNAL(triggered()),  this, SLOT(slotLanguageProperties()));
    ///@todo tooltip

    KAction* removeGrades = new KAction(this);
    actionCollection()->addAction("vocab_remove_grades", removeGrades);
    removeGrades->setIcon(KIcon("edit-clear"));
    removeGrades->setText(i18n("Remove Grades"));
    connect(removeGrades, SIGNAL(triggered(bool)), this, SLOT(removeGrades()));
    removeGrades->setWhatsThis(i18n("Remove all grades from the current document"));
    removeGrades->setToolTip(removeGrades->whatsThis());
    removeGrades->setStatusTip(removeGrades->whatsThis());

// -- PRACTICE --------------------------------------------------

    KAction* configurePractice = new KAction(this);
    configurePractice->setText(i18n("Configure Practice..."));
    configurePractice->setIcon(KIcon("practice-setup"));
    configurePractice->setWhatsThis(i18n("Set up and start a test"));
    configurePractice->setToolTip(configurePractice->whatsThis());
    configurePractice->setStatusTip(configurePractice->whatsThis());
    actionCollection()->addAction("practice_configure", configurePractice);
    connect(configurePractice, SIGNAL(triggered(bool)), SLOT(configurePractice()));

    KAction* startPractice = new KAction(this);
    startPractice->setText(i18n("Start Practice..."));
    startPractice->setIcon(KIcon("practice-start"));
    startPractice->setWhatsThis(i18n("Start a test with the last settings"));
    startPractice->setToolTip(startPractice->whatsThis());
    startPractice->setStatusTip(startPractice->whatsThis());
    actionCollection()->addAction("practice_start", startPractice);
    connect(startPractice, SIGNAL(triggered(bool)), SLOT(startPractice()));

    KAction* showStatistics = new KAction(this);
    actionCollection()->addAction("show_statistics", showStatistics);
    showStatistics->setIcon(KIcon("statistics"));
    showStatistics->setText(i18n("&Statistics..."));
    connect(showStatistics, SIGNAL(triggered(bool)), this, SLOT(slotShowStatistics()));
    showStatistics->setWhatsThis(i18n("Show and reset statistics for the current vocabulary"));
    showStatistics->setToolTip(showStatistics->whatsThis());
    showStatistics->setStatusTip(showStatistics->whatsThis());


// -- SETTINGS --------------------------------------------------
    KAction* configApp = KStandardAction::preferences(this, SLOT(slotGeneralOptions()), actionCollection());
    configApp->setWhatsThis(i18n("Show the configuration dialog"));
    configApp->setToolTip(configApp->whatsThis());
    configApp->setStatusTip(configApp->whatsThis());

    actionCollection()->addAction(KStandardAction::TipofDay,  "help_tipofday", this, SLOT( tipOfDay() ));
}

void ParleyMainWindow::initWelcomeScreen()
{
    WelcomeScreen* welcomeScreen = new WelcomeScreen(this);
    qobject_cast<QStackedWidget*>(centralWidget())->addWidget(welcomeScreen);

    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)), this, SLOT(hideWelcomeScreen()));
}

void ParleyMainWindow::removeGrades()
{
    m_document->document()->lesson()->resetGrades(-1, KEduVocContainer::Recursive);
}

void ParleyMainWindow::setShowWelcomeScreen(bool show)
{
    QStackedWidget* central = qobject_cast<QStackedWidget*>(centralWidget());
    int index = int(show);
    if (central->currentIndex() != index) {
        central->setCurrentIndex(int(show));
    } else {
        return;
    }

//     if (show) {
//         // hide dock widgets
//         m_dockWidgetVisibility.clear();
//         foreach(QDockWidget* dock, m_dockWidgets) {
//             m_dockWidgetVisibility.append(!dock->isHidden());
//             dock->close();
//         }
//     } else {
//         // restore dock widgets
//         if(m_dockWidgets.count() != m_dockWidgetVisibility.count()) {
//             return;
//         }
//         int i = 0;
//         foreach(QDockWidget* dock, m_dockWidgets) {
//             if (m_dockWidgetVisibility[i]) {
//                 dock->show();
//             }
//             i++;
//         }
//     }
}

void ParleyMainWindow::hideWelcomeScreen()
{
    setShowWelcomeScreen(false);
}

ParleyDocument* ParleyMainWindow::parleyDocument()
{
    return m_document;
}

Editor* ParleyMainWindow::editor()
{
    return m_editor;
}

#include "parleymainwindow.moc"
