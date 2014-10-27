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

#include <kross/core/manager.h>
#include <kross/core/action.h>

#include <QDebug>

/*!
    \fn ScriptTest::test()

    Expirement With Kross
 */
void ScriptTest::test()
{
//    foreach(QString s, Kross::Manager::self().interpreters())
//        qDebug() << s;

    Kross::Action action(this, "MyScript");
    action.setFile("myscript.py");
    QVariantList args;
    QVariant result = action.callFunction("testTuple", args);
    qDebug() << result;


    /*Kross::Action action(this,"MyScript");
    action.setFile("myscript.py");
    QVariantList args;
    args << "love";
    QVariant result = action.callFunction("fetchTranslation",args);
    QList<QVariant> translations = result.toList();

    for (int i = 0; i < translations.size(); i++)
        if (translations[i].canConvert<QString>())
            qDebug() << translations[i].toString();
        //if (translations[i].canConvert<QString>()) {*/

    /*qDebug() << result;
    QVariant list_result = action.callFunction("returnlist");
    qDebug() << list_result;
    QList<QVariant> ql = list_result.toList();
    for (int i = 0; i < ql.size(); i++)
        if (ql[i].canConvert<int>())
            qDebug() << ql[i].toInt(NULL);*/
    //foreach (QVariant v, list_result.

}

/*!
    \fn ScriptTest::testJS()
 */
void ScriptTest::testJS()
{
    Kross::Action action(this, "MyJSScript");
    action.setFile("myscript.js");
    QVariantList args;
    args << "hello";
    QVariant script_result = action.callFunction("fetchTranslation", args);
    qDebug() << script_result;
}
