/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "translationscript.h"

#include <QDebug>
#include <QVariant>

TranslationScript::TranslationScript(QString filename)
{
    action = new Kross::Action(this, "Action");
    action->setFile(filename);
}


TranslationScript::~TranslationScript()
{
}

/*!
    \fn TranslationScript::getLanguagePairs()
 */
void TranslationScript::getLanguagePairs()
{
    QVariant script_result = action->callFunction("getLanguagePairs");

    foreach(const QVariant & langpair, script_result.toList()) {
        QVariantList pair = langpair.toList();
        QString from = pair[0].toString();
        QString to = pair[1].toString();
//         qDebug() << from << " -> " << to;
    }
}

/*!
    \fn TranslationScript::translateWord(QString Word)
 */
QStringList TranslationScript::translateWord(QString word, QString from, QString to)
{
    QVariantList args;
    args << word << from << to;
    QVariant script_result = action->callFunction("fetchTranslation", args);
    QList<QVariant> translations = script_result.toList();

    QStringList result;

    for (int i = 0; i < translations.size(); i++) {
//         qDebug() << translations[i].toString() << " " << translations[i].toString().length();
        result.push_back(translations[i].toString());
    }

    return result;
}
