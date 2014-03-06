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

#ifndef PRACTICE_MAINWINDOW_H
#define PRACTICE_MAINWINDOW_H

#include <KXmlGuiWindow>
#include <QtGui/QKeyEvent>
#include "sessionmanager.h"

class ParleyMainWindow;
class KToggleFullScreenAction;
class QPropertyAnimation;

namespace Practice
{
class PracticeStateMachine;
class GuiFrontend;

class PracticeMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    PracticeMainWindow(SessionManager* sessionManager, ParleyMainWindow* mainWindow = 0);
    ~PracticeMainWindow();

    void startPractice();

public Q_SLOTS:
    void toggleFullScreenMode(bool fullScreen);
    void practiceFinished();

Q_SIGNALS:
    void enterPressed();
    void stopPractice();

protected:
    virtual void resizeEvent(QResizeEvent *e);
    virtual bool event(QEvent *event);

private:
    void initActions();

    GuiFrontend* m_guiFrontend;
    PracticeStateMachine* m_stateMachine;
    ParleyMainWindow* m_mainWindow;
    QWidget* m_floatingToolBar;
    KToggleFullScreenAction* m_fullScreenAction;
    QPropertyAnimation* m_animation;
};
}

#endif
