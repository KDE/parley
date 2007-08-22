/***************************************************************************

                     tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin         : Sat Nov 27 20:20:34 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "TenseEntryPage.h"

#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>

#include <kvtlanguages.h>
#include "EntryDlg.h"

TenseEntryPage::TenseEntryPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    m_doc = doc;
    m_currentRow = -1;
    m_currentTranslation = -1;

    setupUi(this);

    connect(third_p_common, SIGNAL(toggled(bool)), SLOT(slotThirdPCommonToggled(bool)));
    connect(third_s_common, SIGNAL(toggled(bool)), SLOT(slotThirdSCommonToggled(bool)));
    connect(b_next, SIGNAL(clicked()), SLOT(slotNextConj()));
    connect(tensebox, SIGNAL(activated(int)), SLOT(slotTenseSelected(int)));

    connect(thirdN_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdNPluralChanged(const QString&)));
    connect(thirdN_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdNSingularChanged(const QString&)));
    connect(thirdM_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdMPluralChanged(const QString&)));
    connect(thirdM_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdMSingularChanged(const QString&)));
    connect(thirdF_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdFPluralChanged(const QString&)));
    connect(thirdF_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdFSingularChanged(const QString&)));
    connect(second_plural, SIGNAL(textChanged(const QString&)), SLOT(secondPluralChanged(const QString&)));
    connect(second_singular, SIGNAL(textChanged(const QString&)), SLOT(secondSingularChanged(const QString&)));
    connect(first_plural, SIGNAL(textChanged(const QString&)), SLOT(firstPluralChanged(const QString&)));
    connect(first_singular, SIGNAL(textChanged(const QString&)), SLOT(firstSingularChanged(const QString&)));

    selection = "";
    /*
      // FIXME: fill labels with prefixes ?

      label_first_plural->setText (con_prefix.pers1Plural (CONJ_PREFIX));
      label_first_singular->setText (con_prefix.pers1Singular (CONJ_PREFIX));
      label_second_singular->setText (con_prefix.pers2Singular (CONJ_PREFIX));
      label_second_plural->setText (con_prefix.pers2Plural (CONJ_PREFIX));
      label_thirdM_singular->setText (con_prefix.pers3MaleSingular (CONJ_PREFIX));
      label_thirdM_plural->setText (con_prefix.pers3MalePlural (CONJ_PREFIX));
      label_thirdF_singular->setText (con_prefix.pers3FemaleSingular (CONJ_PREFIX));
      label_thirdF_plural->setText (con_prefix.pers3FemalePlural (CONJ_PREFIX));
      label_thirdN_singular->setText (con_prefix.pers3NaturalSingular (CONJ_PREFIX));
      label_thirdN_plural->setText (con_prefix.pers3NaturalPlural (CONJ_PREFIX));
    */
}

void TenseEntryPage::setData(int row, int col)
{
    m_currentRow = row;
    m_currentTranslation = col;

    prefix = m_doc->conjugation(m_currentTranslation);

    for (int i = 0; i <  KEduVocConjugation::tenseCount(); i++)
        tensebox->addItem(KEduVocConjugation::getName(i));

    conjugations = m_doc->entry(m_currentRow)->translation(m_currentTranslation).conjugation();
    slotTenseSelected(0);
    updateFields();

    setModified(false);
}


void TenseEntryPage::firstPluralChanged(const QString& s)
{
    conjugations.setPers1Plural(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::firstSingularChanged(const QString& s)
{
    conjugations.setPers1Singular(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::secondSingularChanged(const QString& s)
{
    conjugations.setPers2Singular(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::secondPluralChanged(const QString& s)
{
    conjugations.setPers2Plural(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::thirdFPluralChanged(const QString& s)
{
    conjugations.setPers3FemalePlural(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::thirdFSingularChanged(const QString& s)
{
    conjugations.setPers3FemaleSingular(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::thirdMSingularChanged(const QString& s)
{
    conjugations.setPers3MaleSingular(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::thirdNSingularChanged(const QString& s)
{
    conjugations.setPers3NaturalSingular(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::thirdNPluralChanged(const QString& s)
{
    conjugations.setPers3NaturalPlural(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::thirdMPluralChanged(const QString& s)
{
    conjugations.setPers3MalePlural(selection, s);
    updateFields();
    setModified(true);
}


void TenseEntryPage::slotTenseSelected(int sel)
{
    selection = KEduVocConjugation::getAbbrev(sel);
    first_plural->setText(conjugations.pers1Plural(selection));
    first_singular->setText(conjugations.pers1Singular(selection));
    second_plural->setText(conjugations.pers2Plural(selection));
    second_singular->setText(conjugations.pers2Singular(selection));
    thirdM_plural->setText(conjugations.pers3MalePlural(selection));
    thirdM_singular->setText(conjugations.pers3MaleSingular(selection));
    thirdF_plural->setText(conjugations.pers3FemalePlural(selection));
    thirdF_singular->setText(conjugations.pers3FemaleSingular(selection));
    thirdN_plural->setText(conjugations.pers3NaturalPlural(selection));
    thirdN_singular->setText(conjugations.pers3NaturalSingular(selection));

    bool common = conjugations.pers3SingularCommon(selection);
    third_s_common->setChecked(common);
    thirdM_singular->setEnabled(!common);
    thirdN_singular->setEnabled(!common);

    common = conjugations.pers3PluralCommon(selection);
    third_p_common->setChecked(common);
    thirdN_plural->setEnabled(!common);
    thirdM_plural->setEnabled(!common);
}


void TenseEntryPage::slotThirdSCommonToggled(bool common)
{
    conjugations.setPers3SingularCommon(selection, common);
    thirdM_singular->setEnabled(!common);
    thirdN_singular->setEnabled(!common);
    setModified(true);
}


void TenseEntryPage::slotThirdPCommonToggled(bool common)
{
    conjugations.setPers3PluralCommon(selection, common);
    thirdN_plural->setEnabled(!common);
    thirdM_plural->setEnabled(!common);
    setModified(true);
}


void TenseEntryPage::slotNextConj()
{
    int j;
    for (int i = tensebox->currentIndex()+1; i < tensebox->count(); i++) {

        for (j = 0; j < conjugations.entryCount(); j++) {
            if (KEduVocConjugation::getAbbrev(i) == conjugations.getType(j)) {
                tensebox->setCurrentIndex(i);
                slotTenseSelected(i);
                return;
            }
        }
    }

    for (int i = 0; i < tensebox->currentIndex()-1; i++) {
        for (j = 0; j < conjugations.entryCount(); j++) {
            if (KEduVocConjugation::getAbbrev(i) == conjugations.getType(j)) {
                tensebox->setCurrentIndex(i);
                slotTenseSelected(i);
                return;
            }
        }
    }
}


void TenseEntryPage::updateFields()
{
    b_next->setEnabled(conjugations.entryCount() > 1); // next button
}


bool TenseEntryPage::isModified()
{
    return m_modified;
}

void TenseEntryPage::setModified(bool mod)
{
    m_modified = mod;
    if (mod) {
        emit sigModified();
    }
}

void TenseEntryPage::commitData()
{
    conjugations.cleanUp();
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setConjugation(conjugations);
    setModified(false);
}

void TenseEntryPage::clear()
{
    conjugations = KEduVocConjugation();
    updateFields();
}
#include "TenseEntryPage.moc"
