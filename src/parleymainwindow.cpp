/*
    main part of parley
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2008 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "parleymainwindow.h"

#include <config-parley.h>
#include "editor/editor.h"
#include "statistics/statisticsmainwindow.h"
#include "settings/parleyprefs.h"
#include "practice/guifrontend.h"
#include "practice/practicesummarycomponent.h"
#include "practice/configure/configurepracticedialog.h"

#include "parleyactions.h"

#include "prefs.h"
#include "dashboard/dashboard.h"

#include <KActionCollection>
#include <KRecentFilesAction>
#include <KMessageBox>
#include <KTipDialog>
#include <KXMLGUIFactory>
#include <KToolBar>
#include <QMenuBar>

#include <QTimer>

using namespace Editor;

ParleyMainWindow* ParleyMainWindow::s_instance = 0;
ParleyMainWindow* ParleyMainWindow::instance()
{
    return s_instance;
}

ParleyMainWindow::ParleyMainWindow(const QUrl& filename)
    : KXmlGuiWindow(0)
    , m_currentComponent(NoComponent)
    , m_currentComponentWindow(0)
    , m_sessionManager(this)
{
    s_instance = this;
    m_document = new ParleyDocument(this);

    connect(m_document, &ParleyDocument::documentChanged, this, &ParleyMainWindow::documentUpdated);

    setCentralWidget(new QWidget());
    centralWidget()->setLayout(new QHBoxLayout());
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    initActions();

    bool startWithDashboard = false;

    setupGUI(ToolBar | Keys | Create); ///@todo frameworks KXMLGui warnings

    if (!filename.isEmpty()) {
        m_document->open(filename);
    } else {
        bool openLastFile = Prefs::autoOpenLast();
        if (openLastFile && m_recentFilesAction->actions().count() > 0
            && m_recentFilesAction->action(m_recentFilesAction->actions().count() - 1)->isEnabled()) {
            m_recentFilesAction->action(m_recentFilesAction->actions().count() - 1)->trigger();
        } else {
            startWithDashboard = true;
        }
    }

    // save position of dock windows etc
    setAutoSaveSettings();

    if (startWithDashboard) {
	showDashboard();
    } else {
        showEditor();
    }

    connect(this, &ParleyMainWindow::preferencesChanged, this, &ParleyMainWindow::slotApplyPreferences);
    menuBar()->show();

    // finally show tip-of-day (if the user wants it)
    //QTimer::singleShot( 0, this, SLOT(startupTipOfDay()) );
}

ParleyMainWindow::~ParleyMainWindow()
{
    guiFactory()->removeClient(m_currentComponentWindow);
    centralWidget()->layout()->removeWidget(m_currentComponentWindow);
    delete m_currentComponentWindow;

    // Prevent calling this slot with already deleted m_document as it is no longer needed anyway
    if (m_document->document()) {
        disconnect(m_document->document().get(), &KEduVocDocument::destroyed,
                this, &ParleyMainWindow::slotUpdateWindowCaption);
    }
    delete m_document;
}

void ParleyMainWindow::addRecentFile(const QUrl &url, const QString &name)
{
    m_recentFilesAction->addUrl(url, name);
    m_recentFilesAction->saveEntries(KSharedConfig::openConfig()->group("Recent Files"));
}

void ParleyMainWindow::removeRecentFile(const QUrl &url)
{
    m_recentFilesAction->removeUrl(url);
    m_recentFilesAction->saveEntries(KSharedConfig::openConfig()->group("Recent Files"));
}

void ParleyMainWindow::documentUpdated(const std::shared_ptr<KEduVocDocument> &doc)
{
    if (doc) {
        connect(doc.get(), &KEduVocDocument::docModified,
                this, &ParleyMainWindow::slotUpdateWindowCaption);
        connect(doc.get(), &KEduVocDocument::destroyed,
                this, &ParleyMainWindow::slotUpdateWindowCaption);
        slotUpdateWindowCaption();
    }
}

void ParleyMainWindow::updateRecentFilesModel()
{
    emit recentFilesChanged();
}

void ParleyMainWindow::saveOptions()
{
    Prefs::self()->save();
}

void ParleyMainWindow::slotUpdateWindowCaption()
{
    QString title;
    bool modified = false;
    if (m_document->document()) {
        title = i18nc("Title and a modified status indicator.  [*] is exact and will be shown only when document is modified"
                      , "%1 [*]",m_document->document()->title());
        modified = m_document->document()->isModified();
        if (title == i18n("Untitled")) {
            title = QStringLiteral("[*]");
        }
    }
    setCaption(title, modified);
}

void ParleyMainWindow::slotGeneralOptions()
{
    ParleyPrefs* dialog = new ParleyPrefs(m_document->document().get(), this, QStringLiteral("settings"),  Prefs::self());
    connect(dialog, &ParleyPrefs::settingsChanged, this, &ParleyMainWindow::preferencesChanged);
    dialog->show();
}

void ParleyMainWindow::slotApplyPreferences()
{
    m_document->enableAutoBackup((m_currentComponent != DashboardComponent) && Prefs::autoBackup());
}

void ParleyMainWindow::slotCloseDocument()
{
    if (!queryClose()) {
        return;
    }
    showDashboard();
    m_document->close();
}

void ParleyMainWindow::configurePractice()
{
    ConfigurePracticeDialog configurePracticeDialog(m_document->document().get(), this, QStringLiteral("practice settings"),  Prefs::self());
    configurePracticeDialog.exec();
}

void ParleyMainWindow::startPractice()
{
    if (Prefs::learningLanguage() == Prefs::knownLanguage()) {
        KMessageBox::sorry(this,
                           i18n("You cannot start to practice when the known language is the same as the language to learn."),
                           i18n("Select languages"));
        return;
    }
    qDebug() <<"Starting Switch Practice";
    switchComponent(PracticeComponent);
    qDebug() <<"Finished Switch Practice";
}

void ParleyMainWindow::practiceFinished()
{
    switchComponent(m_componentBeforePractice);
}

bool ParleyMainWindow::queryClose()
{
    if (m_document->queryClose()) {
        Prefs::self()->save();
        return true;
    }
    return false;
}

QSize ParleyMainWindow::sizeHint() const
{
    return QSize(800, 600).expandedTo(KXmlGuiWindow::minimumSizeHint());
}

void ParleyMainWindow::tipOfDay()
{
    KTipDialog::showTip(this, QStringLiteral("parley/tips"), true);
}

void ParleyMainWindow::startupTipOfDay()
{
    KTipDialog::showTip(this, QStringLiteral("parley/tips"));
}

void ParleyMainWindow::slotFileNew()
{
    m_document->slotFileNew();
    slotUpdateWindowCaption();
}

void ParleyMainWindow::initActions()
{
    ParleyActions::create(ParleyActions::FileNew, this, SLOT(slotFileNew()), actionCollection());
    ParleyActions::create(ParleyActions::FileOpen, m_document, SLOT(slotFileOpen()), actionCollection());
    ParleyActions::createDownloadAction(m_document, SLOT(slotGHNS()), actionCollection());
    ParleyActions::create(ParleyActions::FileOpenDownloaded, m_document, SLOT(openGHNS()), actionCollection());

    m_recentFilesAction = ParleyActions::createRecentFilesAction(
        m_document, SLOT(slotFileOpenRecent(QUrl)), actionCollection());
    m_recentFilesAction->loadEntries(KSharedConfig::openConfig()->group("Recent Files"));

    ParleyActions::create(ParleyActions::FileSave, m_document, SLOT(save()), actionCollection());
    ParleyActions::create(ParleyActions::FileSaveAs, m_document, SLOT(saveAs()), actionCollection());
#ifdef HAVE_LIBXSLT
    ParleyActions::create(ParleyActions::FileExport, m_document, SLOT(exportDialog()), actionCollection());
#endif

    ParleyActions::create(ParleyActions::FileProperties, m_document, SLOT(documentProperties()), actionCollection());

    ParleyActions::create(ParleyActions::FileClose, this, SLOT(slotCloseDocument()), actionCollection());
    ParleyActions::create(ParleyActions::FileQuit, this, SLOT(close()), actionCollection());
    ParleyActions::create(ParleyActions::Preferences, this, SLOT(slotGeneralOptions()), actionCollection());

    actionCollection()->addAction(KStandardAction::TipofDay,  QStringLiteral("help_tipofday"), this, SLOT(tipOfDay()));
}

void ParleyMainWindow::showDashboard()
{
    switchComponent(DashboardComponent);
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
        m_sessionManager.setDocument(m_document->document().get());
        if (!m_sessionManager.allEntryCount()) {
            return;
        }
    }

    // Remove and delete the old component window if there is one active.
    if (m_currentComponentWindow) {
        guiFactory()->removeClient(m_currentComponentWindow);
        centralWidget()->layout()->removeWidget(m_currentComponentWindow);
        m_currentComponentWindow->deleteLater();
    }

    switch (component) {
    case DashboardComponent: {
        Dashboard *dashboard = new Dashboard(this);
        m_currentComponentWindow = dashboard;
        showDocumentActions(true, false);
        //dashboard->updateRecentFilesModel();
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
        ///@todo trust the dirty bit
        m_document->document()->setModified(true);
        Practice::PracticeMainWindow *practiceWindow = new Practice::PracticeMainWindow(&m_sessionManager, this);
        connect(practiceWindow, &Practice::PracticeMainWindow::stopPractice, this, &ParleyMainWindow::showPracticeSummary);
        m_currentComponentWindow = practiceWindow;
        qDebug() <<" Practice Slotted up";
        showDocumentActions(false, false);
        practiceWindow->startPractice();
        qDebug() <<" Practice Slotted up2";
        break;
    }
    case PracticeSummary: {
        Practice::PracticeSummaryComponent* summary = new Practice::PracticeSummaryComponent(&m_sessionManager, this);
        m_currentComponentWindow = summary;
        showDocumentActions(true, true);
        break;
    }
    default:
        break;
    }
    //qDebug() << "new component" << m_currentComponentWindow;

    guiFactory()->addClient(m_currentComponentWindow);
    centralWidget()->layout()->addWidget(m_currentComponentWindow);
    m_currentComponentWindow->show();
    switch (component) {
    case DashboardComponent: {
        setVisibleToolbar(QString());
        break;
    }
    case ConfigurePracticeComponent: {
        setVisibleToolbar(QStringLiteral("statisticsToolBar"));
        break;
    }
    case EditorComponent: {
        setVisibleToolbar(QStringLiteral("editorToolBar"));
        break;
    }
    case PracticeComponent: {
        setVisibleToolbar(QStringLiteral("practiceToolBar"));
        break;
    }
    case PracticeSummary: {
        setVisibleToolbar(QStringLiteral("practiceSummaryToolBar"));
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
    actionCollection()->action(QStringLiteral("file_new"))->setVisible(open);
    actionCollection()->action(QStringLiteral("file_open"))->setVisible(open);
    actionCollection()->action(QStringLiteral("file_open_recent"))->setVisible(open);
    actionCollection()->action(QStringLiteral("file_ghns"))->setVisible(open);
    actionCollection()->action(QStringLiteral("file_open_downloaded"))->setVisible(open);

    actionCollection()->action(QStringLiteral("file_save"))->setVisible(edit);
    actionCollection()->action(QStringLiteral("file_save_as"))->setVisible(edit);
    actionCollection()->action(QStringLiteral("file_close"))->setVisible(edit);
#ifdef HAVE_LIBXSLT
    actionCollection()->action("file_export")->setVisible(edit);
#endif
    actionCollection()->action(QStringLiteral("file_properties"))->setVisible(edit);
    actionCollection()->action(QStringLiteral("file_close"))->setVisible(edit);
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
