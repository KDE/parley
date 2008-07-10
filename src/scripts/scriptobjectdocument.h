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
    public:
        ScriptObjectDocument();

        ~ScriptObjectDocument();

    public slots:
        void callFromScriptTest();
};

#endif
