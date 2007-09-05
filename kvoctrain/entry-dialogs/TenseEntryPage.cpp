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

    if ( m_currentTranslation < 0 ) {
        kDebug() << "Invalid tense for TenseEntryPage";
        return;
    }

    prefix = m_doc->identifier(m_currentTranslation).personalPronouns();

    tensebox->clear();
    tensebox->addItems(m_doc->tenseDescriptions());

    m_conjugations = m_doc->entry(m_currentRow)->translation(m_currentTranslation).conjugations();
    slotTenseSelected(0);

    setModified(false);
}


void TenseEntryPage::firstPluralChanged(const QString& s)
{
    m_conjugations[selection].setPers1Plural(s);
    setModified(true);
}


void TenseEntryPage::firstSingularChanged(const QString& s)
{
    m_conjugations[selection].setPers1Singular(s);
    setModified(true);
}


void TenseEntryPage::secondSingularChanged(const QString& s)
{
    m_conjugations[selection].setPers2Singular(s);
    setModified(true);
}


void TenseEntryPage::secondPluralChanged(const QString& s)
{
    m_conjugations[selection].setPers2Plural(s);
    setModified(true);
}


void TenseEntryPage::thirdFPluralChanged(const QString& s)
{
    m_conjugations[selection].setPers3FemalePlural(s);
    setModified(true);
}


void TenseEntryPage::thirdFSingularChanged(const QString& s)
{
    m_conjugations[selection].setPers3FemaleSingular(s);
    setModified(true);
}


void TenseEntryPage::thirdMSingularChanged(const QString& s)
{
    m_conjugations[selection].setPers3MaleSingular(s);
    setModified(true);
}


void TenseEntryPage::thirdNSingularChanged(const QString& s)
{
    m_conjugations[selection].setPers3NaturalSingular(s);
    setModified(true);
}


void TenseEntryPage::thirdNPluralChanged(const QString& s)
{
    m_conjugations[selection].setPers3NaturalPlural(s);
    setModified(true);
}


void TenseEntryPage::thirdMPluralChanged(const QString& s)
{
    m_conjugations[selection].setPers3MalePlural(s);
    setModified(true);
}


void TenseEntryPage::slotTenseSelected(int sel)
{
    selection = m_doc->tenseDescriptions().value(sel);
    first_plural->setText(m_conjugations[selection].pers1Plural());
    first_singular->setText(m_conjugations[selection].pers1Singular());
    second_plural->setText(m_conjugations[selection].pers2Plural());
    second_singular->setText(m_conjugations[selection].pers2Singular());
    thirdM_plural->setText(m_conjugations[selection].pers3MalePlural());
    thirdM_singular->setText(m_conjugations[selection].pers3MaleSingular());
    thirdF_plural->setText(m_conjugations[selection].pers3FemalePlural());
    thirdF_singular->setText(m_conjugations[selection].pers3FemaleSingular());
    thirdN_plural->setText(m_conjugations[selection].pers3NaturalPlural());
    thirdN_singular->setText(m_conjugations[selection].pers3NaturalSingular());

    bool common = m_conjugations[selection].pers3SingularCommon();
    third_s_common->setChecked(common);
    thirdM_singular->setEnabled(!common);
    thirdN_singular->setEnabled(!common);

    common = m_conjugations[selection].pers3PluralCommon();
    third_p_common->setChecked(common);
    thirdN_plural->setEnabled(!common);
    thirdM_plural->setEnabled(!common);
}


void TenseEntryPage::slotThirdSCommonToggled(bool common)
{
    m_conjugations[selection].setPers3SingularCommon(common);
    thirdM_singular->setEnabled(!common);
    thirdN_singular->setEnabled(!common);
    setModified(true);
}


void TenseEntryPage::slotThirdPCommonToggled(bool common)
{
    m_conjugations[selection].setPers3PluralCommon(common);
    thirdN_plural->setEnabled(!common);
    thirdM_plural->setEnabled(!common);
    setModified(true);
}


void TenseEntryPage::slotNextConj()
{
    QString tense = tensebox->currentText();
    int newIndex = m_doc->tenseDescriptions().indexOf(tense) +1;
    if ( newIndex >= m_doc->tenseDescriptions().count() ) {
        newIndex = 0;
    }
    QString newTense = m_doc->tenseDescriptions().value(newIndex);
    int tenseboxIndex = tensebox->findText(newTense);
    tensebox->setCurrentIndex(tenseboxIndex);
    slotTenseSelected(tenseboxIndex);
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
//     conjugations.cleanUp();
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setConjugations(m_conjugations);
    setModified(false);
}

void TenseEntryPage::clear()
{
    m_conjugations.clear();
}
#include "TenseEntryPage.moc"
