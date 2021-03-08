/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "parleyunittestutilities.h"

#include "parleydocument.h"

#include <QTest>

/** This class is a unittest stub with 2 tests that always pass
 */

class AllPass2Test : public QObject
{
    Q_OBJECT


private slots: //must be private

    /** Test that always passes*/
    void testAlwaysPass1();

    /** Test that always passes*/
    void testAlwaysPass2();

};

void AllPass2Test::testAlwaysPass1()
{
    QCOMPARE( true,  true );
}

void AllPass2Test::testAlwaysPass2()
{
    QCOMPARE( true,  true );
}

QTEST_APPLESS_MAIN( AllPass2Test )

#include "allpass2test.moc"
