/*
    SPDX-FileCopyrightText: 2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ABSTRACTBACKENDMODE_H
#define ABSTRACTBACKENDMODE_H

#include <QVariant>

#include "testentry.h"
#include "abstractfrontend.h"


namespace Practice
{

class AbstractBackendMode : public QObject
{
    Q_OBJECT

public:

    AbstractBackendMode(AbstractFrontend *frontend, QObject *parent);
    virtual ~AbstractBackendMode() {}

    /** start practicing a new word. sets some default that can be overwritten by the modes.
      * m_frontend->showQuestion() should be called after the initialization. */
    virtual bool setTestEntry(TestEntry* current);

    /** add a new synonym to the list of shown/answered synonyms depending on which mode we
      * are in. */
    void addSynonym(const QString& entry) {
        if (!entry.isEmpty()) m_synonyms.append(entry);
    }

    /**
     * The pregrade of the current entry - this has a default
     * implementation to return the pregrade for the current translation.
     *
     * @see currentGradeForEntry() for a comment about reimplementation.
     *
     * @return the pregrade
     */
    virtual grade_t currentPreGradeForEntry() const;
    /**
     * The grade of the current entry - this has a default
     * implementation to return the grade for the current translation.
     *
     * This is used in the frontend to visualize the confidence
     * level. If the practice is not on the translation itself but for
     * example conjugations, the mode needs to re-implement this
     * function in order to change the grade of the correct parts
     * of the translation.
     *
     * For modes that work on several words, this should return the
     * worst grade of them.
     *
     * @return the grade
     */
    virtual grade_t currentGradeForEntry() const;

    /**
     * Change the grades for the current entry.
     * The default implementation changes the grade of the current translation.
     * Modes working on other parts of the translations need to re-implement this.
     */
    virtual void updateGrades();

public Q_SLOTS:
    /** the frontend requested a hint */
    virtual void hintAction() = 0;

    /**
     * Check if the current answer is right.
     * This function should emit one of these signals: answerRight, answerWrongRetry, answerWrongShowSolution or showSolution
     */
    virtual void checkAnswer() = 0;

Q_SIGNALS:
    void removeCurrentEntryFromPractice();

    /** ask for the next word to be practiced */
    void nextEntry();

    void answerRight();
    void answerWrongRetry();
    void answerWrongShowSolution();
    void showSolution();

protected:
    // Support functions for derived classes

    /** Update the grade for the current entry.
     *
     * If the answer from the user was correct, using isCorrectAnswer
     * as the indicator, then the grade of the current entry is
     * increased one step if there was no previous wrong answer;
     * otherwise it's set to pregrade 1, grade 0. The exception is if
     * the word is new (pregrade 0, grade 0). In that case if it's
     * correct it's presumed that the user knows the word reasonably
     * well and the grade is set to a higher level. The exact value of
     * this level is arbitrarily chosen now and may become
     * configurable in the future.
     *
     * @param text the translation or similar in the kedudocument that should be changed
     * @param isCorrectAnswer true if the current answer is correct
     * @param hasNoPreviousBadAnswers true if there never was any wrong answer for this word in the current session
     */
    void updateGrade(KEduVocText &text, bool isCorrectAnswer, bool hasNoPreviousBadAnswers);

protected:
    AbstractFrontend* m_frontend;
    TestEntry* m_current;
    QStringList m_synonyms;
};

}

#endif // ABSTRACTBACKENDMODE_H
