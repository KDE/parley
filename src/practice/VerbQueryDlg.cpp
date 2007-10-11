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

#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include <KLocale>

#include <kvttablemodel.h>

VerbQueryDlg::VerbQueryDlg(KEduVocDocument *doc, QWidget *parent) : PracticeDialog(i18n("Verb Training"), doc, parent)
{
    mw = new Ui::VerbQueryDlgForm();
    mw->setupUi(mainWidget());

    mw->stopPracticeButton->setIcon( KIcon("list-remove") );
    mw->editEntryButton->setIcon( KIcon("edit") );
    mw->verify->setIcon(KIcon("ok"));
    mw->know_it->setIcon(KIcon("go-next"));
    mw->dont_know->setIcon(KIcon("go-next"));

    connect(mw->stopPracticeButton, SIGNAL(clicked()), SLOT(close()));
    connect(mw->editEntryButton, SIGNAL(clicked()), SLOT(editEntry()));

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(skipUnknown()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(skipKnown()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    mw->know_it->setVisible(Prefs::iKnow());
    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

    mw->imageGraphicsView->setVisible(false);

    KConfigGroup cg(KGlobal::config(), "VerbQueryDialog");
    restoreDialogSize(cg);
}


VerbQueryDlg::~VerbQueryDlg()
{
    KConfigGroup cg(KGlobal::config(), "VerbQueryDialog");
    KDialog::saveDialogSize(cg);
}


void VerbQueryDlg::setEntry(TestEntry* entry)
{
    m_tenses.clear();

    PracticeDialog::setEntry(entry);

    mw->progCount->setText( QString::number(entry->statisticCount()) );

    mw->timebar->setVisible(Prefs::showCounter());
    mw->timelabel->setVisible(Prefs::showCounter());
    mw->verify->setDefault(true);

    mw->dualGroupBox->setVisible( m_doc->identifier(Prefs::toIdentifier()).personalPronouns().dualExists());

    mw->singularFirstPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::First, KEduVocConjugation::Singular));
    mw->singularSecondPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::Second, KEduVocConjugation::Singular));
    mw->singularThirdMalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular));
    mw->singularThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular));
    mw->singularThirdNeutralPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Singular));

    mw->dualFirstPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::First, KEduVocConjugation::Dual));
    mw->dualSecondPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::Second, KEduVocConjugation::Dual));
    mw->dualThirdMalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual));
    mw->dualThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual));
    mw->dualThirdNeutralPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Dual));

    mw->pluralFirstPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::First, KEduVocConjugation::Plural));
    mw->pluralSecondPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::Second, KEduVocConjugation::Plural));
    mw->pluralThirdMalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural));
    mw->pluralThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural));
    mw->pluralThirdNeutralPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Plural));

    QStringList tenses = entry->exp->translation(Prefs::toIdentifier()).conjugationTenses();
    foreach ( QString tense, tenses ) {
        if ( !entry->exp->translation(Prefs::toIdentifier()).conjugation(tense).isEmpty() ) {
            m_tenses.append(tense);
        }
    }

kDebug() << "Conjugation: " <<  m_entry->exp->translation(Prefs::toIdentifier()).text() << m_tenses;

    if ( m_tenses.count() == 0 ) {
        kDebug() << "Warning, no conjugations found.";
        nextEntry();
        return;
    }

    nextTense();

    mw->singularFirstPersonLineEdit->setFocus();

    imageShowFromEntry( mw->imageGraphicsView, entry );
}


