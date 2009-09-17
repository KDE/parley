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
    
    switch ( Prefs::testType() ) {
        case Prefs::EnumTestType::FlashCardTest:
            m_modes.append(AbstractFrontend::FlashCard);
        case Prefs::EnumTestType::MultipleChoiceTest:
            m_modes.append(AbstractFrontend::MultipleChoice);
        case Prefs::EnumTestType::WrittenTest:
            m_modes.append(AbstractFrontend::Written);
        case Prefs::EnumTestType::MixedLettersTest:
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::ComparisonTest:
        case Prefs::EnumTestType::ConjugationTest:
        case Prefs::EnumTestType::SynonymTest:
        case Prefs::EnumTestType::AntonymTest:
        case Prefs::EnumTestType::ExampleTest:
        case Prefs::EnumTestType::ParaphraseTest:
            break;
    }
    KDebug() << "Selected Test Types: " << m_modes;
}

QList<AbstractFrontend::Mode> PracticeOptions::modes()
{
    return m_modes;
}
