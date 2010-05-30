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


#ifndef MULTIPLECHOICEBACKENDMODE_H
#define MULTIPLECHOICEBACKENDMODE_H

#include "abstractbackendmode.h"
#include "practice/testentrymanager.h"

namespace Practice {

class MultipleChoiceBackendMode : public AbstractBackendMode
{
    Q_OBJECT
    
public:
    MultipleChoiceBackendMode(const PracticeOptions& PracticeOptions, AbstractFrontend *frontend, QObject *parent, Practice::TestEntryManager* testEntryManager);
    
    virtual bool setTestEntry(TestEntry* current);

    virtual void checkAnswer();

public Q_SLOTS:
    virtual void hintAction();
    
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

private:
    TestEntryManager* m_testEntryManager;
    KRandomSequence m_randomSequence;
    QString m_question;
    int m_numberOfChoices;
    QStringList m_choices;
    int m_correctAnswer;
    QList<int> m_hints;
};

}

#endif
