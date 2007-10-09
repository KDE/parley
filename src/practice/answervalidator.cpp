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

#include <keduvocexpression.h>
#include <keduvocdocument.h>
#include <keduvocidentifier.h>

#include <sonnet/speller.h>
#include <kdebug.h>


AnswerValidator::AnswerValidator(KEduVocDocument* doc)
{
    m_doc = doc;
    m_speller = 0;
    m_errorType = Wrong;
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

double AnswerValidator::checkUserAnswer(const QString & userAnswer)
{
    m_userAnswer = userAnswer;
    defaultCorrector();

    return m_grade;
}

void AnswerValidator::defaultCorrector()
{
    ///@todo can solution.length() be zero?
    QString solution = m_expression->translation(m_translation).text();
    if ( solution == m_userAnswer ) {
        m_errorType = Correct;
        m_grade = 1.0;
        return;
    }

    int levensthein = levenshteinDistance( solution, m_userAnswer );

    if ( m_spellerAvailable ) {
        spellcheckerMisspelled(m_userAnswer);
        spellcheckerInSuggestionList(solution, m_userAnswer);
    }

    m_grade = 1.0 - ((double)levensthein/ qMax(solution.length(), m_userAnswer.length()));

    kDebug() << "defaultCorrector" << m_userAnswer << "-" << solution << "has levensthein distance: " << levensthein << " grade: " << m_grade;
}

