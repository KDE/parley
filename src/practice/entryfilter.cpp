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

#include "prefs.h"

#include <keduvocdocument.h>
#include <keduvocwordtype.h>
#include <keduvocexpression.h>

#include <KDialog>

EntryFilter::EntryFilter(QObject * parent, KEduVocDocument* doc) :QObject(parent)
{
    m_doc = doc;
    m_fromTranslation = Prefs::questionLanguage();
    m_toTranslation = Prefs::solutionLanguage();
    expireEntries();
}

EntryFilter::~ EntryFilter()
{
}

void EntryFilter::expireEntries()
{
    if ( Prefs::expire() ) {
        int counter = 0;
        foreach (KEduVocExpression* entry, m_entries) {
            int grade = entry->translation(m_toTranslation)->gradeFrom(m_fromTranslation).grade();

            const QDateTime &date =  entry->translation(m_toTranslation)->gradeFrom(m_fromTranslation).practiceDate();

            const QDateTime &expireDate = QDateTime::currentDateTime().addSecs( -Prefs::expireItem(grade) );

            if ( date < expireDate && grade > 0) {
                // decrease the grade
                entry->translation(m_toTranslation)->gradeFrom(m_fromTranslation).decGrade();

                // prevent from endless dropping
                entry->translation(m_toTranslation)->gradeFrom(m_fromTranslation).setPracticeDate( QDateTime::currentDateTime().addSecs( -Prefs::expireItem( grade - 2) ) );
                counter++;
            }
        }
        kDebug() << "Expired words dropped their grade: " << counter;
    }
}

