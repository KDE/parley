/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "practicemainwindow.h"


#include <QHBoxLayout>
#include <QToolButton>
#include <QPropertyAnimation>

#include <KActionCollection>
#include <KAction>
#include <KToolBar>
#include <KMenuBar>
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
    setXMLFile("practiceui.rc");
    setObjectName("Practice");

    m_guiFrontend = new GuiFrontend(this);
    setCentralWidget(m_guiFrontend->widget());

    m_stateMachine = new PracticeStateMachine(m_guiFrontend, mainWindow->parleyDocument(), sessionManager, this);

    // setModified - otherwise we may not ask to save progress
    mainWindow->parleyDocument()->document()->setModified(true);

    initActions();

    connect(this, SIGNAL(enterPressed()), m_guiFrontend, SIGNAL(continueAction()));
    connect(m_stateMachine, SIGNAL(practiceFinished()), this, SLOT(practiceFinished()));

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);
}

PracticeMainWindow::~PracticeMainWindow()
{
    //m_floatingToolBar is a child of m_mainWindow will be deleted with its children
    //before or after this.  So don't access it in toggleFullScreenMode.
    m_floatingToolBar = 0;
    toggleFullScreenMode(false);

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
}

void PracticeMainWindow::initActions()
{
    KAction* stopPracticeAction = new KAction(this);
    stopPracticeAction->setText(i18n("Stop Practice"));
    stopPracticeAction->setIcon(KIcon("practice-stop"));
    stopPracticeAction->setHelpText(i18n("Stop practicing"));
    actionCollection()->addAction("practice_stop", stopPracticeAction);
    connect(stopPracticeAction, SIGNAL(triggered()), m_stateMachine, SLOT(slotPracticeFinished()));

    m_fullScreenAction = KStandardAction::fullScreen(this,
                         SLOT(toggleFullScreenMode(bool)),
                         m_mainWindow,
                         actionCollection());

    KAction* toggleAnswerState = new KAction(this);
    toggleAnswerState->setText(i18n("Change answer to right/wrong"));
    toggleAnswerState->setHelpText(i18n("When you answered, Parley will display that the answer was right or wrong.\nThis shortcut changes how the answer is counted."));
    actionCollection()->addAction("toggle_answer_state", toggleAnswerState);
    toggleAnswerState->setShortcut(Qt::CTRL + Qt::Key_Space);
    connect(toggleAnswerState, SIGNAL(triggered()), m_guiFrontend, SLOT(toggleResultState()));

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
    m_mainWindow->toolBar("practiceToolBar")->setVisible(!fullScreen);
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
    knownLangSettings.readConfig();
    QFont knownLangFont = knownLangSettings.practiceFont();
    m_guiFrontend->setKnownLangFont(knownLangFont);

    QString learningLangLocale = m_mainWindow->parleyDocument()->document()->identifier(Prefs::learningLanguage()).locale();
    LanguageSettings learningLangSettings(learningLangLocale);
    learningLangSettings.readConfig();
    QFont learningLangFont = learningLangSettings.practiceFont();
    m_guiFrontend->setLearningLangFont(learningLangFont);

    m_stateMachine->start();
}

void PracticeMainWindow::practiceFinished()
{
    m_guiFrontend->modeWidgetDestroyed(0);
    delete m_stateMachine;
    emit stopPractice();
}

#include "practicemainwindow.moc"
