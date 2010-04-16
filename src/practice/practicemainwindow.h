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
#include "testentrymanager.h"

class ParleyMainWindow;

namespace Practice {
class DefaultBackend;
class GuiFrontend;    

class PracticeMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    PracticeMainWindow(TestEntryManager* m_testEntryManager, ParleyMainWindow* parent = 0);
    ~PracticeMainWindow();
    
    void startPractice();

public Q_SLOTS:
    void toggleFullScreenMode(bool fullScreen);

Q_SIGNALS:
    void enterPressed();
    void stopPractice();

protected:
    virtual void keyPressEvent(QKeyEvent* e);
    
private:    
    void initActions();
    
    GuiFrontend* m_guiFrontend;
    DefaultBackend* m_backend;
    ParleyMainWindow* m_parent;
    QWidget* m_floatingToolBar;

};
}

#endif
