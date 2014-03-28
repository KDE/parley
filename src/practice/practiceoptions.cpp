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
    // FIXME: Look into if this is needed at all.  It looks like they are never used.
    m_languageFrom = Prefs::knownLanguage();
    m_languageTo = Prefs::learningLanguage();
}

Prefs::EnumPracticeMode::type PracticeOptions::mode() const
{
    return Prefs::practiceMode();;
}

int Practice::PracticeOptions::numberMultipleChoiceAnswers() const
{
    return Prefs::numberMultipleChoiceAnswers();
}
