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
    if (!bilingualTest())
        m_solutionTestType = m_questionTestType = static_cast<Prefs::EnumTestType::type>(Prefs::questionLanguage());
    else
    {
        m_solutionTestType = static_cast<Prefs::EnumTestType::type>(Prefs::solutionLanguage());
        m_questionTestType = static_cast<Prefs::EnumTestType::type>(Prefs::questionLanguage());
    }
    //m_entry = (PracticeEntry*)0xdeadbeef;
}

PracticeEntryManager::~PracticeEntryManager()
{
    //qDeleteAll(m_entriesOriginal);
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
        emit signalSetFinished();
        return;
    }

    m_entriesRemaining = m_entriesOriginal;

    if (!Prefs::testOrderLesson())
        shuffle();

    m_entry = m_entriesRemaining.first();
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
        case Prefs::EnumTestType::ExampleTest:
        case Prefs::EnumTestType::ParaphraseTest:
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::FlashCardsTest: // this isn't really written, but it doesn't matter.
        case Prefs::EnumTestType::MixedLettersTest:
            return Written;
        case Prefs::EnumTestType::ConjugationTest:
        case Prefs::EnumTestType::ComparisonTest:
            return MultipleData;
        default:
            return Written; // if this happens, we have bigger problems.
    }
}

bool PracticeEntryManager::bilingualTest() const
{
    // if a mode supports two languages, it is bilingual. Otherwise, it is not.
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::MixedLettersTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
        case Prefs::EnumTestType::FlashCardsTest:
            return true;
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::ExampleTest:
        case Prefs::EnumTestType::ParaphraseTest:
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::ComparisonTest:
        case Prefs::EnumTestType::ConjugationTest:
        default:
            return false;
    }
}


void PracticeEntryManager::slotNewFlashcardBack()
{
     if (!m_entry)   return;


        emit signalNewImage( m_entry->expression()->translation(m_solutionTestType)->imageUrl(), true);
        emit signalNewSound( m_entry->expression()->translation(m_solutionTestType)->soundUrl());

        emit signalNewText(m_solution);

}

