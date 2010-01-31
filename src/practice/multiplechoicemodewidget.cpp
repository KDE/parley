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
#include "multiplechoicedata.h"

#include "ui_practice_widget_multiplechoice.h"

#include <KDebug>
#include <kcolorscheme.h>

using namespace Practice;


MultiplechoiceModeWidget::MultiplechoiceModeWidget (QWidget* parent )
    : AbstractModeWidget (parent)
{
    m_ui = new Ui::MultiplechoicePracticeWidget();
    m_ui->setupUi(this);

    m_choiceButtons = QList<QRadioButton*>();
    m_choiceButtons << m_ui->choice1 << m_ui->choice2 << m_ui->choice3 << m_ui->choice4 << m_ui->choice5;

    KColorScheme scheme(QPalette::Active);
    m_correctPalette = QApplication::palette();
    m_correctPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::PositiveText).color());
    m_correctPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::PositiveText).color());
    m_wrongPalette = QApplication::palette();
    m_wrongPalette.setColor(QPalette::WindowText, scheme.foreground(KColorScheme::NegativeText).color());
    m_wrongPalette.setColor(QPalette::Text, scheme.foreground(KColorScheme::NegativeText).color());
}

void MultiplechoiceModeWidget::setQuestion(const QVariant& question)
{
    if (!question.canConvert<MultipleChoiceData>()) {
        kWarning() << "expected MultipleChoiceData";
        return;
    }
    MultipleChoiceData data = question.value<MultipleChoiceData>();
    m_ui->questionLabel->setText(data.question);

    kDebug() << data.question << data.choices;

    if (data.choices.size() < 5) {
        kWarning() << "stringlist too short!";
        return;
    }
    m_ui->choice1->setText(data.choices.at(0));
    m_ui->choice1->setToolTip(data.choices.at(0));
    m_ui->choice2->setText(data.choices.at(1));
    m_ui->choice2->setToolTip(data.choices.at(1));
    m_ui->choice3->setText(data.choices.at(2));
    m_ui->choice3->setToolTip(data.choices.at(2));
    m_ui->choice4->setText(data.choices.at(3));
    m_ui->choice4->setToolTip(data.choices.at(3));
    m_ui->choice5->setText(data.choices.at(4));
    m_ui->choice5->setToolTip(data.choices.at(4));

    m_ui->choice1->setFocus();
}

void MultiplechoiceModeWidget::showQuestion()
{
    foreach(QRadioButton *radio, m_choiceButtons) {
        radio->setPalette(QApplication::palette());
        //necessary trick to uncheck'em all
        radio->setAutoExclusive(false);
        radio->setChecked(false);
        radio->setAutoExclusive(true);
    }
}

void MultiplechoiceModeWidget::setSolution(const QVariant& solution)
{
    m_solution = solution.toInt();
}

void MultiplechoiceModeWidget::showSolution()
{
    int input = userInput().toInt();
    m_choiceButtons[m_solution]->setPalette(m_correctPalette);
    if (input != m_solution) {
        m_choiceButtons[input]->setPalette(m_wrongPalette);
    }
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
