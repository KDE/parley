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
#include "translationscript.h"

#include <KDebug>
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
    \fn ScriptTest::getLanguagePairs()
 */
void TranslationScript::getLanguagePairs()
{
    QVariant script_result = action->callFunction("getLanguagePairs");

    foreach(const QVariant & langpair, script_result.toList()) {
        QVariantList pair = langpair.toList();
        QString from = pair[0].toString();
        QString to = pair[1].toString();
//         kDebug() << from << " -> " << to;
    }
}

/*!
    \fn ScriptTest::translateWord(QString Word)
 */
QStringList TranslationScript::translateWord(QString word, QString from, QString to)
{
    QVariantList args;
    args << word << from << to;
    QVariant script_result = action->callFunction("fetchTranslation", args);
    QList<QVariant> translations = script_result.toList();

    QStringList result;

    for (int i = 0; i < translations.size(); i++) {
//         kDebug() << translations[i].toString() << " " << translations[i].toString().length();
        result.push_back(translations[i].toString());
    }

    return result;
}
