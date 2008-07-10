//
// C++ Implementation: scriptobjectlesson
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scriptobjectlesson.h"

namespace Scripting
{

    ScriptObjectLesson::ScriptObjectLesson(KEduVocLesson * lesson)
            : QObject()
    {
        m_lesson = lesson;
    }


    ScriptObjectLesson::~ScriptObjectLesson()
    {
    }


}
