/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "practicemainwindow.h"


#include <QHBoxLayout>
#include <QToolButton>
#include <QPropertyAnimation>

#include <KActionCollection>
#include <QAction>
#include <KToolBar>
#include <QMenuBar>
#include <KToggleFullScreenAction>
#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>

#include "parleymainwindow.h"
#include "guifrontend.h"
#include "practicestatemachine.h"
#include <languagesettings.h>

using namespace Practice;


PracticeMainWindow::PracticeMainWindow(SessionManagerBase* sessionManager,
                                       ParleyMainWindow* mainWindow)
    : KXmlGuiWindow(mainWindow), m_mainWindow(mainWindow)
{
    // KXmlGui
    setXMLFile(QStringLiteral("practiceui.rc"));
    setObjectName(QStringLiteral("Practice"));

    m_guiFrontend = new GuiFrontend(this);
    setCentralWidget(m_guiFrontend->widget());

    m_stateMachine = new PracticeStateMachine(m_guiFrontend, mainWindow->parleyDocument(), sessionManager, this);

    // setModified - otherwise we may not ask to save progress
    ///@todo trust the dirty bit
    mainWindow->parleyDocument()->document()->setModified(true);

    initActions();

    connect(this, &PracticeMainWindow::enterPressed, m_guiFrontend, &GuiFrontend::continueAction);
    connect(m_stateMachine, &PracticeStateMachine::practiceFinished, this, &PracticeMainWindow::practiceFinished);

    KConfigGroup cfg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), objectName());
    applyMainWindowSettings(cfg);
}

PracticeMainWindow::~PracticeMainWindow()
{
    //m_floatingToolBar is a child of m_mainWindow will be deleted with its children
    //before or after this.  So don't access it in toggleFullScreenMode.
    m_floatingToolBar = 0;
    toggleFullScreenMode(false);

    KConfigGroup cfg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), objectName());
    saveMainWindowSettings(cfg);
}

void PracticeMainWindow::initActions()
{
    QAction* stopPracticeAction = new QAction(this);
    stopPracticeAction->setText(i18n("Stop Practice"));
    stopPracticeAction->setIcon(QIcon::fromTheme(QStringLiteral("practice-stop")));
    stopPracticeAction->setToolTip(i18n("Stop practicing"));
    actionCollection()->addAction(QStringLiteral("practice_stop"), stopPracticeAction);
    connect(stopPracticeAction, &QAction::triggered, m_stateMachine, &PracticeStateMachine::slotPracticeFinished);

    m_fullScreenAction = KStandardAction::fullScreen(this,
                         SLOT(toggleFullScreenMode(bool)),
                         m_mainWindow,
                         actionCollection());

    QAction* toggleAnswerState = new QAction(this);
    toggleAnswerState->setText(i18n("Change answer to right/wrong"));
    toggleAnswerState->setToolTip(i18n("When you answered, Parley will display that the answer was right or wrong.\nThis shortcut changes how the answer is counted."));
    actionCollection()->addAction(QStringLiteral("toggle_answer_state"), toggleAnswerState);
    toggleAnswerState->setShortcut(Qt::CTRL | Qt::Key_Space);
    connect(toggleAnswerState, &QAction::triggered, m_guiFrontend, &GuiFrontend::toggleResultState);

    //m_floatingToolBar now a child of m_mainWindow and will be deleted with its parent
    m_floatingToolBar = new QWidget(m_mainWindow);
    QHBoxLayout *layout = new QHBoxLayout();
    m_floatingToolBar->setLayout(layout);
    m_floatingToolBar->setAutoFillBackground(true);
    QToolButton *fullScreenButton = new QToolButton(m_floatingToolBar);
    fullScreenButton->setDefaultAction(m_fullScreenAction);
    fullScreenButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fullScreenButton->setIconSize(QSize(22, 22));
    fullScreenButton->setAutoRaise(true);
    QToolButton *stopPracticeButton = new QToolButton(m_floatingToolBar);
    stopPracticeButton->setDefaultAction(stopPracticeAction);
    stopPracticeButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    stopPracticeButton->setIconSize(QSize(22, 22));
    stopPracticeButton->setAutoRaise(true);
    layout->addWidget(fullScreenButton);
    layout->addWidget(stopPracticeButton);
    layout->addStretch();
    m_animation = new QPropertyAnimation(m_floatingToolBar, "geometry", this);
    m_animation->setDuration(150);
}

void PracticeMainWindow::resizeEvent(QResizeEvent *)
{
    m_floatingToolBar->resize(m_mainWindow->width(), m_floatingToolBar->sizeHint().height());
    m_animation->setStartValue(QRect(QPoint(0, -m_floatingToolBar->height()), m_floatingToolBar->size()));
    m_animation->setEndValue(QRect(QPoint(0, 0), m_floatingToolBar->size()));
    m_animation->setDirection(QAbstractAnimation::Backward);
    m_animation->stop();
    m_floatingToolBar->setGeometry(m_animation->startValue().toRect());
}

bool PracticeMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::HoverMove && m_fullScreenAction->isChecked()) {
        QPoint pos = static_cast<QHoverEvent*>(event)->pos();
        if (m_animation->direction() == QAbstractAnimation::Backward && pos.y() <= m_floatingToolBar->height()) {
            m_animation->setDirection(QAbstractAnimation::Forward);
            m_animation->start();
        }
        if (m_animation->direction() == QAbstractAnimation::Forward && pos.y() > (m_floatingToolBar->height() + 20)) {
            m_animation->setDirection(QAbstractAnimation::Backward);
            m_animation->start();
        }

    }
    return KXmlGuiWindow::event(event);
}

void PracticeMainWindow::toggleFullScreenMode(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(m_mainWindow, fullScreen);
    m_mainWindow->toolBar(QStringLiteral("practiceToolBar"))->setVisible(!fullScreen);
    m_mainWindow->menuBar()->setVisible(!fullScreen);
    if ( m_floatingToolBar != 0 ) {
        m_floatingToolBar->setVisible(fullScreen);
    }
    m_mainWindow->setSettingsDirty();
}

void PracticeMainWindow::startPractice()
{
    // Set the fonts for the known language and learning language.
    // These are used in the practice state machine to set the
    // questionfont and answerfont in the mode widget.
    QString knownLangLocale = m_mainWindow->parleyDocument()->document()->identifier(Prefs::knownLanguage()).locale();
    LanguageSettings knownLangSettings(knownLangLocale);
    knownLangSettings.load();
    QFont knownLangFont = knownLangSettings.practiceFont();
    m_guiFrontend->setKnownLangFont(knownLangFont);

    QString learningLangLocale = m_mainWindow->parleyDocument()->document()->identifier(Prefs::learningLanguage()).locale();
    LanguageSettings learningLangSettings(learningLangLocale);
    learningLangSettings.load();
    QFont learningLangFont = learningLangSettings.practiceFont();
    m_guiFrontend->setLearningLangFont(learningLangFont);

    m_stateMachine->start();
}

void PracticeMainWindow::practiceFinished()
{
    delete m_stateMachine;
    emit stopPractice();
}
