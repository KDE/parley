/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef TESTENTRY_H
#define TESTENTRY_H

#include <functional>

#include <QFlags>

#include <KEduVocExpression>

class TestEntry
{
public:
    enum ErrorType {
        SpellingMistake       =  0x01, /** < misspelled */
        CapitalizationMistake =  0x02, /** < capitalization error (whAt) */
        AccentMistake         =  0x04, /** < an accent is missing or wrong (é) */
        PunctuationMistake    =  0x08, /** < punctuation mistake (doesnt) */
        ArticleWrong          =  0x10, /** < solution is correct with the article interchanged */
        ArticleMissing        =  0x20, /** < solution is correct with the article missing*/
        FalseFriend           =  0x40, /** < a false friend */
        Synonym               =  0x80, /** < a synonym (may be correct) */
        Empty                 =  0x100, /** < empty answer string */
        UnrelatedWord         =  0x200, /** < a valid word but no connection to the solution */
        Incomplete            =  0x400, /** < the part that was entered is right, but not complete */
        Correct               =  0x800, /** < no error, solution was right */
        Wrong                 =  0x1000  /** < some error, solution was somehow wrong check other bits */
    };

    Q_DECLARE_FLAGS(ErrorTypes, ErrorType)

    explicit TestEntry(KEduVocExpression *entry);

    /// update the internal statistics for this practice with a right result
    void updateStatisticsRightAnswer(grade_t currentPreGrade, grade_t currentGrade);

    /// update the internal statistics for this practice with a wrong result
    void updateStatisticsWrongAnswer(grade_t currentPreGrade, grade_t currentGrade);

    /**
     check if the entry was finished and the practice backend
     may update the grades that will be saved to the file persistently
     @return whether the entry is done
    */
    bool shouldChangeGrades();

    int answeredCorrectInSequence();
    int statisticCount();
    int statisticGoodCount();
    int statisticBadCount();
    bool correctAtFirstAttempt();
    bool isUnseenQuestion() const;

    /**
     * In conjugation mode, use this tense for the entry.
     */
    QString conjugationTense() const;
    void setConjugationTense(const QString& tense);

    /**
     * In conjugation mode, use these pronouns for the entry.
     */
    QList<KEduVocWordFlags> conjugationPronouns() const;
    void setConjugationPronouns(const QList<KEduVocWordFlags>& flags);

    void setLastErrors(ErrorTypes errorTypes);
    ErrorTypes lastErrors();

    void setLastPercentage(double percent);
    double lastPercentage();

    void addUserAnswer(const QString& answer) {
        if (!answer.isEmpty()) m_userAnswers.append(answer);
    }
    QStringList userAnswers() {
        return m_userAnswers;
    }

    void setLanguageFrom(int from);
    void setLanguageTo(int to);
    int languageFrom() const;
    int languageTo() const;

    KEduVocExpression *entry() const;

    grade_t practiceModeDependentMinGrade() const;
    grade_t practiceModeDependentMinPreGrade() const;
    grade_t practiceModeDependentMaxGrade() const;
    grade_t practiceModeDependentMaxPreGrade() const;

private:
    grade_t practiceModeDependentGrade(std::function<grade_t(KEduVocText)> gradeFunc,
                                       std::function<grade_t(grade_t, grade_t)> minMaxFunc) const;

private:
    /// the entry itself
    KEduVocExpression *m_entry;

    int  m_languageFrom;   // Index of the language this entry is from
    int  m_languageTo;     // Index of the language this entry is from

    // These are for the current entry only, so that we can display
    // statistics about it after the training session is finished..
    int m_statisticCount;
    int m_statisticGoodCount;
    int m_statisticBadCount;
    int m_answeredCorrectInSequence;
    bool m_correctAtFirstAttempt;
    bool m_shouldChangeGrades;  // Set to true when grades should be changed after the practice
    bool m_isUnseenQuestion;    // Is set to true for questions never seen before.

    double m_lastPercentage;
    ErrorTypes m_lastError;

    QStringList m_userAnswers;

    //! Only affects conjugation training.
    QString                 m_conjugationTense;
    //! Only affects conjugation training.
    QList<KEduVocWordFlags> m_conjugationPronouns;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TestEntry::ErrorTypes)

#endif
