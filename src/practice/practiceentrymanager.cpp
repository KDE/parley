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

PracticeEntryManager::PracticeEntryManager(QObject * parent)
        : QObject(parent),
        m_iter(QList<PracticeEntry*>()) // it has no empty constructor. Annoying...
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

    m_iter = m_entries;
    if (!Prefs::testOrderLesson())
        shuffle();
}

const QString PracticeEntryManager::currentSolution() const
{
    switch (Prefs::testType())
    {
        // Implement special crap here.
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::ComparisonTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::ConjugationTest:
            return "IMPLEMENT SOLUTION LOGIC";
            // TODO Implement logic here.

       // All other modes are normal and just use the text() field.
       default:
        return m_entry->expression()->translation(Prefs::solutionLanguage())->text();
    }
}

void PracticeEntryManager::appendEntryToList(PracticeEntry* entry)
{
    m_entries.append(entry);
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
    KRandomSequence(0).randomize(m_entries);
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
       // All other modes are normal and just use the text() field.
       default:
        return question->text();
    }
}

void PracticeEntryManager::slotNewEntry()
{
    if (m_iter.hasNext())
    {
        m_entry = m_iter.next();
        KEduVocTranslation * original = m_entry->expression()->translation(Prefs::questionLanguage());
        kDebug() << original->text();

        // It doesn't matter if these are empty since we would emit empty KUrls/QStrings anyway
        emit signalNewText(findTextForCurrentMode(original));

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
