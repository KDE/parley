/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "entryfilter.h"

#include <QPushButton>
#include <QDebug>

#include <KMessageBox>
#include <KLocalizedString>
#include <KRandom>

#include <keduvocdocument.h>
#include <keduvocwordtype.h>
#include <keduvocexpression.h>

#include "documentsettings.h"
#include "testentry.h"

//using namespace Practice;


// Blocking times for pregrade levels.
//
// It could be argued that these should be configurable but I am not
// sure what that would bring us or the user. Definitely not improved
// learning...
//
// FIXME: Find out what the optimal values are.
int preGradeTimes[] = {
    0,
    3 * 60 + 30,                // 1: 3.5 minutes
    7 * 60,                     // 2: 7 minutes
    15 * 60,                    // 3: 15 minutes
    1 * 3600,                   // 4: 1 hour
    2 * 3600,                   // 5: 2 hours
    4 * 3600,                   // 6: 4 hours
    8 * 3600,                   // 7: 8 hours
};


EntryFilter::EntryFilter(KEduVocDocument* doc, QObject * parent)
    : QObject(parent)
    , m_doc(doc)
    , m_dialog(0)
{
    if (Prefs::practiceMode() == Prefs::EnumPracticeMode::ConjugationPractice) {
        DocumentSettings documentSettings(m_doc->url().url() + QString::number(m_toTranslation));
        documentSettings.load();
        m_tenses = documentSettings.conjugationTenses();
        qDebug() << "Tenses" << m_tenses;
    }
}

static void debugEntry(const QString &comment, KEduVocExpression *vocexp,
                       KEduVocTranslation *from, KEduVocTranslation *to)
{
    Q_UNUSED(vocexp);

    qDebug() << comment << "from" << from->text() << "to" << to->text();
}

