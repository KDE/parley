/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "scripts.h"
#include "translationscript.h"

#include <QStringList>
#include <QTextCodec>

#include <QDebug>

int main()
{
    //QTextCodec* Utf8Codec  = QTextCodec::codecForName("utf-8");

    //TranslationScript script("google_translation.py");
    //script.getLanguagePairs();
    //qDebug() << script.translateWord("car","en","it");

    TranslationScript s("ectaco.py");
//     qDebug() << s.translateWord("test","en","pl");
    foreach(const QString & word, s.translateWord("test", "en", "pl"))
    qDebug() << "trns: " << word;

    return 0;
}
