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
    m_translator = 0;
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
void ScriptObjectParley::callTranslateWord ( const QString & word )
{
    emit translateWord ( word );
}


/**
 * 
 * @param word 
 * @param fromLanguage 
 * @param toLanguage 
 * @param translation 
 */
void ScriptObjectParley::addTranslation ( QString word,QString fromLanguage,QString toLanguage, QString translation )
{
    if (m_translator) {
        m_translator->addTranslation(word,fromLanguage,toLanguage,translation);
    }
}


/**
 * 
 * @param translator 
 */
void ScriptObjectParley::setTranslator(Translator* translator)
{
    m_translator = translator;
}
