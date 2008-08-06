//
// C++ Implementation: testentrymanager
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

 /***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <KRandomSequence>
#include <KMessageBox>
#include <KApplication>
#include <KDebug>

#include "prefs.h"
#include "practiceold/entryfilter.h"

#include "practiceentrymanager.h"
#include "practiceentry.h"

#include <keduvocwordtype.h>

PracticeEntryManager::PracticeEntryManager(QObject * parent)
        : QObject(parent)
{
    m_fromTranslation = Prefs::questionLanguage();
    m_toTranslation = Prefs::solutionLanguage();
    m_testType = Prefs::testType();
}

PracticeEntryManager::~PracticeEntryManager()
{
    qDeleteAll(m_entries);
}

void PracticeEntryManager::filterTestEntries()
{
    EntryFilter filter(this, m_doc);
    QList<KEduVocExpression*> tempList = filter.entries();
    foreach(KEduVocExpression* expr, tempList)
        m_entries.append(new PracticeEntry(expr));
    kDebug() << m_entries;
}

void PracticeEntryManager::open(KEduVocDocument* doc)
{
    m_doc = doc;

    // should not happen
    if (!m_doc) {
        kDebug() << "No document -- aborting.";
        return;
    }

    filterTestEntries();
    if (m_entries.count() == 0) {
        kDebug() << "No entries selected -- aborting";
        return;
    }

    if (!Prefs::testOrderLesson())
        shuffle();
}

const QString PracticeEntryManager::currentSolution() const
{
    QStringList l;
    switch (Prefs::testType())
    {
        // Implement special crap here.
        case Prefs::EnumTestType::ArticleTest:
            l.append(m_doc->identifier(Prefs::solutionLanguage()).article().article(m_entry->expression()->translation(Prefs::solutionLanguage())->wordType()->wordType() | KEduVocWordFlag::Definite));
            l.append(m_doc->identifier(Prefs::solutionLanguage()).article().article(m_entry->expression()->translation(Prefs::solutionLanguage())->wordType()->wordType() | KEduVocWordFlag::Indefinite));
            return l.join(" / ");
            break;
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::SynonymTest:
            return "IMPLEMENT SOLUTION LOGIC";
            // TODO Implement logic here.

       // All other modes are normal and just use the text() field.
       default:
        return m_entry->expression()->translation(Prefs::solutionLanguage())->text();
    }
}

const QStringList PracticeEntryManager::currentSolutions() const
{
    QStringList l;
    switch (Prefs::testType())
    {
        // Implement special crap here.
        case Prefs::EnumTestType::ComparisonTest:
            l << m_entry->expression()->translation(Prefs::solutionLanguage())->text();
            l << m_entry->expression()->translation(Prefs::solutionLanguage())->comparative();
            l << m_entry->expression()->translation(Prefs::solutionLanguage())->superlative();
            return l;
            break;
        case Prefs::EnumTestType::ConjugationTest:
            kDebug() << "IMPLEMENT SOLUTION LOGIC";
            return l;
            // TODO Implement logic here.
        default:
            kDebug() << "bad practice mode";
            return l;
    }
    return l;
}


void PracticeEntryManager::appendEntryToList(PracticeEntry* entry)
{
    m_entries.append(entry);
    m_finishedEntries.removeAll(entry); // to avoid duplicate entries.
    // when this entry was emitted, this was decremented, so now that we
    // get it back, we'll increment to stay even.
    ++m_numberEntriesRemaining;
}

// We append incorrect entries to the end of the list, so taking the
// current size each time wouldn't be what we want.
// So we'll only use the original size.
int PracticeEntryManager::totalEntryCount() const
{
    static int original_size = m_entries.count();
    return original_size;
}

int PracticeEntryManager::activeEntryCount() const
{
    return m_numberEntriesRemaining;
}


void PracticeEntryManager::shuffle()
{
    KRandomSequence().randomize(m_entries);
}

bool PracticeEntryManager::isMultipleDataTestType(Prefs::EnumTestType::type type)
{
    switch (type)
    {
        // this is likely to grow.
       case Prefs::EnumTestType::ComparisonTest:
           return true;
       default:
            return false;
    }
}

QStringList PracticeEntryManager::findTextListForCurrentMode(KEduVocTranslation* question)
{
    QStringList result;
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::ComparisonTest:
            result.append(question->text());
            result.append(question->comparative());
            result.append(question->superlative());
            break;
        default:
            kDebug() << "unhandled mode";
            break;
    }
    return result;
}

QString PracticeEntryManager::findTextForCurrentMode(KEduVocTranslation* question)
{
    QStringList modified;
    switch (Prefs::testType())
    {
        // Use the paraphrase field.
       case Prefs::EnumTestType::ParaphraseTest:
        kDebug() << question->paraphrase();
        return question->paraphrase();

       // Use the example field, but blank out the solution.
       case Prefs::EnumTestType::ExampleTest:
        // This is the logic to blank the solution.
        foreach(QString word, question->example().split(" "))
        {
            if (word.contains(question->text(), Qt::CaseInsensitive))
            {
                modified.append("<font color=\"#ff0000\">");
                modified.append("...");
                modified.append("</font>");
            }
            else
                modified.append(word);
        }
        kDebug() << modified.join(" ");
        return modified.join(" ");
        break;
       // All other single-datum modes are normal and just use the text() field.
       default:
        return question->text();
    }
}

void PracticeEntryManager::slotNewEntry()
{
    kDebug() << m_entries;
    if (!m_entries.isEmpty())
    {
        m_entry = m_entries.takeFirst();
        m_finishedEntries.append(m_entry);
        KEduVocTranslation * original = m_entry->expression()->translation(Prefs::questionLanguage());
        kDebug() << original->text();


        if (isMultipleDataTestType(Prefs::testType()))
        {
            kDebug() << "multi";
            emit signalNewTextList(findTextListForCurrentMode(original));
        }
        else
        {
            kDebug() << "single";
            emit signalNewText(findTextForCurrentMode(original));
        }


        // It doesn't matter if these are empty since we would emit empty KUrls/QStrings anyway
        // if sound/images are disabled, these connect to nothing and are ignored.
        emit signalNewImage(original->imageUrl());
        emit signalNewSound(original->soundUrl());

        emit signalNewSolution(m_entry->expression()->translation(Prefs::solutionLanguage())->text());

        emit signalEntryChanged(m_entry, m_entries);
        emit signalNewEntry();
        --m_numberEntriesRemaining;
    }
    else
    {
        emit signalSetFinished();
        kDebug() << "finished";
    }
}
