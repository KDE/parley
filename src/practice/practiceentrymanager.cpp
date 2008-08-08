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
#include <KRandom>
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
    //m_entry = (PracticeEntry*)0xdeadbeef;
}

PracticeEntryManager::~PracticeEntryManager()
{
    qDeleteAll(m_entriesOriginal);
}

void PracticeEntryManager::filterTestEntries()
{
    EntryFilter filter(this, m_doc);
    QList<KEduVocExpression*> tempList = filter.entries();
    foreach(KEduVocExpression* expr, tempList)
    {
        if (expr)
            m_entriesOriginal.append(new PracticeEntry(expr));
    }
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
    if (m_entriesOriginal.count() == 0) {
        kDebug() << "No entries selected -- aborting";
        return;
    }

    m_entriesRemaining = m_entriesOriginal;

    if (!Prefs::testOrderLesson())
        shuffle();
}


void PracticeEntryManager::appendEntryToList(PracticeEntry* entry)
{
    m_entriesRemaining.append(entry);
    m_entriesFinished.removeAll(entry); // to avoid duplicate entries.
}

int PracticeEntryManager::totalEntryCount() const
{
    return m_entriesOriginal.count();
}

int PracticeEntryManager::activeEntryCount() const
{
    return m_entriesRemaining.size();
}


void PracticeEntryManager::shuffle()
{
    KRandomSequence().randomize(m_entriesRemaining);
}

PracticeEntryManager::TestCategory PracticeEntryManager::testCategory() const
{
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
            return MultipleChoice;
        case Prefs::EnumTestType::ConjugationTest:
        case Prefs::EnumTestType::ExampleTest:
        case Prefs::EnumTestType::ParaphraseTest:
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::FlashCardsTest: // this isn't really written, but it doesn't matter.
        case Prefs::EnumTestType::MixedLettersTest:
            return Written;
        case Prefs::EnumTestType::ComparisonTest:
            return MultipleChoiceWithoutPrompt;
        default:
            return Written; // if this happens, we have bigger problems.
    }
}

void PracticeEntryManager::slotNewEntry()
{
    if (!m_entriesRemaining.isEmpty())
    {
        m_entry = m_entriesRemaining.takeFirst();
        // if they get it wrong, we remove it from this list later.
        m_entriesFinished.append(m_entry);
        KEduVocTranslation * original = m_entry->expression()->translation(Prefs::questionLanguage());
        kDebug() << original->text();
        KEduVocTranslation* solution = makeSolution();

        switch (testCategory())
        {
            case MultipleChoiceWithoutPrompt:
                emit signalNewChoices(makeChoices(solution));
                break;

            // Note the fall-through!
            case MultipleChoice:
                emit signalNewChoices(makeChoices(solution));
            case Written:
                emit signalNewText(currentQuestion());
                break;
        }


        // It doesn't matter if these are empty since we would emit empty KUrls/QStrings anyway
        // if sound/images aren't used in a mode, these connect to nothing and are ignored.
        emit signalNewImage(original->imageUrl());
        emit signalNewSound(original->soundUrl());

        emit signalNewEntry(m_entry);

        // nobody is listening in the modes this doesn't work on.
        emit signalNewSolution(currentSolution());
    }
    else
    {
        emit signalSetFinished();
        kDebug() << "finished";
    }
}



