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

namespace Practice {    

class PracticeMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    PracticeMainWindow(QWidget* parent = 0);
    ~PracticeMainWindow();
    
Q_SIGNALS:
    void enterPressed();
    void stopPractice();

protected:
    virtual void keyPressEvent(QKeyEvent* e);
    
private:
    void initActions();
};
}

#endif