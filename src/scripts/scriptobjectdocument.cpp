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

namespace Scripting {

ScriptObjectDocument::ScriptObjectDocument(KEduVocDocument * doc)
 : QObject()
{
    m_doc = doc;
}


ScriptObjectDocument::~ScriptObjectDocument()
{
}

}