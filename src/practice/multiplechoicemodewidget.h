/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef MULTIPLECHOICEMODEWIDGET_H
#define MULTIPLECHOICEMODEWIDGET_H

#include "abstractwidget.h"


namespace Ui {
    class MultiplechoicePracticeWidget;
}

namespace Practice {
    
class MultiplechoiceModeWidget: public AbstractModeWidget
{    
    Q_OBJECT
    
public:
    MultiplechoiceModeWidget(QWidget *parent = 0);
    virtual QVariant userInput();
    
    virtual void setQuestion(const QVariant& question);
    virtual void setSolution(const QVariant& solution);
    virtual void setHint(const QVariant& hint) {}
    virtual void setFeedback(const QVariant& feedback) {}
    virtual void setResultState(AbstractFrontend::ResultState resultState) {}

    virtual void setQuestionSound(const KUrl& soundUrl);
    virtual void setSolutionSound(const KUrl& soundUrl);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    
public Q_SLOTS:
    virtual void showQuestion();
    virtual void showSolution();
    
private:
    Ui::MultiplechoicePracticeWidget* m_ui;
};

}

#endif // FLASHCARDMODEWIDGET_H

