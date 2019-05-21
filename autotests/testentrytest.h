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