QList<TestEntry*> EntryFilter::entries(bool showDialog)
{
    switch (Prefs::practiceDirection()) {
    case Prefs::EnumPracticeDirection::KnownToLearning:
        m_numSets = 1;
        m_fromTranslation = Prefs::knownLanguage();
        m_toTranslation   = Prefs::learningLanguage();
        break;
    case Prefs::EnumPracticeDirection::LearningToKnown:
        m_numSets = 1;
        m_fromTranslation = Prefs::learningLanguage();
        m_toTranslation   = Prefs::knownLanguage();
        break;
    case Prefs::EnumPracticeDirection::MixedDirectionsWordsOnly:
        m_numSets = 2;
        m_fromTranslation = Prefs::knownLanguage();
        m_toTranslation   = Prefs::learningLanguage();
        break;
    case Prefs::EnumPracticeDirection::MixedDirectionsWithSound:
        // FIXME: Not yet supported. Use same settings as MixedModeWordsOnly
        m_numSets = 2;
        m_fromTranslation = Prefs::knownLanguage();
        m_toTranslation   = Prefs::learningLanguage();
        break;
    default:
        // Use KnownToLearning as default.
        m_numSets = 1;
        m_fromTranslation = Prefs::knownLanguage();
        m_toTranslation   = Prefs::learningLanguage();
        break;
    }

    for (int pass = 0; pass < m_numSets; ++pass) {
        // If we only do one pass, then from/to translation are already set.
        // But in mixed mode we need to set up pass 1 and later.
        if (pass == 1) {
            m_fromTranslation = Prefs::learningLanguage();
            m_toTranslation   = Prefs::knownLanguage();
        }

	//qDebug() << "Filter for " << m_fromTranslation << " to " << m_toTranslation;
        expireEntries(pass);

        collectEntries(pass);
    }

    static QString noEntriesError =
        i18n("The vocabulary document contains no entries that can be used for the chosen type"
             " of practice.");

    //qDebug() << "Document contains " << m_entries[0].count() + m_entries[1].count() << " valid entries.";
    if (m_entries[0].count() + m_entries[1].count() == 0) {
	if (showDialog) {
	    KMessageBox::error(0, noEntriesError);
	}
        return QList<TestEntry*>();
    }

    updateTotal();

    bool ignoreBlocked = false;
    int numSelected = m_currentSelection[0].count() + m_currentSelection[1].count();
    if (numSelected == 0 && showDialog) {
        m_button_dialog = new QDialogButtonBox;
        m_button_dialog->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

        QWidget *widget = new QWidget();
        ui.setupUi(widget);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget( widget );
        layout->addWidget( m_button_dialog );

        m_dialog = new QDialog;
        m_dialog->setLayout( layout );
        m_dialog->setWindowTitle(i18n("Start Practice"));

        connect(m_button_dialog, &QDialogButtonBox::accepted, m_dialog, &QDialog::accept);
        connect(m_button_dialog, &QDialogButtonBox::rejected, m_dialog, &QDialog::reject);


        int numEntries = m_entries[0].count() + m_entries[1].count();
        ui.lessonLabel        ->setText(QString::number(numEntries
                                                        - m_entriesLesson[0].count()
                                                        - m_entriesLesson[1].count()));
        ui.wordTypeLabel      ->setText(QString::number(numEntries
                                                        - m_entriesWordType[0].count()
                                                        - m_entriesWordType[1].count()));
        ui.blockedLabel       ->setText(QString::number(numEntries
                                                        - m_entriesNotBlocked[0].count()
                                                        - m_entriesNotBlocked[1].count()));
        ui.timesWrongLabel    ->setText(QString::number(numEntries
                                                        - m_entriesTimesWrong[0].count()
                                                        - m_entriesTimesWrong[1].count()));
        ui.timesPracticedLabel->setText(QString::number(numEntries
                                                        - m_entriesTimesPracticed[0].count()
                                                        - m_entriesTimesPracticed[1].count()));
        ui.minMaxGradeLabel   ->setText(QString::number(numEntries
                                                        - m_entriesMinMaxGrade[0].count()
                                                        - m_entriesMinMaxGrade[1].count()));

        ui.documentTotalLabel->setText(QString::number(numEntries));
        updateTotal();

        ui.lessonCheckBox->setChecked(m_entriesLesson[0].count() + m_entriesLesson[1].count() == 0);
        ui.wordTypeCheckBox->setChecked(m_entriesWordType[0].count() + m_entriesWordType[1].count() == 0);
        ui.blockedCheckBox->setChecked(m_entriesNotBlocked[0].count() + m_entriesNotBlocked[1].count() == 0);
        ui.timesWrongCheckBox->setChecked(m_entriesTimesWrong[0].count() + m_entriesTimesWrong[1].count() == 0);
        ui.timesPracticedCheckBox->setChecked(m_entriesTimesPracticed[0].count() + m_entriesTimesPracticed[1].count() == 0);
        ui.minMaxGradeCheckBox->setChecked(m_entriesMinMaxGrade[0].count() + m_entriesMinMaxGrade[1].count() == 0);

        connect(ui.lessonCheckBox, &QAbstractButton::toggled, this, &EntryFilter::checkBoxChanged);
        connect(ui.wordTypeCheckBox, &QAbstractButton::toggled, this, &EntryFilter::checkBoxChanged);
        connect(ui.blockedCheckBox, &QAbstractButton::toggled, this, &EntryFilter::checkBoxChanged);
        connect(ui.timesWrongCheckBox, &QAbstractButton::toggled, this, &EntryFilter::checkBoxChanged);
        connect(ui.timesPracticedCheckBox, &QAbstractButton::toggled, this, &EntryFilter::checkBoxChanged);
        connect(ui.minMaxGradeCheckBox, &QAbstractButton::toggled, this, &EntryFilter::checkBoxChanged);

        updateTotal();

        if (!Prefs::wordTypesInPracticeEnabled()) {
            ui.wordTypeLabel->setVisible(false);
            ui.wordTypeCheckBox->setVisible(false);
        }

        if (m_dialog->exec() == QDialog::Rejected) {
            delete m_dialog;
            return QList<TestEntry*>();
        }
        ignoreBlocked = ui.blockedCheckBox->isChecked();
        delete m_dialog;
    }

    // Finally, create the list of test entries from the selected
    // lists of EduVocTranslations.
    if (Prefs::practiceMode() == Prefs::EnumPracticeMode::ConjugationPractice) {
        QList< TestEntry* > ret = conjugationTestEntries(ignoreBlocked);
        if (ret.count() == 0) {
            KMessageBox::error(0, noEntriesError);
        }
        return ret;
    } else {
        // FIXME: Create entries already from the beginning so we
        //        don't have to work with kvtml translations first and
        //        then entries later.
        QList<TestEntry*> testEntries;
        for (int setNo = 0; setNo < m_numSets; ++setNo) {
            int from;
            int to;

            if (setNo == 0) {
                switch (Prefs::practiceDirection()) {
                case Prefs::EnumPracticeDirection::KnownToLearning:
                    from = Prefs::knownLanguage();
                    to   = Prefs::learningLanguage();
                    break;
                case Prefs::EnumPracticeDirection::LearningToKnown:
                    from = Prefs::learningLanguage();
                    to   = Prefs::knownLanguage();
                    break;
                case Prefs::EnumPracticeDirection::MixedDirectionsWordsOnly:
                case Prefs::EnumPracticeDirection::MixedDirectionsWithSound:
                default:
                    from = Prefs::knownLanguage();
                    to   = Prefs::learningLanguage();
                    break;
                }
            }
            else {
                from = Prefs::learningLanguage();
                to   = Prefs::knownLanguage();
            }

            foreach(KEduVocExpression * entry, m_currentSelection[setNo]) {
                // Set the from and to translation for the entry itself.
                TestEntry *testEntry = new TestEntry(entry);

                testEntry->setLanguageFrom(from);
                testEntry->setLanguageTo(to);

                randomizedInsert(testEntries, testEntry);
            }
        }
        return testEntries;
    }
}


