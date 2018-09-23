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

using namespace UnitTestUtilities;

/** This class is a unittest stub with 2 tests that always pass
 */

class AllPass1Test : public QObject
{
    Q_OBJECT


private slots: //must be private

    /** Test that always passes*/
    void testAlwaysPass1();

    /** Test that always passes*/
    void testAlwaysPass2();

};

void AllPass1Test::testAlwaysPass1()
{
    QCOMPARE( true,  true );
}

void AllPass1Test::testAlwaysPass2()
{
    //This just forces these to link and test the CMake file

    MinimalTempVocDoc tempfile;

    KEduVocDocument *docWithLock = new KEduVocDocument();
    int docError = docWithLock->open(tempfile.fileName());
    QCOMPARE( docError, int( KEduVocDocument::NoError ) );

    KUrl kurl = KUrl::fromPath(tempfile.fileName() );
    //ParleyDocument *doc = new ParleyDocument(0);

    QVERIFY( doc != NULL );
}

QTEST_MAIN( AllPass1Test )

#include "allpass1test.moc"
