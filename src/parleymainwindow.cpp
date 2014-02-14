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
#include "statistics/statisticsmainwindow.h"
#include "settings/parleyprefs.h"
#include "configure-practice/configurepracticedialog.h"
#include "practice/guifrontend.h"
#include "practice/practiceoptions.h"
#include "practice/practicesummarycomponent.h"

#include "parleyactions.h"

#include "prefs.h"
#include "welcomescreen/welcomescreen.h"

#include <KActionCollection>
#include <KRecentFilesAction>
#include <KMessageBox>
#include <KTipDialog>
#include <KXMLGUIFactory>
#include <KXmlGuiWindow>
#include <KToolBar>
#include <KMenuBar>

#include <QtCore/QTimer>

using namespace Editor;

ParleyMainWindow* ParleyMainWindow::s_instance = 0;
ParleyMainWindow* ParleyMainWindow::instance()
{
    return s_instance;
}

ParleyMainWindow::ParleyMainWindow(const KUrl& filename)
    : KXmlGuiWindow(0)
    , m_currentComponent(NoComponent)
    , m_currentComponentWindow(0)
    , m_testEntryManager(this)
{
    s_instance = this;
    m_document = new ParleyDocument(this);

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QHBoxLayout());
    centralWidget()->layout()->setMargin(0);

    initActions();

    bool startWithWelcomeScreen = false;

    setupGUI(ToolBar | Keys | Create);

    if (!filename.url().isEmpty()) {
        m_document->open(filename);
    } else {
        bool openLastFile = Prefs::autoOpenLast();
        if (openLastFile && m_recentFilesAction->actions().count() > 0
                && m_recentFilesAction->action(m_recentFilesAction->actions().count() - 1)->isEnabled()) {
            m_recentFilesAction->action(m_recentFilesAction->actions().count() - 1)->trigger();
        } else {
            startWithWelcomeScreen = true;
        }
    }

    // save position of dock windows etc
    setAutoSaveSettings();

    if (startWithWelcomeScreen) {
        showWelcomeScreen();
    } else {
        showEditor();
    }

    connect(this, SIGNAL(preferencesChanged()), this, SLOT(slotApplyPreferences()));
    menuBar()->show();

    // finally show tip-of-day (if the user wants it)
    //QTimer::singleShot( 0, this, SLOT( startupTipOfDay() ) );
}

ParleyMainWindow::~ParleyMainWindow()
{
    guiFactory()->removeClient(m_currentComponentWindow);
    centralWidget()->layout()->removeWidget(m_currentComponentWindow);
    delete m_currentComponentWindow;
    delete m_document;
}

void ParleyMainWindow::addRecentFile(const KUrl &url, const QString &name)
{
    m_recentFilesAction->addUrl(url, name);
    m_recentFilesAction->saveEntries(KGlobal::config()->group("Recent Files"));
}

void ParleyMainWindow::updateRecentFilesModel()
{
    emit recentFilesChanged();
}

void ParleyMainWindow::saveOptions()
{
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
    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SIGNAL(preferencesChanged()));
    dialog->show();
}

void ParleyMainWindow::slotApplyPreferences()
{
    m_document->enableAutoBackup((m_currentComponent != WelcomeComponent) && Prefs::autoBackup());
}

void ParleyMainWindow::slotCloseDocument()
{
    if (!queryClose()) {
        return;
    }
    showWelcomeScreen();
    m_document->close();
}

void ParleyMainWindow::configurePractice()
{
    ConfigurePracticeDialog configurePracticeDialog(m_document->document(), this, "practice settings",  Prefs::self());
    configurePracticeDialog.exec();
}

