/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRACTICE_MAINWINDOW_H
#define PRACTICE_MAINWINDOW_H

#include "sessionmanagerbase.h"
#include <KXmlGuiWindow>
#include <QKeyEvent>

class ParleyMainWindow;
class KToggleFullScreenAction;
class QPropertyAnimation;

///@brief contains all objects for the practice window
namespace Practice
{
class PracticeStateMachine;
class GuiFrontend;

class PracticeMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit PracticeMainWindow(SessionManagerBase *sessionManager, ParleyMainWindow *mainWindow = nullptr);
    ~PracticeMainWindow() override;

    void startPractice();

public Q_SLOTS:
    void toggleFullScreenMode(bool fullScreen);
    void practiceFinished();

Q_SIGNALS:
    void enterPressed();
    void stopPractice();

protected:
    void resizeEvent(QResizeEvent *e) override;
    bool event(QEvent *event) override;

private:
    void initActions();

    GuiFrontend *m_guiFrontend{nullptr};
    PracticeStateMachine *m_stateMachine{nullptr};
    ParleyMainWindow *m_mainWindow{nullptr};
    QWidget *m_floatingToolBar{nullptr};
    KToggleFullScreenAction *m_fullScreenAction{nullptr};
    QPropertyAnimation *m_animation{nullptr};
};
}

#endif
