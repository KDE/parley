/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "answervalidator.h"

#include "prefs.h"

#include <keduvocidentifier.h>
#include <keduvocexpression.h>
#include <keduvoctranslation.h>
#include <keduvocdocument.h>

#include <KLocalizedString>
#include <sonnet/speller.h>
#include <KDebug>

/// temporary namespace for string manipulation functions
/// could move into KStringHandler eventually

namespace ParleyStringHandler
{


QString stripAccents(const QString& original)
{
    QString noAccents;
    QString decomposed = original.normalized(QString::NormalizationForm_D);

    for (int i = 0; i < decomposed.length(); ++i)
    {
        if (decomposed[i].category() != 1)
        {
            noAccents.append(decomposed[i]);
        }
    }

    kDebug() << original << " without accents: " << noAccents;

    return noAccents;
}
}


const double AnswerValidator::LEVENSHTEIN_THRESHOLD = 0.2;
const double AnswerValidator::UNRELATED_WORD_GRADE = 0.0;
const double AnswerValidator::FALSE_FRIEND_GRADE = 0.0;
const double AnswerValidator::SPELLING_MISTAKE_PER_LETTER_PUNISHMENT = 0.2;
const double AnswerValidator::CAPITALIZATION_MISTAKE_PUNISHMENT = 0.1;
const double AnswerValidator::ACCENT_MISTAKE_PUNISHMENT = 0.1;
const double AnswerValidator::WRONG_ARTICLE_PUNISHMENT = 0.1;

AnswerValidator::AnswerValidator(KEduVocDocument* doc, QObject * parent)
    : QObject(parent)
{
    m_doc = doc;
    m_entry = 0;
    m_speller = 0;
    m_spellerAvailable = false;
}

AnswerValidator::~AnswerValidator()
{
    delete m_speller;
}


void AnswerValidator::setLanguage(int translation)
{
    m_translation = translation;

    // default: try locale

    if (!m_speller)
    {
        m_speller = new Sonnet::Speller(m_doc->identifier(translation).locale());
    }
    else
    {
        m_speller->setLanguage(m_doc->identifier(translation).locale());
    }

    // we might succeed with language name instead.
    if (!m_speller->isValid())
    {
        m_speller->setLanguage(m_doc->identifier(translation).name());
    }

    if (!m_speller->isValid())
    {
        kDebug() << "No spellchecker for current language found: " << m_doc->identifier(m_translation).locale();
        kDebug() << "Available dictionaries: " << m_speller->availableLanguages()
        << "\n names: " << m_speller->availableLanguageNames()
        << "\n backends: " << m_speller->availableBackends();
        m_spellerAvailable = false;
    }
    else
    {
        m_spellerAvailable = true;
    }
}


void AnswerValidator::slotSetExpression(KEduVocExpression * entry)
{
    m_entry = entry;

    if (m_entry)
    {
        m_solution = m_entry->translation(m_translation)->text();
    }
}

int AnswerValidator::levenshteinDistance(const QString& s, const QString& t)
{
    int m = s.length();
    int n = t.length();

    int dWidth = m + 1 ;

    // make sure the matrix is big enough

    if (m_d.size() < (m + 1) * (n + 1))
    {
        m_d.resize((m + 1) * (n + 1));
    }

    int i;

    int j;

    // init 0..m, 0..n as starting values - distance to ""

    for (i = 0; i <= m; i++)
    {
        m_d[i + 0*dWidth] = i;
    }

    for (j = 0; j <= n; j++)
    {
        m_d[0 + j*dWidth] = j;
    }

    int cost;

    for (i = 1; i <= m; i++)
    {
        for (j = 1; j <= n; j++)
        {
            if (s[i-1] == t[j-1])
            {
                // if current char is equal, no cost for substitution
                cost = 0;
            }
            else
            {
                cost = 1;
            }

            m_d[i + j*dWidth] = qMin(qMin(   // min of three possibilities

                                         m_d[i-1 + (j)*dWidth] + 1,       // deletion
                                         m_d[i   + (j-1)*dWidth] + 1),     // insertion
                                     m_d[i-1 + (j-1)*dWidth] + cost);   // substitution
        }
    }

    return m_d[m + n*dWidth];
}

