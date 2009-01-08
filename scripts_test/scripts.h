/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SCRIPTS_TEST_SCRIPTS_H
#define SCRIPTS_TEST_SCRIPTS_H

#include <QObject>

class ScriptTest
    :public QObject /* necessary to instantiate Kross::Action */
{
public:
    
    void test();
    void testJS();
};

#endif
