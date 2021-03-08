/*
    SPDX-FileCopyrightText: 2019 Hartmut Riesenbeck <hartmut.riesenbeck@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "sessionmanagerfixedtest.h"

#include "sessionmanagerfixed.h"
#include "prefs.h"
#include "documentsettings.h"
#include <KEduVocDocument>
#include <KEduVocWordtype>
#include <QTest>


namespace SessionManagerFixedTests
{

void SessionManagerFixedTest::initTestCase()
{
    // Prevent writing by tast case modified pareleys config file to current users configuration
    QStandardPaths::setTestModeEnabled(true);
}


void SessionManagerFixedTest::cleanupTestCase()
{
    if (QStandardPaths::isTestModeEnabled()) {
        QFileInfo prefsFile(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation),
                            Prefs::self()->config()->name());
        if (prefsFile.exists()) {
            QFile::remove(prefsFile.filePath());
        }
    }
}


void SessionManagerFixedTest::setupBasicPreferences() const
{
    Prefs::setLearningLanguage(0);
    Prefs::setKnownLanguage(1);
    Prefs::setPracticeDirection(Prefs::EnumPracticeDirection::KnownToLearning);
    Prefs::setAllowImageInsteadOfWord(false);
    Prefs::setExpire(false);
    Prefs::setWordTypesInPracticeEnabled(false);
    Prefs::setBlock(true);
    Prefs::setPracticeMinimumWrongCount(0);
    Prefs::setPracticeMaximumWrongCount(1000);
    Prefs::setPracticeMinimumTimesAsked(0);
    Prefs::setPracticeMaximumTimesAsked(1000);
    Prefs::setPracticeMinimumGrade(0);
    Prefs::setPracticeMaximumGrade(7);
    Prefs::setSessionMaxSize(20);
}


void SessionManagerFixedTest::initDocumentPracticeModeDependent(KEduVocDocument &doc,
                                                                int nEntries) const
{
    KEduVocWordType *root = doc.wordTypeContainer();
    KEduVocWordType *noun = new KEduVocWordType(QStringLiteral("Noun"), root);
    noun->setWordType(KEduVocWordFlag::Noun);
    root->appendChildContainer(noun);

    KEduVocWordType *nounChild = new KEduVocWordType(QStringLiteral("Masculine"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Masculine);
    noun->appendChildContainer(nounChild);
    nounChild = new KEduVocWordType(QStringLiteral("Feminine"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Feminine);
    noun->appendChildContainer(nounChild);
    nounChild = new KEduVocWordType(QStringLiteral("Neuter"), noun);
    nounChild->setWordType(KEduVocWordFlag::Noun | KEduVocWordFlag::Neuter);
    noun->appendChildContainer(nounChild);

    KEduVocWordType *verb = new KEduVocWordType(QStringLiteral("Verb"), root);
    verb->setWordType(KEduVocWordFlag::Verb);
    root->appendChildContainer(verb);

    KEduVocWordType *adjective = new KEduVocWordType(QStringLiteral("Adjective"), root);
    adjective->setWordType(KEduVocWordFlag::Adjective);
    root->appendChildContainer(adjective);

    KEduVocLesson *lesson = new KEduVocLesson(QStringLiteral("Lesson 1"), doc.lesson());
    doc.lesson()->appendChildContainer(lesson);

    doc.appendIdentifier();
    doc.appendIdentifier();
    doc.identifier(0).setName(QStringLiteral("Languge 0"));
    doc.identifier(0).setLocale(QStringLiteral("l0"));
    doc.identifier(1).setName(QStringLiteral("Languge 1"));
    doc.identifier(1).setLocale(QStringLiteral("l1"));

    for (int i = 0; i < nEntries ; i++) {
        KEduVocExpression *expression = new KEduVocExpression;
        expression->setTranslation(0, QStringLiteral("translation 0 word %1").arg(i));
        expression->setTranslation(1, QStringLiteral("translation 1 word %1").arg(i));
        lesson->appendEntry(expression);

        if (Prefs::practiceMode() == Prefs::EnumPracticeMode::GenderPractice) {
            auto container = doc.wordTypeContainer()->childOfType(KEduVocWordFlag::Noun
                                                                  | KEduVocWordFlag::Masculine);
            expression->translation(0)->setWordType(container);
            expression->translation(1)->setWordType(container);

            // Set top entry vocabel to already practiced and leave article as new
            expression->translation(0)->setGrade(KV_LEV1_GRADE);
            expression->translation(1)->setGrade(KV_LEV1_GRADE);
        }

        if (Prefs::practiceMode() == Prefs::EnumPracticeMode::ConjugationPractice) {
            KEduVocConjugation conjugation0;
            conjugation0.setConjugation(KEduVocText(QStringLiteral("conjugation %1 00").arg(i)),
                                        KEduVocWordFlag::Singular | KEduVocWordFlag::First);
            conjugation0.setConjugation(KEduVocText(QStringLiteral("conjugation %1 01").arg(i)),
                                        KEduVocWordFlag::Singular | KEduVocWordFlag::Second);
            conjugation0.setConjugation(KEduVocText(QStringLiteral("conjugation %1 03").arg(i)),
                                        KEduVocWordFlag::Singular | KEduVocWordFlag::Third
                                        | KEduVocWordFlag::Neuter);
            expression->translation(0)->setConjugation(QStringLiteral("tense"), conjugation0);

            KEduVocConjugation conjugation1;
            conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation %1 10").arg(i)),
                                        KEduVocWordFlag::Singular | KEduVocWordFlag::First);
            conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation %1 11").arg(i)),
                                        KEduVocWordFlag::Singular | KEduVocWordFlag::Second);
            conjugation1.setConjugation(KEduVocText(QStringLiteral("conjugation %1 13").arg(i)),
                                        KEduVocWordFlag::Singular | KEduVocWordFlag::Third
                                        | KEduVocWordFlag::Neuter);
            expression->translation(1)->setConjugation(QStringLiteral("tense"), conjugation1);

            auto container = doc.wordTypeContainer()->childOfType(KEduVocWordFlag::Verb);
            expression->translation(0)->setWordType(container);
            expression->translation(1)->setWordType(container);

            // Set top level entry to already practiced and leave conjugations as new
            expression->translation(0)->setGrade(KV_LEV1_GRADE);
            expression->translation(1)->setGrade(KV_LEV1_GRADE);
        }

        if (Prefs::practiceMode() == Prefs::EnumPracticeMode::ComparisonPractice) {
            expression->translation(0)->setComparativeForm(
                KEduVocText(QStringLiteral("comparative %1 0").arg(i)));
            expression->translation(0)->setSuperlativeForm(
                KEduVocText(QStringLiteral("superlative %1 0").arg(i)));
            expression->translation(1)->setComparativeForm(
                KEduVocText(QStringLiteral("comparative %1 1").arg(i)));
            expression->translation(1)->setSuperlativeForm(
                KEduVocText(QStringLiteral("superlative %1 1").arg(i)));

            auto container = doc.wordTypeContainer()->childOfType(KEduVocWordFlag::Adjective);
            expression->translation(0)->setWordType(container);
            expression->translation(1)->setWordType(container);

            // Set top level entry to already practiced and leave comparative forms as new
            expression->translation(0)->setGrade(KV_LEV1_GRADE);
            expression->translation(1)->setGrade(KV_LEV1_GRADE);
        }
    }

    doc.setUrl(QUrl::fromLocalFile(QStringLiteral("dummy_fquJ4CM7.kvtml")));
    doc.setModified(false);
}


void SessionManagerFixedTest::test_maximumNumberOfNewWordsPerSession_written()
{
    constexpr int MAX_NUMBER_OF_NEW_WORDS = 5;

    setupBasicPreferences();
    Prefs::setSessionMaxNewWords(MAX_NUMBER_OF_NEW_WORDS);
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::WrittenPractice);

    KEduVocDocument doc;
    // Fill document with 10 unpracticed entries appropriate to current practice mode
    initDocumentPracticeModeDependent(doc, 10);

    QWidget dummy;
    Practice::SessionManagerFixed sessionManager(&dummy);
    sessionManager.setDocument(&doc);

    QCOMPARE(sessionManager.allEntryCount(), MAX_NUMBER_OF_NEW_WORDS);
}


void SessionManagerFixedTest::test_maximumNumberOfNewWordsPerSession_gender()
{
    constexpr int MAX_NUMBER_OF_NEW_WORDS = 5;

    setupBasicPreferences();
    Prefs::setSessionMaxNewWords(MAX_NUMBER_OF_NEW_WORDS);
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::GenderPractice);

    KEduVocDocument doc;
    // Fill document with 10 unpracticed entries appropriate to current practice mode
    initDocumentPracticeModeDependent(doc, 10);

    QWidget dummy;
    Practice::SessionManagerFixed sessionManager(&dummy);
    sessionManager.setDocument(&doc);

    QCOMPARE(sessionManager.allEntryCount(), MAX_NUMBER_OF_NEW_WORDS);
}


void SessionManagerFixedTest::test_maximumNumberOfNewWordsPerSession_conjugation()
{
    constexpr int MAX_NUMBER_OF_NEW_WORDS = 5;

    setupBasicPreferences();
    Prefs::setSessionMaxNewWords(MAX_NUMBER_OF_NEW_WORDS);
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ConjugationPractice);

    KEduVocDocument doc;
    // Fill document with 10 unpracticed entries appropriate to current practice mode
    initDocumentPracticeModeDependent(doc, 10);

    DocumentSettings documentSettings(doc.url().url() + QStringLiteral("0"));
    documentSettings.load();
    documentSettings.setConjugationTenses({QStringLiteral("tense")});
    documentSettings.save();

    QWidget dummy;
    Practice::SessionManagerFixed sessionManager(&dummy);
    sessionManager.setDocument(&doc);

    QCOMPARE(sessionManager.allEntryCount(), MAX_NUMBER_OF_NEW_WORDS);
}


void SessionManagerFixedTest::test_maximumNumberOfNewWordsPerSession_comparsion()
{
    constexpr int MAX_NUMBER_OF_NEW_WORDS = 5;

    setupBasicPreferences();
    Prefs::setSessionMaxNewWords(MAX_NUMBER_OF_NEW_WORDS);
    Prefs::setPracticeMode(Prefs::EnumPracticeMode::ComparisonPractice);

    KEduVocDocument doc;
    // Fill document with 10 unpracticed entries appropriate to current practice mode
    initDocumentPracticeModeDependent(doc, 10);

    QWidget dummy;
    Practice::SessionManagerFixed sessionManager(&dummy);
    sessionManager.setDocument(&doc);

    QCOMPARE(sessionManager.allEntryCount(), MAX_NUMBER_OF_NEW_WORDS);
}

}

QTEST_MAIN(SessionManagerFixedTests::SessionManagerFixedTest)
