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

/**
Implements the Document object to be used by the scripts

@code
import Parley
doc = Parley.document()
@endcode

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptObjectDocument : public QObject
{
        Q_OBJECT

        Q_PROPERTY ( QString name READ getName WRITE setName )

    public:
        ScriptObjectDocument ( KEduVocDocument * doc );

        ~ScriptObjectDocument();

        QString getName() const { return m_name; }
        void setName ( const QString & name ) { m_name = name; }

    public slots:
        /**
        * Call from script (test function)
        */
        void callFromScriptTest();
        void printName();

    private:
        QString m_name;
        KEduVocDocument * m_doc;
};

#endif
