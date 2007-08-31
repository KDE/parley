/***************************************************************************

                    query dialog for verbs

    -----------------------------------------------------------------------

    begin         : Fri Dec 3 18:28:18 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

VerbQueryDlg::VerbQueryDlg(KEduVocDocument *doc, QWidget *parent) : QueryDlgBase(i18n("Verb Training"), doc, parent)
{
    mw = new Ui::VerbQueryDlgForm();
    mw->setupUi(mainWidget());

    connect(mw->dont_know, SIGNAL(clicked()), SLOT(dontKnowClicked()));
    connect(mw->know_it, SIGNAL(clicked()), SLOT(knowItClicked()));
    connect(mw->verify, SIGNAL(clicked()), SLOT(verifyClicked()));
    connect(mw->show_all, SIGNAL(clicked()), SLOT(showSolution()));

    connect(mw->p3pmField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pmChanged(const QString&)));
    connect(mw->p3snField, SIGNAL(textChanged(const QString&)), SLOT(slotP3snChanged(const QString&)));
    connect(mw->p3pnField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pnChanged(const QString&)));
    connect(mw->p3smField, SIGNAL(textChanged(const QString&)), SLOT(slotP3smChanged(const QString&)));
    connect(mw->p3pfField, SIGNAL(textChanged(const QString&)), SLOT(slotP3pfChanged(const QString&)));
    connect(mw->p3sfField, SIGNAL(textChanged(const QString&)), SLOT(slotP3sfChanged(const QString&)));
    connect(mw->p2pField, SIGNAL(textChanged(const QString&)), SLOT(slotP2pChanged(const QString&)));
    connect(mw->p2sField, SIGNAL(textChanged(const QString&)), SLOT(slotP2sChanged(const QString&)));
    connect(mw->p1pField, SIGNAL(textChanged(const QString&)), SLOT(slotP1pChanged(const QString&)));
    connect(mw->p1sField, SIGNAL(textChanged(const QString&)), SLOT(slotP1sChanged(const QString&)));

    connect(this, SIGNAL(user1Clicked()), this, SLOT(slotUser1()));

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


void VerbQueryDlg::initFocus() const
{
    mw->p1sField->setFocus();
}


void VerbQueryDlg::setQuery(int entry,
                            int col,
                            int q_cycle,
                            int q_num,
                            int q_start,
                            KEduVocExpression *exp,
                            const KEduVocConjugation &prefix,
                            const KEduVocConjugation &conjug)
{
    m_expression = exp;
    m_row = entry;
    m_queryOriginalColumn = col;
    mw->timebar->setEnabled(Prefs::showCounter());
    mw->timelabel->setEnabled(Prefs::showCounter());
    mw->show_all->setDefault(true);
    QString s;
    s.setNum(q_cycle);
    mw->progCount->setText(s);

    conjugations = conjug;

///@todo conjugations enable setting personal pronouns
    mw->p1sLabel->setText(prefix.pers1Singular(CONJ_PREFIX));
    mw->p2sLabel->setText(prefix.pers2Singular(CONJ_PREFIX));
    mw->p3smLabel->setText(prefix.pers3MaleSingular(CONJ_PREFIX));
    mw->p3sfLabel->setText(prefix.pers3FemaleSingular(CONJ_PREFIX));
    mw->p3snLabel->setText(prefix.pers3NaturalSingular(CONJ_PREFIX));

    mw->p1pLabel->setText(prefix.pers1Plural(CONJ_PREFIX));
    mw->p2pLabel->setText(prefix.pers2Plural(CONJ_PREFIX));
    mw->p3pmLabel->setText(prefix.pers3MalePlural(CONJ_PREFIX));
    mw->p3pfLabel->setText(prefix.pers3FemalePlural(CONJ_PREFIX));
    mw->p3pnLabel->setText(prefix.pers3NaturalPlural(CONJ_PREFIX));

    mw->countbar->setMaximum(q_start);
    mw->countbar->setValue(q_start - q_num + 1);

    startTimer();

    all_known = true;
    current = -1;
    next();
}


bool VerbQueryDlg::next()
{
    resetAllFields();
    QString tense;

    QString s;
    s = m_expression->translation(m_queryOriginalColumn).text();
    mw->baseLabel->setText(s);

    if (current < conjugations.entryCount() - 1)
        current++;

    tense = conjugations.getType(current);
    QString msg = i18n("Current tense is: %1.", tense);

    mw->instructionLabel->setText(msg);


    mw->p1sField->setText("");
    mw->p1sField->setEnabled(!conjugations.pers1Singular(tense).isEmpty());

    mw->p2sField->setText("");
    mw->p2sField->setEnabled(!conjugations.pers2Singular(tense).isEmpty());

    mw->p3smField->setText("");
    mw->p3smField->setEnabled(!conjugations.pers3MaleSingular(tense).isEmpty());

    mw->p3sfField->setText("");
    mw->p3sfField->setEnabled(!conjugations.pers3FemaleSingular(tense).isEmpty());

    mw->p3snField->setText("");
    mw->p3snField->setEnabled(!conjugations.pers3NaturalSingular(tense).isEmpty());

    mw->p1pField->setText("");
    mw->p1pField->setEnabled(!conjugations.pers1Plural(tense).isEmpty());

    mw->p2pField->setText("");
    mw->p2pField->setEnabled(!conjugations.pers2Plural(tense).isEmpty());

    mw->p3pmField->setText("");
    mw->p3pmField->setEnabled(!conjugations.pers3MalePlural(tense).isEmpty());

    mw->p3pfField->setText("");
    mw->p3pfField->setEnabled(!conjugations.pers3FemalePlural(tense).isEmpty());

    mw->p3pnField->setText("");
    mw->p3pnField->setEnabled(!conjugations.pers3NaturalPlural(tense).isEmpty());

    bool common = conjugations.pers3SingularCommon(tense);
    if (common) {
        mw->p3smField->setEnabled(false);
        mw->p3snField->setEnabled(false);
    }

    common = conjugations.pers3PluralCommon(tense);
    if (common) {
        mw->p3pmField->setEnabled(false);
        mw->p3pnField->setEnabled(false);
    }

    return false;
}


void VerbQueryDlg::showSolution()
{
    resetAllFields();
    mw->dont_know->setDefault(true);

    QString type = conjugations.getType(current);

    mw->p1sField->setText(conjugations.pers1Singular(type));
    mw->p2sField->setText(conjugations.pers2Singular(type));
    mw->p3smField->setText(conjugations.pers3MaleSingular(type));
    mw->p3sfField->setText(conjugations.pers3FemaleSingular(type));
    mw->p3snField->setText(conjugations.pers3NaturalSingular(type));

    mw->p1pField->setText(conjugations.pers1Plural(type));
    mw->p2pField->setText(conjugations.pers2Plural(type));
    mw->p3pmField->setText(conjugations.pers3MalePlural(type));
    mw->p3pfField->setText(conjugations.pers3FemalePlural(type));
    mw->p3pnField->setText(conjugations.pers3NaturalPlural(type));

    verifyField(mw->p1sField, conjugations.pers1Singular(type));
    verifyField(mw->p2sField, conjugations.pers2Singular(type));
    verifyField(mw->p3sfField, conjugations.pers3FemaleSingular(type));

    bool common = conjugations.pers3SingularCommon(type);
    if (!common) {
        verifyField(mw->p3smField, conjugations.pers3MaleSingular(type));
        verifyField(mw->p3snField, conjugations.pers3NaturalSingular(type));
    }

    verifyField(mw->p1pField, conjugations.pers1Plural(type));
    verifyField(mw->p2pField, conjugations.pers2Plural(type));
    verifyField(mw->p3pfField, conjugations.pers3FemalePlural(type));

    common = conjugations.pers3PluralCommon(type);
    if (!common) {
        verifyField(mw->p3pmField, conjugations.pers3MalePlural(type));
        verifyField(mw->p3pnField, conjugations.pers3NaturalPlural(type));
    }

}


void VerbQueryDlg::verifyClicked()
{
    QString type = conjugations.getType(current);

    bool known = true;

    if (!verifyField(mw->p1sField, conjugations.pers1Singular(type)))
        known = false;

    if (!verifyField(mw->p2sField, conjugations.pers2Singular(type)))
        known = false;

    if (!verifyField(mw->p3sfField, conjugations.pers3FemaleSingular(type)))
        known = false;

    bool common = conjugations.pers3SingularCommon(type);
    if (!common) {
        if (!verifyField(mw->p3smField, conjugations.pers3MaleSingular(type)))
            known = false;

        if (!verifyField(mw->p3snField, conjugations.pers3NaturalSingular(type)))
            known = false;
    }

    if (!verifyField(mw->p1pField, conjugations.pers1Plural(type)))
        known = false;

    if (!verifyField(mw->p2pField, conjugations.pers2Plural(type)))
        known = false;

    if (!verifyField(mw->p3pfField, conjugations.pers3FemalePlural(type)))
        known = false;

    common = conjugations.pers3PluralCommon(type);
    if (!common) {
        if (!verifyField(mw->p3pmField, conjugations.pers3MalePlural(type)))
            known = false;

        if (!verifyField(mw->p3pnField, conjugations.pers3NaturalPlural(type)))
            known = false;
    }

    if (known)
        knowItClicked();
    else {
        all_known = false;
        mw->dont_know->setDefault(true);
    }
}


void VerbQueryDlg::resetAllFields()
{
    resetQueryWidget(mw->p1sField);
    resetQueryWidget(mw->p2sField);
    resetQueryWidget(mw->p3sfField);
    resetQueryWidget(mw->p3smField);
    resetQueryWidget(mw->p3snField);

    resetQueryWidget(mw->p1pField);
    resetQueryWidget(mw->p2pField);
    resetQueryWidget(mw->p3pfField);
    resetQueryWidget(mw->p3pmField);
    resetQueryWidget(mw->p3pnField);
}





void VerbQueryDlg::knowItClicked()
{
    resetAllFields();
    if (current >= conjugations.entryCount() - 1) {
        if (all_known)
            emit sigQueryChoice(Known);
        else
            emit sigQueryChoice(Unknown);
    } else
        next();
}


void VerbQueryDlg::dontKnowClicked()
{
    all_known = false;
    if (current >= conjugations.entryCount() - 1)
        emit sigQueryChoice(Unknown);
    else {
        m_timer->start(1000);
        m_timerCount = Prefs::maxTimePer();
        next();
    }
}


void VerbQueryDlg::slotUser1()
{

    if (m_timer != 0)
        m_timer->stop();

    emit sigEditEntry(m_row, m_queryOriginalColumn);
}


void VerbQueryDlg::slotP3pfChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p3pfField);
}


void VerbQueryDlg::slotP3snChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p3snField);
}


void VerbQueryDlg::slotP3smChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p3smField);
}


void VerbQueryDlg::slotP3pnChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p3pnField);
}


void VerbQueryDlg::slotP3sfChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p3sfField);
}


void VerbQueryDlg::slotP1sChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p1sField);
}


void VerbQueryDlg::slotP2sChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p2sField);
}


void VerbQueryDlg::slotP3pmChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p3pmField);
}


void VerbQueryDlg::slotP1pChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p1pField);
}


void VerbQueryDlg::slotP2pChanged(const QString&)
{
    mw->verify->setDefault(true);
    resetQueryWidget(mw->p2pField);
}

#include "VerbQueryDlg.moc"