QList<KEduVocExpression*> EntryFilter::entries()
{
    // set up the lists/sets of filtered vocabulary
    m_entries = m_doc->lesson()->entries(KEduVocLesson::Recursive).toSet();
    kDebug() << "Document contains " << m_entries.count() << " entries.";
    if (m_entries.count() == 0) {
        // message box?
        return m_entries.toList();
    }

    lessonEntries();
    wordTypeEntries();
    blockedEntries();
    gradeEntries();
    timesWrongEntries();
    timesPracticedEntries();
    minMaxGradeEntries();

   // if (m_entries.count() == 0) {
        kDebug() << "Creating practice filter dialog.";
        m_dialog = new KDialog();
        m_dialog->setCaption(i18n("Start Practice"));
        QWidget *widget = new QWidget;
        ui.setupUi(widget);
        m_dialog->setMainWidget(widget);
        m_dialog->setButtons( KDialog::Ok | KDialog::Cancel );
        
        ui.lessonLabel->setText(QString::number(m_entriesLesson.count()));
        ui.wordTypeLabel->setText(QString::number(m_entriesWordType.count()));
        ui.blockedLabel->setText(QString::number(m_entriesBlocked.count()));
        ui.gradeLabel->setText(QString::number(m_entriesGrade.count()));
        ui.timesWrongLabel->setText(QString::number(m_entriesTimesWrong.count()));
        ui.timesPracticedLabel->setText(QString::number(m_entriesTimesPracticed.count()));
        ui.minMaxGradeLabel->setText(QString::number(m_entriesMinMaxGrade.count()));
    
        ui.documentTotalLabel->setText(QString::number(m_entries.count()));
        updateDialogTotal();

        connect( m_dialog, SIGNAL(okClicked()), this, SLOT(userSelectionAccepted()) );
        connect( m_dialog, SIGNAL(cancelClicked()), this, SLOT(userSelectionCanceled()) );
        connect( ui.lessonCheckBox, SIGNAL(toggled(bool)), this, SLOT(filterLesson(bool)));
        m_dialog->enableButtonOk(false);
        if (m_dialog->exec() == KDialog::Cancel) {
            delete m_dialog;
            return QList<KEduVocExpression*>();
        }

   // }

    /*
    ///@todo respect sub lessons and their inPractice state!
    // append lesson entries
    foreach ( KEduVocContainer *container, m_doc->lesson()->childContainers() ) {
    KEduVocLesson *lesson = static_cast<KEduVocLesson*>(container);
    if ( lesson->inPractice() ) {
    int lessonLimit = m_entries.count();
    foreach ( KEduVocExpression *entry, lesson->entries(KEduVocLesson::Recursive) ) {
    if ( Prefs::testOrderLesson() ) {
                    // insert after the last entry of the last lesson
    m_entries.insert(
    lessonLimit + m_randomSequence->getLong(lessonLimit - m_entries.count()),
    new TestEntry(entry) );
} else {
                    // insert at total random position
    m_entries.insert(
    m_randomSequence->getLong(m_entries.count()),
    new TestEntry(entry) );

}
}
}
}

    if ( m_entries.count() == 0 ) {
    if ( KMessageBox::questionYesNo(0, i18n("<p>The lessons you selected for the practice contain no vocabulary.</p><p>Hint: To select a lesson set a checkmark next to it in the lesson column on the left.</p><p>Would you like to include all lessons?</p>"), i18n("No Entries in Selected Lessons") ) == KMessageBox::Yes ) {
    kDebug() << "Adding all lessons.";
            ///@todo reuse the above - make it a function?
    foreach ( KEduVocContainer *container, m_doc->lesson()->childContainers() ) {
    KEduVocLesson *lesson = static_cast<KEduVocLesson*>(container);
    int lessonLimit = m_entries.count();
    foreach ( KEduVocExpression * entry, lesson->entries() ) {
    if ( Prefs::testOrderLesson() ) {
                        // insert after the last entry of the last lesson
    m_entries.insert(
    lessonLimit + m_randomSequence->getLong(lessonLimit - m_entries.count()),
    new TestEntry(entry) );
} else {
                        // insert at total random position
    m_entries.insert(
    m_randomSequence->getLong(m_entries.count()),
    new TestEntry(entry) );

}
}
}
} else {
    return;
}
}
    kDebug() << "Found " << m_entries.count() << " entries in selected lessons.";

    // remove empty entries
    for ( int i = m_entries.count() - 1; i >= 0; i-- ) {
    if ( m_entries.value(i)->entry()->translation(TestEntry::gradeFrom())->text().isEmpty() ||
    m_entries.value(i)->entry()->translation(TestEntry::gradeTo())->text().isEmpty() ) {
    delete m_entries.takeAt(i);
}
}
    kDebug() << "Found " << m_entries.count() << " entries that are not empty.";

    // expire (decrease grade after a certain amount of time)
    expireEntries();


    QList <TestEntry *> removeTestEntryList;
    // word type
    int validWordType = 0;
    int validWrongCount = 0;
    int validPracticeCount = 0;
    int validGrade = 0;
    for ( int i = m_entries.count() - 1; i >= 0; i-- ) {
    bool remove = false;
    const KEduVocGrade& grade =
    m_entries.value(i)->entry()->translation(m_toTranslation)->gradeFrom(m_fromTranslation);
    if ( checkType(m_entries.value(i)->entry()) ) {
    validWordType++;
} else { remove = true; }
    if ( grade.badCount() >= Prefs::practiceMinimumWrongCount() && grade.badCount() <= Prefs::practiceMaximumWrongCount() ) {
    validWrongCount++;
} else { remove = true; }
    if ( grade.practiceCount() >= Prefs::practiceMinimumTimesAsked() && grade.practiceCount() <= Prefs::practiceMaximumTimesAsked() ) {
    validPracticeCount++;
} else { remove = true; }
    if ( grade.grade() >= Prefs::practiceMinimumGrade() && grade.grade() <= Prefs::practiceMaximumGrade() ) {
    validGrade++;
} else { remove = true; }
    if ( remove ) {
    removeTestEntryList.append(m_entries.value(i));
}
}
    kDebug() << "Valid Type: " << validWordType << " Valid Grade: " << validGrade
    << " Valid Wrong Count: " << validWrongCount << " Valid Practice Count: " << validPracticeCount;
    kDebug() << "Found " << removeTestEntryList.count() << " entries with invalid threshold.";

    if (validWordType == 0) {
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
    if ( KMessageBox::questionYesNo(0, i18n("<p>The lessons you selected for the practice contain no entries when the threshold settings are respected.</p><p>Hint: To configure the thresholds use the \"Threshold Page\" in the \"Configure Practice\" dialog.</p><p>Would you like to ignore the threshold setting?</p>"), i18n("No Entries with Current Threshold Settings") ) == KMessageBox::No ) {
    return;
}
} else {
    foreach ( TestEntry* entry, removeTestEntryList ) {
    delete m_entries.takeAt(m_entries.indexOf(entry));
}
}

    // use the old validate methods for now
    for ( int i = m_entries.count() - 1; i >= 0; i-- ) {
    if ( !validate(m_entries.value(i)->entry()) ) {
    delete m_entries.takeAt(i);
}
}
    */


    return m_currentSelection.toList();
}


