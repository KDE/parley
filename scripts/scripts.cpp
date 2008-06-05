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
    args << "hello";
    QVariant result = action.callFunction("fetchTranslation",args);
    kDebug() << result;
    QVariant list_result = action.callFunction("returnlist");
    kDebug() << list_result;
    QList<QVariant> ql = list_result.toList();
    for (int i = 0; i < ql.size(); i++)
        if (ql[i].canConvert<int>())
            kDebug() << ql[i].toInt(NULL);
    //foreach (QVariant v, list_result.
}
