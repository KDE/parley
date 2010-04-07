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


#ifndef FLASHCARDMODEWIDGET_H
#define FLASHCARDMODEWIDGET_H

#include "abstractwidget.h"


namespace Ui {
    class FlashCardPracticeWidget;
}

namespace Practice {
    
class FlashCardModeWidget: public AbstractModeWidget
{    
    Q_OBJECT
    
public:
    FlashCardModeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    virtual QVariant userInput();
    
    virtual void setQuestion(const QVariant& question);
    virtual void setSolution(const QVariant& solution);
    virtual void setHint(const QVariant& hint) {Q_UNUSED(hint);}
    virtual void setFeedback(const QVariant& feedback) {Q_UNUSED(feedback);}
    virtual void setResultState(AbstractFrontend::ResultState resultState) {Q_UNUSED(resultState);}

    virtual void setQuestionSound(const KUrl& soundUrl);
    virtual void setSolutionSound(const KUrl& soundUrl);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    
public Q_SLOTS:
    virtual void showQuestion();
    virtual void showSolution();
    
private:
    Ui::FlashCardPracticeWidget* m_ui;
};

}

#endif // FLASHCARDMODEWIDGET_H

