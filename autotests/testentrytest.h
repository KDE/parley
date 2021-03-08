/*
    SPDX-FileCopyrightText: 2019 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef TESTENTRYTEST_H
#define TESTENTRYTEST_H

#include <QObject>


class KEduVocExpression;
class TestEntry;


namespace TestEntryTests {
/** @file
 * \brief TestEntry test application
 *        Various tests for TestEntry class.
 * @author Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
 */

class TestEntryTest : public QObject
{
Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_practiceModeDependentMinGrade_written();
    void test_practiceModeDependentMinGrade_gender();
    void test_practiceModeDependentMinGrade_conjugation();
    void test_practiceModeDependentMinGrade_comparsion();
    void test_practiceModeDependentMaxGrade_written();
    void test_practiceModeDependentMaxGrade_gender();
    void test_practiceModeDependentMaxGrade_conjugation();
    void test_practiceModeDependentMaxGrade_comparsion();
    void test_practiceModeDependentMinPreGrade_written();
    void test_practiceModeDependentMinPreGrade_gender();
    void test_practiceModeDependentMinPreGrade_conjugation();
    void test_practiceModeDependentMinPreGrade_comparsion();
    void test_practiceModeDependentMaxPreGrade_written();
    void test_practiceModeDependentMaxPreGrade_gender();
    void test_practiceModeDependentMaxPreGrade_conjugation();
    void test_practiceModeDependentMaxPreGrade_comparsion();

private:
    KEduVocExpression *makeKEduVocExpressionGrade();
    KEduVocExpression *makeKEduVocExpressionPreGrade();
    TestEntry *makeTestEntry(KEduVocExpression *expression);

private:
    KEduVocExpression *m_gradeExpression = nullptr;
    TestEntry *m_gradeTestEntry = nullptr;
    KEduVocExpression *m_preGradeExpression = nullptr;
    TestEntry *m_preGradeTestEntry = nullptr;
};
}

#endif // TESTENTRYTEST_H

