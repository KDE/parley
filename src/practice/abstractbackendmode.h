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

#include "testentry.h"
#include "practiceoptions.h"
#include "abstractfrontend.h"

#include <QVariant>

namespace Practice {
    
class AbstractBackendMode :public QObject
{
    Q_OBJECT
    
public:
    AbstractBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend *frontend, QObject *parent);
    virtual ~AbstractBackendMode() {}

    /** start practicing a new word. sets some default that can be overwritten by the modes.
      * m_frontend->showQuestion() should be called after the initialization. */
    virtual bool setTestEntry(TestEntry* current);

    /** add a new synonym to the list of shown/answered synonyms depending on which mode we
      * are in. */
    virtual void addSynonym(const QString& entry) { if (!entry.isEmpty()) m_synonyms.append(entry); }

    /**
     * The grade of the current entry - this has an default implementation to return the grade for the current translation.
     * This is used in the frontend to visualize the grade. If the practice is not on the translation itself but for example
     * conjugations, the mode needs to re-implement this function in order to change the grades of the correct parts of the
     * translation.
     * For modes that work on several words, this should return the worst grade of them.
     * @return the grade
     */
    virtual grade_t currentGradeForEntry();

public Q_SLOTS:
    /** the frontend requested to continue */
    virtual void continueAction() = 0;
    /** the frontend requested a hint */
    virtual void hintAction() = 0;

Q_SIGNALS:
    void removeCurrentEntryFromPractice();

    /** ask for the next word to be practiced */
    void nextEntry();

protected:
    enum State {
        NotAnswered,
        AnswerWasWrong,
        SolutionShown
    };

    /** The current state: depending on the state the continue action will trigger different things */
    State m_state;

    /**
     * The current word is done. Grade it and maybe remove it.
     * All subclasses of AbstractBackendMode should call this when an entry is done!
     * this will call TestEntry::up*dateStatistics() and the updateGrades virtual function in this class.
     */
    void gradeEntryAndContinue();

    /**
     * Change the grades for the current entry.
     * The default implementation changes the grade of the current translation.
     * Modes working on other parts of the translations need to re-implement this.
     */
    virtual void updateGrades();

    PracticeOptions m_practiceOptions;
    AbstractFrontend* m_frontend;
    TestEntry* m_current;
    QStringList m_synonyms;
};

}

#endif // ABSTRACTBACKENDMODE_H
