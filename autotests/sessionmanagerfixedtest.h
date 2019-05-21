/***************************************************************************
 *   Copyright (C) 2019 by Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef SESSIONMANAGERFIXEDTEST_H
#define SESSIONMANAGERFIXEDTEST_H

#include <QObject>

class KEduVocDocument;

namespace SessionManagerFixedTests {
/** @file
 * \brief SessionManagerFixedTest test application
 *        Various tests for SessionManagerFixed class.
 * @author Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
 */

class SessionManagerFixedTest : public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_maximumNumberOfNewWordsPerSession_written();
    void test_maximumNumberOfNewWordsPerSession_gender();
    void test_maximumNumberOfNewWordsPerSession_conjugation();
    void test_maximumNumberOfNewWordsPerSession_comparsion();

private:
    void setupBasicPreferences() const;
    void initDocumentPracticeModeDependent(KEduVocDocument &doc, int nEntries) const;

};
}

#endif // SESSIONMANAGERFIXEDTEST_H

