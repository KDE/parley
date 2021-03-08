/*
    SPDX-FileCopyrightText: 2014 Andreas Xavier <andxav at zoho dot com>
    SPDX-License-Identifier: GPL-2.0-or-later
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
