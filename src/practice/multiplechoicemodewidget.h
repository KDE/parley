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
class QRadioButton;

namespace Practice {
    
class MultiplechoiceModeWidget: public AbstractModeWidget
{    
    Q_OBJECT
    
public:
    MultiplechoiceModeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    virtual QVariant userInput();
    
    virtual void setQuestion(const QVariant& question);
    virtual void setSolution(const QVariant& solution);
    virtual void setHint(const QVariant& hint);
    virtual void setFeedback(const QVariant& feedback);
    virtual void setFeedbackState(AbstractFrontend::ResultState feedbackState) {Q_UNUSED(feedbackState);}
    virtual void setResultState(AbstractFrontend::ResultState resultState) {Q_UNUSED(resultState);}

    virtual void setQuestionSound(const KUrl& soundUrl);
    virtual void setSolutionSound(const KUrl& soundUrl);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    
    virtual void setSynonym(const QString& entry);
    
public Q_SLOTS:
    virtual void showQuestion();
    virtual void showSolution();
    virtual void setNumberOfRadioButtons(const int numberOfChoices);
    virtual void showSynonym();
    
private:
    Ui::MultiplechoicePracticeWidget* m_ui;
    int m_solution;
    QList<QRadioButton*> m_choiceButtons;
    QList<QAction*> m_actions;
};

}

#endif // FLASHCARDMODEWIDGET_H

