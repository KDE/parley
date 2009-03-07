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
#include "answervalidatorold.h"

#include "prefs.h"

#include <keduvocexpression.h>
#include <keduvocdocument.h>
#include <keduvocidentifier.h>

#include <KLocalizedString>
#include <sonnet/speller.h>
#include <kdebug.h>

/// temporary namespace for string manipulation functions
/// could move into KStringHandler eventually
namespace ParleyStringHandlerOld {
    QString stripAccents(const QString& original){
        QString noAccents;
        QString decomposed = original.normalized(QString::NormalizationForm_D);
        for (int i = 0; i < decomposed.length(); ++i) {
            if ( decomposed[i].category() != 1 ) {
                noAccents.append(decomposed[i]);
            }
        }
kDebug() << original << " without accents: " << noAccents;
        return noAccents;
    }
}


const double AnswerValidatorOld::LEVENSHTEIN_THRESHOLD = 0.2;
const double AnswerValidatorOld::UNRELATED_WORD_GRADE = 0.0;
const double AnswerValidatorOld::FALSE_FRIEND_GRADE = 0.0;
const double AnswerValidatorOld::SPELLING_MISTAKE_PER_LETTER_PUNISHMENT = 0.2;
const double AnswerValidatorOld::CAPITALIZATION_MISTAKE_PUNISHMENT = 0.1;
const double AnswerValidatorOld::ACCENT_MISTAKE_PUNISHMENT = 0.1;
const double AnswerValidatorOld::WRONG_ARTICLE_PUNISHMENT = 0.1;

AnswerValidatorOld::AnswerValidatorOld(KEduVocDocument* doc)
{
    m_doc = doc;
    m_entry = 0;
    m_speller = 0;
    m_spellerAvailable = false;
}

AnswerValidatorOld::~AnswerValidatorOld()
{
    delete m_speller;
}


void AnswerValidatorOld::setLanguage(int translation)
{
    m_translation = translation;

    // default: try locale
    if ( !m_speller ) {
        m_speller = new Sonnet::Speller(m_doc->identifier(translation).locale());
    } else {
        m_speller->setLanguage(m_doc->identifier(translation).locale());
    }

    // we might succeed with language name instead.
    if ( !m_speller->isValid() ) {
        m_speller->setLanguage(m_doc->identifier(translation).name());
    }

    if ( !m_speller->isValid() ) {
        kDebug() << "No spellchecker for current language found: " << m_doc->identifier(m_translation).locale();
        kDebug() << "Available dictionaries: " << m_speller->availableLanguages()
                << "\n names: " << m_speller->availableLanguageNames()
                << "\n backends: " << m_speller->availableBackends();
        m_spellerAvailable = false;
    } else {
        m_spellerAvailable = true;
    }
}


void AnswerValidatorOld::setTestEntry(TestEntry * entry)
{
    m_entry = entry;
    if (m_entry) {
        m_solution = m_entry->entry()->translation(m_translation)->text();
    }
}

int AnswerValidatorOld::levenshteinDistance(const QString& s, const QString& t)
{
    int m = s.length();
    int n = t.length();

    int dWidth = m+1 ;

    // make sure the matrix is big enough
    if( m_d.size() < (m+1) * (n+1)) {
        m_d.resize( (m+1) * (n+1) );
    }

    int i;
    int j;

    // init 0..m, 0..n as starting values - distance to ""
    for ( i = 0; i <= m; i++ )
    {
        m_d[i + 0*dWidth] = i;
    }
    for ( j = 0; j <= n; j++ ) {
        m_d[0 + j*dWidth] = j;
    }

    int cost;
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if ( s[i-1] == t[j-1] ) {
                // if current char is equal, no cost for substitution
                cost = 0;
            } else {
                cost = 1;
            }
            m_d[i + j*dWidth] = qMin( qMin ( // min of three possibilities
                      m_d[i-1 + (j  )*dWidth] + 1,     // deletion
                      m_d[i   + (j-1)*dWidth] + 1),     // insertion
                      m_d[i-1 + (j-1)*dWidth] + cost);   // substitution
        }
    }
    return m_d[m + n*dWidth];
}