void EntryFilter::expireEntries(int setNo)
{
    if (Prefs::expire()) {
        int counter = 0;
        foreach(KEduVocExpression * entry, m_entries[setNo]) {
            int grade = entry->translation(m_toTranslation)->grade();

            const QDateTime &date =  entry->translation(m_toTranslation)->practiceDate();

            const QDateTime &expireDate = QDateTime::currentDateTime().addSecs(-Prefs::expireItem(grade));

            if (date < expireDate && grade > 0) {
                // decrease the grade
                entry->translation(m_toTranslation)->decGrade();

                // prevent from endless dropping
                entry->translation(m_toTranslation)->setPracticeDate(QDateTime::currentDateTime().addSecs(-Prefs::expireItem(grade - 2)));
                counter++;
            }
        }
        qDebug() << "Expired words dropped their confidence: " << counter;
    }
}


void EntryFilter::collectEntries(int setNo)
{
    // Set up the lists/sets of filtered vocabulary
    m_entries[setNo] = m_doc->lesson()->entries(KEduVocLesson::Recursive).toSet();
    cleanupInvalid(setNo);

    // FIXME the filtering needs to be done for each word or the grammar modes get included with written or somesuch

    /* FIXME
    if (Prefs::genderPractice())
    {
        if (m_doc->identifier(m_toTranslation).article().isEmpty())
        {
            KMessageBox::error(0, i18n("The vocabulary document contains no articles for the current language. Please add some in the Edit->Grammar menu."));
            return QList<KEduVocExpression*>();
        }
    }
    */

    lessonEntries(setNo);
    wordTypeEntries(setNo);
    blockedEntries(setNo);
    timesWrongEntries(setNo);
    timesPracticedEntries(setNo);
    minMaxGradeEntries(setNo);
}


void EntryFilter::lessonEntries(int setNo)
{
    foreach(KEduVocExpression * entry, m_entries[setNo]) {
        if (entry->lesson()->inPractice()) {
            m_entriesLesson[setNo].insert(entry);
        }
    }
}

void EntryFilter::wordTypeEntries(int setNo)
{
    if (Prefs::wordTypesInPracticeEnabled()) {
        foreach(KEduVocExpression * entry, m_entries[setNo]) {
            if (entry->translation(m_toTranslation)->wordType()) {
                if (entry->translation(m_toTranslation)->wordType()->inPractice()) {
                    m_entriesWordType[setNo].insert(entry);
                }
            }
        }
    } else {
        m_entriesWordType[setNo] = m_entries[setNo];
    }
}

void EntryFilter::blockedEntries(int setNo)
{
    if (!Prefs::block()) {
        m_entriesNotBlocked[setNo] = m_entries[setNo];
        return;
    }

    switch (Prefs::practiceMode()) {
    case Prefs::EnumPracticeMode::ConjugationPractice:
        foreach(KEduVocExpression * entry, m_entries[setNo]) {
            if (!isConjugationBlocked(entry->translation(m_toTranslation))) {
                m_entriesNotBlocked[setNo].insert(entry);
            }
        }
        break;
    case Prefs::EnumPracticeMode::GenderPractice:
        foreach(KEduVocExpression * entry, m_entries[setNo]) {
            KEduVocText article = entry->translation(m_toTranslation)->article();
            if (!isBlocked(&article)) {
                m_entriesNotBlocked[setNo].insert(entry);
            }
        }
        break;
    case Prefs::EnumPracticeMode::ComparisonPractice:
        foreach(KEduVocExpression * entry, m_entries[setNo]) {
            KEduVocTranslation* translation = entry->translation(m_toTranslation);
            KEduVocText comparative = translation->comparativeForm();
            KEduVocText superlative = translation->superlativeForm();
            if (!isBlocked(&(comparative))
                    || !isBlocked(&(superlative))) {
                m_entriesNotBlocked[setNo].insert(entry);
            }
        }
        break;
    default:
        foreach(KEduVocExpression * entry, m_entries[setNo]) {
            if (!isBlocked(entry->translation(m_toTranslation))) {
                m_entriesNotBlocked[setNo].insert(entry);
                //debugEntry("Not blocked:", entry,
                //           entry->translation(m_fromTranslation),
                //           entry->translation(m_toTranslation));
            }
            else {
                //debugEntry("Blocked:", entry,
                //           entry->translation(m_fromTranslation),
                //           entry->translation(m_toTranslation));
            }
        }
        break;
    }
}

