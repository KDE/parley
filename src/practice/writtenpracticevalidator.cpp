/***************************************************************************
Copyright 2010 Benjamin Schleinzer <ben-kde@schleinzer.eu>
Copyright 2010 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "writtenpracticevalidator.h"
#include "testentry.h"
#include "prefs.h"

#include <keduvoctranslation.h>
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

using namespace Practice;

WrittenPracticeValidator::WrittenPracticeValidator(int translation, KEduVocDocument* doc)
:m_entry(0),
m_doc(doc),
m_error(0),
m_speller(0),
m_spellerAvailable(false)
{
    setLanguage(translation);
}

WrittenPracticeValidator::~WrittenPracticeValidator()
{
    delete m_speller;
}

void WrittenPracticeValidator::setEntry(TestEntry* entry)
{
    m_entry = entry;
}

void WrittenPracticeValidator::setLanguage(int translation)
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

bool WrittenPracticeValidator::spellcheckerAvailable()
{
    return m_spellerAvailable;
}

void WrittenPracticeValidator::validateAnswer(const QString& answer)
{
    if ( m_entry == 0 ) {
        kError() << "No entry set, cannot verify answer.";
        return;
    }

    QString correct = m_entry->entry()->translation(m_translation)->text();

    kDebug() << "Correct answer should be: " << correct;
    m_error = 0;

    //Check for empty answers and valid answers first
    if( answer.isEmpty()){
        m_error.operator|=(TestEntry::Wrong);
        kDebug() << "Empty answer ";
    } else if(isCorrect(answer)){
        m_error.operator|=(TestEntry::Correct);
    } else {
        //Check for all valid errors to build a list of
        //possible mistakes. This provides us with usefull informations
        //that we can use to give feedback to the user.
        if(isCapitalizationMistake(correct,answer)) {
            m_error.operator|=(TestEntry::Correct);
        } else if(isAccentMistake(correct,answer)) {
            m_error.operator|=(TestEntry::Correct);
        } else if(isSynonymMistake(answer)) {
            m_error.operator|=(TestEntry::Correct);
        } else {
            m_error.operator|=(TestEntry::Wrong);
            kDebug() << "Wrong answer: " << answer;
        }
    }
    kDebug() << "Error code " << m_error;
    
    m_entry->setLastErrors(m_error);
}

QString WrittenPracticeValidator::getCorrectedAnswer(){
    return m_correctedAnswer;
}

bool WrittenPracticeValidator::isCorrect(const QString& answer)
{
    if(answer == m_entry->entry()->translation(m_translation)->text()) {
        kDebug() << "Correct answer was given";
        return true;
    }
    return false;
}

bool WrittenPracticeValidator::isSynonymMistake(const QString& answer)
{
    foreach(KEduVocTranslation *synonym, m_entry->entry()->translation(m_translation)->synonyms()) {
        if (synonym->text() == answer ||
            (Prefs::ignoreCapitalizationMistakes() && isCapitalizationMistake(synonym->text(),answer)) ||
            (Prefs::ignoreAccentMistakes() && isAccentMistake(synonym->text(),answer))) {
            kDebug() << "Synonym entered: " << synonym->text() << " answer: " << answer;
            m_correctedAnswer = synonym->text();
            m_error = m_error|TestEntry::Synonym;
            //only return true if accept these kinds of mistakes
            //otherwise just set the error flag
            if(Prefs::countSynonymsAsCorrect())
                return true;
        }
    }
    return false;
}

bool WrittenPracticeValidator::isCapitalizationMistake(const QString& original, const QString& answer)
{
    if (answer.toLower() == original.toLower()) {
        kDebug() << "CapitalizationMistake: " << original << " answer: " << answer;
        m_error.operator|=(TestEntry::CapitalizationMistake);
        m_correctedAnswer = answer;
        //only return true if accept these kinds of mistakes
        //otherwise just set the error flag
        if(Prefs::ignoreCapitalizationMistakes())
            return true;
    }
    return false;
}

bool WrittenPracticeValidator::isAccentMistake(const QString& original, const QString& answer)
{
    QString stripedOriginal = ParleyStringHandlerOld::stripAccents(original);
    QString stripedAnswer = ParleyStringHandlerOld::stripAccents(answer);
    if ( stripedOriginal == stripedAnswer ||
        (Prefs::ignoreCapitalizationMistakes() && isCapitalizationMistake(stripedOriginal,stripedAnswer))) {
        kDebug() << "AccentMistake: " << original << " answer: " << answer;
        m_error.operator|=(m_error|TestEntry::AccentMistake);
        m_correctedAnswer = answer;
        //only return true if accept these kinds of mistakes
        //otherwise just set the error flag
        if(Prefs::ignoreAccentMistakes())
            return true;
    }
    return false;
}