bool AnswerValidatorOld::spellcheckerMisspelled(const QString& userAnswer)
{
    if (!m_spellerAvailable) {
        return true;
    }
    return m_speller->isMisspelled(userAnswer);
}

bool AnswerValidatorOld::spellcheckerInSuggestionList(const QString& solution, const QString& userAnswer)
{
    if ( !m_spellerAvailable ) {
        return false;
    }

    kDebug() << "entered: " << userAnswer << " misspelled: " << m_speller->isMisspelled(userAnswer) << " suggestions: " << m_speller->suggest(userAnswer);

    if ( m_speller->suggest(userAnswer).contains(solution) ) {
        kDebug() << "I think this is a spelling error.";
        return true;
    } else {
        kDebug() << "No, this is a different word I think.";
        return false;
    }
}


void AnswerValidatorOld::simpleCorrector()
{
kDebug() << "simpleCorrector";
    if ( m_entry == 0 ) {
        kError() << "No entry set, cannot verify answer.";
        return;
    }

    ///@todo can solution.length() be zero? *should* be caught by Parley
    if ( m_solution == m_userAnswer ) {
        m_entry->setLastErrors(TestEntry::Correct);
        m_entry->setLastPercentage(1.0);
//         m_htmlCorrection = i18n("Your answer is right!");
kDebug() << "right";
        return;
    }

    TestEntry::ErrorTypes errorTypes = TestEntry::UnknownMistake;

    foreach(KEduVocTranslation *synonym, m_entry->entry()->translation(m_translation)->synonyms()) {
        if (synonym->text() == m_userAnswer) {
            m_entry->setLastErrors(TestEntry::Synonym);
            if ( Prefs::countSynonymsAsCorrect() ) {
                m_entry->setLastPercentage(1.0);
            } else {
                m_entry->setLastPercentage(0.0); // bit harsh maybe
            }
            return;
        }
    }

    int levensthein = levenshteinDistance( m_solution, m_userAnswer );

    m_entry->setLastPercentage(1.0 - ((double)levensthein/ qMax(m_solution.length(), m_userAnswer.length())));

    kDebug() << "simpleCorrector" << m_userAnswer << "-" << m_solution << "has levensthein distance: " << levensthein << " grade: " << m_entry->lastPercentage();
}


void AnswerValidatorOld::defaultCorrector()
{
    ///@todo does not work completely yet.
    ///@todo can solution.length() be zero? *should* be caught by Parley
    if ( m_solution == m_userAnswer ) {
        m_entry->setLastErrors(TestEntry::Correct);
        m_entry->setLastPercentage(1.0);
        return;
    }

    if ( m_userAnswer.isEmpty() ) {
        m_entry->setLastErrors(TestEntry::Empty);
        m_entry->setLastPercentage(0.0);
        return;
    }

    foreach(KEduVocTranslation *synonym, m_entry->entry()->translation(m_translation)->synonyms()) {
        if (synonym->text() == m_userAnswer) {
            m_entry->setLastErrors(TestEntry::Synonym);
            if ( Prefs::countSynonymsAsCorrect() ) {
                m_entry->setLastPercentage(1.0);
            } else {
                m_entry->setLastPercentage(0.0); // bit harsh maybe
            }
            return;
        }
    }

    int numberSolutionWords = m_solution.simplified().split(' ').count();
    int numberAnswerWords = m_userAnswer.simplified().split(' ').count();

    if ( numberSolutionWords == 1 ) {
        double grade;
        TestEntry::ErrorTypes errors;
        wordCompare(m_solution, m_userAnswer, grade, errors);
        m_entry->setLastPercentage(grade);
        m_entry->setLastErrors(errors);
        return;
    }

    if ( numberSolutionWords == 2 ) {
        // could be noun + article
        QStringList solutionWords = m_solution.simplified().split(' ');

        if ( m_translation >= 0 ) {
            if (m_doc->identifier(m_translation).article().isArticle(solutionWords.value(0)) ) {
                // yes, the answer is an article + noun
                if ( numberAnswerWords == 1 ) {
                    double percent;
                    TestEntry::ErrorTypes errors;
                    wordCompare(solutionWords.value(1), m_userAnswer.simplified(), percent, errors);
                    m_entry->setLastPercentage(qMax(percent-WRONG_ARTICLE_PUNISHMENT, 0.0));
                    m_entry->setLastErrors(errors|TestEntry::ArticleMissing);
                    return;
                }
                if ( numberAnswerWords == 2 ) {
                    double percent;
                    TestEntry::ErrorTypes errors;
                    wordCompare(solutionWords.value(1), m_userAnswer.simplified().split(' ').value(1), percent, 
errors);

                    if ( m_userAnswer.simplified().split(' ').value(0) == solutionWords.value(0) ) {
                        m_entry->setLastErrors(errors);
                    } else {
                        m_entry->setLastPercentage(qMax(percent-WRONG_ARTICLE_PUNISHMENT, 0.0));
                        m_entry->setLastErrors(errors|TestEntry::ArticleWrong);
                    }
                    return;
                }
            }
        }
    }

    // ok, more than one word (or one+article)
    sentenceAnalysis();
}