/*
bool TestEntryManager::validate(KEduVocExpression *expr)
{

    ///@todo word type, min/max asked/wrong/grade

    switch (m_testType) {
        case Prefs::EnumTestType::SynonymTest:
            return !expr->translation(m_toTranslation)->synonym().simplified().isEmpty();
            break;
        case Prefs::EnumTestType::AntonymTest:
            return !expr->translation(m_toTranslation)->antonym().simplified().isEmpty();
            break;
        case Prefs::EnumTestType::ParaphraseTest:
            return !expr->translation(m_toTranslation)->paraphrase().simplified().isEmpty();
            break;
        case Prefs::EnumTestType::ExampleTest:
            return !expr->translation(m_toTranslation)->example().simplified().isEmpty();
            break;

        case Prefs::EnumTestType::ConjugationTest:
        case Prefs::EnumTestType::ArticleTest:
        case Prefs::EnumTestType::ComparisonTest:
    // already in checkType
            return true;
            break;

        default:
            if ( validateWithSettings(expr) ) {
                return true;
            }
        ///@todo not sure about swap dir stuff...
//         if (Prefs::swapDirection()) {
//             int temp = m_fromTranslation;
//             m_fromTranslation = m_toTranslation;
//             m_toTranslation = temp;
//             return validateWithSettings(expr);
//         } // swapDirection
//         break;
    }
    return false;
}



bool TestEntryManager::validateWithSettings(KEduVocExpression *expr)
{
    if ( !compareBlocking(expr->translation(m_toTranslation)->gradeFrom(m_fromTranslation).grade(), expr->translation(m_toTranslation)->gradeFrom(m_fromTranslation).practiceDate(), Prefs::block())) {
        return false;
    }
    return true;
}
*/

/*
bool EntryFilter::checkType(KEduVocExpression * entry)
{
    switch (Prefs::testType()) {
    // if we do a grammar test, check only if the grammar type is valid
        case Prefs::EnumTestType::ArticleTest:
            return entry->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::NounMale ||
                    entry->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::NounFemale ||
                    entry->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::NounNeutral;

        case Prefs::EnumTestType::ComparisonTest:
            if ( Prefs::comparisonIncludeAdjective() ) {
                if ( entry->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::Adjective ) {
                    return !entry->translation(m_toTranslation)->comparative().isEmpty() ||
                            !entry->translation(m_toTranslation)->superlative().isEmpty();
                }
            }
            if ( Prefs::comparisonIncludeAdverb() ) {
                if ( entry->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::Adverb ) {
                    return !entry->translation(m_toTranslation)->comparative().isEmpty() ||
                            !entry->translation(m_toTranslation)->superlative().isEmpty();
                }
            }

        case Prefs::EnumTestType::ConjugationTest:
            if ( entry->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::Verb ) {
                return entry->translation(m_toTranslation)->conjugations().count() > 0;
            }
            return false;


    }

    return false;
}
*/


void EntryFilter::filterLesson(bool filter)
{
    if (filter) {
        ui.lessonLabel->setText("yes");
    } else {
        ui.lessonLabel->setText("no");
    }
    updateDialogTotal();
}

void EntryFilter::updateDialogTotal()
{
    QSet< KEduVocExpression * > selected = m_entries;
    if (ui.lessonCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesLesson);
    }
    if (ui.wordTypeCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesWordType);
    }
    if (ui.blockedCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesBlocked);
    }
    if (ui.gradeCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesGrade);
    }
    if (ui.timesWrongCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesTimesWrong);
    }
    if (ui.timesPracticedCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesTimesPracticed);
    }
    if (ui.minMaxGradeCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesMinMaxGrade);
    }
    ui.totalLabel->setText(QString::number(selected.count()));

    m_dialog->enableButtonOk(selected.count() > 0);
    m_currentSelection = selected;
}

void EntryFilter::lessonEntries()
{
    foreach(KEduVocExpression* entry, m_entries) {
        foreach(KEduVocLesson* lesson, entry->lessons()) {
            if (lesson->inPractice()) {
                m_entriesLesson.insert(entry);
                break;
            }
        }
    }
}

void EntryFilter::wordTypeEntries()
{
    foreach(KEduVocExpression* entry, m_entries) {
        if (entry->translation(m_toTranslation)->wordType()) {
            if(entry->translation(m_toTranslation)->wordType()->inPractice()) {
                m_entriesWordType.insert(entry);
            }
        }
    }
}

void EntryFilter::blockedEntries()
{
    if (!Prefs::block()) {
        m_entriesBlocked = m_entries;
        return;
    }

    foreach(KEduVocExpression* entry, m_entries) {
        int grade = entry->translation(m_toTranslation)->gradeFrom(m_fromTranslation).grade();
        if (grade == KV_NORM_GRADE || Prefs::blockItem(grade) == 0) {
            m_entriesBlocked.insert(entry);
        } else {
            QDateTime date = entry->translation(m_toTranslation)->gradeFrom(m_fromTranslation).practiceDate();
            if (date.addSecs(Prefs::blockItem(grade)) < QDateTime::currentDateTime()) {
                m_entriesBlocked.insert(entry);
            }
        }
    }
}

void EntryFilter::gradeEntries()
{
    
}

void EntryFilter::timesWrongEntries()
{
    
}

void EntryFilter::timesPracticedEntries()
{
    
}

void EntryFilter::minMaxGradeEntries()
{
   
}


#include "entryfilter.moc"
