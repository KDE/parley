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

#include "practiceentry.h"

/**
* @class PracticeEntryManager
* @author David Capel <wot.narg@gmail.com>
* @brief This stores the question set and provides information about the current question (the question, answer, image, etc)
*/

class PracticeEntryManager : public QObject
{
        Q_OBJECT

    public:
        PracticeEntryManager(QObject* parent = 0);
        ~PracticeEntryManager();

        void open(KEduVocDocument*);

        const QString currentSolution() const;

        /// Append an expression to the end of the internal list.
        /// Used when the user gets the answer wrong and we want to save the question for later.
        void appendEntryToList(PracticeEntry*);

        /// Returns the current KEduVocDocument for the practice set.
        KEduVocDocument* document() {return m_doc;}

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
        void shuffle();
        QString findTextForCurrentMode(KEduVocTranslation* question);

    public slots:
        void slotNewEntry();

    protected:
        KEduVocDocument * m_doc;
        QList<PracticeEntry*> m_entries;
        PracticeEntry * m_entry;
        QListIterator<PracticeEntry*> m_iter;

        int m_fromTranslation;
        int m_toTranslation;
        int m_testType;
        int m_numberEntriesRemaining;


    signals:
        /// Emitted when new text is available.
        /// An empty QString signals that there is no assosiated text.
        void signalNewText(const QString&);
        /// Emitted when a new solution is available.
        /// Used when modes need the answer but don't need the hassle of the raw PracticeEntry.
        void signalNewSolution(const QString&);
        /// Emitted when a new image is available.
        /// An empty KUrl signals that there is no assosiated image.
        void signalNewImage(const KUrl&);
        /// Emitted when a new sound is available.
        /// An empty KUrl signals that there is no assosiated sound.
        void signalNewSound(const KUrl&);
        /// Emitted when the question is changed.
        /// This is used so @class Statistics can update the grades and related information.
        /// It is additionally used by some input widgets.
        void signalEntryChanged(PracticeEntry*, QList<PracticeEntry*>);
        /// Used so various widgets will know to reset themselves.
        void signalNewEntry();
        /// Emitted when the set of questions is finished.
        void signalSetFinished();

};



#endif