void ParleyMainWindow::startPractice()
{
    switchComponent(PracticeComponent);
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

void ParleyMainWindow::tipOfDay()
{
    KTipDialog::showTip(this, "parley/tips", true);
}

void ParleyMainWindow::startupTipOfDay()
{
    KTipDialog::showTip(this, "parley/tips");
}

void ParleyMainWindow::initActions()
{
    ParleyActions::create(ParleyActions::FileNew, m_document, SLOT(slotFileNew()), actionCollection());
    ParleyActions::create(ParleyActions::FileOpen, m_document, SLOT(slotFileOpen()), actionCollection());
    ParleyActions::createDownloadAction(m_document, SLOT(slotGHNS()), actionCollection());
    ParleyActions::create(ParleyActions::FileOpenDownloaded, m_document, SLOT(openGHNS()), actionCollection());

    m_recentFilesAction = ParleyActions::createRecentFilesAction(
                              m_document, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());

    m_recentFilesAction->loadEntries(KGlobal::config()->group("Recent Files"));

    ParleyActions::create(ParleyActions::FileSave, m_document, SLOT(save()), actionCollection());
    ParleyActions::create(ParleyActions::FileSaveAs, m_document, SLOT(saveAs()), actionCollection());
#ifdef HAVE_LIBXSLT
    ParleyActions::create(ParleyActions::FileExport, m_document, SLOT(exportDialog()), actionCollection());
#endif

    ParleyActions::create(ParleyActions::FileProperties, m_document, SLOT(documentProperties()), actionCollection());

    ParleyActions::create(ParleyActions::FileClose, this, SLOT(slotCloseDocument()), actionCollection());
    ParleyActions::create(ParleyActions::FileQuit, this, SLOT(close()), actionCollection());
    ParleyActions::create(ParleyActions::Preferences, this, SLOT(slotGeneralOptions()), actionCollection());

    actionCollection()->addAction(KStandardAction::TipofDay,  "help_tipofday", this, SLOT(tipOfDay()));
}

void ParleyMainWindow::showWelcomeScreen()
{
    switchComponent(WelcomeComponent);
}

void ParleyMainWindow::showEditor()
{
    switchComponent(EditorComponent);
}

void ParleyMainWindow::showPracticeConfiguration()
{
    switchComponent(ConfigurePracticeComponent);
}

void ParleyMainWindow::showPractice()
{
    switchComponent(PracticeComponent);
}

void ParleyMainWindow::showPracticeSummary()
{
    switchComponent(PracticeSummary);
}

void ParleyMainWindow::switchComponent(Component component)
{
    if (component == PracticeComponent) {

        StatisticsMainWindow *statisticsWidget = qobject_cast<StatisticsMainWindow*>(m_currentComponentWindow);
        if (statisticsWidget) {
            statisticsWidget->syncConfig();
        }

        // Don't start a practice when there are no words to practice.
        // This has to be checked before deleting the old component.
        m_testEntryManager.setDocument(m_document->document());
        if (!m_testEntryManager.totalEntryCount()) {
            return;
        }
    }

    // Remove and delete the old component window if there is one active.
    if (m_currentComponentWindow) {
        guiFactory()->removeClient(m_currentComponentWindow);
        centralWidget()->layout()->removeWidget(m_currentComponentWindow);
        delete(m_currentComponentWindow);
    }

    switch (component) {
    case WelcomeComponent: {
        WelcomeScreen *welcome = new WelcomeScreen(this);
        m_currentComponentWindow = welcome;
        showDocumentActions(true, false);
        welcome->updateRecentFilesModel();
        break;
    }
    case ConfigurePracticeComponent: {
        StatisticsMainWindow *statisticsWidget = new StatisticsMainWindow(m_document->document(), this);
        m_currentComponentWindow = statisticsWidget;
        showDocumentActions(true, true);
        break;
    }
    case EditorComponent: {
        EditorWindow *editor = new EditorWindow(this);
        m_currentComponentWindow = editor;
        showDocumentActions(true, true);
        editor->updateDocument(m_document->document());
        break;
    }
    case PracticeComponent: {
        m_document->document()->setModified(true);
        Practice::PracticeMainWindow *practiceWindow = new Practice::PracticeMainWindow(&m_testEntryManager, this);
        connect(practiceWindow, SIGNAL(stopPractice()), this, SLOT(showPracticeSummary()));
        m_currentComponentWindow = practiceWindow;
        showDocumentActions(false, false);
        practiceWindow->startPractice();
        break;
    }
    case PracticeSummary: {
        Practice::PracticeSummaryComponent* summary = new Practice::PracticeSummaryComponent(&m_testEntryManager, this);
        m_currentComponentWindow = summary;
        showDocumentActions(true, true);
        break;
    }
    default:
        break;
    }
    //kDebug() << "new component" << m_currentComponentWindow;

    guiFactory()->addClient(m_currentComponentWindow);
    centralWidget()->layout()->addWidget(m_currentComponentWindow);
    m_currentComponentWindow->show();
    switch (component) {
    case WelcomeComponent: {
        setVisibleToolbar(QString());
        break;
    }
    case ConfigurePracticeComponent: {
        setVisibleToolbar("statisticsToolBar");
        break;
    }
    case EditorComponent: {
        setVisibleToolbar("editorToolBar");
        break;
    }
    case PracticeComponent: {
        setVisibleToolbar("practiceToolBar");
        break;
    }
    case PracticeSummary: {
        setVisibleToolbar("practiceSummaryToolBar");
        break;
    }
    default:
        break;
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
#ifdef HAVE_LIBXSLT
    actionCollection()->action("file_export")->setVisible(edit);
#endif
    actionCollection()->action("file_properties")->setVisible(edit);
    actionCollection()->action("file_close")->setVisible(edit);
}

void ParleyMainWindow::setVisibleToolbar(const QString& name)
{
    Q_FOREACH(KToolBar * toolbar, toolBars()) {
        if (toolbar && toolbar->objectName() == name) {
            toolbar->show();
        } else if (toolbar) {
            toolbar->hide();
        }
    }
}

ParleyDocument* ParleyMainWindow::parleyDocument()
{
    return m_document;
}

ParleyMainWindow::Component ParleyMainWindow::currentComponent()
{
    return m_currentComponent;
}

#include "parleymainwindow.moc"
