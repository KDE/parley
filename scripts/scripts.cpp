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
    QVariant result = action.callFunction("translate",args);
    kDebug() << result;
}
