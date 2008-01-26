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
    m_dialog = 0;
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
    cleanupInvalid();
    
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

    updateTotal();

    if (m_currentSelection.count() == 0) {
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
        updateTotal();

        connect( m_dialog, SIGNAL(okClicked()), this, SLOT(userSelectionAccepted()) );
        connect( m_dialog, SIGNAL(cancelClicked()), this, SLOT(userSelectionCanceled()) );
        connect( ui.lessonCheckBox, SIGNAL(toggled(bool)), this, SLOT(filterLesson(bool)));
        m_dialog->enableButtonOk(false);
        if (m_dialog->exec() == KDialog::Cancel) {
            delete m_dialog;
            return QList<KEduVocExpression*>();
        }

    }

   /*

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



void EntryFilter::filterLesson(bool filter)
{
    if (filter) {
        ui.lessonLabel->setText("yes");
    } else {
        ui.lessonLabel->setText("no");
    }
    updateTotal();
}

void EntryFilter::updateTotal()
{
    QSet< KEduVocExpression * > selected = m_entries;
    if (!m_dialog || ui.lessonCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesLesson);
    }
    if (!m_dialog || ui.wordTypeCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesWordType);
    }
    if (!m_dialog || ui.blockedCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesBlocked);
    }
    if (!m_dialog || ui.gradeCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesGrade);
    }
    if (!m_dialog || ui.timesWrongCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesTimesWrong);
    }
    if (!m_dialog || ui.timesPracticedCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesTimesPracticed);
    }
    if (!m_dialog || ui.minMaxGradeCheckBox->isChecked()) {
        selected = selected.intersect(m_entriesMinMaxGrade);
    }

    if (m_dialog) {
        ui.totalLabel->setText(QString::number(selected.count()));
        m_dialog->enableButtonOk(selected.count() > 0);
    }

    m_currentSelection = selected;
}

void EntryFilter::lessonEntries()
{
    /// @todo maybe randomize
    foreach(KEduVocExpression* entry, m_entries) {
        foreach(KEduVocLesson* lesson, entry->lessons()) {
            if (lesson->inPractice()) {
                m_entriesLesson.insert(entry);
                break;
            }
        }
    }

    //if ( Prefs::testOrderLesson() ) {

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

void EntryFilter::cleanupInvalid()
{
    bool typeTest = Prefs::testType() == Prefs::EnumTestType::ArticleTest
            || Prefs::testType() == Prefs::EnumTestType::ComparisonTest
            || Prefs::testType() == Prefs::EnumTestType::ConjugationTest;

    QSet<KEduVocExpression*>::iterator i;
    for (i = m_entries.begin(); i != m_entries.end(); ++i) {
        // remove empty entries
        if (!(*i)->translation(m_toTranslation)->text().isEmpty() 
              || !(*i)->translation(m_fromTranslation)->text().isEmpty()) {
            i = m_entries.erase(i);
        } else if (typeTest) {
            if(!(*i)->translation(m_toTranslation)->wordType()) {
                i = m_entries.erase(i);
            } else {
                switch (Prefs::testType()) {
    // if we do a grammar test, check if the grammar type is valid
                case Prefs::EnumTestType::ArticleTest:
                    if (! ((*i)->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::NounMale ||
                          (*i)->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::NounFemale ||
                          (*i)->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::NounNeutral)) {
                        i = m_entries.erase(i);
                    }
                case Prefs::EnumTestType::ComparisonTest:
                    if (! ((Prefs::comparisonIncludeAdjective() &&(*i)->translation(m_toTranslation)->wordType()->wordType()
                                == KEduVocWordType::Adjective)
                           || (Prefs::comparisonIncludeAdverb() &&(*i)->translation(m_toTranslation)->wordType()->wordType()
                           == KEduVocWordType::Adverb))) {
                        i = m_entries.erase(i);
                    } else {
                        if ((*i)->translation(m_toTranslation)->comparative().isEmpty() &&
                                    (*i)->translation(m_toTranslation)->superlative().isEmpty()) {
                            i = m_entries.erase(i);
                        }
                    }
                case Prefs::EnumTestType::ConjugationTest:
                    if ( (*i)->translation(m_toTranslation)->wordType()->wordType() == KEduVocWordType::Verb || (*i)->translation(m_toTranslation)->conjugations().count() == 0) {
                        i = m_entries.erase(i);
                    } // conjugation
                } // switch
            } // type valid
        } // if typeTest
    } // for
}


#include "entryfilter.moc"
