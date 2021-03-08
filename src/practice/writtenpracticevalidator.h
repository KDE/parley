/*
    SPDX-FileCopyrightText: 2010 Benjamin Schleinzer <ben-kde@schleinzer.eu>
    SPDX-FileCopyrightText: 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef WRITTENPRACTICEVALIDATOR_H
#define WRITTENPRACTICEVALIDATOR_H

#include <Sonnet/Speller>

#include "parleydocument.h"

#include "testentry.h"


namespace Practice
{

class WrittenPracticeValidator
{

public:

    WrittenPracticeValidator(int translation, KEduVocDocument* doc);
    ~WrittenPracticeValidator();

    void setEntry(TestEntry* entry);
    void validateAnswer(const QString& answer);
    bool spellcheckerAvailable();
    QString getCorrectedAnswer();

private:

    void setLanguage(int translation);

    /** Simple check if the words are the same */
    bool isCorrect(const QString& correct, const QString& answer);

    /** Check if the answer is a synonym of the testentry
      *  also check for capitalization mistakes and accent mistakes
      *  if the option is set */
    bool isSynonymMistake(const QString& answer);

    /** Check if the answer is a punctuation mistakes */
    bool isPunctuationMistake(const QString& original, const QString& answer);

    /** Check if the answer is a capitalization mistakes */
    bool isCapitalizationMistake(const QString& original, const QString& answer);

    /** Check is an accent mistake was made. Take capitalization into account
      * if the option was set in the preferences */
    bool isAccentMistake(const QString& original, const QString& answer);

    TestEntry* m_entry;
    KEduVocDocument* m_doc;
    QString m_correctedAnswer;
    int m_translation;
    TestEntry::ErrorTypes m_error;

    /// spellchecker
    Sonnet::Speller* m_speller;
    bool m_spellerAvailable;
};

}

#endif // WRITTENPRACTICEVALIDATOR_H
