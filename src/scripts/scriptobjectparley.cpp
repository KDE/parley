//
// C++ Implementation: scriptobjectparley
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjectparley.h"

#include <KDebug>

ScriptObjectParley::ScriptObjectParley()
{
}


ScriptObjectParley::~ScriptObjectParley()
{
}


/**
 * Just to call it from the script to test it out ;)
 */
void ScriptObjectParley::callFromScript()
{
    kDebug() << QString ( "Called from script!!!!" );
}


/**
 * Test function to emit the translate signal
 * @param word 
 */
void ScriptObjectParley::callTranslateWord(const QString & word)
{
    emit translateWord(word);
}
