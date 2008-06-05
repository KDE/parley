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
    foreach(QString s, Kross::Manager::self().interpreters())
        kDebug() << s;

    Kross::Action action(this,"MyScript");
    action.setFile("myscript.py");
    QVariantList args;
    args << "love";
    QVariant result = action.callFunction("fetchTranslation",args);
    QList<QVariant> translations = result.toList();

    for (int i = 0; i < translations.size(); i++)
        if (translations[i].canConvert<QString>())
            kDebug() << translations[i].toString();
        //if (translations[i].canConvert<QString>()) {

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
QStringList ScriptTest::translateWord(QString word)
{
    Kross::Action action(this,"MyScript");
    action.setFile("myscript.py");
    QVariantList args;
    args << word;
    QVariant script_result = action.callFunction("fetchTranslation",args);
    QList<QVariant> translations = script_result.toList();

    QStringList result;

    for (int i = 0; i < translations.size(); i++)
        result.push_back(translations[i].toString());

//     kDebug() << result;
    return result;
}
