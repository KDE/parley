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

#include "parleymainwindow.h"
#include "guifrontend.h"
#include "defaultbackend.h"

#include <KActionCollection>
#include <KAction>
#include <KToolBar>
#include <KMenuBar>
#include <KToggleFullScreenAction>
#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>

#include <QHBoxLayout>
#include <QToolButton>
#include <QPropertyAnimation>

using namespace Practice;

PracticeMainWindow::PracticeMainWindow(TestEntryManager* testEntryManager, ParleyMainWindow* parent)
    : KXmlGuiWindow(parent), m_parent(parent)
{
    // KXmlGui
    setXMLFile("practiceui.rc");
    setObjectName("Practice");

    m_guiFrontend = new GuiFrontend(this);
    setCentralWidget(m_guiFrontend->widget());

    Practice::PracticeOptions options;
    m_backend = new Practice::DefaultBackend(m_guiFrontend, parent->parleyDocument(), options, testEntryManager, this);

    // setModified - otherwise we may not ask to save progress
    parent->parleyDocument()->document()->setModified(true);

    initActions();

    connect(this, SIGNAL(enterPressed()), m_guiFrontend, SIGNAL(continueAction()));
    connect(m_backend, SIGNAL(practiceFinished()), this, SIGNAL(stopPractice()));

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);
}

PracticeMainWindow::~PracticeMainWindow()
{
    toggleFullScreenMode(false);
    delete m_floatingToolBar;
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
    connect(stopPracticeAction, SIGNAL(triggered()), this, SIGNAL(stopPractice()));

    m_fullScreenAction = KStandardAction::fullScreen(this,
                                                     SLOT(toggleFullScreenMode(bool)),
                                                     m_parent,
                                                     actionCollection());

    m_floatingToolBar = new QWidget(m_parent);
    QHBoxLayout *layout = new QHBoxLayout();
    m_floatingToolBar->setLayout(layout);
    m_floatingToolBar->setAutoFillBackground(true);
    QToolButton *fullScreenButton = new QToolButton(m_floatingToolBar);
    fullScreenButton->setDefaultAction(m_fullScreenAction);
    fullScreenButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fullScreenButton->setIconSize(QSize(22,22));
    fullScreenButton->setAutoRaise(true);
    QToolButton *stopPracticeButton = new QToolButton(m_floatingToolBar);
    stopPracticeButton->setDefaultAction(stopPracticeAction);
    stopPracticeButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    stopPracticeButton->setIconSize(QSize(22,22));
    stopPracticeButton->setAutoRaise(true);
    layout->addWidget(fullScreenButton);
    layout->addWidget(stopPracticeButton);
    layout->addStretch();
    m_animation = new QPropertyAnimation(m_floatingToolBar, "geometry", this);
    m_animation->setDuration(150);
}

void PracticeMainWindow::keyPressEvent(QKeyEvent* e)
{
    ///@todo: adapt to gui changes
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        if (focusWidget()->metaObject()->className() == QLatin1String("QRadioButton")) {
            emit enterPressed();
            return;
        }
    }
    KXmlGuiWindow::keyPressEvent(e);
}

void PracticeMainWindow::resizeEvent(QResizeEvent *e)
{
    m_floatingToolBar->resize(m_parent->width(), m_floatingToolBar->sizeHint().height());
    m_animation->setStartValue(QRect(QPoint(0,-m_floatingToolBar->height()), m_floatingToolBar->size()));
    m_animation->setEndValue(QRect(QPoint(0,0), m_floatingToolBar->size()));
    m_animation->setDirection(QAbstractAnimation::Backward);
    m_animation->stop();
    m_floatingToolBar->setGeometry(m_animation->startValue().toRect());
}

bool PracticeMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::HoverMove && m_fullScreenAction->isChecked()) {
        QPoint pos = static_cast<QHoverEvent*>(event)->pos();
        if(m_animation->direction() == QAbstractAnimation::Backward && pos.y() <= m_floatingToolBar->height()) {
            m_animation->setDirection(QAbstractAnimation::Forward);
            m_animation->start();
        }
        if (m_animation->direction() == QAbstractAnimation::Forward && pos.y() > (m_floatingToolBar->height()+20)) {
            m_animation->setDirection(QAbstractAnimation::Backward);
            m_animation->start();
        }

    }
    return KXmlGuiWindow::event(event);
}

void PracticeMainWindow::startPractice()
{
    m_backend->startPractice();
}

void PracticeMainWindow::toggleFullScreenMode(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(m_parent, fullScreen);
    m_parent->toolBar("practiceToolBar")->setVisible(!fullScreen); //TODO: save if it was visible
    m_parent->menuBar()->setVisible(!fullScreen);
    m_floatingToolBar->setVisible(fullScreen);
}

#include "practicemainwindow.moc"
