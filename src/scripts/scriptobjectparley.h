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

#include "translator.h"

#include <QObject>

/**
Implements the object that will be given to Kross scripts to use for accessing parley features

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptObjectParley : public QObject
{
        Q_OBJECT
    public:
        ScriptObjectParley();

        ~ScriptObjectParley();
        void callTranslateWord ( const QString & word,const QString& fromLanguage,const QString& toLanguage );
        void setTranslator ( Translator* translator );

    public Q_SLOTS:
        void addTranslation ( QString word,QString fromLanguage,QString toLanguage, QString translation );

    Q_SIGNALS:
        void translateWord ( QString word, QString fromLanguage, QString toLanguage );

    private:
        Translator* m_translator;
};

#endif