bool VerbQueryDlg::nextTense()
{
    resetAllFields();
    QString tense;

    mw->verbNameLabel->setText( m_entry->exp->translation(Prefs::toIdentifier()).text() );


    tense = m_tenses.value(0);
    QString tenseText = i18n("Current tense is: %1", tense);

    mw->instructionLabel->setText(tenseText);


kDebug() << "Conjugation: " <<  m_entry->exp->translation(Prefs::toIdentifier()).text() << tense << " empty: " << m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].isEmpty();


    if (m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].isEmpty() ) {
        return false;
    }

    bool empty;

    // singular
    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::First, KEduVocConjugation::Singular).isEmpty();
    mw->singularFirstPersonLabel->setVisible(!empty);
    mw->singularFirstPersonLineEdit->setVisible(!empty);
    mw->singularFirstPersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::Second, KEduVocConjugation::Singular).isEmpty();
    mw->singularSecondPersonLabel->setVisible(!empty);
    mw->singularSecondPersonLineEdit->setVisible(!empty);
    mw->singularSecondPersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular).isEmpty();
    mw->singularThirdMalePersonLabel->setVisible(!empty);
    mw->singularThirdMalePersonLineEdit->setVisible(!empty);
    mw->singularThirdMalePersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular).isEmpty();
    mw->singularThirdFemalePersonLabel->setVisible(!empty);
    mw->singularThirdFemalePersonLineEdit->setVisible(!empty);
    mw->singularThirdFemalePersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Singular).isEmpty();
    mw->singularThirdNeutralPersonLabel->setVisible(!empty);
    mw->singularThirdNeutralPersonLineEdit->setVisible(!empty);
    mw->singularThirdNeutralPersonLineEdit->setText("");

    // dual
    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::First, KEduVocConjugation::Dual).isEmpty();
    mw->dualFirstPersonLabel->setVisible(!empty);
    mw->dualFirstPersonLineEdit->setVisible(!empty);
    mw->dualFirstPersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::Second, KEduVocConjugation::Dual).isEmpty();
    mw->dualSecondPersonLabel->setVisible(!empty);
    mw->dualSecondPersonLineEdit->setVisible(!empty);
    mw->dualSecondPersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual).isEmpty();
    mw->dualThirdMalePersonLabel->setVisible(!empty);
    mw->dualThirdMalePersonLineEdit->setVisible(!empty);
    mw->dualThirdMalePersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual).isEmpty();
    mw->dualThirdFemalePersonLabel->setVisible(!empty);
    mw->dualThirdFemalePersonLineEdit->setVisible(!empty);
    mw->dualThirdFemalePersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Dual).isEmpty();
    mw->dualThirdNeutralPersonLabel->setVisible(!empty);
    mw->dualThirdNeutralPersonLineEdit->setVisible(!empty);
    mw->dualThirdNeutralPersonLineEdit->setText("");

    // plural
    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::First, KEduVocConjugation::Plural).isEmpty();
    mw->pluralFirstPersonLabel->setVisible(!empty);
    mw->pluralFirstPersonLineEdit->setVisible(!empty);
    mw->pluralFirstPersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::Second, KEduVocConjugation::Plural).isEmpty();
    mw->pluralSecondPersonLabel->setVisible(!empty);
    mw->pluralSecondPersonLineEdit->setVisible(!empty);
    mw->pluralSecondPersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural).isEmpty();
    mw->pluralThirdMalePersonLabel->setVisible(!empty);
    mw->pluralThirdMalePersonLineEdit->setVisible(!empty);
    mw->pluralThirdMalePersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural).isEmpty();
    mw->pluralThirdFemalePersonLabel->setVisible(!empty);
    mw->pluralThirdFemalePersonLineEdit->setVisible(!empty);
    mw->pluralThirdFemalePersonLineEdit->setText("");

    empty = m_entry->exp->translation(Prefs::toIdentifier()).
        conjugations()[tense].conjugation(
        KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Plural).isEmpty();
    mw->pluralThirdNeutralPersonLabel->setVisible(!empty);
    mw->pluralThirdNeutralPersonLineEdit->setVisible(!empty);
    mw->pluralThirdNeutralPersonLineEdit->setText("");

    return false;
}


void VerbQueryDlg::showSolution()
{
    setAnswerTainted();

    resetAllFields();
    mw->dont_know->setDefault(true);

    QString tense = m_tenses.value(0);

    mw->singularFirstPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Singular));
    mw->singularSecondPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Singular));
    mw->singularThirdMalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular));
    mw->singularThirdFemalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular));
    mw->singularThirdNeutralPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Singular));

    mw->dualFirstPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Dual));
    mw->dualSecondPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Dual));
    mw->dualThirdMalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual));
    mw->dualThirdFemalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual));
    mw->dualThirdNeutralPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Dual));

    mw->pluralFirstPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Plural));
    mw->pluralSecondPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Plural));
    mw->pluralThirdMalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural));
    mw->pluralThirdFemalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural));
    mw->pluralThirdNeutralPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Plural));

    // verify singular
    verifyField(mw->singularFirstPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Singular));
    verifyField(mw->singularSecondPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Singular));
    verifyField(mw->singularThirdMalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular));
    verifyField(mw->singularThirdFemalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular));
    verifyField(mw->singularThirdNeutralPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Singular));

    // verify dual
    verifyField(mw->dualFirstPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Dual));
    verifyField(mw->dualSecondPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Dual));
    verifyField(mw->dualThirdMalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual));
    verifyField(mw->dualThirdFemalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual));
    verifyField(mw->dualThirdNeutralPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Dual));

    // verify plural
    verifyField(mw->pluralFirstPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Plural));
    verifyField(mw->pluralSecondPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Plural));
    verifyField(mw->pluralThirdMalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural));
    verifyField(mw->pluralThirdFemalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural));
    verifyField(mw->pluralThirdNeutralPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Plural));

}


