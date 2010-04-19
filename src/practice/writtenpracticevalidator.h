/***************************************************************************
    Copyright 2007 Benjamin Schleinzer <ben-kde@schleinzer.eu>
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

#ifndef WRITTENPRACTICEVALIDATOR_H
#define WRITTENPRACTICEVALIDATOR_H

#include <sonnet/speller.h>
#include "practice/testentry.h"


class WrittenPracticeValidator
{
public:

    WrittenPracticeValidator(int translation);
    ~WrittenPracticeValidator();

    void setEntry(TestEntry* entry);
    void validateAnswer(const QString& answer);
    bool spellcheckerAvailable();
    QString getCorrectedAnswer();

private:

    void setLanguage(int translation);
    
    /** Check if the answer is a synonym of the testentry */
    bool isCorrect(const QString& answer);
    bool isSynonymMistake(const QString& answer);
    bool isCapitalizationMistake(const QString& original, const QString& answer);
    bool isAccentMistake(const QString& original, const QString& answer);
    
    TestEntry* m_entry;
    QString m_correctedAnswer;
    int m_translation;
    TestEntry::ErrorTypes m_error;

    /// spellchecker
    Sonnet::Speller* m_speller;
    bool m_spellerAvailable;
};

#endif // WRITTENPRACTICEVALIDATOR_H
