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
#include "translator.h"
#include "scripting/parley.h"

#include <KDebug>

Translator::Translator()
{
}

Translator::Translator( QObject * parent )
    : m_parent(parent)
{
}

Translator::~Translator()
{
}

void Translator::addTranslation ( QString word, QString fromLanguage, QString toLanguage, QString translation )
{
    if ( word.trimmed() == "" ) return;

    QString t = word+fromLanguage+toLanguage;
    kDebug() << "Translation for " << word << "in cache: " << m_translations.contains ( t );
    if ( !m_translations.contains ( t ) ) {
        m_translations[t] = new QSet<QString>();
    }
    m_translations[t]->insert ( translation.simplified() );
}

QSet<QString>* Translator::getTranslation ( QString word, QString fromLanguage, QString toLanguage )
{
    if (word.isEmpty() || fromLanguage.isEmpty() || toLanguage.isEmpty()) return 0;

    QString t = word+fromLanguage+toLanguage;
    kDebug() << "Fetch translation " << word << "(" << fromLanguage << "to" << toLanguage << ")"
             << "already in cache:" << m_translations.contains(t);
    if (!m_translations.contains(t)) {
        Scripting::Parley * p = dynamic_cast<Scripting::Parley*>(m_parent);
        if (p) {
            p->callTranslateWord(word,fromLanguage,toLanguage);
        }
    }

    if ( m_translations.contains ( t ) )
        return m_translations.value ( t );
    else
        return 0;
}
