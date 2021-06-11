/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sessionmanagerbase.h"

// Qt
#include <QDateTime>
#include <QRandomGenerator>

// kdelibs
#include <KLocalizedString>
#include <KConfig>
#include <QDebug>
#include <KMessageBox>

// kdeedulibs
#include <KEduVocLesson>
#include <KEduVocDocument>

// parley
#include "entryfilter.h"
#include <prefs.h>


using namespace Practice;

SessionManagerBase::SessionManagerBase(QWidget* parent)
    : m_parent(parent)
    , m_learningLanguageIndex(0)
    , m_knownLanguageIndex(1)
    , m_currentEntry(-1)
    , m_totalTime(0)
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

    // Make sure that there are at least 2 languages in the file before
    // starting the practice. If we don't do this, it will crash later.
    if (m_doc->identifierCount() < 2) {
        KMessageBox::error(0, i18n("The vocabulary collection contains fewer than two languages."),
                                   i18n("Could not start practice"));
        return;
    }

    if (Prefs::learningLanguage() >= m_doc->identifierCount()
        || Prefs::knownLanguage() >= m_doc->identifierCount())
    {
        Prefs::setLearningLanguage(0);
        Prefs::setKnownLanguage(1);
    }

    m_learningLanguageIndex = Prefs::learningLanguage();
    m_knownLanguageIndex    = Prefs::knownLanguage();
//     qDebug() << "Practice: learning language:" << m_doc->identifier(m_learningLanguageIndex).name()
//              << " known language:" << m_doc->identifier(m_knownLanguageIndex).name();

    // Create the list of available entries for this training session.
    EntryFilter filter(m_doc, m_parent);
    m_allTestEntries = filter.entries();

//     qDebug() << "Entries: ----------------";
//     qDebug() << "Found " << m_allTestEntries.count() << " entries after filtering.";
//     foreach (TestEntry *entry, m_allTestEntries) {
//         qDebug() << "Entry: " << entry->languageFrom() << entry->entry()->translation(entry->languageFrom())->text()
//                  << "to" << entry->languageTo() << entry->entry()->translation(entry->languageTo())->text();
//     }

    // Create the list actual entries in this training session.  This
    // is a pure virtual function and must be implemented by the
    // concrete session managers.
    initializeTraining();
}

QString SessionManagerBase::title() const
{
    return m_doc->title();
}

void SessionManagerBase::practiceStarted()
{
    qDebug() << "start practice timer";
    m_time.start();
}

void SessionManagerBase::practiceFinished()
{
    m_totalTime = m_time.elapsed();
    qDebug() << "stop practice timer" << m_totalTime;
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
    // time if possible.
    int lastEntry = m_currentEntry;
    if (m_currentEntries.count() > 0) {
        // Choose one of the current entries randomly.
	m_currentEntry = QRandomGenerator::global()->bounded(m_currentEntries.count());

        // Do not allow to ask the same entry twice in a row.
        if (m_currentEntries.count() > 1) {
            while (m_currentEntry == lastEntry) {
                m_currentEntry = QRandomGenerator::global()->bounded(m_currentEntries.count());
            }
        }

        qDebug() << "nextTrainingEntry:"
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
    qDebug() << "Test statistics: ";
    foreach(TestEntry * entry, m_allTestEntries) {
        qDebug()
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
    QList<KEduVocExpression*> allEntries = m_doc->lesson()->entries(KEduVocLesson::Recursive);
    int numValidEntries = 0;
    int count = numberChoices;

    // if the current entry has predefined multiple choice entries defined, use them first
    TestEntry *currentEntry = m_currentEntries.at(m_currentEntry);
    QStringList predefinedChoices = currentEntry->entry()->translation(currentEntry->languageTo())->getMultipleChoice();
    while (!predefinedChoices.isEmpty() && count > 0) {
        choices.append(predefinedChoices.takeAt(QRandomGenerator::global()->bounded(predefinedChoices.count())));
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

            // FIXME: Use trainingmode2 also here!
            if (isValidMultipleChoiceAnswer(exp)) {
                choices.append(exp->translation(currentEntry->languageTo())->text());
            }
        }
    } else {
        QList<KEduVocExpression*> exprlist;
        while (count > 0) {
            int nr;
            // if there are enough non-empty fields, fill the options only with those
            do {
                nr = QRandomGenerator::global()->bounded(allEntries.count());
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

        // FIXME: Use trainingmode2 too here
        for (int i = 0; i < exprlist.count(); i++) {
            choices.append(exprlist[i]->translation(currentEntry->languageTo())->text());
        }
    }

    qDebug() << "choices:" << choices << " answerLang = " << currentEntry->languageTo() ;
    return choices;
}


// ----------------------------------------------------------------
//                         Protected methods


bool SessionManagerBase::isValidMultipleChoiceAnswer(KEduVocExpression *e)
{
    // entry is empty
    if (e->translation(m_learningLanguageIndex)->text().trimmed().isEmpty())
        return false;

    // FIXME: Must test individual solution & question languages per
    // entry in mixed mode training.
    //
    // entry is a synonym of the solution
    if (e->translation(m_learningLanguageIndex)->synonyms().contains(m_currentEntries.at(m_currentEntry)->entry()->translation(m_learningLanguageIndex)))
        return false;

    // Entry has the same text as the solution.
    if (e->translation(m_learningLanguageIndex)->text().simplified()
        == m_currentEntries.at(m_currentEntry)->entry()->translation(m_learningLanguageIndex)->text().simplified())
        return false;

    return true;
}
