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

#include "multiplechoicemodewidget.h"

#include "ui_practice_widget_multiplechoice.h"

#include <KDebug>

using namespace Practice;


MultiplechoiceModeWidget::MultiplechoiceModeWidget (QWidget* parent )
    : AbstractModeWidget (parent)
{
    m_ui = new Ui::MultiplechoicePracticeWidget();
    m_ui->setupUi(this);
}

void MultiplechoiceModeWidget::setQuestion(const QVariant& question)
{
    m_ui->questionLabel->setText(question.toString());
    
    m_ui->choice1->setFocus();
}

void MultiplechoiceModeWidget::showQuestion()
{

}

void MultiplechoiceModeWidget::setSolution(const QVariant& solution)
{
    if (!solution.canConvert(QVariant::StringList)) {
        kWarning() << "expected stringlist";
        return;
        if (solution.toStringList().size() < 5) {
            kWarning() << "stringlist too short!";
            return;
        }
    }
    kDebug() << solution;
    
    QStringList answers(solution.toStringList());
    kDebug() << answers;
    m_ui->choice1->setText(answers.at(0));
    m_ui->choice2->setText(answers.at(1));
    m_ui->choice3->setText(answers.at(2));
    m_ui->choice4->setText(answers.at(3));
    m_ui->choice5->setText(answers.at(4));
    
    //m_ui->solutionLabel->setText(solution.toString());
}

void MultiplechoiceModeWidget::showSolution()
{

}

QVariant MultiplechoiceModeWidget::userInput()
{
    if (m_ui->choice1->isChecked()) return 0;
    if (m_ui->choice2->isChecked()) return 1;
    if (m_ui->choice3->isChecked()) return 2;
    if (m_ui->choice4->isChecked()) return 3;
    if (m_ui->choice5->isChecked()) return 4;
    return QVariant();
}

void MultiplechoiceModeWidget::setQuestionSound(const KUrl& soundUrl)
{

}

void MultiplechoiceModeWidget::setSolutionSound(const KUrl& soundUrl)
{

}

void MultiplechoiceModeWidget::setSolutionPronunciation(const QString& pronunciationText)
{

}

void MultiplechoiceModeWidget::setQuestionPronunciation(const QString& pronunciationText)
{

}

#include "multiplechoicemodewidget.moc"