bool AnswerValidator::spellcheckerMisspelled(const QString& userAnswer)
{
    if (!m_spellerAvailable)
    {
        return true;
    }

    return m_speller->isMisspelled(userAnswer);
}

bool AnswerValidator::spellcheckerInSuggestionList(const QString& solution, const QString& userAnswer)
{
    if (!m_spellerAvailable)
    {
        return false;
    }

    kDebug() << "entered: " << userAnswer << " misspelled: " << m_speller->isMisspelled(userAnswer) << " suggestions: " << m_speller->suggest(userAnswer);

    if (m_speller->suggest(userAnswer).contains(solution))
    {
        kDebug() << "I think this is a spelling error.";
        return true;
    }
    else
    {
        kDebug() << "No, this is a different word I think.";
        return false;
    }
}


void AnswerValidator::simpleCorrector()
{
    kDebug() << "simpleCorrector";

    if (m_entry == 0)
    {
        kError() << "No entry set, cannot verify answer.";
        return;
    }

    ///@todo can solution.length() be zero? *should* be caught by Parley
    if (m_solution == m_userAnswer)
    {
        emit signalCorrection(1.0, Statistics::Correct, m_userAnswer);
        return;
    }
/*
    // TODO we should probably use a corrector here, but there is a chance it would cause infinite recursion
    if ( m_entry->translation(m_translation)->synonym() == m_userAnswer ) {
        if ( Prefs::countSynonymsAsCorrect() ) {
            // synonym, good for you
            emit signalCorrection(1.0, (Statistics::ErrorType) Statistics::Correct | Statistics::Synonym, m_userAnswer);
        } else {
            // it is the synonym but we don't accept it
            emit signalCorrection(0.0, Statistics::Synonym, m_userAnswer); // bit harsh maybe
        }
        return;
    }*/


    int levensthein = levenshteinDistance(m_solution, m_userAnswer);

    emit signalCorrection(1.0 - ((double)levensthein / qMax(m_solution.length(), m_userAnswer.length())), Statistics::UnknownMistake, m_userAnswer);

}


void AnswerValidator::defaultCorrector()
{
    ///@todo does not work completely yet.
    ///@todo can solution.length() be zero? *should* be caught by Parley
    if (m_solution == m_userAnswer)
    {
        emit signalCorrection(1.0, Statistics::Correct, m_userAnswer);
        return;
    }

    if (m_userAnswer.isEmpty())
    {
        emit signalCorrection(1.0, Statistics::Empty, m_userAnswer);
        return;
    }
/*
    // TODO we should probably use a corrector here, but there is a chance it would cause infinite recursion
    if ( m_entry->translation(m_translation)->synonym() == m_userAnswer ) {
        if ( Prefs::countSynonymsAsCorrect() ) {
            // synonym, good for you
            emit signalCorrection(1.0, (Statistics::ErrorType) Statistics::Correct | Statistics::Synonym, m_userAnswer);
        } else {
            // it is the synonym but we don't accept it
            emit signalCorrection(0.0, Statistics::Synonym, m_userAnswer); // bit harsh maybe
        }
        return;
    }*/

    int numberSolutionWords = m_solution.simplified().split(" ").count();

    int numberAnswerWords = m_userAnswer.simplified().split(" ").count();

    if (numberSolutionWords == 1)
    {
        double grade;
        Statistics::ErrorType error;
        wordCompare(m_solution, m_userAnswer, grade, error);
        emit signalCorrection(grade, error, m_userAnswer);
        return;
    }

    if (numberSolutionWords == 2)
    {
        // could be noun + article
        QStringList solutionWords = m_solution.simplified().split(" ");

        if (m_translation >= 0)
        {
            if (m_doc->identifier(m_translation).article().isArticle(solutionWords.value(0)))
            {
                // yes, the answer is an article + noun
                if (numberAnswerWords == 1)
                {
                    double percent;
                    Statistics::ErrorType errors;
                    wordCompare(solutionWords.value(1), m_userAnswer.simplified(), percent, errors);
                    errors = static_cast<Statistics::ErrorType>((int)errors | Statistics::ArticleMissing);
                    emit signalCorrection(qMax(percent - WRONG_ARTICLE_PUNISHMENT, 0.0), errors, m_userAnswer);
                    return;
                }

                if (numberAnswerWords == 2)
                {
                    double grade;
                    Statistics::ErrorType errors;
                    wordCompare(solutionWords.value(1), m_userAnswer.simplified().split(" ").value(1), grade, errors);

                    if (m_userAnswer.simplified().split(" ").value(0) == solutionWords.value(0))
                    {
                        emit signalCorrection(grade, errors, m_userAnswer);
                        return;
                    }
                    else
                    {
                       emit signalCorrection(qMax(grade - WRONG_ARTICLE_PUNISHMENT, 0.0), static_cast<Statistics::ErrorType>((int)errors | Statistics::ArticleWrong), m_userAnswer);
                       return;
                    }
                }
            }
        }
    }

    // ok, more than one word (or one+article)
    sentenceAnalysis();
}

