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
        QStringList* getTranslation ( QString word, QString fromLanguage, QString toLanguage );

    private:
//         QMap<Translation,QStringList*> m_translations;
         QMap<QString,QStringList*> m_translations;
//          QHash<QString,QStringList*> m_translations;
};

#endif
