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

#include "abstractbackend.h"

namespace Practice {
    
class PracticeOptions
{
public:  
    PracticeOptions();
    
    int languageFrom() {return m_languageFrom;}
    int languageTo() {return m_languageTo;}
    
    QList<AbstractBackend::Mode> modes();
    
private:
    void loadPreferences();
    
    int m_languageFrom;
    int m_languageTo;
    
    QList<AbstractBackend::Mode> m_modes;
    
};
}
#endif // PRACTICEOPTIONS_H
