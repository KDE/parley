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
#include "configure-practice/configurepracticedialog.h"
#include "practiceold/vocabularypractice.h"
#include "practice/guifrontend.h"
#include "practice/defaultbackend.h"
#include "practice/practiceoptions.h"

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

using namespace Editor;

ParleyMainWindow* ParleyMainWindow::s_instance = 0;
ParleyMainWindow* ParleyMainWindow::instance()
{
    return s_instance;
}

ParleyMainWindow::ParleyMainWindow(const KUrl& filename)
    :KXmlGuiWindow(0)
    ,m_currentComponent(NoComponent)
    ,m_practiceFrontend(0)
    ,m_practiceBackend(0)
    ,m_statisticsWidget(0)
{
    s_instance = this;
    m_document = ParleyDocument::instance();

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QHBoxLayout());

    initWelcomeScreen();

    m_editor = new EditorWindow(this);
    m_editor->hide();

    initActions();

    bool showWelcomeScreen = false;

    setupGUI(ToolBar | Keys | StatusBar | Create);

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

ParleyMainWindow::~ParleyMainWindow()
{
    m_editor->saveState();
    ParleyDocument::destroy();
}

void ParleyMainWindow::addRecentFile(const KUrl &url, const QString &name)
{
    m_recentFilesAction->addUrl(url, name);
    m_recentFilesAction->saveEntries(KGlobal::config()->group("Recent Files"));
}

void ParleyMainWindow::updateRecentFilesModel()
{
    m_welcomeScreen->updateRecentFilesModel();
}

void ParleyMainWindow::saveOptions()
{
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
    QString title;
    bool modified = false;
    if (m_document->document()) {
        title = m_document->document()->title();
        modified = m_document->document()->isModified();
        if (title == i18n("Untitled")) {
            title.clear();
        }
    }
    setCaption(title, modified);
}

void ParleyMainWindow::slotGeneralOptions()
{
    ParleyPrefs* dialog = new ParleyPrefs(m_document->document(), this, "settings",  Prefs::self());
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(slotApplyPreferences()));
    dialog->show();
}

void ParleyMainWindow::slotApplyPreferences()
{
    m_document->enableAutoBackup((m_currentComponent != WelcomeComponent) && Prefs::autoBackup());

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
    //m_document->newDocument();

    m_document->close();
    emit documentChanged();
    showWelcomeScreen();
}

void ParleyMainWindow::slotShowStatistics()
{
    showStatistics();
}

void ParleyMainWindow::configurePractice()
{
    ConfigurePracticeDialog configurePracticeDialog(m_document->document(), this, "practice settings",  Prefs::self());
    configurePracticeDialog.exec();
}

void ParleyMainWindow::startPractice()
{
    bool newPractice = true;
    if (newPractice) {
        ///@todo: instead of creating a new instance only a new document should be set
        m_componentBeforePractice = m_currentComponent;
        switchComponent(NoComponent); // unload the last component (could be a practice window)
//         m_practice = new ParleyPracticeMainWindow(m_document->document(), 0);
        delete m_practiceFrontend;
        delete m_practiceBackend;
        m_practiceFrontend = new Practice::GuiFrontend(this);
        
        Practice::PracticeOptions options;
        m_practiceBackend = new Practice::DefaultBackend(m_practiceFrontend, m_document, options, this);
        
        switchComponent(PracticeComponent);
        m_practiceBackend->startPractice();
        
        connect(m_practiceBackend, SIGNAL(practiceFinished()), this, SLOT(practiceFinished()));
        
//         m_practice->show();
//         if (lastComponent == EditorComponent) {
//             connect(m_practice, SIGNAL(signalPracticeFinished()), this, SLOT(showEditor()));
//         } else {
//             connect(m_practice, SIGNAL(signalPracticeFinished()), this, SLOT(showWelcomeScreen()));
//         }
        // If starting the practice fails (e.g. there are no entries selected), the signalPracticeFinished() signal
        // is emitted in the constructor and thus before the connect
//         if (m_practice->finished() && lastComponent == EditorComponent) {
//             showEditor();
//         } else if(m_practice->finished()) {
//             showWelcomeScreen();
//         }
   } else { // old dialog based practice
        hide();
        VocabularyPractice practice(m_document->document(), this);
        practice.startPractice();
        show();
   }
}

void ParleyMainWindow::practiceFinished()
{
    switchComponent(m_componentBeforePractice);
}

bool ParleyMainWindow::queryClose()
{
    bool erg = queryExit();
    if (erg && m_document->document()) {
        m_document->document()->setModified(false);  // avoid double query on exit via system menu
    }
    return erg;
}