void AnswerValidator::checkUserAnswer(const QString & solution, const QString & userAnswer)
{
    m_solution = solution;
    m_userAnswer = userAnswer;

    defaultCorrector();
}


void AnswerValidator::wordCompare(const QString & solution, const QString & userWord, double& grade, Statistics::ErrorType& ErrorType)
{
    ///@todo add to other errors... ?

    // nothing to be done here if it's right
    if (solution == userWord)
    {
        grade = 1.0;
        ErrorType = Statistics::Correct;
        return;
    }

    if (solution.toLower() == userWord.toLower())
    {
        grade = 1.0 - CAPITALIZATION_MISTAKE_PUNISHMENT;
        ErrorType = Statistics::CapitalizationMistake;
        return ;
    }

    if (ParleyStringHandler::stripAccents(solution) ==    ParleyStringHandler::stripAccents(userWord))
    {
        grade = 1.0 - ACCENT_MISTAKE_PUNISHMENT;
        ErrorType = Statistics::AccentMistake;
        return ;
    }

    int levenshtein = levenshteinDistance(solution, userWord);

    if (m_spellerAvailable)
    {
        bool inSuggestions = false;
        bool isMisspelled = m_speller->isMisspelled(userWord);

        if (m_speller->suggest(userWord).contains(solution))
        {
            inSuggestions = true;
        }

        // probably misspelled
        if (isMisspelled && inSuggestions)
        {
            grade = 1.0 - qMax(levenshtein * SPELLING_MISTAKE_PER_LETTER_PUNISHMENT, 1.0);
            ErrorType = Statistics::SpellingMistake;
            return;
        }

        // this is a different word but sounds similar!
        if (!isMisspelled && inSuggestions)
        {
            grade = FALSE_FRIEND_GRADE;
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">NOOOO! That was a false friend!</font> ");
            ErrorType = ErrorType = Statistics::FalseFriend;
            return ;
        }

        // unrelated word
        if (!isMisspelled && !inSuggestions)
        {
            grade = UNRELATED_WORD_GRADE;
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">Do you have any idea what you are talking about? (Wrong word, you spelled it correct I guess.)</font> ");
            ErrorType = Statistics::UnrelatedWord;
            return;
        }

        // complete nonsense, unless levenshtein comes to the rescue
        if (isMisspelled && !inSuggestions)
        {
            if (((double)levenshtein / qMax(solution.length(), userWord.length())) < LEVENSHTEIN_THRESHOLD)
            {
//                 htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">Seems like you got the spellig wrong.</font> ");
                ErrorType = Statistics::SpellingMistake;
                return;
            }
            else
            {
//                 htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">I don't know that word and it is not similar to the solution.</font> ");
                ErrorType = Statistics::UnknownMistake;
                return;
            }
        }
    }
    else
    {
        if (((double)levenshtein / qMax(solution.length(), userWord.length())) < LEVENSHTEIN_THRESHOLD)
        {
            grade = 1.0 - ((double)levenshtein / qMax(solution.length(), userWord.length()));
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No spellchecker, but seems like a spelling error.</font> ");
            ErrorType = Statistics::SpellingMistake;
            return;
        }
        else
        {
            grade = 1.0 - ((double)levenshtein / qMax(solution.length(), userWord.length()));
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No dictionary and no clue.</font> ");
            ErrorType = Statistics::UnknownMistake;
            return;
        }
    }

    // cannot get here