KEduVocTranslation * PracticeEntryManager::makeSolution()
{
    QStringList l;
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::SynonymTest:
                m_solution =  m_entry->expression()->translation(Prefs::solutionLanguage())->synonyms()[KRandom::random() % m_entry->expression()->translation(Prefs::solutionLanguage())->synonyms().size()]->text();
                return m_entry->expression()->translation(Prefs::solutionLanguage())->synonyms()[KRandom::random() % m_entry->expression()->translation(Prefs::solutionLanguage())->synonyms().size()];
        case Prefs::EnumTestType::AntonymTest:
                m_solution = m_entry->expression()->translation(Prefs::solutionLanguage())->antonyms()[ KRandom::random() % m_entry->expression()->translation(Prefs::solutionLanguage())->antonyms().size()]->text();
                return m_entry->expression()->translation(Prefs::solutionLanguage())->antonyms()[ KRandom::random() % m_entry->expression()->translation(Prefs::solutionLanguage())->antonyms().size()];
        case Prefs::EnumTestType::ArticleTest:
                m_solution = makeArticleAnswer(m_entry->expression()->translation(Prefs::solutionLanguage())->wordType()->wordType());
                return m_entry->expression()->translation(Prefs::solutionLanguage());
        case Prefs::EnumTestType::ParaphraseTest:
        case Prefs::EnumTestType::ExampleTest:
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::FlashCardsTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
        case Prefs::EnumTestType::MixedLettersTest:
            m_solution = m_entry->expression()->translation(Prefs::solutionLanguage())->text();
            return m_entry->expression()->translation(Prefs::solutionLanguage());
        default:
            kDebug() << "bad practice mode";
            return 0;
    }
}

QString PracticeEntryManager::currentSolution() const
{
    return m_solution;
}

QStringList PracticeEntryManager::currentSolutions() const
{
    QStringList l;
    // we ignore newEntry in this case -- there is no need of it.
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::ComparisonTest:
            l << m_entry->expression()->translation(Prefs::solutionLanguage())->text();
            l << m_entry->expression()->translation(Prefs::solutionLanguage())->comparative();
            l << m_entry->expression()->translation(Prefs::solutionLanguage())->superlative();
            return l;
        case Prefs::EnumTestType::ConjugationTest:
            // TODO Implement logic here.
            kDebug() << "IMPLEMENT SOLUTION LOGIC";
            return l;
        default:
            kDebug() << "bad practice mode";
            return QStringList();
    }
}

QString PracticeEntryManager::currentQuestion() const
{
    QStringList modified;
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::ExampleTest:
            // This is the logic to blank the solution.
            foreach(QString word, m_entry->expression()->translation(Prefs::questionLanguage())->example())
            {
                if (word.contains(m_entry->expression()->translation(Prefs::questionLanguage())->text(), Qt::CaseInsensitive))
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
        case Prefs::EnumTestType::ParaphraseTest:
            return m_entry->expression()->translation(Prefs::questionLanguage())->paraphrase();
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::FlashCardsTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
        case Prefs::EnumTestType::MixedLettersTest:
            return m_entry->expression()->translation(Prefs::questionLanguage())->text();
        case Prefs::EnumTestType::ConjugationTest:
            return QString(); // TODO do this logic
        case Prefs::EnumTestType::ComparisonTest:
            return QString(); // this mode has no prompt, per se
        default:
            kDebug() << "bad practice mode";
            return QString();
    }
}

QStringList PracticeEntryManager::makeChoices(KEduVocTranslation* solution) const
{
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
            return makeMultipleChoiceChoices(solution);
        case Prefs::EnumTestType::ArticleTest:
            return makeArticleChoices(currentSolution());
        case Prefs::EnumTestType::ComparisonTest:
            return currentSolutions(); // no special code needed.
        default:
            kDebug() << "bad practice mode";
            return QStringList();
    }
}

