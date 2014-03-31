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
     * The grade of the current entry - this has an default implementation to return the grade for the current translation.
     * This is used in the frontend to visualize the grade. If the practice is not on the translation itself but for example
     * conjugations, the mode needs to re-implement this function in order to change the grades of the correct parts of the
     * translation.
     * For modes that work on several words, this should return the worst grade of them.
     * @return the grade
     */
    virtual grade_t currentGradeForEntry();

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
    AbstractFrontend* m_frontend;
    TestEntry* m_current;
    QStringList m_synonyms;
};

}

#endif // ABSTRACTBACKENDMODE_H
