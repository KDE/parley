//
// C++ Implementation: scriptobjectdocument
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjectdocument.h"

#include <KDebug>

ScriptObjectDocument::ScriptObjectDocument()
 : QObject()
{
}


ScriptObjectDocument::~ScriptObjectDocument()
{
}


/**
 * Call from script (test function)
 */
void ScriptObjectDocument::callFromScriptTest()
{
    kDebug() << "Document object : Test";
}
