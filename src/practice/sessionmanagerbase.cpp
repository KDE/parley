/***************************************************************************
    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
                    (C) 2014      Inge Wallin <inge@lysator.liu.se>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sessionmanagerbase.h"

// Qt
#include <QDateTime>

// kdelibs
#include <klocale.h>
#include <kconfig.h>
#include <KDebug>
#include <KMessageBox>

// kdeedulibs
#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <keduvocdocument.h>

// parley
#include "entryfilter.h"
#include <prefs.h>


using namespace Practice;

SessionManagerBase::SessionManagerBase(QWidget* parent)
    : m_parent(parent)
    , m_fromTranslation(0)
    , m_toTranslation(1)
    , m_currentEntry(-1)
    , m_totalTime(0)
    , m_randomSequence(QDateTime::currentDateTime().toTime_t())
{
}

SessionManagerBase::~SessionManagerBase()
{
    qDeleteAll(m_allTestEntries);
}

void SessionManagerBase::setDocument(KEduVocDocument* doc)
{
    qDeleteAll(m_allTestEntries);
    m_allTestEntries.clear();
    m_notAskedTestEntries.clear();
    m_currentEntries.clear();

    m_doc = doc;

    // Don't crash when trying to start practicing a document with only one language
    if (m_doc->identifierCount() < 2) {
        KMessageBox::error(0, i18n("The vocabulary collection contains fewer than two languages.",
                                   i18n("Could not start practice")));
        return;
    }

#if 0
    if (Prefs::questionLanguage() >= m_doc->identifierCount()
        || Prefs::solutionLanguage() >= m_doc->identifierCount()) {
        kDebug() << "Invalid language selection" << m_fromTranslation << " to " << m_toTranslation;
        Prefs::setQuestionLanguage(0);
        Prefs::setSolutionLanguage(1);
    }
#else
    // FIXME: Is this relevant?  The code in entryfilter seems to ignore settings here.
    if (Prefs::learningLanguage() >= m_doc->identifierCount()
        || Prefs::knownLanguage() >= m_doc->identifierCount()) {
        kDebug() << "Invalid language selection" << m_fromTranslation
                 << " to " << m_toTranslation;
        Prefs::setLearningLanguage(0);
        Prefs::setKnownLanguage(1);
    }
#endif

    setLanguages(Prefs::knownLanguage(), Prefs::learningLanguage());
    kDebug() << "Practice: learning language: " << m_doc->identifier(m_fromTranslation).name()
             << " known language: " << m_doc->identifier(m_toTranslation).name();

    filterTestEntries();
    kDebug() << "Found " << m_allTestEntries.count() << " entries after filtering.";

    initializeTraining();
}

QString SessionManagerBase::title() const
{
    return m_doc->title();
}

void SessionManagerBase::practiceStarted()
{
    kDebug() << "start practice timer";
    m_time.start();
}

void SessionManagerBase::practiceFinished()
{
    m_totalTime = m_time.elapsed();
    kDebug() << "stop practice timer" << m_totalTime << m_time.toString();
}

int SessionManagerBase::totalTime()
{
    // seconds instead of ms
    return m_totalTime / (1000);
}

TestEntry* SessionManagerBase::nextTrainingEntry()
{
    // In some session types an entry will move out of the "current
    // entries" when they are correctly answered and then we need to
    // refill.
    while (m_currentEntries.count() < Prefs::testNumberOfEntries() &&
            m_notAskedTestEntries.count() > 0) {
        m_currentEntries.append(m_notAskedTestEntries.takeAt(0));
    }

    // Return one of the current entries, but not the same as last
    // time if posible.
    int lastEntry = m_currentEntry;
    if (m_currentEntries.count() > 0) {
        // Choose one of the current entries randomly.
        m_currentEntry = m_randomSequence.getLong(m_currentEntries.count());

        // Do not allow to ask the same entry twice in a row.
        if (m_currentEntries.count() > 1) {
            while (m_currentEntry == lastEntry) {
                m_currentEntry = m_randomSequence.getLong(m_currentEntries.count());
            }
        }

        kDebug() << "nextTrainingEntry:"
                 << m_currentEntry << " = " << m_currentEntries.value(m_currentEntry)->entry()->translation(0)->text()
                 << " (" << m_currentEntries.count() + m_notAskedTestEntries.count()
                 << "entries remaining)";

        return m_currentEntries.value(m_currentEntry);
    } else {
        return 0;
    }
}


void SessionManagerBase::removeCurrentEntryFromPractice()
{
    if (m_currentEntry >= 0) {
        m_currentEntries.takeAt(m_currentEntry);
    }
}

QList<TestEntry*> SessionManagerBase::allTestEntries() const
{
    return m_allTestEntries;
}

int SessionManagerBase::allEntryCount() const
{
    return m_allTestEntries.count();
}

int SessionManagerBase::activeEntryCount()
{
    return m_notAskedTestEntries.count() + m_currentEntries.count();
}

QList<TestEntry*> SessionManagerBase::allUnansweredTestEntries()
{
    QList<TestEntry*> result;

    result.append(m_notAskedTestEntries);
    result.append(m_currentEntries);

    return result;
}

// ----------------------------------------------------------------
//                         Statistics


int SessionManagerBase::statisticTotalCorrectFirstAttempt()
{
    int count = 0;
    foreach(TestEntry * entry, m_allTestEntries) {
        if (entry->correctAtFirstAttempt()) {
            count++;
        }
    }
    return count;
}

int SessionManagerBase::statisticTotalWrong()
{
    int count = 0;
    foreach(TestEntry * entry, m_allTestEntries) {
        if (entry->statisticBadCount()) {
            count++;
        }
    }
    return count;
}

int SessionManagerBase::statisticTotalUnanswered()
{
    int count = 0;
    foreach(TestEntry * entry, m_allTestEntries) {
        if (entry->statisticCount() == 0) {
            count++;
        }
    }
    return count;
}

void SessionManagerBase::printStatistics()
{
    kDebug() << "Test statistics: ";
    foreach(TestEntry * entry, m_allTestEntries) {
        kDebug()
                << " asked: " << entry->statisticCount()
                << " +" << entry->statisticGoodCount() << " -" << entry->statisticBadCount()
                << "Entry: " << entry->entry()->translation(0)->text();
    }
}


// ----------------------------------------------------------------
//                 Multiple choice questions


QStringList SessionManagerBase::multipleChoiceAnswers(int numberChoices)
{
    QStringList choices;
    KRandomSequence randomSequence;
    QList<KEduVocExpression*> allEntries = m_doc->lesson()->entries(KEduVocLesson::Recursive);
    int numValidEntries = 0;
    int count = numberChoices;

    // if the current entry has predefined multiple choice entries definied, use them first
    TestEntry *currentEntry = m_currentEntries.at(m_currentEntry);
    QStringList predefinedChoices = currentEntry->entry()->translation(currentEntry->languageTo())->multipleChoice();
    while (!predefinedChoices.isEmpty() && count > 0) {
        choices.append(predefinedChoices.takeAt(randomSequence.getLong(predefinedChoices.count())));
        count--;
    }

    // find out if we got enough valid entries to fill all the options
    for (int i = 0; i < allEntries.count(); ++i) {
        if (isValidMultipleChoiceAnswer(allEntries.value(i)))
            numValidEntries++;
        if (numValidEntries >= numberChoices)
            break;
    }

    // if we don't have enough valid entries, just try everything and use what we can get
    if (numValidEntries < numberChoices) {
        for (int i = choices.count(); i < allEntries.count(); ++i) {
            KEduVocExpression *exp = allEntries.value(i);

            if (isValidMultipleChoiceAnswer(exp)) {
                choices.append(exp->translation(m_toTranslation)->text());
            }
        }
    } else {
        QList<KEduVocExpression*> exprlist;
        while (count > 0) {
            int nr;
            // if there are enough non-empty fields, fill the options only with those
            do {
                nr = randomSequence.getLong(allEntries.count());
            } while (!isValidMultipleChoiceAnswer(allEntries.value(nr)));
            // append if new entry found
            bool newex = true;
            for (int i = 0; newex && i < exprlist.count(); i++) {
                if (exprlist[i] == allEntries.value(nr))
                    newex = false;
            }
            if (newex) {
                count--;
                exprlist.append(allEntries.value(nr));
            }
        }

        for (int i = 0; i < exprlist.count(); i++) {
            choices.append(exprlist[i]->translation(m_toTranslation)->text());
        }
    }

    kDebug() << "choices:" << choices;
    return choices;
}


// ----------------------------------------------------------------
//                         Protected methods


void SessionManagerBase::filterTestEntries()
{
    EntryFilter filter(m_parent, m_doc);
    m_allTestEntries = filter.entries();
}

void SessionManagerBase::setLanguages(int from, int to)
{
    m_fromTranslation = from;
    m_toTranslation = to;
}

bool SessionManagerBase::isValidMultipleChoiceAnswer(KEduVocExpression *e)
{
    // entry is empty
    if (e->translation(m_toTranslation)->text().trimmed().isEmpty())
        return false;

    // FIXME: Must test individual solution & question languages per
    // entry in mixed mode training.
    //
    // entry is a synonym of the solution
    if (e->translation(m_toTranslation)->synonyms().contains(m_currentEntries.at(m_currentEntry)->entry()->translation(m_toTranslation)))
        return false;

    // Entry has the same text as the solution.
    if (e->translation(m_toTranslation)->text().simplified()
        == m_currentEntries.at(m_currentEntry)->entry()->translation(m_toTranslation)->text().simplified())
        return false;
    return true;
}

