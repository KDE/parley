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

#include <KDebug>
#include <KActionCollection>
#include <KAction>
#include <KLocalizedString>
#include <KConfig>
#include <KConfigGroup>

using namespace Practice;

PracticeMainWindow::PracticeMainWindow(ParleyMainWindow* parent)
    : KXmlGuiWindow(parent)
{
    // KXmlGui
    setXMLFile("practiceui.rc");
    setObjectName("Practice");

    m_guiFrontend = new GuiFrontend(this);
    setCentralWidget(m_guiFrontend->widget());
    
    Practice::PracticeOptions options;
    m_backend = new Practice::DefaultBackend(m_guiFrontend, parent->parleyDocument(), options, this);
    
    // setModified - otherwise we may not ask to save progress
    parent->parleyDocument()->document()->setModified(true);

    initActions();

    connect(this, SIGNAL(enterPressed()), this, SLOT(continueAction()));
    connect(this, SIGNAL(stopPractice()), this, SIGNAL(stopPractice()));

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);
}

PracticeMainWindow::~PracticeMainWindow()
{
    delete m_backend;
    delete m_guiFrontend;

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
}

void PracticeMainWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        if (focusWidget()->metaObject()->className() == QLatin1String("QRadioButton")) {
            kDebug() << "enter on radio";
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

#include "practicemainwindow.moc"
