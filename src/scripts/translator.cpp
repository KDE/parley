//
// C++ Implementation: translator
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "translator.h"

#include <KDebug>

class Translation
{
    public:
        QString word;
        QString fromLanguage;
        QString toLanguage;

        Translation() { }
        Translation ( QString pword, QString pfromLanguage, QString ptoLanguage )
        {
            word = pword;
            fromLanguage = pfromLanguage;
            toLanguage = ptoLanguage;
        }
};

inline bool operator< ( const Translation &t1, const Translation &t2 )
{
    if ( t1.word != t2.word )
        return t1.word < t2.word;
    if ( t1.fromLanguage != t2.fromLanguage )
        return t1.fromLanguage < t2.fromLanguage;
    return t1.toLanguage < t2.toLanguage;
}

Translator::Translator()
{
}


Translator::~Translator()
{
}

/**
 * Stores the translation of @p word from language @p fromLanguage, to language @p toLanguage.
 * @param word word that was translated
 * @param fromLanguage language of @p word
 * @param toLanguage language of @p translation
 * @param translation translation of @p word
 */
void Translator::addTranslation ( QString word, QString fromLanguage, QString toLanguage, QString translation )
{
    Translation t( word,fromLanguage,toLanguage );
    kDebug() << m_translations.contains(t);
    if (!m_translations.contains(t))
        m_translations[t] = new QStringList();
    m_translations[t]->push_back ( translation );
}


/**
 * Returns a QStringList with all the translations of @p word from @p fromLanguage to @p toLanguage
 * @param word
 * @param fromLanguage
 * @param toLanguage
 * @return QStringList with the translations
 */
QStringList* Translator::getTranslation ( QString word, QString fromLanguage, QString toLanguage )
{
    Translation t(word,fromLanguage,toLanguage);
    if (m_translations.contains(t))
        return m_translations[t];
    else
        return 0;
}
