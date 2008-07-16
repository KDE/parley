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


    KAction * Scripting::Parley::newAction(const QString & name)
    {
        //unplug action list (orelse it will add twice the same entries
        m_parleyApp->unplugActionList("scripts_actionlist");
        //create new action
        KAction* action = new KAction(m_parleyApp);
        //add to action collection
        m_parleyApp->actionCollection()->addAction(name,action);
        //add it to actions menu list
        m_scriptActions.push_back(action);
        //plug the action list
        m_parleyApp->plugActionList("scripts_actionlist",m_scriptActions);

        return action;

    }

}




