/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "translator.h"
#include "scripting/parley.h"

#include <QDebug>

Translator::Translator()
    : m_parent(nullptr)
{
}

Translator::Translator(QObject * parent)
    : m_parent(parent)
{
}

Translator::~Translator()
{
}

void Translator::addTranslation(const QString &word, const QString &fromLanguage, const QString &toLanguage, const QString &translation)
{
    if (word.trimmed() == QLatin1String("")) return;

    QString t = word + fromLanguage + toLanguage;
    qDebug() << "Translation for " << word << "in cache: " << m_translations.contains(t);
    if (!m_translations.contains(t)) {
        m_translations[t] = new QSet<QString>();
    }
    m_translations[t]->insert(translation.simplified());
}

QSet<QString>* Translator::getTranslation(const QString &word, const QString &fromLanguage, const QString &toLanguage)
{
    if (word.isEmpty() || fromLanguage.isEmpty() || toLanguage.isEmpty()) return 0;

    QString t = word + fromLanguage + toLanguage;
    qDebug() << "Fetch translation " << word << "(" << fromLanguage << "to" << toLanguage << ")"
             << "already in cache:" << m_translations.contains(t);
    if (!m_translations.contains(t)) {
        Scripting::Parley * p = dynamic_cast<Scripting::Parley*>(m_parent);
        if (p) {
            p->callTranslateWord(word, fromLanguage, toLanguage);
        }
    }

    if (m_translations.contains(t))
        return m_translations.value(t);
    else
        return 0;
}