void PracticeEntryManager::slotNewEntry()
{
    // We need to keep this around, but starting a new class member is overkill.
    if (!m_entriesRemaining.isEmpty())
    {
        m_entry = m_entriesRemaining.takeFirst();
        // if they get it wrong, we remove it from this list later.
        m_entriesFinished.append(m_entry);
        m_solution.clear();
        m_solutions.clear();

        KEduVocTranslation* solution = makeSolution();

        // It doesn't matter if these are empty since we would emit empty KUrls/QStrings anyway
        // if sound/images aren't used in a mode, these connect to nothing and are ignored.
        emit signalNewImage( m_entry->expression()->translation(m_questionTestType)->imageUrl(), false);
        emit signalNewSound( m_entry->expression()->translation(m_questionTestType)->soundUrl());


        switch (testCategory())
        {
            case MultipleData:
                emit signalNewText(m_prompts);
                emit signalNewChoices(makeChoices(solution));
                break;

            // Note the fall-through!
            case MultipleChoice:
                emit signalNewChoices(makeChoices(solution));
            case Written:
                emit signalNewText(currentQuestion());
                break;
        }

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
    KEduVocTranslation * t = m_entry->expression()->translation(m_solutionTestType);
    // This is where you should put any special code that relates to generating the solution.
    // If it needs more than 3 or so lines, make a new function and call it here.
    // Make sure to return t (or the KEduVocTranslation* for the solution, if different)
    //
    // If your solution is only one string, put it in m_solution; if it require multiple strings,
    // put it in m_solutions.
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::SynonymTest:
                m_solution =  t->synonyms()[KRandom::random() % t->synonyms().size()]->text();
                return t->synonyms()[KRandom::random() % t->synonyms().size()];
        case Prefs::EnumTestType::AntonymTest:
                m_solution = t->antonyms()[ KRandom::random() % t->antonyms().size()]->text();
                return t->antonyms()[ KRandom::random() % t->antonyms().size()];
        case Prefs::EnumTestType::ArticleTest:
                m_solution = makeArticleAnswer(t->wordType()->wordType());
                return t;
        // add your mode here if it just needs the text of the solution. (eg written mode).
        case Prefs::EnumTestType::ParaphraseTest:
        case Prefs::EnumTestType::ExampleTest:
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::FlashCardsTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
        case Prefs::EnumTestType::MixedLettersTest:
            m_solution = t->text();
            return t;
        case Prefs::EnumTestType::ComparisonTest:
            m_solutions << t->text();
            m_solutions << t->comparative();
            m_solutions << t->superlative();
            return t;
        case Prefs::EnumTestType::ConjugationTest:
            setConjugationData(t);
            return t;
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
    return m_solutions;
}

QString PracticeEntryManager::currentQuestion() const
{
    // This is where you should put any mode-specific code for generating the question.
    // The return value of this is sent to the question elements in the theme files.
    //
    // If your mode requires multiple strings for the quesiton, use currentQuestions().
    QStringList modified;
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::ExampleTest:
            // This is the logic to blank the solution.
            foreach(QString word, m_entry->expression()->translation(m_questionTestType)->example())
            {
                if (word.contains(m_entry->expression()->translation(m_questionTestType)->text(), Qt::CaseInsensitive))
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
            return m_entry->expression()->translation(m_questionTestType)->paraphrase();
        // if the mode only needs the text of the main entry (eg written), add it here.
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::FlashCardsTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
        case Prefs::EnumTestType::MixedLettersTest:
            return m_entry->expression()->translation(m_questionTestType)->text();
        case Prefs::EnumTestType::ConjugationTest:
        case Prefs::EnumTestType::ComparisonTest:
        default:
            kDebug() << "bad practice mode";
            return QString();
    }
}

QStringList PracticeEntryManager::currentQuestions() const
{
    // Put any mode-specific question generation code here.
    // Use this function for modes that require multiple strings
    // If you only need a single string in your question, use currentQuestion()
    QStringList modified;
    switch (Prefs::testType())
    {
        case Prefs::EnumTestType::ConjugationTest:
            return m_prompts;
        case Prefs::EnumTestType::ComparisonTest:
            return QStringList(); // these modes have no quesiton, per se
        default:
            kDebug() << "bad practice mode";
            return QStringList();
    }
}

QStringList PracticeEntryManager::makeChoices(KEduVocTranslation* solution) const
{
    // If your mode uses the multiple choice template, add it here.
    // If you need more than a few lines of code, make a seperate function.
    switch (Prefs::testType())
    {
        // If you just want to use other random entries as the distractors (wrong answers),
        // add your mode here.
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
            return makeMultipleChoiceChoices(solution);
        case Prefs::EnumTestType::ArticleTest:
            return makeArticleChoices(currentSolution());
        case Prefs::EnumTestType::ComparisonTest:
        case Prefs::EnumTestType::ConjugationTest:
            return currentSolutions();
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
        if (!m_entry->expression()->translation(m_questionTestType)->multipleChoice().isEmpty())
        {
            foreach(s, m_entry->expression()->translation(m_questionTestType)->multipleChoice())
                list.append(s);
        }

    // if we only have a few entries, we use them all.
    if (source.size() < Prefs::numberMultipleChoiceAnswers() - list.size())
    {
       foreach(PracticeEntry* pe, source)
       {
            s = pe->expression()->translation(m_solutionTestType)->text();
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
            t = source[r]->expression()->translation(m_solutionTestType);
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

    KEduVocArticle articles = m_doc->identifier(m_solutionTestType).article();

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
        KEduVocArticle articles = m_doc->identifier(m_solutionTestType).article();

        // these are our degenerate options
        if(articles.isEmpty())
        {
            if (flags & KEduVocWordFlag::Masculine) return i18nc("@label the gender of the word: Masculine", "Masculine");
            if (flags & KEduVocWordFlag::Masculine) return i18nc("@label the gender of the word: Female", "Feminine");
            if (flags & KEduVocWordFlag::Masculine) return i18nc("@label the gender of the word: Neuter", "Neuter");
        }

        // Give them default values.
        if (!(flags & KEduVocWordFlag::genders))
        {
            flags |= KEduVocWordFlag::Neuter;
            kDebug() << "Neuter added.";
        }
        if (!(flags & KEduVocWordFlag::numbers))
        {
            flags |= KEduVocWordFlag::Singular;
            kDebug() << "Singular added";
        }

        QString article;
        QString def = articles.article(flags | KEduVocWordFlag::Definite);
        QString indef = articles.article(flags | KEduVocWordFlag::Indefinite);
        if (!def.isEmpty()) parts.append(def);
        if (!indef.isEmpty()) parts.append(indef);
        return parts.join(" / ");
}


QString PracticeEntryManager::tenseDescription(KEduVocWordFlags flags, const QString& tenseName) const
{
    // first we try pronouns
    KEduVocPersonalPronoun pp = m_doc->identifier(m_solutionTestType).personalPronouns();

    QString pronoun = pp.personalPronoun(flags);
    if (pronoun.isEmpty())
    {
        kDebug() << "failed to find a pronoun.";
        // if its empty, we revert to a generic description
        QStringList desc;
        if (flags & KEduVocWordFlag::First)
            desc << i18n("First person");
        else if (flags & KEduVocWordFlag::Second)
            desc << i18n("Second person");
        else if (flags & KEduVocWordFlag::Third)
            desc << i18n("Third person");

        // do genders ever influence conjugation?
        if (flags & KEduVocWordFlag::Masculine)
            desc << i18nc("Grammatical masculine gender", "Masculine");
        else if (flags & KEduVocWordFlag::Feminine)
            desc << i18nc("Grammatical feminine gender", "Feminine");
        else if (flags & KEduVocWordFlag::Neuter)
            desc << i18nc("Grammatical neuter gender", "Neuter");

        if (flags & KEduVocWordFlag::Singular)
            desc << i18nc("Grammatical singular number", "Singular");
        if (flags & KEduVocWordFlag::Dual)
            desc << i18nc("Grammatical dual number (doesn't exist in all languages)", "Dual");
        if (flags & KEduVocWordFlag::Plural)
            desc << i18nc("Grammatical plural number", "Plural");

        pronoun = desc.join(" ");
    }

    return tenseName + ": " + pronoun;
}


void PracticeEntryManager::setConjugationData(KEduVocTranslation * t)
{
    m_prompts.clear();
    m_solutions.clear();

    if (!(t->wordType()->wordType() & KEduVocWordFlag::Verb))
    {
        kDebug() << "attempting to conjugation non-verb" << t->text();
        return;
    }

    m_prompts << i18nc("Grammatical infinitive of a verb", "Infinitive");
    m_solutions << t->text(); // infinitive

    QStringList tenses = t->conjugationTenses();

    QStringList used;
    int i, n;
    i = n = 0;
    unsigned int r, r2;
    while(i < 3 && !tenses.isEmpty())
    {
        r = KRandom::random() % tenses.size();
        used << tenses.takeAt(r);
        ++i;
    }

    kDebug() << used;

    i = 0;
    QList<KEduVocWordFlags> keys;
    KEduVocWordFlags f;
    QString s;
    while(i < 3 && n < 50)
    {
        r = KRandom::random() % used.size();
        keys = t->conjugation(used[r]).keys();
        if (!keys.size()) continue;

        r2 = KRandom::random() % keys.size();
        f = keys[r2];
        s = t->conjugation(used[r]).conjugation(f).text();
        if (!m_solutions.contains(s))
        {
            m_prompts << tenseDescription(f, used[r]);
            m_solutions << s;
            ++i;
        }
        ++n;
    }
}
