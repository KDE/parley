/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "multiplechoicebackendmode.h"

#include <KLocalizedString>

#include <QRandomGenerator>

#include "multiplechoicedata.h"

using namespace Practice;

MultipleChoiceBackendMode::MultipleChoiceBackendMode(AbstractFrontend* frontend, QObject* parent,
                                                     Practice::SessionManagerBase* sessionManager)
    : AbstractBackendMode(frontend, parent)
    , m_sessionManager(sessionManager)
{
    m_numberOfChoices = Prefs::numberMultipleChoiceAnswers();
}

bool MultipleChoiceBackendMode::setTestEntry(TestEntry* current)
{
    m_current = current;
    m_hints.clear();
    m_choices.clear();
    m_question.clear();

    prepareChoices(current);
    populateFrontEnd();
    return true;
}

void MultipleChoiceBackendMode::prepareChoices(TestEntry* current)
{
    Q_UNUSED(current)
    setQuestion(m_current->entry()->translation(m_current->languageFrom())->text());

    QStringList choices = m_sessionManager->multipleChoiceAnswers(m_numberOfChoices - 1);
    foreach(const QString & choice, choices) {
        int position = QRandomGenerator::global()->bounded(m_choices.count() + 1);
        m_choices.insert(position, choice);
    }
    int correctAnswer = QRandomGenerator::global()->bounded(m_choices.count() + 1);
    m_choices.insert(correctAnswer, m_current->entry()->translation(m_current->languageTo())->text());
    setCorrectAnswer(correctAnswer);
}

void MultipleChoiceBackendMode::populateFrontEnd()
{
    MultipleChoiceData data;
    data.question = m_question;
    data.choices = m_choices;

    m_frontend->setQuestion(QVariant::fromValue<MultipleChoiceData>(data));
    m_frontend->setSolution(m_correctAnswer);
    m_frontend->setQuestionSound(m_current->entry()->translation(m_current->languageFrom())->soundUrl());
    m_frontend->setSolutionSound(m_current->entry()->translation(m_current->languageTo())->soundUrl());
    m_frontend->setQuestionPronunciation(m_current->entry()->translation(m_current->languageFrom())->pronunciation());
    m_frontend->setSolutionPronunciation(m_current->entry()->translation(m_current->languageTo())->pronunciation());
    m_frontend->setResultState(AbstractFrontend::QuestionState);
    m_frontend->showQuestion();
}


void MultipleChoiceBackendMode::setQuestion(const QString& question)
{
    m_question = question;
}

int MultipleChoiceBackendMode::numberOfChoices()
{
    return m_numberOfChoices;
}

void MultipleChoiceBackendMode::setChoices(const QStringList& choices)
{
    m_choices = choices;
}

void MultipleChoiceBackendMode::setCorrectAnswer(int index)
{
    qDebug() << "correct: " << index << m_choices.at(index);
    m_correctAnswer = index;
}

void MultipleChoiceBackendMode::checkAnswer()
{
    if (!m_frontend->userInput().isNull() && m_frontend->userInput().toInt() == m_correctAnswer) {
        emit answerRight();
    } else {
        if (!m_frontend->userInput().isNull()) {
            m_current->addUserAnswer(m_choices.at(m_frontend->userInput().toInt()));
        }
        emit answerWrongShowSolution();
    }
}

void MultipleChoiceBackendMode::hintAction()
{
    if (m_choices.count() - m_hints.count() <= 2) {
        // show solution
        m_frontend->setFeedback(i18n("You revealed the answer by using too many hints."));
        emit answerWrongShowSolution();
        return;
    }

    int hint = -1;
    do {
	hint = QRandomGenerator::global()->bounded(m_choices.count());
    } while (hint == m_correctAnswer || m_hints.contains(hint));
    m_hints.append(hint);
    m_frontend->setHint(QVariant(hint));
}
