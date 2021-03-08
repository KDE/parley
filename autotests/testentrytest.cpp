/*
    SPDX-FileCopyrightText: 2019 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "testentrytest.h"

#include "testentry.h"
#include "prefs.h"
#include <QTest>

namespace TestEntryTests{

void TestEntryTest::initTestCase()
{
    /*
     * A TestEntry object is created with following grade levels set:
     *   translation index 0
     *     base - grade 1
     *     article - grade 2
     *     conjugations - grades 3, 4 and 5
     *     comparatives - grades 6 and 7
     *   translation index 1
     *     base - grade 0
     *     article - grade 0
     *     conjugations - grades 0, 0 and 0
     *     comparatives - grades 0, 0 and 0
     */
    m_gradeExpression = makeKEduVocExpressionGrade();
    m_gradeTestEntry = makeTestEntry(m_gradeExpression);

    /*
     * A TestEntry object is created with following pregrade levels set:
     *   translation index 0
     *     base - pregrad 1
     *     article - pregrad 2
     *     conjugations - pregrads 3, 4 and 5
     *     comparatives - pregrads 6 and 7
     *   translation index 1
     *     base - pregrad 0
     *     article - pregrad 0
     *     conjugations - pregrads 0, 0 and 0
     *     comparatives - pregrads 0, 0 and 0
     */
    m_preGradeExpression = makeKEduVocExpressionPreGrade();
    m_preGradeTestEntry = makeTestEntry(m_preGradeExpression);

    // Prevent writing by tast case modified pareleys config file to current users configuration
    QStandardPaths::setTestModeEnabled(true);
}


KEduVocExpression *TestEntryTest::makeKEduVocExpressionGrade()
{
    KEduVocExpression *expression = new KEduVocExpression;
    expression->setTranslation(0, QStringLiteral("translation 0"));
    KEduVocTranslation *translation0 = expression->translation(0);

    translation0->setGrade(KV_LEV1_GRADE);

    KEduVocText text;
    text.setGrade(KV_LEV2_GRADE);
    translation0->setArticle(text);

    KEduVocConjugation conjugation0;
    text.setText(QStringLiteral("conjugation 00"));
    text.setGrade(KV_LEV3_GRADE);
    conjugation0.setConjugation(text, KEduVocWordFlag::Singular | KEduVocWordFlag::First);
    text.setText(QStringLiteral("conjugation 01"));
    text.setGrade(KV_LEV4_GRADE);
    conjugation0.setConjugation(text, KEduVocWordFlag::Singular | KEduVocWordFlag::Second);
    text.setText(QStringLiteral("conjugation 03"));
    text.setGrade(KV_LEV5_GRADE);
    conjugation0.setConjugation(text, KEduVocWordFlag::Singular
                                      | KEduVocWordFlag::Third
                                      | KEduVocWordFlag::Neuter);
    translation0->setConjugation(QStringLiteral("tense"), conjugation0);

    text.setText(QStringLiteral("comparative 0"));
    text.setGrade(KV_LEV6_GRADE);
    translation0->setComparativeForm(text);

    text.setText(QStringLiteral("superlative 0"));
    text.setGrade(KV_LEV7_GRADE);
    translation0->setSuperlativeForm(text);


    expression->setTranslation(1, QStringLiteral("translation 1"));
    KEduVocTranslation *translation1 = expression->translation(1);

    translation1->setArticle(KEduVocText());

    KEduVocConjugation conjugation1;
    conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation 10")),
                                KEduVocWordFlag::Singular | KEduVocWordFlag::First);
    conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation 11")),
                                KEduVocWordFlag::Singular | KEduVocWordFlag::Second);
    conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation 13")),
                                KEduVocWordFlag::Singular | KEduVocWordFlag::Third
                                | KEduVocWordFlag::Neuter);
    translation1->setConjugation(QStringLiteral("tense"), conjugation1);

    translation1->setComparativeForm(KEduVocText(QStringLiteral("comparative 1")));
    translation1->setSuperlativeForm(KEduVocText(QStringLiteral("superlative 1")));

    return expression;
}


