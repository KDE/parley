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

#include "abstractwidget.h"
#include "abstractfrontend.h"

#include <KXmlGuiWindow>
#include <KUrl>

#include "practicemainwindow.h"

namespace Ui {
    class PracticeMainWindow;
}

namespace Practice {

class GuiFrontend : public AbstractFrontend
{
    Q_OBJECT

public:
    GuiFrontend(QObject* parent = 0);
    virtual QVariant userInput();
    KXmlGuiWindow* getWindow();

    virtual void setQuestion(const QVariant& question);
    virtual void setQuestionImage(const KUrl& img);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    virtual void setQuestionSound(const KUrl& soundUrl);
    
    virtual void setSolution(const QVariant& solution);
    virtual void setSolutionImage(const KUrl& img);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setSolutionSound(const KUrl& soundUrl);
    
    virtual void setHint(const QVariant& hint);
    virtual void setFeedback(const QVariant& feedback);
       
    virtual void setResultState(ResultState resultState);
    virtual ResultState resultState();

    /// update lesson label
    void setLessonName(const QString& lessonName);
    virtual void setFinishedWordsTotalWords(int finished, int total);
    
public Q_SLOTS:
    void setMode(Mode mode);
    void showQuestion();
    void showSolution();

private Q_SLOTS:
    void resultRadioButtonsChanged();
    void continueAction();
   
private:
    PracticeMainWindow* m_mainWindow;
    Ui::PracticeMainWindow* m_ui;
    AbstractModeWidget* m_centralWidget;
    ResultState m_resultState;
    KUrl m_lastImage;
};

}

#endif // PRACTICE_GUIFRONTEND_H
