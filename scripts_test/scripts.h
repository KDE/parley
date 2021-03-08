/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SCRIPTS_TEST_SCRIPTS_H
#define SCRIPTS_TEST_SCRIPTS_H

#include <QObject>

class ScriptTest : public QObject /* necessary to instantiate Kross::Action */
{
public:

    void test();
    void testJS();
};

#endif
