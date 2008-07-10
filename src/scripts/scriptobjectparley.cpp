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
#include "scriptobjectdocument.h"
#include "translator.h"

#include "../parley.h"

#include <KLocale>
#include <KGlobal>
#include <KDebug>

namespace Scripting {

ScriptObjectParley::ScriptObjectParley ( ParleyApp * parley ) : QObject()
{
    m_translator = 0;
    m_parleyApp = parley;
    m_doc = new ScriptObjectDocument(m_parleyApp->parleyDocument()->document());
}


ScriptObjectParley::~ScriptObjectParley()
{
}

/**
 * Test function to emit the translate signal
 * @param word
 */
void ScriptObjectParley::callTranslateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage )
{
    emit translationStarted ( word, fromLanguage, toLanguage );
    emit translateWord ( word, fromLanguage, toLanguage );
    emit translationFinished ( word, fromLanguage, toLanguage );
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
    if ( m_translator )
        m_translator->addTranslation ( word,fromLanguage,toLanguage,translation );
}


/**
 *
 * @param translator
 */
void ScriptObjectParley::setTranslator ( Translator* translator )
{
    m_translator = translator;
}

/**
 * Returns a list of all available language codes (to be used by the scripts)
 * @return
 */
QStringList ScriptObjectParley::languageCodes()
{
    /// @todo Change it into a QMap property (Parley.languageCodes)
    return KGlobal::locale()->allLanguagesList();
}


/**
 * Gives the language name of the given @p code language code.
 * @param code Language code
 * @return Language name
 */
QString ScriptObjectParley::languageCodeToName ( QString code )
{
    return KGlobal::locale()->languageCodeToName ( code );
}


/**
 * Returns the Parley currently open document
 * @return Document object
 */
QObject* ScriptObjectParley::getDocument()
{
    return m_doc;
}

/**
 * Open the Parley Document @p file
 * Usage:
 * @code
 * import Parley
 * Parley.open("Vocab/MyVocab.kvtml")
 * @endcode
 * @param file Parley Document file path (ex. /home/kde-user/MyVocab.kvtml)
 */
void ScriptObjectParley::open ( QString filename )
{
    KUrl k;
    k.setFileName(filename);
    kDebug() << k;
    m_parleyApp->parleyDocument()->open(k,false);
}

}