KEduVocExpression *TestEntryTest::makeKEduVocExpressionPreGrade()
{
    KEduVocExpression *expression = new KEduVocExpression;
    expression->setTranslation(0, QStringLiteral("translation 0"));
    KEduVocTranslation *translation0 = expression->translation(0);

    translation0->setPreGrade(KV_LEV1_GRADE);

    KEduVocText text;
    text.setPreGrade(KV_LEV2_GRADE);
    translation0->setArticle(text);

    KEduVocConjugation conjugation0;
    text.setText(QStringLiteral("conjugation 00"));
    text.setPreGrade(KV_LEV3_GRADE);
    conjugation0.setConjugation(text, KEduVocWordFlag::Singular | KEduVocWordFlag::First);
    text.setText(QStringLiteral("conjugation 01"));
    text.setPreGrade(KV_LEV4_GRADE);
    conjugation0.setConjugation(text, KEduVocWordFlag::Singular | KEduVocWordFlag::Second);
    text.setText(QStringLiteral("conjugation 03"));
    text.setPreGrade(KV_LEV5_GRADE);
    conjugation0.setConjugation(text, KEduVocWordFlag::Singular
                                      | KEduVocWordFlag::Third
                                      | KEduVocWordFlag::Neuter);
    translation0->setConjugation(QStringLiteral("tense"), conjugation0);

    text.setText(QStringLiteral("comparative 0"));
    text.setPreGrade(KV_LEV6_GRADE);
    translation0->setComparativeForm(text);

    text.setText(QStringLiteral("superlative 0"));
    text.setPreGrade(KV_LEV7_GRADE);
    translation0->setSuperlativeForm(text);


    expression->setTranslation(1, QStringLiteral("translation 1"));
    KEduVocTranslation *translation1 = expression->translation(1);

    translation1->setArticle(KEduVocText());

    KEduVocConjugation conjugation1;
    conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation 10")),
                                KEduVocWordFlag::Singular | KEduVocWordFlag::First);
    conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation 11")),
                                KEduVocWordFlag::Singular | KEduVocWordFlag::Second);
    conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation 13")),
                                KEduVocWordFlag::Singular | KEduVocWordFlag::Third
                                | KEduVocWordFlag::Neuter);

    translation1->setConjugation(QStringLiteral("tense"), conjugation1);
    translation1->setComparativeForm(KEduVocText(QStringLiteral("comparative 1")));
    translation1->setSuperlativeForm(KEduVocText(QStringLiteral("superlative 1")));

    return expression;
}


TestEntry *TestEntryTest::makeTestEntry(KEduVocExpression *expression)
{
    TestEntry *testEntry = new TestEntry(expression);
    testEntry->setLanguageTo(0);
    testEntry->setLanguageFrom(1);
    testEntry->setConjugationTense(QStringLiteral("tense"));
    testEntry->setConjugationPronouns({
        KEduVocWordFlag::Singular | KEduVocWordFlag::First,
        KEduVocWordFlag::Singular | KEduVocWordFlag::Second,
        KEduVocWordFlag::Singular | KEduVocWordFlag::Third | KEduVocWordFlag::Neuter
    });

    return testEntry;
}


void TestEntryTest::cleanupTestCase()
{
    delete m_gradeTestEntry;
    delete m_gradeExpression;
    delete m_preGradeTestEntry;
    delete m_preGradeExpression;

    // Remove temporary parley config file
    if (QStandardPaths::isTestModeEnabled()) {
        QFileInfo prefsFile(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation),
                            Prefs::self()->config()->name());
        if (prefsFile.exists()) {
            QFile::remove(prefsFile.filePath());
        }
    }
}


void TestEntryTest::test_practiceModeDependentMinGrade_written()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::WrittenPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMinGrade(),
             static_cast<grade_t>(KV_LEV1_GRADE));
}


void TestEntryTest::test_practiceModeDependentMinGrade_gender()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::GenderPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMinGrade(),
             static_cast<grade_t>(KV_LEV2_GRADE));
}


void TestEntryTest::test_practiceModeDependentMinGrade_conjugation()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ConjugationPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMinGrade(),
             static_cast<grade_t>(KV_LEV3_GRADE));
}


void TestEntryTest::test_practiceModeDependentMinGrade_comparsion()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ComparisonPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMinGrade(),
             static_cast<grade_t>(KV_LEV6_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxGrade_written()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::WrittenPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMaxGrade(),
             static_cast<grade_t>(KV_LEV1_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxGrade_gender()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::GenderPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMaxGrade(),
             static_cast<grade_t>(KV_LEV2_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxGrade_conjugation()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ConjugationPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMaxGrade(),
             static_cast<grade_t>(KV_LEV5_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxGrade_comparsion()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ComparisonPractice);
    QCOMPARE(m_gradeTestEntry->practiceModeDependentMaxGrade(),
             static_cast<grade_t>(KV_LEV7_GRADE));
}


void TestEntryTest::test_practiceModeDependentMinPreGrade_written()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::WrittenPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMinPreGrade(),
             static_cast<grade_t>(KV_LEV1_GRADE));
}


void TestEntryTest::test_practiceModeDependentMinPreGrade_gender()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::GenderPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMinPreGrade(),
             static_cast<grade_t>(KV_LEV2_GRADE));
}


void TestEntryTest::test_practiceModeDependentMinPreGrade_conjugation()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ConjugationPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMinPreGrade(),
             static_cast<grade_t>(KV_LEV3_GRADE));
}


void TestEntryTest::test_practiceModeDependentMinPreGrade_comparsion()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ComparisonPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMinPreGrade(),
             static_cast<grade_t>(KV_LEV6_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxPreGrade_written()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::WrittenPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMaxPreGrade(),
             static_cast<grade_t>(KV_LEV1_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxPreGrade_gender()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::GenderPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMaxPreGrade(),
             static_cast<grade_t>(KV_LEV2_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxPreGrade_conjugation()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ConjugationPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMaxPreGrade(),
             static_cast<grade_t>(KV_LEV5_GRADE));
}


void TestEntryTest::test_practiceModeDependentMaxPreGrade_comparsion()
{
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ComparisonPractice);
    QCOMPARE(m_preGradeTestEntry->practiceModeDependentMaxPreGrade(),
             static_cast<grade_t>(KV_LEV7_GRADE));
}

}

QTEST_GUILESS_MAIN(TestEntryTests::TestEntryTest)
