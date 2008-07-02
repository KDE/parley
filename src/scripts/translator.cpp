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
    if (word.trimmed() == "") return;
//     Translation t( word,fromLanguage,toLanguage );
    QString t = word+fromLanguage+toLanguage;
    kDebug() << m_translations.contains ( t );
    if ( !m_translations.contains ( t ) )
        m_translations[t] = new QSet<QString>();
    m_translations[t]->insert ( translation );
//     m_translations[t]->push_back ( translation );
}


/**
 * Returns a QStringList with all the translations of @p word from @p fromLanguage to @p toLanguage
 * @param word
 * @param fromLanguage
 * @param toLanguage
 * @return QStringList with the translations (or an empty QStringList if no translations found)
 */
QSet<QString>* Translator::getTranslation ( QString word, QString fromLanguage, QString toLanguage )
{
//     Translation t(word,fromLanguage,toLanguage);
    QString t = word+fromLanguage+toLanguage;
    kDebug() << t;
    if ( m_translations.contains ( t ) )
        return m_translations.value ( t );
    else
        return 0;
}