//     htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No dictionary and no clue.</font> ");
    ErrorType = Statistics::UnknownMistake;

    return;
}


void AnswerValidator::sentenceAnalysis()
{
    QStringList solutionWords;
    QStringList userAnswerWords;

    // filter !?.,;:¿ etc and throw them away
    solutionWords = m_solution.simplified().split(QRegExp("\\W"), QString::SkipEmptyParts);
    userAnswerWords = m_userAnswer.simplified().split(QRegExp("\\W"), QString::SkipEmptyParts);
    kDebug() << "Solution words: " << solutionWords;
    kDebug() << "Answer: " << userAnswerWords;

    QStringList correctWords;
    QStringList wrongWords;

    for (int i = 0; i < userAnswerWords.count(); i++)
    {
        int pos = solutionWords.indexOf(userAnswerWords.value(i));

        if (pos >= 0)
        {
            correctWords.append(userAnswerWords.value(i));
            solutionWords.removeAt(pos);
            userAnswerWords.removeAt(i);
        }
        else
        {
            wrongWords.append(userAnswerWords.value(i));
        }
    }

    kDebug() << " remaining: solution: " << solutionWords.count()

    << "user: " << userAnswerWords.count();

    QList< QPair<QString, QString> > pairs = bestPairs(solutionWords, userAnswerWords);

    for (int i = 0; i < pairs.count(); i++)
    {
        kDebug() << "Possible pair: " << pairs.value(i).first << " and " << pairs.value(i).second;
    }


    QString correction;

    correction.append("Correct: ");
    foreach(const QString &correctWord, correctWords)
    {
        correction.append(QString::fromLatin1("<font color=\"#188C18\">") + correctWord + QString::fromLatin1("</font> "));
    }

    correction.append(" Wrong: ");
    foreach(const QString &wrongWord, wrongWords)
    {
        correction.append(QString::fromLatin1("<font color=\"#8C1818\">") + wrongWord + QString::fromLatin1("</font> "));
    }

    int levenshtein = levenshteinDistance(m_solution, m_userAnswer);

    kDebug() << correction;
    kDebug() << "IMPLEMENT ME TO ACTUALLY EVALUATE THE ABOVE AND GENERATE A GRADE!";
    emit signalCorrection(1.0 - ((double)levenshtein / qMax(m_solution.length(), m_userAnswer.length())),Statistics::UnknownMistake, m_userAnswer);
}


QList< QPair < QString , QString > > AnswerValidator::bestPairs(const QStringList& solutionWords , const QStringList& userAnswerWords)
{
    int nSol = solutionWords.count();
    int nUser = userAnswerWords.count();

    QByteArray d;
    d.resize(nSol * nUser);

    QList< QPair < QString , QString > > pairList;

    // matrix of levenshteinDistances

    for (int i = 0; i < nSol; i++)
    {
        for (int j = 0; j < nUser; j++)
        {
            d[i + nSol*j] = levenshteinDistance(solutionWords.value(i), userAnswerWords.value(j));
        }
    }

    int MAX_LEVENSHTEIN = 5;

    // check if another pair is possible
    int min;
    int posSol = -1;
    int posUser = -1;

    do
    {
        min = MAX_LEVENSHTEIN;

        for (int i = 0; i < nSol; i++)
        {
            for (int j = 0; j < nUser; j++)
            {
                if (d.at(i + j*nSol) < min)
                {
                    min = d.at(i + j * nSol);
                    posSol = i;
                    posUser = j;
                }
            }
        }

        if (min < MAX_LEVENSHTEIN && posUser != -1 && posSol != -1)
        {
            pairList.append(qMakePair(solutionWords.value(posSol), userAnswerWords.value(posUser)));

            // taken
            d[posSol + posUser*nSol] = MAX_LEVENSHTEIN;
        }
    }
    while (min < MAX_LEVENSHTEIN);

    return pairList;
}


bool AnswerValidator::spellcheckerAvailable()
{
    return m_spellerAvailable;
}
