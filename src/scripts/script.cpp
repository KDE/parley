//
// C++ Implementation: script
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "script.h"

Script::Script()
{
}


Script::~Script()
{
}

/**
 * Returns if the script was successfully activated. If not, Script object should
 * be destroyed.
 */
bool Script::isActivated()
{
    return m_activated;
}
