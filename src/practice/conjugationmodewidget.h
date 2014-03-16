/***************************************************************************
    Copyright 2010 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef CONJUGATIONMODEWIDGET_H
#define CONJUGATIONMODEWIDGET_H

#include "abstractwidget.h"


namespace Ui
{
class ConjugationPracticeWidget;
}

namespace Practice
{
class PersonConjugationSolutionWidgets;

class ConjugationModeWidget: public AbstractModeWidget
{
    Q_OBJECT

public:
    ConjugationModeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    virtual ~ConjugationModeWidget();
    virtual QVariant userInput();

    virtual void setQuestion(const QVariant& question);
    virtual void setSolution(const QVariant& solution);
    virtual void setHint(const QVariant& hint);
    virtual void setFeedback(const QVariant& feedback);
    virtual void setFeedbackState(AbstractFrontend::ResultState feedbackState) {
        Q_UNUSED(feedbackState);
    }
    virtual void setResultState(AbstractFrontend::ResultState resultState) {
        Q_UNUSED(resultState);
    }

    virtual void setQuestionFont(const QFont& font);
    virtual void setSolutionFont(const QFont& font);
    virtual void setQuestionSound(const KUrl& soundUrl);
    virtual void setSolutionSound(const KUrl& soundUrl);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    virtual void setSynonym(const QString& /*entry*/) {}

public Q_SLOTS:
    virtual void showQuestion();
    virtual void showSolution();
    virtual void showSynonym() {}

private Q_SLOTS:
    void nextConjugationForm();

private:
    virtual void setNumberOfConjugationWidgets(const int numberOfForms);

    Ui::ConjugationPracticeWidget* m_ui;
    QStringList m_solution;
    QList<PersonConjugationSolutionWidgets*> m_personWidgets;
    QFont m_solutionFont;
};

}

#endif