QStringList PracticeEntryManager::makeMultipleChoiceChoices(KEduVocTranslation* solution) const
{
    QStringList list;
    QString s;
    int timeout = 0;
    long r;

    QList<PracticeEntry*> source = m_entriesOriginal;

    list.append(solution->text());

    if (Prefs::testType() == Prefs::EnumTestType::MultipleChoiceTest)
        if (!m_entry->expression()->translation(Prefs::questionLanguage())->multipleChoice().isEmpty())
        {
            foreach(s, m_entry->expression()->translation(Prefs::questionLanguage())->multipleChoice())
                list.append(s);
        }

    // if we only have a few entries, we use them all.
    if (source.size() < Prefs::numberMultipleChoiceAnswers() - list.size())
    {
       foreach(PracticeEntry* pe, source)
       {
            s = pe->expression()->translation(Prefs::solutionLanguage())->text();
            if (!list.contains(s))
                list.append(s);
       }
    }

    else
    {
        KEduVocWordFlags cwt = KEduVocWordFlag::NoInformation;
        // if word type is not set, it could have a null pointer exception.
        if (solution->wordType())
            KEduVocWordFlags cwt = solution->wordType()->wordType();
        // filter everything that isn't a part of speech flag.
        cwt &= KEduVocWordFlag::partsOfSpeech;
        KEduVocWordFlags wt = KEduVocWordFlag::NoInformation;

        KEduVocTranslation * t;
        while (list.size() < Prefs::numberMultipleChoiceAnswers() && timeout < 50) // prevent infinite loop
        {
            ++timeout;
            r = KRandom::random() % source.size();
            t = source[r]->expression()->translation(Prefs::solutionLanguage());
            s = t->text();
            if (!list.contains(s))
            {
                if (Prefs::testType() == Prefs::EnumTestType::AntonymTest && solution->antonyms().contains(t))
                    continue;
                if (Prefs::testType() == Prefs::EnumTestType::SynonymTest && solution->synonyms().contains(t))
                    continue;

                if (Prefs::multipleChoiceWordTypeConsistancy() && cwt)
                {
                    if (t->wordType())
                        wt = t->wordType()->wordType();
                    else
                        wt = KEduVocWordFlag::NoInformation;
                    wt &= KEduVocWordFlag::partsOfSpeech;
                    if (wt == cwt)
                        list.append(s);
               }
               else
                    list.append(s);
            }
        }
    }
    KRandomSequence(0).randomize(list);

    return list;
}

QStringList PracticeEntryManager::makeArticleChoices(const QString& solution) const
{
    static QStringList list = QStringList();
    QString def, indef;
    QStringList parts;

    KEduVocArticle articles = m_doc->identifier(Prefs::solutionLanguage()).article();

    // avoid regeneration every question.
    if (list.isEmpty())
    {
        // set the choices
        if(articles.isEmpty())
        {
            list.append(i18nc("@label the gender of the word: Masculine", "Masculine"));
            list.append(i18nc("@label the gender of the word: Female", "Feminine"));
            list.append(i18nc("@label the gender of the word: Neuter", "Neuter"));
        }
        else
        {
            QString answer;
            answer = makeArticleAnswer(KEduVocWordFlag::Singular | KEduVocWordFlag::Masculine);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Singular |KEduVocWordFlag::Feminine);
            if (!answer.isEmpty() && !list.contains(answer)) // the contains makes sure there are no dupes
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Singular | KEduVocWordFlag::Neuter);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Plural | KEduVocWordFlag::Masculine);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Plural | KEduVocWordFlag::Feminine);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Plural | KEduVocWordFlag::Neuter);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Dual | KEduVocWordFlag::Masculine);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Dual | KEduVocWordFlag::Feminine);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);


            answer = makeArticleAnswer(KEduVocWordFlag::Dual | KEduVocWordFlag::Neuter);
            if (!answer.isEmpty() && !list.contains(answer))
                list.append(answer);
        }
    }

    QStringList tempList = list;

    // we don't want any dupes
    if (tempList.contains(solution))
        tempList.removeAll(solution);

    while (tempList.size() > Prefs::numberMultipleChoiceAnswers() - 1)
    {
       tempList.removeAt(KRandom::random() % tempList.size());
    }

    tempList.append(solution);

    return tempList;
}

QString PracticeEntryManager::makeArticleAnswer(KEduVocWordFlags flags) const
{
        QStringList parts;
        KEduVocArticle articles = m_doc->identifier(Prefs::solutionLanguage()).article();

        QString article;
        QString def = articles.article(flags | KEduVocWordFlag::Definite);
        QString indef = articles.article(flags | KEduVocWordFlag::Indefinite);
        if (!def.isEmpty()) parts.append(def);
        if (!indef.isEmpty()) parts.append(indef);
        return parts.join(" / ");
}
