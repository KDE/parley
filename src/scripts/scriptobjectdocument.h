//
// C++ Interface: scriptobjectdocument
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTOBJECTDOCUMENT_H
#define SCRIPTOBJECTDOCUMENT_H

#include <keduvocdocument.h>

#include <QObject>
#include <KDebug>

namespace Scripting
{

    /**
    Implements the Document object to be used by the scripts

    @code
    import Parley
    doc = Parley.document
    @endcode

        @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    */
    class Document : public QObject
    {
            Q_OBJECT

            Q_PROPERTY ( QString name READ getName WRITE setName )

            /** Read-Only property that gives the root of the lesson tree
            * @code
            * import Parley
            * root = Parley.document.rootLesson
            * @endcode
            */
            Q_PROPERTY ( QObject * rootLesson READ getRootLesson )
        public:
            Document ( KEduVocDocument * doc );

            ~Document();

            QString getName() const { return m_name; }
            void setName ( const QString & name ) { m_name = name; }
            QObject * getRootLesson();

        public slots:
            void callFromScriptTest() { kDebug() << "Document object : Test"; }
            void printName() { kDebug() << m_name; }

        private:
            QString m_name;
            KEduVocDocument * m_doc;
    };

}
#endif
