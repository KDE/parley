//
// C++ Interface: scriptobjectparley
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTOBJECTPARLEY_H
#define SCRIPTOBJECTPARLEY_H

#include "scriptobjectdocument.h"

#include <QObject>

class ParleyApp;
class Translator;

/**
Implements the object that will be given to Kross scripts to use for accessing parley features

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptObjectParley : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QObject * document READ getDocument)
    public:
        ScriptObjectParley ( ParleyApp * parley );

        ~ScriptObjectParley();
        void callTranslateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
        void setTranslator ( Translator* translator );
        QObject* getDocument();

    public Q_SLOTS:
        void addTranslation ( QString word,QString fromLanguage,QString toLanguage, QString translation );
        QStringList languageCodes();
        QString languageCodeToName ( QString code );
        void open ( QString filename );

    Q_SIGNALS:
        void translateWord ( QString word, QString fromLanguage, QString toLanguage );
        void translationFinished ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
        void translationStarted ( const QString & word,const QString& fromLanguage,const QString& toLanguage );

    private:
        Translator* m_translator;
        ScriptObjectDocument* m_doc;
        ParleyApp * m_parleyApp;
};

#endif
