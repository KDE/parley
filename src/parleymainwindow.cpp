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
#include "statistics-dialogs/StatisticsDialog.h"
#include "settings/parleyprefs.h"
#include "settings/TitlePage.h"
#include "configure-practice/configurepracticedialog.h"
#include "configure-practice/practicestartdialog.h"
#include "practiceold/testentrymanager.h"
#include "practice/parleypracticemainwindow.h"
#include "prefs.h"
#include "welcomescreen/welcomescreen.h"

#include <KActionCollection>
#include <KRecentFilesAction>
#include <KMessageBox>
#include <KTipDialog>
#include <knewstuff2/ui/knewstuffaction.h>
#include <KXMLGUIFactory>
#include <KToolBar>

#include <QtCore/QTimer>

ParleyMainWindow::ParleyMainWindow(const QString& appName, const KUrl & filename) : KXmlGuiWindow(0), m_currentComponent(NoComponent), m_practice(0)
{
    m_appName = appName;
    m_document = new ParleyDocument(this);

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QHBoxLayout());

    initWelcomeScreen();

    initActions();

    m_editor = new Editor(this);
    m_editor->hide();

    bool showWelcomeScreen = false;

    if ( !filename.url().isEmpty() ) {
        m_document->open(filename);
    } else {
        bool openLastFile = Prefs::autoOpenLast();
        if (openLastFile && m_recentFilesAction->actions().count() > 0
            && m_recentFilesAction->action(m_recentFilesAction->actions().count()-1)->isEnabled() ) {
            m_recentFilesAction->action(m_recentFilesAction->actions().count()-1)->trigger();
        } else {
            showWelcomeScreen = true;
        }
    }

    if (!initialGeometrySet()) {
        resize(QSize(800, 600).expandedTo(minimumSizeHint()));
    }
    setupGUI(ToolBar | Keys | StatusBar | Create);

    // currently there is no way to remove the mainToolBar that is always there by default (defined in ui_standards.rc)
    // as a workaround the toolbar is deleted right after it is created
    KToolBar *mainToolBar = toolBar("mainToolBar");
    removeToolBar(mainToolBar);
    delete mainToolBar;
    setupToolbarMenuActions();

    // save position of dock windows etc
    setAutoSaveSettings();

    if (showWelcomeScreen) {
        this->showWelcomeScreen();
    } else {
        showEditor();
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
    QString title = m_document->document()->title();
    if (title == i18n("Untitled")) {
        title.clear();
    }
    setCaption(title, m_document->document()->isModified());
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
    showWelcomeScreen();
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
        startPractice();
    }
}

void ParleyMainWindow::startPractice()
{
    PracticeStartDialog *dialog = new PracticeStartDialog(this, m_document->document());
    int result = dialog->exec();
    delete dialog;
    if(!result) {
        return;
    }

    if (Prefs::oldPractice()) {
        hide();
        TestEntryManager testManager(m_document->document(), this);
        testManager.startPractice();
        show();
    } else {
        ///@todo: instead of creating a new instance only a new document should be set
        switchComponent(NoComponent); // unload the last component (could be a practice window)
        m_practice = new ParleyPracticeMainWindow(m_document->document(), 0);
        switchComponent(PracticeComponent);
        m_practice->show();
        connect(m_practice, SIGNAL(signalPracticeFinished()), this, SLOT(showEditor()));
    }
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

// -- SETTINGS --------------------------------------------------
    KAction* configApp = KStandardAction::preferences(this, SLOT(slotGeneralOptions()), actionCollection());
    configApp->setWhatsThis(i18n("Show the configuration dialog"));
    configApp->setToolTip(configApp->whatsThis());
    configApp->setStatusTip(configApp->whatsThis());

    actionCollection()->addAction(KStandardAction::TipofDay,  "help_tipofday", this, SLOT( tipOfDay() ));
}

void ParleyMainWindow::initWelcomeScreen()
{
    m_welcomeScreen = new WelcomeScreen(this);
    m_welcomeScreen->hide();
}

void ParleyMainWindow::showWelcomeScreen()
{
    switchComponent(WelcomeComponent);
}

void ParleyMainWindow::showEditor()
{
    switchComponent(EditorComponent);
}

void ParleyMainWindow::showPractice()
{
    switchComponent(PracticeComponent);
}

void ParleyMainWindow::switchComponent(Component component)
{
    kDebug() << "switch to component" << component;

    if(m_currentComponent == component) {
        return;
    }

    // Get pointers to the old component (we need them as widgets and gui clients)
    KXMLGUIClient *oldClient = 0;
    QWidget *oldWidget = 0;
    switch (m_currentComponent) {
    case WelcomeComponent:
        oldClient = 0; // The welcome screen doesn't inherit from KXMLGUIClient and doesn't have any actions
        oldWidget = m_welcomeScreen;
        break;
    case EditorComponent:
        oldClient = m_editor;
        oldWidget = m_editor;
        break;
    case PracticeComponent:
        oldClient = m_practice;
        oldWidget = m_practice;
        break;
    default:
        break;
    }
    kDebug() << "old component" << oldClient << oldWidget;

    // Get pointers to the new component (we need them as widgets and gui clients)
    KXMLGUIClient *newClient = 0;
    QWidget *newWidget = 0;
    switch (component) {
    case WelcomeComponent:
        newClient = 0; // The welcome screen doesn't inherit from KXMLGUIClient and doesn't have any actions
        newWidget = m_welcomeScreen;
        showDocumentActions(false);
        m_welcomeScreen->updateRecentFilesModel();
        break;
    case EditorComponent:
        newClient = m_editor;
        newWidget = m_editor;
        showDocumentActions(true);
        break;
    case PracticeComponent:
        newClient = m_practice;
        newWidget = m_practice;
        showDocumentActions(true);
        break;
    default:
        break;
    }
    kDebug() << "new component" << newClient << newWidget;

    // Unload the old actions and load the new ones
    if (oldClient) {
        guiFactory()->removeClient(oldClient);
    }
    if (newClient) {
        guiFactory()->addClient(newClient);
    }

    // Hide the old central widget and insert the new one
    if (oldWidget) {
        centralWidget()->layout()->removeWidget(oldWidget);
        oldWidget->hide();
    }
    if (newWidget) {
        centralWidget()->layout()->addWidget(newWidget);
        newWidget->show();
    }

    m_currentComponent = component;
}

void ParleyMainWindow::showDocumentActions(bool show)
{
    actionCollection()->action("file_save")->setVisible(show);
    actionCollection()->action("file_save_as")->setVisible(show);
    actionCollection()->action("file_close")->setVisible(show);
    actionCollection()->action("file_export")->setVisible(show);
    actionCollection()->action("file_properties")->setVisible(show);
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
