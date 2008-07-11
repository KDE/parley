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

#include "scriptobjectentry.h"

#include <QObject>
#include <KSharedPtr>

namespace Scripting
{

    /**
    Lesson script object class

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class ScriptObjectLesson : public QObject
    {
            Q_OBJECT
            /// name of the lesson (Read/Write)
            Q_PROPERTY ( QString name READ getName WRITE setName )
            /// entries (Read Only)
//             Q_PROPERTY ( QList<QObject*> entries READ getEntries )

        public:
            ScriptObjectLesson ( KEduVocLesson * lesson );
            QString getName() { return m_lesson->name(); }
            void setName ( const QString & name ) { m_lesson->setName ( name ); }


            ~ScriptObjectLesson();

        public slots:
//             QList<ScriptObjectEntry> getEntries();
            QList<QVariant> getEntries();
            ScriptObjectEntry * entry(int row, bool recursive = false);
            int entryCount(bool recursive = false);
            void appendEntry(ScriptObjectEntry * entry);
            void insertEntry(int index, ScriptObjectEntry * entry);
            void removeEntry(QObject * entry);

        private:
            KEduVocLesson* m_lesson;
    };

}

#endif
