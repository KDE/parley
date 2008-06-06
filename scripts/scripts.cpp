#include "scripts.h"

#include <kross/core/manager.h>
#include <kross/core/action.h>

#include <KDebug>

/*!
    \fn ScriptTest::test()

    Expirement With Kross
 */
void ScriptTest::test()
{
//    foreach(QString s, Kross::Manager::self().interpreters())
//        kDebug() << s;

    Kross::Action action(this,"MyScript");
    action.setFile("myscript.py");
    QVariantList args;
    QVariant result = action.callFunction("testTuple",args);
    kDebug() << result;


    /*Kross::Action action(this,"MyScript");
    action.setFile("myscript.py");
    QVariantList args;
    args << "love";
    QVariant result = action.callFunction("fetchTranslation",args);
    QList<QVariant> translations = result.toList();

    for (int i = 0; i < translations.size(); i++)
        if (translations[i].canConvert<QString>())
            kDebug() << translations[i].toString();
        //if (translations[i].canConvert<QString>()) {*/

    /*kDebug() << result;
    QVariant list_result = action.callFunction("returnlist");
    kDebug() << list_result;
    QList<QVariant> ql = list_result.toList();
    for (int i = 0; i < ql.size(); i++)
        if (ql[i].canConvert<int>())
            kDebug() << ql[i].toInt(NULL);*/
    //foreach (QVariant v, list_result.

}


/*!
    \fn ScriptTest::translateWord(QString Word)
 */
QStringList ScriptTest::translateWord(QString word, QString from, QString to)
{
    Kross::Action action(this,"MyScript");
    action.setFile("google_translation.py");
    QVariantList args;
    args << word << from << to;
    QVariant script_result = action.callFunction("fetchTranslation",args);
    QList<QVariant> translations = script_result.toList();

    QStringList result;

    for (int i = 0; i < translations.size(); i++)
        result.push_back(translations[i].toString());

    return result;
}


/*!
    \fn ScriptTest::testJS()
 */
void ScriptTest::testJS()
{
    Kross::Action action(this,"MyJSScript");
    action.setFile("myscript.js");
    QVariantList args;
    args << "hello";
    QVariant script_result = action.callFunction("fetchTranslation",args);
    kDebug() << script_result;
}


/*!
    \fn ScriptTest::getLanguagePairs()
 */
void ScriptTest::getLanguagePairs()
{
    Kross::Action action(this,"MyScript2");
    action.setFile("google_translation.py");
    QVariant script_result = action.callFunction("getLanguagePairs");

    foreach(QVariant langpair, script_result.toList()) {
        QVariantList pair = langpair.toList();
        QString from = pair[0].toString();
        QString to = pair[1].toString();
        kDebug() << from << " -> " << to;
    }
}
