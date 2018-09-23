/*
 Copyright 2014 Andreas Xavier <andxav at zoho dot com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 USA
*/

#include "parleyunittestutilities.h"

#include "parleydocument.h"

#include <QtTest>

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
