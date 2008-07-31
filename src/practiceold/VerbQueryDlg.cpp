/***************************************************************************

                    query dialog for verbs

    -----------------------------------------------------------------------

    begin         : Fri Dec 3 18:28:18 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "VerbQueryDlg.h"

#include "documentsettings.h"

#include <keduvocdocument.h>

#include <KLocale>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

VerbQueryDlg::VerbQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Verb Training"), doc, parent)
{
    mw = new Ui::VerbQueryDlgForm();
    mw->setupUi(mainWidget());

    DocumentSettings currentSettings(m_doc->url().url());
    currentSettings.readConfig();
    m_activeTenses = currentSettings.conjugationTenses();

kDebug() << "Practicing tenses: " << m_activeTenses;

    mw->stopPracticeButton->setIcon( KIcon("process-stop") );
    mw->verify->setIcon(KIcon("dialog-ok"));
    mw->know_it->setIcon(KIcon("go-next"));
    mw->dont_know->setIcon(KIcon("go-next"));

    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    mw->know_it->setVisible(Prefs::skipKnownEnabled());
    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    mw->imageGraphicsView->setVisible(false);

    m_hasDualConjugations = m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().dualExists();
    setupPersonalPronouns();

    setupConjugationLineEditMap();
    foreach ( QLineEdit* line, m_conjugationWidgets ) {
        connect(line, SIGNAL(textChanged()), SLOT(textChanged()));
        ///@todo update upon change
    }

    KConfigGroup cg(KGlobal::config(), "VerbQueryDialog");
    restoreDialogSize(cg);
}


VerbQueryDlg::~VerbQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "VerbQueryDialog");
    KDialog::saveDialogSize(cg);
}


void VerbQueryDlg::setupPersonalPronouns()
{
    mw->singularFirstPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::First | KEduVocWordFlag::Singular));
    mw->singularSecondPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Second | KEduVocWordFlag::Singular));
    mw->singularThirdMalePersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Singular));
    mw->singularThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Singular));
    mw->singularThirdNeutralPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Singular));


    mw->dualGroupBox->setVisible(m_hasDualConjugations);
    if ( m_hasDualConjugations ) {
        mw->dualFirstPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::First | KEduVocWordFlag::Dual));
        mw->dualSecondPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Second | KEduVocWordFlag::Dual));
        mw->dualThirdMalePersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third| KEduVocWordFlag::Masculine | KEduVocWordFlag::Dual));
        mw->dualThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Dual));
        mw->dualThirdNeutralPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Dual));
    }

    mw->pluralFirstPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::First | KEduVocWordFlag::Plural));
    mw->pluralSecondPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Second | KEduVocWordFlag::Plural));
    mw->pluralThirdMalePersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine |  KEduVocWordFlag::Plural));
    mw->pluralThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Plural));
    mw->pluralThirdNeutralPersonLabel->setText(m_doc->identifier(Prefs::solutionLanguage()).personalPronouns().personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter |  KEduVocWordFlag::Plural));
}


void VerbQueryDlg::setupConjugationLineEditMap()
{
    // singular
    m_conjugationWidgets[KEduVocWordFlag::First| KEduVocWordFlag::Singular] = mw->singularFirstPersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Second| KEduVocWordFlag::Singular] = mw->singularSecondPersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Masculine| KEduVocWordFlag::Singular] = mw->singularThirdMalePersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Feminine| KEduVocWordFlag::Singular] = mw->singularThirdFemalePersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Neuter| KEduVocWordFlag::Singular] = mw->singularThirdNeutralPersonLineEdit;
    // dual
    m_conjugationWidgets[KEduVocWordFlag::First| KEduVocWordFlag::Dual] = mw->dualFirstPersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Second| KEduVocWordFlag::Dual] = mw->dualSecondPersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Masculine| KEduVocWordFlag::Dual] = mw->dualThirdMalePersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Feminine| KEduVocWordFlag::Dual] = mw->dualThirdFemalePersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Neuter| KEduVocWordFlag::Dual] = mw->dualThirdNeutralPersonLineEdit;
    // plural
    m_conjugationWidgets[KEduVocWordFlag::First| KEduVocWordFlag::Plural] = mw->pluralFirstPersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Second| KEduVocWordFlag::Plural] = mw->pluralSecondPersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Masculine| KEduVocWordFlag::Plural] = mw->pluralThirdMalePersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Feminine| KEduVocWordFlag::Plural] = mw->pluralThirdFemalePersonLineEdit;
    m_conjugationWidgets[KEduVocWordFlag::Third | KEduVocWordFlag::Neuter| KEduVocWordFlag::Plural] = mw->pluralThirdNeutralPersonLineEdit;
}


void VerbQueryDlg::clearLineEdits()
{
    foreach ( int i, m_conjugationWidgets.keys() ) {
        QLineEdit* conjugationLineEdit = m_conjugationWidgets.value(i);
        conjugationLineEdit->setText(QString());
        setWidgetStyle(conjugationLineEdit, Default);
        conjugationLineEdit->setReadOnly(false);

        if ( m_entry->entry()->translation(Prefs::solutionLanguage())->
                conjugation(m_currentTense).conjugation((KEduVocWordFlag::Flags)i).isEmpty() ) {
            conjugationLineEdit->setVisible(false);
        } else {
            conjugationLineEdit->setVisible(true);
        }
    }
}


void VerbQueryDlg::setEntry(TestEntry* entry)
{
    PracticeDialog::setEntry(entry);

    // setup a list of tenses to practice
    m_tenses.clear();
    QStringList tenses = entry->entry()->translation(Prefs::solutionLanguage())->conjugationTenses();
    foreach ( const QString &tense, tenses ) {
        if ( !entry->entry()->translation(Prefs::solutionLanguage())->conjugation(tense).isEmpty() ) {
            if ( m_activeTenses.contains( tense ) ) {
                m_tenses.append(tense);
            }
        }
    }

    kDebug() << "Conjugation: " <<  m_entry->entry()->translation(Prefs::solutionLanguage())->text() << m_tenses;
    // skip if none is found
    if ( m_tenses.isEmpty() ) {
        kDebug() << "Warning, no conjugations found.";
        resultCorrect(); // don't ask this again
        emit showSolutionFinished();
        return;
    }

    mw->verbNameLabel->setText( m_entry->entry()->translation(Prefs::solutionLanguage())->text() );

    mw->progCount->setText( QString::number(entry->statisticCount()) );

    mw->timebar->setVisible(Prefs::practiceTimeout());
    mw->timelabel->setVisible(Prefs::practiceTimeout());
    mw->verify->setDefault(true);

    mw->singularFirstPersonLineEdit->setFocus();

    imageShowFromEntry( mw->imageGraphicsView );

    setupTense( m_tenses.value(0) );
}


// bool VerbQueryDlg::nextTense()
// {
//     clearLineEdits();
//     QString tense;
//
//     mw->verbNameLabel->setText( m_entry->entry()->translation(Prefs::solutionLanguage())->text() );
//
//
//     tense = m_tenses.value(0);
//     QString tenseText = i18n("Current tense is: %1", tense);
//
//     mw->instructionLabel->setText(tenseText);
//
//     setWidgetStyle(mw->tenseLabel, HintStyle);
//     mw->tenseLabel->setText(tense);
//
// kDebug() << "Conjugation: " <<  m_entry->entry()->translation(Prefs::solutionLanguage())->text() << tense << " empty: " << m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].isEmpty();
//
//
//     if (m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].isEmpty() ) {
//         return false;
//     }
//
//     bool empty;
//
//     // singular
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::First | KEduVocWordFlag::Singular).isEmpty();
//     mw->singularFirstPersonLabel->setVisible(!empty);
//     mw->singularFirstPersonLineEdit->setVisible(!empty);
//     mw->singularFirstPersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Second | KEduVocWordFlag::Singular).isEmpty();
//     mw->singularSecondPersonLabel->setVisible(!empty);
//     mw->singularSecondPersonLineEdit->setVisible(!empty);
//     mw->singularSecondPersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Singular).isEmpty();
//     mw->singularThirdMalePersonLabel->setVisible(!empty);
//     mw->singularThirdMalePersonLineEdit->setVisible(!empty);
//     mw->singularThirdMalePersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Singular).isEmpty();
//     mw->singularThirdFemalePersonLabel->setVisible(!empty);
//     mw->singularThirdFemalePersonLineEdit->setVisible(!empty);
//     mw->singularThirdFemalePersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Singular).isEmpty();
//     mw->singularThirdNeutralPersonLabel->setVisible(!empty);
//     mw->singularThirdNeutralPersonLineEdit->setVisible(!empty);
//     mw->singularThirdNeutralPersonLineEdit->setText("");
//
//     // dual
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::First | KEduVocWordFlag::Dual).isEmpty();
//     mw->dualFirstPersonLabel->setVisible(!empty);
//     mw->dualFirstPersonLineEdit->setVisible(!empty);
//     mw->dualFirstPersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Second | KEduVocWordFlag::Dual).isEmpty();
//     mw->dualSecondPersonLabel->setVisible(!empty);
//     mw->dualSecondPersonLineEdit->setVisible(!empty);
//     mw->dualSecondPersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Dual).isEmpty();
//     mw->dualThirdMalePersonLabel->setVisible(!empty);
//     mw->dualThirdMalePersonLineEdit->setVisible(!empty);
//     mw->dualThirdMalePersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Dual).isEmpty();
//     mw->dualThirdFemalePersonLabel->setVisible(!empty);
//     mw->dualThirdFemalePersonLineEdit->setVisible(!empty);
//     mw->dualThirdFemalePersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Dual).isEmpty();
//     mw->dualThirdNeutralPersonLabel->setVisible(!empty);
//     mw->dualThirdNeutralPersonLineEdit->setVisible(!empty);
//     mw->dualThirdNeutralPersonLineEdit->setText("");
//
//     // plural
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::First | KEduVocWordFlag::Plural).isEmpty();
//     mw->pluralFirstPersonLabel->setVisible(!empty);
//     mw->pluralFirstPersonLineEdit->setVisible(!empty);
//     mw->pluralFirstPersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Second | KEduVocWordFlag::Plural).isEmpty();
//     mw->pluralSecondPersonLabel->setVisible(!empty);
//     mw->pluralSecondPersonLineEdit->setVisible(!empty);
//     mw->pluralSecondPersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Plural).isEmpty();
//     mw->pluralThirdMalePersonLabel->setVisible(!empty);
//     mw->pluralThirdMalePersonLineEdit->setVisible(!empty);
//     mw->pluralThirdMalePersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Plural).isEmpty();
//     mw->pluralThirdFemalePersonLabel->setVisible(!empty);
//     mw->pluralThirdFemalePersonLineEdit->setVisible(!empty);
//     mw->pluralThirdFemalePersonLineEdit->setText("");
//
//     empty = m_entry->entry()->translation(Prefs::solutionLanguage())->
//         conjugations()[tense].conjugation(
//         KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Plural).isEmpty();
//     mw->pluralThirdNeutralPersonLabel->setVisible(!empty);
//     mw->pluralThirdNeutralPersonLineEdit->setVisible(!empty);
//     mw->pluralThirdNeutralPersonLineEdit->setText("");
//
//     return false;
// }


void VerbQueryDlg::showSolution()
{
    setAnswerTainted();

    mw->dont_know->setDefault(true);

    QString tense = m_tenses.value(0);

    foreach ( int i, m_conjugationWidgets.keys() ) {
        m_conjugationWidgets.value(i)->setText(m_entry->entry()->translation(Prefs::solutionLanguage())->conjugations()[tense].conjugation((KEduVocWordFlag::Flags)i).text());
        setWidgetStyle(m_conjugationWidgets.value(i), PositiveResult);
        m_conjugationWidgets.value(i)->setReadOnly(true);
    }
}


void VerbQueryDlg::verifyClicked()
{
    QString tense = m_tenses.value(0);

    const KEduVocConjugation conj = m_entry->entry()->translation(Prefs::solutionLanguage())->conjugations()[tense];

    QString solution;
    bool all_correct = true;
    bool all_filled = true;
    foreach ( int i, m_conjugationWidgets.keys() ) {
        solution = m_entry->entry()->translation(Prefs::solutionLanguage())->conjugation(m_currentTense).conjugation((KEduVocWordFlag::Flags)i).text();
        if ( !solution.isEmpty() ) {
            if ( m_conjugationWidgets.value(i)->text() == solution ) {
                setWidgetStyle(m_conjugationWidgets[i], PositiveResult);
            } else {
                if ( !m_conjugationWidgets[i]->text().isEmpty() ) {
                    setWidgetStyle(m_conjugationWidgets[i], NegativeResult);
                    setAnswerTainted();
                    all_correct = false;
                }
            }
            if ( m_conjugationWidgets.value(i)->text().isEmpty() ) {
                // set the focus to the first empty field
                if ( all_filled ) {
                    m_conjugationWidgets.value(i)->setFocus();
                    all_filled = false;
                }
                all_correct = false;
            }
        }
    }
    if ( !all_filled ) {
        return;
    }

    if ( all_correct ) {
        m_tenses.removeAt(m_tenses.indexOf(m_currentTense));
        if ( m_tenses.isEmpty() ) {
            resultCorrect();
            emit showSolutionFinished();
        } else {
            setupTense( m_tenses.value(0));
        }
    }
}


void VerbQueryDlg::setupTense(const QString & tense)
{
    m_currentTense = tense;
    QString tenseText = i18n("Current tense is: %1", tense);
    mw->instructionLabel->setText(tenseText);
    mw->tenseLabel->setText(tense);
    clearLineEdits();
    mw->singularFirstPersonLineEdit->setFocus();
}


void VerbQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}



#include "VerbQueryDlg.moc"
