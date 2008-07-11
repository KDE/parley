//
// C++ Interface: scriptobjectentry
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTINGSCRIPTOBJECTENTRY_H
#define SCRIPTINGSCRIPTOBJECTENTRY_H

#include <keduvocexpression.h>

#include <QObject>

namespace Scripting
{

    /**
    Entry class to be used by the scripts

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class ScriptObjectEntry : public QObject
    {
            Q_OBJECT
        public:
            ScriptObjectEntry ( KEduVocExpression * entry );
            ScriptObjectEntry ( ScriptObjectEntry & other );

            ~ScriptObjectEntry();
            KEduVocExpression * kEduVocEntry() { return m_entry; }
            
        public slots:
            /// for testing purposes only
            QString getTranslation();

        private:
            KEduVocExpression* m_entry;
    };

}

#endif