bool EntryFilter::isConjugationBlocked(KEduVocTranslation* translation) const
{
    foreach(const QString & tense, translation->conjugationTenses()) {
        if (m_tenses.contains(tense)) {
            QList<KEduVocWordFlags> pronouns = translation->conjugation(tense).keys();
            foreach(const KEduVocWordFlags & pronoun, pronouns) {
                KEduVocText grade = translation->conjugation(tense).conjugation(pronoun);
                if (!isBlocked(&(grade))) {
                    // just need to find any form that is not blocked for generating test entries
                    // exact filtering is done later in conjugationTestEntries
                    return false;
                }
            }
        }
    }
    return true;
}

bool EntryFilter::isBlocked(const KEduVocText* const text) const
{
    grade_t grade    = text->grade();
    grade_t preGrade = text->preGrade();

    // Sanity checks.
    // Note that grade_t is unsigned so no need to check < 0.
    //
    // FIXME: This should be done when the prefs are first read.
    if (preGrade > KV_MAX_GRADE) {
        preGrade = KV_MAX_GRADE;
    }
    if (grade > KV_MAX_GRADE) {
        grade = KV_MAX_GRADE;
    }

    QDateTime now = QDateTime::currentDateTime();

    if ((grade == KV_NORM_GRADE && preGrade == KV_NORM_GRADE)
        || (grade > 0 && Prefs::blockItem(grade) == 0))
    {
        // Always include untrained words and all words when blocking is off.

        //qDebug() << "Not blocked, test 1; word =" << text->text() << "grade =" << grade
        //         << "blockItem(grade) =" << Prefs::blockItem(grade);

        return false;
    }
    else if (grade == KV_NORM_GRADE) {
        // Test for pregrade blocking.
        QDateTime date = text->practiceDate();
        if (date.addSecs(preGradeTimes[preGrade]) < now) {
            //qDebug() << "Not blocked, test 2";
            return false;
        }
    }
    else {
        // Test for grade blocking.

        QDateTime date = text->practiceDate();
        if (date.addSecs(Prefs::blockItem(grade)) < now) {
            //qDebug() << "Not blocked, test 3";
            return false;
        }
    }

    return true;
}

void EntryFilter::timesWrongEntries(int setNo)
{
    foreach(KEduVocExpression * entry, m_entries[setNo]) {
        if (entry->translation(m_toTranslation)->badCount() >= Prefs::practiceMinimumWrongCount() && entry->translation(m_toTranslation)->badCount() <= Prefs::practiceMaximumWrongCount()) {
            m_entriesTimesWrong[setNo].insert(entry);
        }
    }
}

void EntryFilter::timesPracticedEntries(int setNo)
{
    foreach(KEduVocExpression * entry, m_entries[setNo]) {
        if (entry->translation(m_toTranslation)->practiceCount() >= Prefs::practiceMinimumTimesAsked() && entry->translation(m_toTranslation)->practiceCount() <= Prefs::practiceMaximumTimesAsked()) {
            m_entriesTimesPracticed[setNo].insert(entry);
        }
    }
}

