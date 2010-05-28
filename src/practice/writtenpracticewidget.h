/***************************************************************************
    Copyright 2009-2010 Frederik Gladhorn <gladhorn@kde.org>
    Copyright 2009-2010 Daniel Laidig <laidig@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_WRITTENPRACTICEWIDGET_H
#define PRACTICE_WRITTENPRACTICEWIDGET_H

#include "abstractwidget.h"

namespace Ui {
class WrittenPracticeWidget;
}

namespace Practice {
class LatexRenderer;

class WrittenPracticeWidget : public Practice::AbstractModeWidget
{
    Q_OBJECT

public:
    WrittenPracticeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    virtual QVariant userInput();

    virtual void setQuestion(const QVariant& question);
    virtual void setSolution(const QVariant& solution);
    virtual void setHint(const QVariant& hint);
    virtual void setFeedback(const QVariant& feedback);
    virtual void setFeedbackState(AbstractFrontend::ResultState feedbackState);
    virtual void setResultState(AbstractFrontend::ResultState resultState);

    virtual void setQuestionSound(const KUrl& soundUrl);
    virtual void setSolutionSound(const KUrl& soundUrl);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    
    virtual void setSynonym(const QString& entry);
        
public slots:
    void showQuestion();
    void showSolution();
    void continueClicked();
    void showSynonym();

protected:
    Ui::WrittenPracticeWidget* m_ui;

private:
    QString m_solution;
    QString m_synonym;
    AbstractFrontend::ResultState m_resultState;
    AbstractFrontend::ResultState m_feedbackState;
    QList<QWidget*> synonymWidgets;
    LatexRenderer *m_latexRenderer;
};

}

#endif // PRACTICE_WRITTENPRACTICEWIDGET_H
