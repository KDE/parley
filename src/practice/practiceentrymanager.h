/***************************************************************************

                            manage entries

    -----------------------------------------------------------------------

    begin         : Sat Jul 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                    (C) 2008 David Capel <wot.narg@gmail.com>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICEENTRYMANAGER_H
#define PRACTICEENTRYMANAGER_H


#include <QObject>
#include <QList>

#include <keduvocexpression.h>
#include <keduvoctranslation.h>
#include <keduvocdocument.h>
#include "prefs.h"

class PracticeEntry;

/**
* @class PracticeEntryManager
* @author David Capel <wot.narg@gmail.com>
* @brief This stores the question set and provides information about the current question (the question, answer, image, etc). Additionally, it handles the logic for generating the answers and solutions.
*/
class PracticeEntryManager : public QObject
{
        Q_OBJECT

    public:
        enum TestCategory
        {
            Written, ///< The test uses the written format
            MultipleChoice, ///< The test uses the multiple choice format.
            MultipleData ///< The test uses multiple pieces of data in a single question.
        };


        PracticeEntryManager(QObject* parent = 0);
        ~PracticeEntryManager();

        /// Opens the given @class KEduVocDocument, filters the entries, and prepares itself for practice.
        /// This must be called before this class is used.
        void open(KEduVocDocument*);

        /// Returns the solution to the current entry.
        QString currentSolution() const;

        /// Returns the solutions for modes where there are multiple pieces of data.
        QStringList currentSolutions() const;

        /// Append an expression to the end of the internal list.
        /// Used when the user gets the answer wrong and we want to save the question for later.
        void appendEntryToList(PracticeEntry*);

        /// Returns the current KEduVocDocument for the practice set.
        KEduVocDocument* document() {return m_doc;}

        /// Returns the list of finished (correctly answered) entries.
        QList<PracticeEntry*> entriesFinished() { return m_entriesFinished; };

        /**
        * The number of entries in the practice set
        * @return size of the set
        */
        int totalEntryCount() const;

        /**
        * The number of entries that are still to be practiced
        * @return size of the unused portion of the set
        */
        int activeEntryCount() const;

    protected:
        /**
        * Select appropriate entries for the practice (respect blocking settings etc)
        * m_allTestEntries will be filled by this.
        */
        void filterTestEntries();
        /// Shuffles the order of the entries.
        void shuffle();
        /// Creates the solution and stores it to m_solution or m_solutions
        /// This should be the only place where those are modified to keep consistancy.
        /// @return the @class KEduVocTranslation of the solution.
        KEduVocTranslation* makeSolution();
        /// Creates a set of choices (including the solution and distractors) for use in multiple choice modes.
        /// This calls other functions to do the actual work.
        QStringList makeChoices(KEduVocTranslation* solution) const;
        /// Creates a set of choices using random entries (depending on settings, these can be filtered by word type).
        QStringList makeMultipleChoiceChoices(KEduVocTranslation * solution) const;
        /// Creates a set of articles to use as the choices.
        QStringList makeArticleChoices(const QString& solution) const;
        /// Finds or creates an article that matches the given word flags
        QString makeArticleAnswer(KEduVocWordFlags wordTypeFlags) const;
        /// Generates the questions and solutions for conjugation mode.
        void setConjugationData(KEduVocTranslation * solution);

        /// Returns a question for the current entry.
        QString currentQuestion() const;
        /// Returns a set of questions for the current entry (used for multiple data modes)
        QStringList currentQuestions() const;

        /// Returns the @c TestCategory of the current mode.
        TestCategory testCategory() const;
        /// Returns true if the current mode is bilingual.
        bool bilingualTest() const;

        /// Finds an personal pronoun or generates a generic description to represent a tense.
        QString tenseDescription(KEduVocWordFlags flags, const QString& tenseName = "") const;

    public slots:
        /// Emits data for a new entry, using the below signals.
        void slotNewEntry();
        /// Emits data for the back of the flashcard.
        /// Unused in all other modes.
        void slotNewFlashcardBack();

    protected:
        KEduVocDocument * m_doc;
        QList<PracticeEntry*> m_entriesOriginal;
        QList<PracticeEntry*> m_entriesRemaining;
        QList<PracticeEntry*> m_entriesFinished;
        PracticeEntry * m_entry;
        QString m_solution;
        QStringList m_solutions;
        QStringList m_prompts;

        Prefs::EnumTestType::type m_solutionTestType;
        Prefs::EnumTestType::type m_questionTestType;

    signals:
        /// Emitted when new text is available.
        /// An empty QString signals that there is no assosiated text.
        void signalNewText(const QString&);
        /// Emitted when a set of new text prompts is available.
        /// Used for modes that need multiple pieces of data
        void signalNewText(const QStringList&);
        /// Emitted for multiple-choice-style modes and modes that need multiple pieces of data.
        void signalNewChoices(const QStringList&);

        /// Emitted when a new image is available.
        /// An empty KUrl signals that there is no assosiated image.
        /// The boolean is ignored for all modes except flashcards.
        void signalNewImage(const KUrl&, bool);

        /// Emitted when a new sound is available.
        /// An empty KUrl signals that there is no assosiated sound.
        void signalNewSound(const KUrl&);

        /// Used so various widgets will know to reset themselves.
        void signalNewEntry(PracticeEntry*);

        /// Emitted when the set of questions is finished.
        void signalSetFinished();

        /// The text of the solution.
        void signalNewSolution(const QString&);

    friend class PracticeSummary;
};



#endif
