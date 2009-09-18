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


#ifndef PRACTICEOPTIONS_H
#define PRACTICEOPTIONS_H

#include "abstractfrontend.h"

namespace Practice {
    
class PracticeOptions
{
public:  
    PracticeOptions();
    
    int languageFrom() const {return m_languageFrom;}
    int languageTo() const {return m_languageTo;}
    
    QList<AbstractFrontend::Mode> modes() const;
    
    int numberMultipleChoiceAnswers() const;
    
private:
    void loadPreferences();
    
    int m_languageFrom;
    int m_languageTo;
    
    QList<AbstractFrontend::Mode> m_modes;
    
};
}
#endif // PRACTICEOPTIONS_H
