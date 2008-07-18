/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "../scriptmanager.h"

#include "./parley.h"
#include "document.h"
#include "lesson.h"

#include "../../vocabulary/vocabularymodel.h"

#include "../translator.h"

#include "../../parley.h"

#include <KLocale>
#include <KGlobal>
#include <KDebug>
#include <KActionCollection>
#include <KMenuBar>

namespace Scripting
{

    Parley::Parley ( ParleyApp * parley ) : QObject(), m_parleyApp ( parley )
    {
        m_translator = 0;
        m_doc = new Document ( m_parleyApp->parleyDocument()->document() );
    }

    Parley::~Parley()
    {
    }

    void Parley::callTranslateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage )
    {
        emit translationStarted ( word, fromLanguage, toLanguage );
        emit translateWord ( word, fromLanguage, toLanguage );
        emit translationFinished ( word, fromLanguage, toLanguage );
    }

    void Parley::addTranslation ( QString word,QString fromLanguage,QString toLanguage, QString translation )
    {
        if ( m_translator )
            m_translator->addTranslation ( word,fromLanguage,toLanguage,translation );
    }

    QStringList Parley::languageCodes()
    {
        /// @todo Change it into a QMap property (Parley.languageCodes)
        return KGlobal::locale()->allLanguagesList();
    }

    QString Parley::languageCodeToName ( QString code )
    {
        return KGlobal::locale()->languageCodeToName ( code );
    }

    void Parley::open ( QString filename )
    {
        KUrl k;
        k.setFileName ( filename );
        kDebug() << k;
        m_parleyApp->parleyDocument()->open ( k,false );
    }

    QObject* Scripting::Parley::getActiveLesson()
    {
        return new Lesson ( m_parleyApp->m_vocabularyModel->lesson() );
    }


    KAction * Scripting::Parley::newAction ( const QString & name )
    {
        //create new action
        KAction* action = new KAction ( m_parleyApp );
        m_parleyApp->m_scriptManager->addScriptAction(name,action);
        return action;

    }

}




