/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "practiceoptions.h"

#include "prefs.h"

using namespace Practice;

PracticeOptions::PracticeOptions()
{
    loadPreferences();
}

void PracticeOptions::loadPreferences()
{
    m_languageFrom = Prefs::questionLanguage();                                                                  
    m_languageTo = Prefs::solutionLanguage();
    
    /*
    switch ( Prefs::testType() ) {
        case Prefs::EnumTestType::WrittenTest:
        case Prefs::EnumTestType::MultipleChoiceTest:
        case Prefs::EnumTestType::MixedLettersTest:
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::ComparisonTest:
        case Prefs::EnumTestType::ConjugationTest:
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::ExampleTest:
        case Prefs::EnumTestType::ParaphraseTest:
    */
    
    m_modes.append(AbstractFrontend::FlashCard);
    // m_modes.append(AbstractFrontend::Written);
}

QList<AbstractFrontend::Mode> PracticeOptions::modes()
{
    return m_modes;
}
