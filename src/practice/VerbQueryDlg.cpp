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

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(editEntry()));

    mw->dont_know->setShortcut(QKeySequence(Qt::Key_Escape));

    mw->countbar->setFormat("%v/%m");
    mw->timebar->setFormat("%v");

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
    PracticeDialog::setEntry(entry);

    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->show_all->setDefault(true);

    mw->dualGroupBox->setVisible( m_doc->identifier(Prefs::toIdentifier()).personalPronouns().dualExists());



    mw->singularFirstPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::First, KEduVocConjugation::Singular));
    mw->singularSecondPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::Second, KEduVocConjugation::Singular));
    mw->singularThirdMalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular));
    mw->singularThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular));
    mw->singularThirdNeuterPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular));

    mw->dualFirstPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::First, KEduVocConjugation::Dual));
    mw->dualSecondPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::Second, KEduVocConjugation::Dual));
    mw->dualThirdMalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual));
    mw->dualThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual));
    mw->dualThirdNeuterPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Dual));

    mw->pluralFirstPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::First, KEduVocConjugation::Plural));
    mw->pluralSecondPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::Second, KEduVocConjugation::Plural));
    mw->pluralThirdMalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural));
    mw->pluralThirdFemalePersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural));
    mw->pluralThirdNeuterPersonLabel->setText(m_doc->identifier(Prefs::toIdentifier()).personalPronouns().personalPronoun(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural));









    all_known = true;
    m_currentTense = -1;
    next();

    mw->singularFirstPersonLineEdit->setFocus();
}


bool VerbQueryDlg::next()
{
    resetAllFields();
    QString tense;

    QString s;
    s = m_entry->exp->translation(Prefs::toIdentifier()).text();
    mw->verbNameLabel->setText(s);

    if (m_currentTense < m_entry->exp->translation(Prefs::toIdentifier()).conjugations().count() - 1) {
        m_currentTense++;
    }

    tense = m_entry->exp->translation(Prefs::toIdentifier()).conjugations().keys().value(m_currentTense);
    QString msg = i18n("Current tense is: %1.", tense);

    mw->instructionLabel->setText(msg);

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
        KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular).isEmpty();
    mw->singularThirdNeuterPersonLabel->setVisible(!empty);
    mw->singularThirdNeuterPersonLineEdit->setVisible(!empty);
    mw->singularThirdNeuterPersonLineEdit->setText("");

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
        KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Dual).isEmpty();
    mw->dualThirdNeuterPersonLabel->setVisible(!empty);
    mw->dualThirdNeuterPersonLineEdit->setVisible(!empty);
    mw->dualThirdNeuterPersonLineEdit->setText("");

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
        KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural).isEmpty();
    mw->pluralThirdNeuterPersonLabel->setVisible(!empty);
    mw->pluralThirdNeuterPersonLineEdit->setVisible(!empty);
    mw->pluralThirdNeuterPersonLineEdit->setText("");





























    mw->singularSecondPersonLineEdit->setText("");
    mw->singularSecondPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Singular).isEmpty());

    mw->singularThirdMalePersonLineEdit->setText("");
    mw->singularThirdMalePersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular).isEmpty());

    mw->singularThirdFemalePersonLineEdit->setText("");
    mw->singularThirdFemalePersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular).isEmpty());

    mw->singularThirdNeuterPersonLineEdit->setText("");
    mw->singularThirdNeuterPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular).isEmpty());



mw->dualFirstPersonLineEdit->setText("");
    mw->dualFirstPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Dual).isEmpty());

    mw->dualSecondPersonLineEdit->setText("");
    mw->dualSecondPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Dual).isEmpty());

    mw->dualThirdMalePersonLineEdit->setText("");
    mw->dualThirdMalePersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual).isEmpty());

    mw->dualThirdFemalePersonLineEdit->setText("");
    mw->dualThirdFemalePersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual).isEmpty());

    mw->dualThirdNeuterPersonLineEdit->setText("");
    mw->dualThirdNeuterPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Dual).isEmpty());


    mw->pluralFirstPersonLineEdit->setText("");
    mw->pluralFirstPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Plural).isEmpty());

    mw->pluralSecondPersonLineEdit->setText("");
    mw->pluralSecondPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Plural).isEmpty());

    mw->pluralThirdMalePersonLineEdit->setText("");
    mw->pluralThirdMalePersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural).isEmpty());

    mw->pluralThirdFemalePersonLineEdit->setText("");
    mw->pluralThirdFemalePersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural).isEmpty());

    mw->pluralThirdNeuterPersonLineEdit->setText("");
    mw->pluralThirdNeuterPersonLineEdit->setEnabled(!m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural).isEmpty());

//     bool common = m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular);
//     if (common) {
//         mw->singularThirdMalePersonLineEdit->setEnabled(false);
//         mw->singularThirdNeuterPersonLineEdit->setEnabled(false);
//     }
//
//     common = m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural);
//     if (common) {
//         mw->pluralThirdMalePersonLineEdit->setEnabled(false);
//         mw->pluralThirdNeuterPersonLineEdit->setEnabled(false);
//     }

    return false;
}


