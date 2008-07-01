//
// C++ Interface: translator
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QMap>
#include <QStringList>

class Translation;

/**
Keeps the translated words

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class Translator
{
    public:
        Translator();

        ~Translator();
        void addTranslation ( QString word, QString fromLanguage, QString toLanguage, QString translation );
        QStringList* getTranslation ( QString word, QString fromLanguage, QString toLanguage );

    protected:
        QMap<Translation,QStringList*> m_translations;
};

#endif
