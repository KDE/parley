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

#include <keduvocexpression.h>
#include <keduvocdocument.h>
#include <keduvocidentifier.h>

#include <KLocalizedString>
#include <sonnet/speller.h>
#include <kdebug.h>

const double AnswerValidator::LEVENSHTEIN_THRESHOLD = 0.2;
const double AnswerValidator::UNRELATED_WORD_GRADE = 0.0;
const double AnswerValidator::FALSE_FRIEND_GRADE = 0.0;
const double AnswerValidator::SPELLING_MISTAKE_PER_LETTER_PUNISHMENT = 0.2;
const double AnswerValidator::CAPITALIZATION_MISTAKE_PUNISHMENT = 0.1;
const double AnswerValidator::WRONG_ARTICLE_PUNISHMENT = 0.1;

AnswerValidator::AnswerValidator(KEduVocDocument* doc)
{
    m_doc = doc;
    m_speller = 0;
    m_errorType = UnknownMistake;
    m_grade = 0.0;
    m_spellerAvailable = false;
}

AnswerValidator::~AnswerValidator()
{
    delete m_speller;
}

void AnswerValidator::setSolution(KEduVocExpression * expression, int translation)
{
    m_expression = expression;
    m_translation = translation;
    m_solution = m_expression->translation(m_translation).text();

    if ( !m_speller ) {
        m_speller = new Sonnet::Speller(m_doc->identifier(translation).locale());
    } else {
        m_speller->setLanguage(m_doc->identifier(translation).locale());
    }

    if ( !m_speller->isValid() ) {
        kDebug() << "No spellchecker for current language found: " << m_doc->identifier(m_translation).locale();
        kDebug() << "Avaliable dictionaries: " << m_speller->availableLanguages()
            << "\n names: " << m_speller->availableLanguageNames()
            << "\n backends: " << m_speller->availableBackends();
        m_spellerAvailable = false;
    } else {
        m_spellerAvailable = true;
    }
}

int AnswerValidator::levenshteinDistance(QString s, QString t)
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

bool AnswerValidator::spellcheckerMisspelled(QString userAnswer)
{
    if (!m_spellerAvailable) {
        return true;
    }
    return m_speller->isMisspelled(userAnswer);
}

bool AnswerValidator::spellcheckerInSuggestionList(QString solution, QString userAnswer)
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


void AnswerValidator::simpleCorrector()
{
    ///@todo can solution.length() be zero? *should* be caught by Parley
    QString solution = m_expression->translation(m_translation).text();
    if ( solution == m_userAnswer ) {
        m_errorType = Correct;
        m_grade = 1.0;
        m_htmlCorrection = i18n("Your answer is right!");
        return;
    }

    // check synonym
    if ( m_expression->translation(m_translation).synonym() == m_userAnswer ) {
        m_errorType = Synonym;
        if ( Prefs::countSynonymsAsCorrect() ) {
            m_grade = 1.0;
            m_htmlCorrection = i18n("You entered a synonym.");
        } else {
            m_grade = 0.0; // bit harsh maybe
            m_htmlCorrection = i18n("You entered a synonym.");
        }
        return;
    }

    int levensthein = levenshteinDistance( solution, m_userAnswer );

    m_grade = 1.0 - ((double)levensthein/ qMax(solution.length(), m_userAnswer.length()));

    kDebug() << "defaultCorrector" << m_userAnswer << "-" << solution << "has levensthein distance: " << levensthein << " grade: " << m_grade;

    m_htmlCorrection = i18n("Your answer was wrong I'm afraight. Estimated %1\% correct.", (int) (m_grade*100));
}


void AnswerValidator::defaultCorrector()
{

///@todo lacks i18n and does not work completely yet.

    ///@todo can solution.length() be zero? *should* be caught by Parley
    if ( m_solution == m_userAnswer ) {
        m_errorType = Correct;
        m_grade = 1.0;
        return;
    }

    if ( m_userAnswer.isEmpty() ) {
        m_errorType = Empty;
        m_grade = 0.0;
        return;
    }

    if ( m_expression ) {
        // check synonym
        if ( m_expression->translation(m_translation).synonym() == m_userAnswer ) {
            m_errorType = Synonym;
            if ( Prefs::countSynonymsAsCorrect() ) {
                // synonym, good for you
                m_grade = 1.0;
            } else {
                // it is the synonym but we don't accept it
                m_grade = 0.0; // bit harsh maybe
            }
            return;
        }
    }

    int numberSolutionWords = m_solution.simplified().split(" ").count();
    int numberAnswerWords = m_userAnswer.simplified().split(" ").count();

    if ( numberSolutionWords == 2 ) {
        // could be noun + article
        kDebug() << "IMPLEMENT ME: TWO WORDS TO CHECK - IS IT AN ARTICLE?";
        QStringList solutionWords = m_solution.simplified().split(" ");

        if ( m_translation >= 0 ) {
            if (m_doc->identifier(m_translation).article().isArticle(solutionWords.value(0)) ) {
                // yes, the answer is an article + noun
                if ( numberAnswerWords == 1 ) {
                    wordCompare(solutionWords.value(1), m_userAnswer.simplified(), m_grade, m_htmlCorrection);
                    m_htmlCorrection.append(" And the article is missing!");
                    m_grade = qMin(m_grade-WRONG_ARTICLE_PUNISHMENT, 0.0);
                    return;
                }
                if ( numberAnswerWords == 2 ) {
                    wordCompare(solutionWords.value(1), m_userAnswer.simplified().split(" ").value(1), m_grade, m_htmlCorrection);

                    if ( m_userAnswer.simplified().split(" ").value(0) == solutionWords.value(0) ) {
                        m_htmlCorrection.append(" Well, at least you guessed the article right.");
                    } else {
                        m_htmlCorrection.append(" And the article is wrong!");
                        m_grade = qMin(m_grade-WRONG_ARTICLE_PUNISHMENT, 0.0);
                    }
                    return;
                }
            }
        }
    }

    if ( m_solution.simplified().split(" ").count() > 1 ) {
        scentenceAnalysis();
    } else {
        // put the grade in m_grade and get the error type
        ErrorType error = wordCompare(m_solution, m_userAnswer, m_grade, m_htmlCorrection);
    }


//     int levensthein = levenshteinDistance( m_solution, m_userAnswer );

//     m_grade = 1.0 - ((double)levensthein/ qMax(m_solution.length(), m_userAnswer.length()));

//     kDebug() << "defaultCorrector" << m_userAnswer << "-" << m_solution << "has levensthein distance: " << levensthein << " grade: " << m_grade;
}


