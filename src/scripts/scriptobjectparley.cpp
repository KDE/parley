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
#include "scriptobjectlesson.h"

#include "../vocabulary/vocabularymodel.h"

#include "translator.h"

#include "../parley.h"

#include <KLocale>
#include <KGlobal>
#include <KDebug>

namespace Scripting
{

    ScriptObjectParley::ScriptObjectParley ( ParleyApp * parley ) : QObject()
    {
        m_translator = 0;
        m_parleyApp = parley;
        m_doc = new ScriptObjectDocument ( m_parleyApp->parleyDocument()->document() );
    }


    ScriptObjectParley::~ScriptObjectParley()
    {
    }

    void ScriptObjectParley::callTranslateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage )
    {
        emit translationStarted ( word, fromLanguage, toLanguage );
        emit translateWord ( word, fromLanguage, toLanguage );
        emit translationFinished ( word, fromLanguage, toLanguage );
    }

    void ScriptObjectParley::addTranslation ( QString word,QString fromLanguage,QString toLanguage, QString translation )
    {
        if ( m_translator )
            m_translator->addTranslation ( word,fromLanguage,toLanguage,translation );
    }

    QStringList ScriptObjectParley::languageCodes()
    {
        /// @todo Change it into a QMap property (Parley.languageCodes)
        return KGlobal::locale()->allLanguagesList();
    }

    QString ScriptObjectParley::languageCodeToName ( QString code )
    {
        return KGlobal::locale()->languageCodeToName ( code );
    }

    void ScriptObjectParley::open ( QString filename )
    {
        KUrl k;
        k.setFileName ( filename );
        kDebug() << k;
        m_parleyApp->parleyDocument()->open ( k,false );
    }

    QObject* Scripting::ScriptObjectParley::getActiveLesson()
    {
        return new ScriptObjectLesson(m_parleyApp->m_vocabularyModel->lesson());
    }

}

