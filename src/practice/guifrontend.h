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

#ifndef PRACTICE_GUIFRONTEND_H
#define PRACTICE_GUIFRONTEND_H

#include "abstractfrontend.h"
#include "abstractwidget.h"

class KXmlGuiWindow;
namespace Ui {
class PracticeMainWindow;
}

namespace Practice {

class MainWindow;
class AbstractModeWidget;

class GuiFrontend : public Practice::AbstractFrontend
{
    Q_OBJECT

public:
    GuiFrontend(QObject* parent = 0);
    virtual QVariant userInput();
    KXmlGuiWindow* getWindow();
    
    
    virtual void setFinishedWordsTotalWords(int finished, int total);
    
    virtual void setHint(const QVariant& hint);
    
    virtual void setQuestionImage(const QPixmap& img);
    virtual void setSolution(const QVariant& solution);
    virtual void setSolutionImage(const QPixmap& img);
        
    /// update lesson label
    void setLessonName(const QString& lessonName);
    
    virtual void setQuestion(const QVariant& question) { m_centralWidget->setQuestion(question); }
    
public slots:
    void setMode(Mode mode);
    void showQuestion();
    void showSolution();

private:
    MainWindow* m_mainWindow;
    Ui::PracticeMainWindow* m_ui;
    AbstractModeWidget* m_centralWidget;
};

}

#endif // PRACTICE_GUIFRONTEND_H
