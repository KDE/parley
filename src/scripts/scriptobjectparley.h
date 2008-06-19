//
// C++ Interface: scriptobjectparley
//
// Description:
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTOBJECTPARLEY_H
#define SCRIPTOBJECTPARLEY_H

#include <QObject>

/**
Implements the object that will be given to Kross scripts to use for accessing parley features

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptObjectParley : public QObject
{
    public:
        ScriptObjectParley();

        ~ScriptObjectParley();
    public Q_SLOTS:
        void callFromScript ();
};

#endif
