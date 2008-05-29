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

#ifndef TESTENTRYMANAGER_H
#define TESTENTRYMANAGER_H


#include <QObject>
#include <QList>

#include "../../libkdeedu/keduvocdocument/keduvocexpression.h"
#include "../../libkdeedu/keduvocdocument/keduvoctranslation.h"
#include "../../libkdeedu/keduvocdocument/keduvocdocument.h"


//#include "testentry.h"
//#include "practiceprefs.h"



/**
* @class TestEntryManager
* @author David Capel <wot.narg@gmail.com>
* @brief This stores the question set and provides information about the current question (the question, answer, image, etc)
*/

class TestEntryManager : public QObject
{
        Q_OBJECT

    public:
        TestEntryManager(QObject* parent = 0);
        
        void open(KEduVocDocument*);

        const QString currentSolution() const;




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

    public slots:
        void slotNewEntry();

    protected:
        KEduVocDocument * m_doc;
        QList<KEduVocExpression*> m_entries;
        KEduVocExpression * m_entry;
        QListIterator<KEduVocExpression*> m_iter;

        int m_fromTranslation;
        int m_toTranslation;
        int m_testType;
        int m_numberEntriesUsed;


    signals:
        /// Emitted when new text is available.
        /// An empty QString signals that there is no assosiated text.
        void signalNewText(const QString&);
        /// Emitted when a new image is available.
        /// An empty KUrl signals that there is no assosiated image.
        void signalNewImage(const KUrl&);
        /// Emitted when a new sound is available.
        /// An empty KUrl signals that there is no assosiated sound.
        void signalNewSound(const KUrl&);
        /// Emitted when the question is changed.
        /// This is used so @class Statistics can update the grades and related information.
        void signalExpressionChanged(KEduVocExpression*);
        /// Emitted when the set of questions is finished.
        void signalQuestionSetFinished();

};



#endif
