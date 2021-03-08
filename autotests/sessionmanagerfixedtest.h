/*
    SPDX-FileCopyrightText: 2019 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
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