void AnswerValidatorOld::checkUserAnswer(const QString & userAnswer)
{
    if ( m_entry == 0 ) {
        kError() << "Error: no entry set for validator.";
        return;
    }

    m_userAnswer = userAnswer;

//     simpleCorrector();
    defaultCorrector();
}

void AnswerValidatorOld::checkUserAnswer(const QString & solution, const QString & userAnswer, const QString& language)
{
kDebug() << "CheckUserAnswer with two strings. The one string version is preferred.";
    if ( !language.isEmpty() ) {

    } else {
        m_spellerAvailable = false;
    }

    m_solution = solution;

    checkUserAnswer(userAnswer);
}


void AnswerValidatorOld::wordCompare(const QString & solution, const QString & userWord, double& grade, TestEntry::ErrorTypes& errorTypes)
{
    ///@todo add to other errors... ?

    // nothing to be done here if it's right
    if ( solution == userWord ) {
        grade = 1.0;
        errorTypes = TestEntry::Correct;
        return;
    }
    if ( solution.toLower() == userWord.toLower() ) {
        grade = 1.0 - CAPITALIZATION_MISTAKE_PUNISHMENT;
        errorTypes = TestEntry::CapitalizationMistake;
        return ;
    }
    if ( ParleyStringHandlerOld::stripAccents(solution) ==    ParleyStringHandlerOld::stripAccents(userWord) ) {
        grade = 1.0 - ACCENT_MISTAKE_PUNISHMENT;
        errorTypes = TestEntry::AccentMistake;
        return ;
    }

    int levenshtein = levenshteinDistance(solution, userWord);

    if ( m_spellerAvailable ) {
        bool inSuggestions = false;
        bool isMisspelled = m_speller->isMisspelled( userWord );
        if ( m_speller->suggest(userWord).contains(solution) ) {
            inSuggestions = true;
        }
        // probably misspelled
        if ( isMisspelled && inSuggestions ) {
            grade = 1.0 - qMax (levenshtein * SPELLING_MISTAKE_PER_LETTER_PUNISHMENT, 1.0);
            errorTypes = TestEntry::SpellingMistake;
            return;
        }
        // this is a different word but sounds similar!
        if ( !isMisspelled && inSuggestions ) {
            grade = FALSE_FRIEND_GRADE;
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">NOOOO! That was a false friend!</font> ");
            errorTypes = errorTypes = TestEntry::FalseFriend;
            return ;
        }
        // unrelated word
        if ( !isMisspelled && !inSuggestions ) {
            grade = UNRELATED_WORD_GRADE;
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">Do you have any idea what you are talking about? (Wrong word, you spelled it correct I guess.)</font> ");
            errorTypes = TestEntry::UnrelatedWord;
            return;
        }
        // complete nonsense, unless levenshtein comes to the rescue
        if ( isMisspelled && !inSuggestions ) {
            if ( ((double)levenshtein/ qMax(solution.length(), userWord.length())) < LEVENSHTEIN_THRESHOLD ) {
//                 htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">Seems like you got the spellig wrong.</font> ");
                errorTypes = TestEntry::SpellingMistake;
                return;
            } else {
//                 htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">I don't know that word and it is not similar to the solution.</font> ");
                errorTypes = TestEntry::UnknownMistake;
                return;
            }
        }
    } else {
        if ( ((double)levenshtein/ qMax(solution.length(), userWord.length())) < LEVENSHTEIN_THRESHOLD ) {
            grade = 1.0 - ((double)levenshtein/ qMax(solution.length(), userWord.length()));
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No spellchecker, but seems like a spelling error.</font> ");
            errorTypes = TestEntry::SpellingMistake;
            return;
        } else {
            grade = 1.0 - ((double)levenshtein/ qMax(solution.length(), userWord.length()));
//             htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No dictionary and no clue.</font> ");
            errorTypes = TestEntry::UnknownMistake;
            return;
        }
    }

    // cannot get here
