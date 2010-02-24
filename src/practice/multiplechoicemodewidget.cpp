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
#include <QtGui/QRadioButton>

using namespace Practice;


MultiplechoiceModeWidget::MultiplechoiceModeWidget (QWidget* parent )
    : AbstractModeWidget (parent)
{
    m_ui = new Ui::MultiplechoicePracticeWidget();
    m_ui->setupUi(this);
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

    if (m_choiceButtons.size() != data.choices.size()){
        qDeleteAll(m_choiceButtons);
        m_choiceButtons.clear();
        setNumberOfRadioButtons(data.choices.size());
    }

    int j = 0;
        foreach(QRadioButton *radio,m_choiceButtons){
            radio->setText(data.choices[j]);
            radio->setToolTip(data.choices[j]);
            j++;
        }
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

void MultiplechoiceModeWidget::setNumberOfRadioButtons(const int numberOfChoices)
{
    for (int i=0;i<numberOfChoices;i++){
        QRadioButton *radio_button = new QRadioButton(this);
        m_ui->verticalLayout->addWidget(radio_button);
        m_choiceButtons.append(radio_button);
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

    int i = 0;
    foreach(QRadioButton *radio,m_choiceButtons){
        if (radio->isChecked()) return i;
        i++;
    }

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
