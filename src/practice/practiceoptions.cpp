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
    
    if (Prefs::flashCardPractice()) {
        m_modes.append(AbstractFrontend::FlashCard);
    }
    if (Prefs::multipleChoicePractice()) {
        m_modes.append(AbstractFrontend::MultipleChoice);
    }
    if (Prefs::writtenPractice()) {
        m_modes.append(AbstractFrontend::Written);
    }
    kDebug() << "Prefs::testType()" << Prefs::testType() << "Selected Test Types: " << m_modes;
}

QList<AbstractFrontend::Mode> PracticeOptions::modes()
{
    return m_modes;
}
