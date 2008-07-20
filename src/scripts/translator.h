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
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QMap>
// #include <QHash>
#include <QStringList>

/*class Translation
{
    public:
        QString word;
        QString fromLanguage;
        QString toLanguage;

        Translation ()
        {
            word = "";
            fromLanguage = "";
            toLanguage = "";
        }

        Translation ( QString pword, QString pfromLanguage, QString ptoLanguage )
        {
            word = pword;
            fromLanguage = pfromLanguage;
            toLanguage = ptoLanguage;
        }

        void operator=(const Translation &t1) {
            word = t1.word;
            fromLanguage = t1.fromLanguage;
            toLanguage = t1.toLanguage;
        }

//         inline bool operator<(const Translation &t2) {
//             if ( word != t2.word )
//                 return word < t2.word;
//             if ( fromLanguage != t2.fromLanguage )
//                 return fromLanguage < t2.fromLanguage;
//             return toLanguage < t2.toLanguage;
//         }
};*/

// inline bool operator<( const Translation &t1, const Translation &t2 )
// {
//     if ( t1.word != t2.word )
//         return t1.word < t2.word;
//     if ( t1.fromLanguage != t2.fromLanguage )
//         return t1.fromLanguage < t2.fromLanguage;
//     return t1.toLanguage < t2.toLanguage;
// }

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
        QSet<QString>* getTranslation ( QString word, QString fromLanguage, QString toLanguage );

    private:
//         QMap<Translation,QStringList*> m_translations;
         QMap<QString,QSet<QString>*> m_translations;
//          QHash<QString,QStringList*> m_translations;
};

#endif