void VerbQueryDlg::showSolution()
{
    resetAllFields();
    mw->dont_know->setDefault(true);

    QString tense = m_entry->exp->translation(Prefs::toIdentifier()).conjugations().keys().value(m_currentTense);

    mw->singularFirstPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Singular));
    mw->singularSecondPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Singular));
    mw->singularThirdMalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular));
    mw->singularThirdFemalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular));
    mw->singularThirdNeuterPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular));

    mw->pluralFirstPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Plural));
    mw->pluralSecondPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Plural));
    mw->pluralThirdMalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural));
    mw->pluralThirdFemalePersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural));
    mw->pluralThirdNeuterPersonLineEdit->setText(m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural));

    verifyField(mw->singularFirstPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Singular));
    verifyField(mw->singularSecondPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Singular));
    verifyField(mw->singularThirdFemalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular));

//     bool common = m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular);
//     if (!common) {
//         verifyField(mw->singularThirdMalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular));
//         verifyField(mw->singularThirdNeuterPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular));
//     }
//
//     verifyField(mw->pluralFirstPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Plural));
//     verifyField(mw->pluralSecondPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Plural));
//     verifyField(mw->pluralThirdFemalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural));
//
//     common = m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural);
//     if (!common) {
//         verifyField(mw->pluralThirdMalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural));
//         verifyField(mw->pluralThirdNeuterPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural));
//     }

}


void VerbQueryDlg::verifyClicked()
{
    QString tense = m_entry->exp->translation(Prefs::toIdentifier()).conjugations().keys().value(m_currentTense);

    bool known = true;

    if (!verifyField(mw->singularFirstPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Singular)))
        known = false;

    if (!verifyField(mw->singularSecondPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Singular)))
        known = false;

    if (!verifyField(mw->singularThirdFemalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular)))
        known = false;

// //     bool common = m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular);
// //     if (!common) {
// //         if (!verifyField(mw->singularThirdMalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular)))
// //             known = false;
// //
// //         if (!verifyField(mw->singularThirdNeuterPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular)))
// //             known = false;
// //     }
//
//     if (!verifyField(mw->pluralFirstPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::First, KEduVocConjugation::Plural)))
//         known = false;
//
//     if (!verifyField(mw->pluralSecondPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::Second, KEduVocConjugation::Plural)))
//         known = false;
//
//     if (!verifyField(mw->pluralThirdFemalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural)))
//         known = false;
//
//     common = m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural);
//     if (!common) {
//         if (!verifyField(mw->pluralThirdMalePersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural)))
//             known = false;
//
//         if (!verifyField(mw->pluralThirdNeuterPersonLineEdit, m_entry->exp->translation(Prefs::toIdentifier()).conjugations()[tense].conjugation(KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural)))
//             known = false;
//     }

    if (known)
//         knowItClicked();
        resultCorrect();
    else {
        all_known = false;
        mw->dont_know->setDefault(true);
    }
}
/*
void VerbQueryDlg::knowItClicked()
{
    resetAllFields();
    if (m_currentTense >= m_entry->exp->translation(Prefs::toIdentifier()).conjugations().count() - 1) {
        if (all_known)
            emit sigQueryChoice(SkipKnown);
        else
            emit sigQueryChoice(SkipUnknown);
    } else
        next();
}


void VerbQueryDlg::dontKnowClicked()
{
    all_known = false;
    if (m_currentTense >= m_entry->exp->translation(Prefs::toIdentifier()).conjugations().count() - 1)
        emit sigQueryChoice(SkipUnknown);
    else {
        startTimer();
        next();
    }
}*/


void VerbQueryDlg::resetAllFields()
{
    resetQueryWidget(mw->singularFirstPersonLineEdit);
    resetQueryWidget(mw->singularSecondPersonLineEdit);
    resetQueryWidget(mw->singularThirdFemalePersonLineEdit);
    resetQueryWidget(mw->singularThirdMalePersonLineEdit);
    resetQueryWidget(mw->singularThirdNeuterPersonLineEdit);

    resetQueryWidget(mw->dualFirstPersonLineEdit);
    resetQueryWidget(mw->dualSecondPersonLineEdit);
    resetQueryWidget(mw->dualThirdFemalePersonLineEdit);
    resetQueryWidget(mw->dualThirdMalePersonLineEdit);
    resetQueryWidget(mw->dualThirdNeuterPersonLineEdit);

    resetQueryWidget(mw->pluralFirstPersonLineEdit);
    resetQueryWidget(mw->pluralSecondPersonLineEdit);
    resetQueryWidget(mw->pluralThirdFemalePersonLineEdit);
    resetQueryWidget(mw->pluralThirdMalePersonLineEdit);
    resetQueryWidget(mw->pluralThirdNeuterPersonLineEdit);
}


void VerbQueryDlg::setProgressCounter(int m_currentTense, int total)
{
    mw->countbar->setMaximum(total);
    mw->countbar->setValue(m_currentTense);
}

#include "VerbQueryDlg.moc"
