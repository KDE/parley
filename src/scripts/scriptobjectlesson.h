//
// C++ Interface: scriptobjectlesson
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTINGSCRIPTOBJECTLESSON_H
#define SCRIPTINGSCRIPTOBJECTLESSON_H

#include <keduvoclesson.h>

#include <QObject>

namespace Scripting
{

    /**
    Lesson script object class

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class ScriptObjectLesson : public QObject
    {
            Q_OBJECT
        public:
            ScriptObjectLesson ( KEduVocLesson * lesson );

            ~ScriptObjectLesson();

        private:
            KEduVocLesson* m_lesson;
    };

}

#endif