void EntryFilter::minMaxGradeEntries(int setNo)
{
    foreach(KEduVocExpression * entry, m_entries[setNo]) {
        int grade = entry->translation(m_toTranslation)->grade();
        if (grade >= Prefs::practiceMinimumGrade() && grade <= Prefs::practiceMaximumGrade()) {
            m_entriesMinMaxGrade[setNo].insert(entry);
        }
    }
}
/*
   if (m_testType == Prefs::EnumTestType::ArticleTest) {
   KMessageBox::information(0,
   i18n("You selected to practice the genders of nouns, but no appropriate nouns could be found. Use \"Edit Entry\" and select Noun as word type and the gender."),
   i18n("No valid word type found"));
   return;
}
   if (m_testType == Prefs::EnumTestType::ComparisonTest) {
   KMessageBox::information(0,
   i18n("You selected to practice comparison forms, but no adjectives or adverbs containing comparison forms could be found. Use \"Edit Entry\" and select Adverb or Adjective as word type and enter the comparison forms."),
   i18n("No valid word type found"));
   return;
}
   if (m_testType == Prefs::EnumTestType::ConjugationTest) {
   KMessageBox::information(0, i18n("You selected to practice conjugations, but no vocabulary containing conjugations in the tenses you selected could be found. Use \"Edit Entry\" and select Verb as word type and enter the conjugation forms."), i18n("No valid word type found"));
   return;
}
}

   if ( removeTestEntryList.count() == m_entries.count() ) {
   if ( KMessageBox::questionYesNo(0, i18n("<p>The units you selected for the practice contain no entries when the threshold settings are respected.</p><p>Hint: To configure the thresholds use the \"Threshold Page\" in the \"Configure Practice\" dialog.</p><p>Would you like to ignore the threshold setting?</p>"), i18n("No Entries with Current Threshold Settings") ) == KMessageBox::No ) {
   return;
}

*/

void EntryFilter::cleanupInvalid(int setNo)
{
    Prefs::EnumPracticeMode::type mode = Prefs::practiceMode();
    bool wordTypeNeeded = (mode == Prefs::EnumPracticeMode::GenderPractice) ||
                          (mode == Prefs::EnumPracticeMode::ComparisonPractice) ||
                          (mode == Prefs::EnumPracticeMode::ConjugationPractice);

    QSet<KEduVocExpression*>::iterator i = m_entries[setNo].begin();
    while (i != m_entries[setNo].end()) {
        KEduVocTranslation  *fromTranslation = (*i)->translation(m_fromTranslation);
        KEduVocTranslation  *toTranslation = (*i)->translation(m_toTranslation);

        // Remove empty entries.
        bool  keep = ((!fromTranslation->text().isEmpty()
                       || (Prefs::allowImageInsteadOfWord()
                           && !fromTranslation->imageUrl().isEmpty()))
                      && !toTranslation->text().isEmpty());
        if (!keep) {
            i = m_entries[setNo].erase(i);
            //debugEntry("Removing empty:", *i, fromTranslation, toTranslation);
            continue;
        }

        // For grammar stuff we need the word to have its word type set, else continue
        if (wordTypeNeeded && !(*i)->translation(m_toTranslation)->wordType()) {
            i = m_entries[setNo].erase(i);
            continue;
        }

        // Grammar modes need different things:
        switch (Prefs::practiceMode()) {

            // example sentences: need the example sentence to exist
        case Prefs::EnumPracticeMode::ExampleSentencesPractice:
            if ((*i)->translation(m_toTranslation)->example().simplified().isEmpty()) {
                i = m_entries[setNo].erase(i);
                continue;
            }
            break;

        case Prefs::EnumPracticeMode::GenderPractice:
            if (!((*i)->translation(m_toTranslation)->wordType()->wordType() & KEduVocWordFlag::Noun)) {
                i = m_entries[setNo].erase(i);
                continue;
            }
            break;

        case Prefs::EnumPracticeMode::ComparisonPractice:
            if (
                // only adjective/adverb
                (((*i)->translation(m_toTranslation)->wordType()->wordType() != KEduVocWordFlag::Adjective)
                 && ((*i)->translation(m_toTranslation)->wordType()->wordType() != KEduVocWordFlag::Adverb))
                // at least one comparison forms is there
                || ((*i)->translation(m_toTranslation)->comparativeForm().text().isEmpty() || (*i)->translation(m_toTranslation)->superlativeForm().text().isEmpty())) {
                i = m_entries[setNo].erase(i);
                continue;
            }
            break;

        case Prefs::EnumPracticeMode::ConjugationPractice: {
            KEduVocTranslation* translation = (*i)->translation(m_toTranslation);
            bool erase = false;

            // Remove entries which are not verbs
            if (translation->wordType()->wordType() != KEduVocWordFlag::Verb) {
                erase = true;
            }

            // Remove entries which don't have any of the tenses which are configured for practice
            QSet<QString> practice_tenses = QSet<QString>::fromList(m_tenses);
            QSet<QString> existing_tenses;
            foreach(const QString & tense, translation->conjugationTenses()) {
                if (!translation->conjugation(tense).isEmpty()) {
                    existing_tenses << tense;
                }
            }
            if (existing_tenses.intersect(practice_tenses).isEmpty()) erase = true;

            if (erase) {
                i = m_entries[setNo].erase(i);
                continue;
            }
            break;
        }
        default:
            break;
        } // switch
        ++i;
    } // while
    //qDebug() << "Invalid items removed. Remaining: " << m_entries[setNo].count();
}

