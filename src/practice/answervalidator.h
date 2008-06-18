/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ANSWERVALIDATOR_H
#define ANSWERVALIDATOR_H

#include <QtCore/QString>
#include <QtCore/QPair>

#include "statistics.h"

class KEduVocDocument;

namespace Sonnet
{

class Speller;
}


/**
Provides different comparison methods to determine the quality of the answer.

    @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/

class AnswerValidator : public QObject
{
           Q_OBJECT
    public:
        AnswerValidator(KEduVocDocument* doc, QObject* parent = 0);

        ~AnswerValidator();

        /**
         * Set the language for the spell checker. (Identifierindex)
         * @param translation
         */
        void setLanguage(int translation);

        bool spellcheckerAvailable();


    public slots:
        void slotSetExpression(KEduVocExpression*);
        /**
        * Checks the user answer. signalCorrection(float, Statistics::ErrorType) is emitted,
        * which tells the 'correctness score' (a number from 0.0 to 1.0
        * that denotes how close it was) and what error was made.
        */
        void checkUserAnswer(const QString& solution, const QString& userAnswer);

    signals:
        /// The float is the 'correctness score', a number from 0.0 to 1.0 that denotes how close
        /// the user's answer was. The ErrorType containts the error(s) that the user made.
        /// signalCorrection(1.0, Statistics::Correct) denotes a correct answer;
        void signalCorrection(float, Statistics::ErrorType);

    private:
        /**
         * Default correction mechanism.
         */
        void defaultCorrector();

        /**
         * Old primitive working corrector.
         */
        void simpleCorrector();

        /**
         * Levenshtein Distance is the minimum number of
         * substitution, insertion and deletion operations necessary
         * to transform two strings into each other.
         * @param s
         * @param t
         * @return the Levenshtein Distance measure
         */
        int levenshteinDistance(const QString& s, const QString& t);

        bool spellcheckerMisspelled(const QString& s);

        bool spellcheckerInSuggestionList(const QString& solution, const QString& userAnswer);

        /**
         * Evaluate the closeness of two words. This is probably the most common case.
         * Should be used for single words. Will not work good for more complex cases.
         * @param word1
         * @param word2
         * @param errorType
         */
        void wordCompare(const QString& solution, const QString& userWord, double& grade, Statistics::ErrorType& errorType);

        /**
         * Split up a sentence and try to work magic with it...
         */
        void sentenceAnalysis();

        /**
         * Try to make words into pairs that are most likely to match
         * @param
         * @param
         * @return
         */
        QList< QPair < QString, QString > > bestPairs(const QStringList& solutionWords , const QStringList& userAnswerWords);

        /// document for language and articles
        KEduVocDocument* m_doc;

        /// the entry to be checked, this provides meta info like word type etc
        KEduVocExpression* m_entry;
        int m_translation;

        /// the solution - usually text of the translation
        QString m_solution;

        /// the answer provided by the user
        QString m_userAnswer;

        /// field used by the LevenshteinDistance to store temp data
        QByteArray m_d;

        /// spellchecker
        Sonnet::Speller* m_speller;
        bool m_spellerAvailable;

        static const double LEVENSHTEIN_THRESHOLD;
        static const double UNRELATED_WORD_GRADE;
        static const double FALSE_FRIEND_GRADE;
        static const double SPELLING_MISTAKE_PER_LETTER_PUNISHMENT;
        static const double ACCENT_MISTAKE_PUNISHMENT;
        static const double CAPITALIZATION_MISTAKE_PUNISHMENT;
        static const double WRONG_ARTICLE_PUNISHMENT;
};

#endif
