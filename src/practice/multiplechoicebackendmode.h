/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef MULTIPLECHOICEBACKENDMODE_H
#define MULTIPLECHOICEBACKENDMODE_H

#include "abstractbackendmode.h"
#include "practice/sessionmanagerbase.h"

namespace Practice
{

class MultipleChoiceBackendMode : public AbstractBackendMode
{
    Q_OBJECT

public:
    MultipleChoiceBackendMode(AbstractFrontend *frontend, QObject *parent,
                              Practice::SessionManagerBase* sessionManager);

    /** Start practicing a new word */
    bool setTestEntry(TestEntry* current) Q_DECL_OVERRIDE;

    void checkAnswer() Q_DECL_OVERRIDE;

public Q_SLOTS:
    void hintAction() Q_DECL_OVERRIDE;

protected:
    /**
     * set the list of possible answers. This function needs to call setCorrectAnswer and setChoices
     */
    virtual void prepareChoices(TestEntry* current);

    /**
     * Set the question/original language
     */
    void setQuestion(const QString& question);

    /**
     * This must include the correct answer. The order of the choices will be preserved.
     */
    void setChoices(const QStringList& choices);

    /**
     * The correct solution, index of the choices.
     */
    void setCorrectAnswer(int index);

    int numberOfChoices();

    /**
     * Configure the multiple choice front end.
     */
    virtual void populateFrontEnd();

private:
    SessionManagerBase* m_sessionManager;
    QString m_question;
    int m_numberOfChoices;
    QStringList m_choices;
    int m_correctAnswer;
    QList<int> m_hints;
};

}

#endif
