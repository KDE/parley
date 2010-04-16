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
#include <KToggleFullScreenAction>
#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>

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

    KStandardAction::fullScreen(this,
                                SLOT(toggleFullScreenMode(bool)),
                                m_parent,
                                actionCollection());
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


void PracticeMainWindow::startPractice()
{
    m_backend->startPractice();
}

void PracticeMainWindow::toggleFullScreenMode(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(m_parent, fullScreen);
}

#include "practicemainwindow.moc"
