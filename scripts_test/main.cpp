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