void VerbQueryDlg::verifyClicked()
{
    QString tense = m_tenses.value(0);

    const KEduVocConjugation conj = m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense];

    bool known = true;

    // singular
    if (!verifyField(mw->singularFirstPersonLineEdit, conj.conjugation(KEduVocConjugation::First, KEduVocConjugation::Singular))) {
        known = false;
    }
    if (!verifyField(mw->singularSecondPersonLineEdit, conj.conjugation(KEduVocConjugation::Second, KEduVocConjugation::Singular))) {
        known = false;
    }
    if (!verifyField(mw->singularThirdMalePersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular))) {
        known = false;
    }
    if (!verifyField(mw->singularThirdFemalePersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular))) {
        known = false;
    }
    if (!verifyField(mw->singularThirdNeutralPersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Singular))) {
        known = false;
    }

    // dual
    if (!verifyField(mw->dualFirstPersonLineEdit, conj.conjugation(KEduVocConjugation::First, KEduVocConjugation::Dual))) {
        known = false;
    }
    if (!verifyField(mw->dualSecondPersonLineEdit, conj.conjugation(KEduVocConjugation::Second, KEduVocConjugation::Dual))) {
        known = false;
    }
    if (!verifyField(mw->dualThirdMalePersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual))) {
        known = false;
    }
    if (!verifyField(mw->dualThirdFemalePersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual))) {
        known = false;
    }
    if (!verifyField(mw->dualThirdNeutralPersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Dual))) {
        known = false;
    }

    // plural
    if (!verifyField(mw->pluralFirstPersonLineEdit, conj.conjugation(KEduVocConjugation::First, KEduVocConjugation::Plural))) {
        known = false;
    }
    if (!verifyField(mw->pluralSecondPersonLineEdit, conj.conjugation(KEduVocConjugation::Second, KEduVocConjugation::Plural))) {
        known = false;
    }
    if (!verifyField(mw->pluralThirdMalePersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural))) {
        known = false;
    }
    if (!verifyField(mw->pluralThirdFemalePersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural))) {
        known = false;
    }
    if (!verifyField(mw->pluralThirdNeutralPersonLineEdit, conj.conjugation(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Plural))) {
        known = false;
    }

    if (known) {
        kDebug() << "correct!";
        resultCorrect();
        emit nextEntry();
    } else {
        setAnswerTainted();
        mw->dont_know->setDefault(true);
    }
}


void VerbQueryDlg::resetAllFields()
{
    setWidgetStyle(mw->singularFirstPersonLineEdit);
    setWidgetStyle(mw->singularSecondPersonLineEdit);
    setWidgetStyle(mw->singularThirdFemalePersonLineEdit);
    setWidgetStyle(mw->singularThirdMalePersonLineEdit);
    setWidgetStyle(mw->singularThirdNeutralPersonLineEdit);

    setWidgetStyle(mw->dualFirstPersonLineEdit);
    setWidgetStyle(mw->dualSecondPersonLineEdit);
    setWidgetStyle(mw->dualThirdFemalePersonLineEdit);
    setWidgetStyle(mw->dualThirdMalePersonLineEdit);
    setWidgetStyle(mw->dualThirdNeutralPersonLineEdit);

    setWidgetStyle(mw->pluralFirstPersonLineEdit);
    setWidgetStyle(mw->pluralSecondPersonLineEdit);
    setWidgetStyle(mw->pluralThirdFemalePersonLineEdit);
    setWidgetStyle(mw->pluralThirdMalePersonLineEdit);
    setWidgetStyle(mw->pluralThirdNeutralPersonLineEdit);
}


void VerbQueryDlg::setProgressCounter(int current, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(current);
}

bool VerbQueryDlg::verifyField(QLineEdit * lineEdit, const QString & userAnswer)
{
    double result = verifyAnswer(lineEdit->text(), userAnswer);
    if ( result == 1.0 ) {
        setWidgetStyle(lineEdit, PositiveResult);
        return true;
    } else {
        setWidgetStyle(lineEdit, NegativeResult);
        return false;
    }
}

#include "VerbQueryDlg.moc"