double AnswerValidator::checkUserAnswer(const QString & userAnswer)
{
    m_htmlCorrection.clear();
    m_userAnswer = userAnswer;
    simpleCorrector();
/// @todo enable me: defaultCorrector();
    return m_grade;
}

double AnswerValidator::checkUserAnswer(const QString & solution, const QString & userAnswer, const QString& language)
{
kDebug() << "CheckUserAnswer with two strings. The one string version is prefered.";
    if ( !language.isEmpty() ) {

    } else {
        m_spellerAvailable = false;
    }

    m_expression = 0;
    m_solution = solution;

    return checkUserAnswer(userAnswer);
}


AnswerValidator::ErrorType AnswerValidator::wordCompare(const QString & solution, const QString & userWord, double& grade, QString& htmlCorrection)
{
    // nothing to be done here if it's right
    if ( solution == userWord ) {
        grade = 1.0;
        htmlCorrection = QString::fromLatin1("<font color=\"#188C18\">You are right!</font> ");
        return Correct;
    }
    if ( solution.toLower() == userWord.toLower() ) {
        grade = CAPITALIZATION_MISTAKE_PUNISHMENT;
        htmlCorrection = QString::fromLatin1("<font color=\"#8C4040\">Watch your capitalization!</font> ");
        return CapitalizationMistake;
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
            htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\"> Oops, I think you misspelled about ") + QString::number(levenshtein) + QString::fromLatin1(" letters. But the word is right.</font> ");
            return SpellingMistake;
        }
        // this is a different word but sounds similiar!
        if ( !isMisspelled && inSuggestions ) {
            grade = FALSE_FRIEND_GRADE;
            htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">NOOOO! That was a false friend!</font> ");
            return FalseFriend;
        }
        // unrelated word
        if ( !isMisspelled && !inSuggestions ) {
            grade = UNRELATED_WORD_GRADE;
            htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">Do you have any idea what you are talking about? (Wrong word, you spelled it correct I guess.)</font> ");
            return UnrelatedWord;
        }
        // complete nonsense, unless levenshtein comes to the rescue
        if ( isMisspelled && !inSuggestions ) {
            if ( ((double)levenshtein/ qMax(solution.length(), userWord.length())) < LEVENSHTEIN_THRESHOLD ) {
                htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">Seems like you got the spellig wrong.</font> ");
                return SpellingMistake;
            } else {
                htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">I don't know that word and it is not similiar to the solution.</font> ");
                return UnknownMistake;
            }
        }
    } else {
        if ( ((double)levenshtein/ qMax(solution.length(), userWord.length())) < LEVENSHTEIN_THRESHOLD ) {
            grade = 1.0 - ((double)levenshtein/ qMax(solution.length(), userWord.length()));
            htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No spellchecker, but seems like a spelling error.</font> ");
            return SpellingMistake;
        } else {
            grade = 1.0 - ((double)levenshtein/ qMax(solution.length(), userWord.length()));
            htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No dictionary and no clue.</font> ");
            return UnknownMistake;
        }
    }

    // cannot get here
    htmlCorrection = QString::fromLatin1("<font color=\"#8C1818\">No dictionary and no clue.</font> ");
    return UnknownMistake;
}


void AnswerValidator::scentenceAnalysis()
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
    foreach (QString correctWord, correctWords) {
        correction.append(QString::fromLatin1("<font color=\"#188C18\">") + correctWord + QString::fromLatin1("</font> "));
    }

    correction.append(" Wrong: ");
    foreach (QString wrongWord, wrongWords) {
        correction.append(QString::fromLatin1("<font color=\"#8C1818\">") + wrongWord + QString::fromLatin1("</font> "));
    }

    int levenshtein = levenshteinDistance(m_solution, m_userAnswer);

kDebug() << correction;
kDebug() << "IMPLEMENT ME TO ACTUALLY EVALUATE THE ABOVE AND GENERATE A GRADE!";
m_grade = 1.0 - ((double)levenshtein/ qMax(m_solution.length(), m_userAnswer.length()));


    m_htmlCorrection = correction;
}

QString AnswerValidator::correctedAnswerHtml()
{
    return m_htmlCorrection;
}

QList< QPair < QString , QString > > AnswerValidator::bestPairs(const QStringList& solutionWords , const QStringList& userAnswerWords )
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
    int posSol;
    int posUser;
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
        if ( min < MAX_LEVENSHTEIN ) {
            pairList.append( qMakePair(solutionWords.value(posSol), userAnswerWords.value(posUser)) );

            // taken
            d[posSol + posUser*nSol] = MAX_LEVENSHTEIN;
        }
    } while ( min < MAX_LEVENSHTEIN );

    return pairList;
}
