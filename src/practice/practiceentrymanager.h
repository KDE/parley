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
* @brief This stores the question set and provides information about the current question (the question, answer, image, etc)
*/

class PracticeEntryManager : public QObject
{
        Q_OBJECT

    public:
        enum TestCategory
        {
            Written,
            MultipleChoice,
            MultipleData
        };


        PracticeEntryManager(QObject* parent = 0);
        ~PracticeEntryManager();

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

        /**
        * Re-emits the current image. Used for flashcards mode.
        */
        void slotEmitImage(bool backsideOfFlashcard);

    protected:
        /**
        * Select appropriate entries for the practice (respect blocking settings etc)
        * m_allTestEntries will be filled by this.
        */
        void filterTestEntries();
        void shuffle();
        KEduVocTranslation* makeSolution(); // sets the solution so it can be gotten later.
        QStringList makeChoices(KEduVocTranslation* solution) const;
        QStringList makeMultipleChoiceChoices(KEduVocTranslation * solution) const;
        QStringList makeArticleChoices(const QString& solution) const;
        QString makeArticleAnswer(KEduVocWordFlags wordTypeFlags) const;
        void setConjugationData(KEduVocTranslation * solution);

        QString currentQuestion() const;
        QStringList currentQuestions() const;

        TestCategory testCategory() const;

        QString tenseDescription(KEduVocWordFlags flags, const QString& tenseName = "") const;

    public slots:
        void slotNewEntry();

    protected:
        KEduVocDocument * m_doc;
        QList<PracticeEntry*> m_entriesOriginal;
        QList<PracticeEntry*> m_entriesRemaining;
        QList<PracticeEntry*> m_entriesFinished;
        PracticeEntry * m_entry;
        QString m_solution;
        QStringList m_solutions;
        QStringList m_prompts;

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
};



#endif