bool ParleyMainWindow::queryExit()
{
    if (!m_document->document() || !m_document->document()->isModified()) {
        return true;
    }
    saveOptions();

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

QSize ParleyMainWindow::sizeHint() const
{
    return QSize(800, 600).expandedTo(KXmlGuiWindow::minimumSizeHint());
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
    fileNew->setWhatsThis(i18n("Creates a new vocabulary collection"));
    fileNew->setToolTip(fileNew->whatsThis());
    fileNew->setStatusTip(fileNew->whatsThis());

    KAction* fileOpen = KStandardAction::open(m_document, SLOT(slotFileOpen()), actionCollection());
    fileOpen->setWhatsThis(i18n("Opens an existing vocabulary collection"));
    fileOpen->setToolTip(fileOpen->whatsThis());
    fileOpen->setStatusTip(fileOpen->whatsThis());

    KAction* fileGHNS = KNS::standardAction(i18n("Download New Vocabularies..."), m_document, SLOT(slotGHNS()), actionCollection(), "file_ghns");
    fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    fileGHNS->setWhatsThis(i18n("Downloads new vocabulary collections"));
    fileGHNS->setToolTip(fileGHNS->whatsThis());
    fileGHNS->setStatusTip(fileGHNS->whatsThis());

    KAction* fileOpenGHNS = new KAction(this);
    actionCollection()->addAction("file_open_downloaded", fileOpenGHNS);
    fileOpenGHNS->setIcon(KIcon("get-hot-new-stuff"));
    fileOpenGHNS->setText(i18n("Open &Downloaded Vocabularies..."));
    connect(fileOpenGHNS, SIGNAL(triggered(bool)), m_document, SLOT(openGHNS()));
    fileOpenGHNS->setWhatsThis(i18n("Open downloaded vocabulary collections"));
    fileOpenGHNS->setToolTip(fileOpenGHNS->whatsThis());
    fileOpenGHNS->setStatusTip(fileOpenGHNS->whatsThis());

    m_recentFilesAction = KStandardAction::openRecent(m_document, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());
    m_recentFilesAction->loadEntries(KGlobal::config()->group("Recent Files"));

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
    fileSave->setWhatsThis(i18n("Save the active vocabulary collection"));
    fileSave->setToolTip(fileSave->whatsThis());
    fileSave->setStatusTip(fileSave->whatsThis());

    KAction* fileSaveAs = KStandardAction::saveAs(m_document, SLOT(saveAs()), actionCollection());
    fileSaveAs->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    fileSaveAs->setWhatsThis(i18n("Save the active vocabulary collection with a different name"));
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
    connect(fileExport, SIGNAL(triggered(bool)), m_document, SLOT(exportDialog()));
    fileExport->setIcon(KIcon("document-export"));
    fileExport->setWhatsThis(i18n("Export to HTML or CSV"));
    fileExport->setToolTip(fileExport->whatsThis());
    fileExport->setStatusTip(fileExport->whatsThis());
#endif

    KAction* fileProperties = new KAction(this);
    actionCollection()->addAction("file_properties", fileProperties);
    fileProperties->setText(i18n("&Properties..."));
    connect(fileProperties, SIGNAL(triggered(bool)), m_editor, SLOT(slotDocumentProperties()));
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

void ParleyMainWindow::showStatistics()
{
    switchComponent(StatisticsComponent);
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
    case StatisticsComponent:
        oldClient = 0;
        oldWidget = m_statisticsWidget;
        break;
    case EditorComponent:
        oldClient = m_editor;
        oldWidget = m_editor;
        break;
   case PracticeComponent:
       oldClient = m_practiceFrontend->getWindow();
       oldWidget = m_practiceFrontend->getWindow();
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
        showDocumentActions(true, false);
        m_welcomeScreen->updateRecentFilesModel();
        break;
    case StatisticsComponent:
        newClient = 0;
        if (!m_statisticsWidget) {
            m_statisticsWidget = new StatisticsWidget(m_document->document(), this);
        } else {
            m_statisticsWidget->setDocument(m_document->document());
        }
        newWidget = m_statisticsWidget;
        showDocumentActions(true, false);
        break;
    case EditorComponent:
        newClient = m_editor;
        newWidget = m_editor;
        showDocumentActions(true, true);
        break;
   case PracticeComponent:
       newClient = m_practiceFrontend->getWindow();
       newWidget = m_practiceFrontend->getWindow();
       showDocumentActions(false, false);
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
    setupToolbarMenuActions();
}

void ParleyMainWindow::showDocumentActions(bool open, bool edit)
{
    actionCollection()->action("file_new")->setVisible(open);
    actionCollection()->action("file_open")->setVisible(open);
    actionCollection()->action("file_open_recent")->setVisible(open);
    actionCollection()->action("file_ghns")->setVisible(open);
    actionCollection()->action("file_open_downloaded")->setVisible(open);

    actionCollection()->action("file_save")->setVisible(edit);
    actionCollection()->action("file_save_as")->setVisible(edit);
    actionCollection()->action("file_close")->setVisible(edit);
    actionCollection()->action("file_export")->setVisible(edit);
    actionCollection()->action("file_properties")->setVisible(edit);
    actionCollection()->action("file_close")->setVisible(edit);
}

ParleyDocument* ParleyMainWindow::parleyDocument()
{
    return m_document;
}

EditorWindow* ParleyMainWindow::editor()
{
    return m_editor;
}

#include "parleymainwindow.moc"