QList< TestEntry* > EntryFilter::conjugationTestEntries(bool ignoreBlocked) const
{
    qDebug() << "Filtering conjugation entries for tenses... " << m_tenses;

    // TODO CM make this configurable
    enum MODE {
        M_SEPARATE,
        M_COMPLETE
    };
    MODE mode = M_SEPARATE;

    QList<TestEntry*> testEntries;
    for (int i = 0; i < m_numSets; ++i) {
        foreach(KEduVocExpression * entry, m_currentSelection[i]) {
            foreach(const QString & tense, entry->translation(m_toTranslation)->conjugationTenses()) {

                // Only include tenses which are both non-empty and which should be practiced
                if (!m_tenses.contains(tense)) {
                    continue;
                }
                KEduVocConjugation& conjugation = entry->translation(m_toTranslation)->conjugation(tense);
                if (conjugation.isEmpty()) {
                    continue;
                }

                bool blocked = true;
                QList<KEduVocWordFlags> pronouns = conjugation.keys();
                foreach(const KEduVocWordFlags & pronoun, pronouns) {
                    KEduVocText* grade = &conjugation.conjugation(pronoun);
                    if (ignoreBlocked || !isBlocked(grade)) {
                        blocked = false;

                        if (mode == M_SEPARATE) {
                            TestEntry* testEntry = new TestEntry(entry);
                            testEntry->setConjugationTense(tense);
                            QList<KEduVocWordFlags> list;
                            list << pronoun;
                            testEntry->setConjugationPronouns(list);
                            randomizedInsert(testEntries, testEntry);
                        }
                    }
                }

                if (!blocked && mode == M_COMPLETE) {
                    TestEntry* testEntry = new TestEntry(entry);
                    testEntry->setConjugationTense(tense);
                    testEntry->setConjugationPronouns(pronouns);
                    randomizedInsert(testEntries, testEntry);
                }
            }
        }
    }
    return testEntries;
}


// ----------------------------------------------------------------
//                 Support functions for the dialog


void EntryFilter::checkBoxChanged(bool filter)
{
    Q_UNUSED(filter)
    updateTotal();
}

void EntryFilter::updateTotal()
{
    QSet< KEduVocExpression * > selected[2];

    for (int i = 0; i < m_numSets; ++i) {
        selected[i] = m_entries[i];
        if (!m_dialog || !ui.lessonCheckBox->isChecked()) {
            selected[i] = selected[i].intersect(m_entriesLesson[i]);
        }
        if (!m_dialog || !ui.wordTypeCheckBox->isChecked()) {
            selected[i] = selected[i].intersect(m_entriesWordType[i]);
        }
        if (!m_dialog || !ui.blockedCheckBox->isChecked()) {
            selected[i] = selected[i].intersect(m_entriesNotBlocked[i]);
        }
        if (!m_dialog || !ui.timesWrongCheckBox->isChecked()) {
            selected[i] = selected[i].intersect(m_entriesTimesWrong[i]);
        }
        if (!m_dialog || !ui.timesPracticedCheckBox->isChecked()) {
            selected[i] = selected[i].intersect(m_entriesTimesPracticed[i]);
        }
        if (!m_dialog || !ui.minMaxGradeCheckBox->isChecked()) {
            selected[i] = selected[i].intersect(m_entriesMinMaxGrade[i]);
        }

        m_currentSelection[i] = selected[i];
    }

    if (m_dialog) {
        int numSelected = selected[0].count() + selected[1].count();
        ui.totalLabel->setText(QString::number(numSelected));
        m_button_dialog->button(QDialogButtonBox::Ok)->setEnabled(numSelected > 0);
    }
}


// ----------------------------------------------------------------
//                             Utilities


void EntryFilter::randomizedInsert(QList<TestEntry*>& list, TestEntry* entry)
{
    list.insert(KRandom::random() % (list.size() + 1), entry);
}