//     htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No dictionary and no clue.</font> ");
    errorTypes = TestEntry::UnknownMistake;
    return;
}


void AnswerValidatorOld::sentenceAnalysis()
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

    for ( int i = 0; i < userAnswerWords.count(); i++ ) {
        int pos = solutionWords.indexOf( userAnswerWords.value(i) );
        if ( pos >= 0 ) {
            correctWords.append(userAnswerWords.value(i));
            solutionWords.removeAt(pos);
            userAnswerWords.removeAt(i);
        } else {
            wrongWords.append(userAnswerWords.value(i));
        }
    }

    kDebug() << " remaining: solution: " << solutionWords.count()
            << "user: " << userAnswerWords.count();

    QList< QPair<QString, QString> > pairs = bestPairs(solutionWords, userAnswerWords);

    for ( int i = 0; i < pairs.count(); i++ ) {
        kDebug() << "Possible pair: " << pairs.value(i).first << " and " << pairs.value(i).second;
    }


    QString correction;
    correction.append("Correct: ");
    foreach (const QString &correctWord, correctWords) {
        correction.append(QString::fromLatin1("<font color=\"#188C18\">") + correctWord + QString::fromLatin1("</font> "));
    }

    correction.append(" Wrong: ");
    foreach (const QString &wrongWord, wrongWords) {
        correction.append(QString::fromLatin1("<font color=\"#8C1818\">") + wrongWord + QString::fromLatin1("</font> "));
    }

    int levenshtein = levenshteinDistance(m_solution, m_userAnswer);

    kDebug() << correction;
    kDebug() << "IMPLEMENT ME TO ACTUALLY EVALUATE THE ABOVE AND GENERATE A GRADE!";
    m_entry->setLastPercentage(1.0 - ((double)levenshtein/ qMax(m_solution.length(), m_userAnswer.length())));
    m_entry->setLastErrors(TestEntry::UnknownMistake);
}


QList< QPair < QString , QString > > AnswerValidatorOld::bestPairs(const QStringList& solutionWords , const QStringList& userAnswerWords )
{
    int nSol = solutionWords.count();
    int nUser = userAnswerWords.count();

    QByteArray d;
    d.resize( nSol * nUser );

    QList< QPair < QString , QString > > pairList;

    // matrix of levenshteinDistances
    for ( int i = 0; i < nSol; i++) {
        for ( int j = 0; j < nUser; j++) {
            d[i + nSol*j] = levenshteinDistance(solutionWords.value(i), userAnswerWords.value(j));
        }
    }

    int MAX_LEVENSHTEIN = 5;

    // check if another pair is possible
    int min;
    int posSol = -1;
    int posUser = -1;
    do {
        min = MAX_LEVENSHTEIN;
        for ( int i = 0; i < nSol; i++ ) {
            for ( int j = 0; j < nUser; j++) {
                if ( d.at(i + j*nSol ) < min ) {
                    min = d.at(i + j*nSol );
                    posSol = i;
                    posUser = j;
                }
            }
        }
        if ( min < MAX_LEVENSHTEIN && posUser != -1 && posSol != -1) {
            pairList.append( qMakePair(solutionWords.value(posSol), userAnswerWords.value(posUser)) );

            // taken
            d[posSol + posUser*nSol] = MAX_LEVENSHTEIN;
        }
    } while ( min < MAX_LEVENSHTEIN );

    return pairList;
}


bool AnswerValidatorOld::spellcheckerAvailable()
{
        return m_spellerAvailable;
}
