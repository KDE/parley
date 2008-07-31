/***************************************************************************

                     tenses of irreg. verbs dialog page

    -----------------------------------------------------------------------

    begin         : Sat Nov 27 20:20:34 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
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

    connect(b_next, SIGNAL(clicked()), SLOT(slotNextConj()));
    connect(tensebox, SIGNAL(activated(int)), SLOT(slotTenseSelected(int)));

    connect(singularFirstPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(singularSecondPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(singularThirdMalePersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(singularThirdFemalePersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(singularThirdNeutralPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));

    connect(pluralFirstPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralSecondPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralThirdMalePersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralThirdFemalePersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralThirdNeutralPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
}

void TenseEntryPage::setData(int row, int col)
{
    m_currentRow = row;
    m_currentTranslation = col;

    if ( m_currentTranslation < 0 ) {
        kDebug() << "Invalid identifier for TenseEntryPage" << m_currentTranslation;
        return;
    }

    KEduVocPersonalPronoun pron = m_doc->identifier(m_currentTranslation).personalPronouns();

    singularFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First, KEduVocWordFlag::Singular ));
    singularSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second, KEduVocWordFlag::Singular ));
    singularThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine, KEduVocWordFlag::Singular ));
    singularThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine, KEduVocWordFlag::Singular ));
    singularThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter, KEduVocWordFlag::Singular ));

    dualFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First, KEduVocWordFlag::Dual ));
    dualSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second, KEduVocWordFlag::Dual ));
    dualThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine, KEduVocWordFlag::Dual ));
    dualThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine, KEduVocWordFlag::Dual ));
    dualThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter, KEduVocWordFlag::Dual ));

    pluralFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First, KEduVocWordFlag::Plural ));
    pluralSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second, KEduVocWordFlag::Plural ));
    pluralThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine, KEduVocWordFlag::Plural ));
    pluralThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine, KEduVocWordFlag::Plural ));
    pluralThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter, KEduVocWordFlag::Plural ));

    tensebox->clear();
    tensebox->addItems(m_doc->tenseDescriptions());

    dualGroupBox->setVisible( m_doc->identifier(m_currentTranslation).personalPronouns().dualExists() );

    bool maleFemaleDifferent = m_doc->identifier(m_currentTranslation).personalPronouns().maleFemaleDifferent();

    singularThirdMalePersonLabel->setVisible(maleFemaleDifferent);
    singularThirdMalePersonLineEdit->setVisible(maleFemaleDifferent);
    singularThirdFemalePersonLabel->setVisible(maleFemaleDifferent);
    singularThirdFemalePersonLineEdit->setVisible(maleFemaleDifferent);

    pluralThirdMalePersonLabel->setVisible(maleFemaleDifferent);
    pluralThirdMalePersonLineEdit->setVisible(maleFemaleDifferent);
    pluralThirdFemalePersonLabel->setVisible(maleFemaleDifferent);
    pluralThirdFemalePersonLineEdit->setVisible(maleFemaleDifferent);

    if ( !maleFemaleDifferent ) {
        singularThirdNeutralPersonLabel->setVisible(true);
        singularThirdNeutralPersonLineEdit->setVisible(true);
        pluralThirdNeutralPersonLabel->setVisible(true);
        pluralThirdNeutralPersonLineEdit->setVisible(true);
    } else {
        bool neutralExists = m_doc->identifier(m_currentTranslation).personalPronouns().neutralExists();
        singularThirdNeutralPersonLabel->setVisible(neutralExists);
        singularThirdNeutralPersonLineEdit->setVisible(neutralExists);
        pluralThirdNeutralPersonLabel->setVisible(neutralExists);
        pluralThirdNeutralPersonLineEdit->setVisible(neutralExists);
    }

    m_conjugations = m_doc->entry(m_currentRow)->translation(m_currentTranslation).conjugations();

    m_lastSelection = tensebox->currentText();
    updateEntries();

    setModified(false);
}


void TenseEntryPage::textChanged(const QString& text)
{
    Q_UNUSED(text);
    setModified(true);
}


void TenseEntryPage::slotTenseSelected(int sel)
{
kDebug() << "selected tense " << sel;
    saveCurrentTense();
    m_lastSelection = m_doc->tenseDescriptions().value(sel);
    updateEntries();
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
    saveCurrentTense();
    m_doc->entry(m_currentRow)->translation(m_currentTranslation).setConjugations(m_conjugations);
    setModified(false);
}

void TenseEntryPage::clear()
{
    m_conjugations.clear();
}

void TenseEntryPage::saveCurrentTense()
{
    QString selection = m_lastSelection;

    KEduVocWordFlag::ConjugationNumber num = KEduVocWordFlag::Singular;

    m_conjugations[selection].setConjugation(singularFirstPersonLineEdit->text(),  KEduVocWordFlag::First , num);
    m_conjugations[selection].setConjugation(singularSecondPersonLineEdit->text(),  KEduVocWordFlag::Second , num);
    m_conjugations[selection].setConjugation(singularThirdMalePersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Masculine , num);
    m_conjugations[selection].setConjugation(singularThirdFemalePersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Feminine , num);
    m_conjugations[selection].setConjugation(singularThirdNeutralPersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Neuter , num);

    num = KEduVocWordFlag::Dual;
    m_conjugations[selection].setConjugation(dualFirstPersonLineEdit->text(),  KEduVocWordFlag::First , num);
    m_conjugations[selection].setConjugation(dualSecondPersonLineEdit->text(),  KEduVocWordFlag::Second , num);
    m_conjugations[selection].setConjugation(dualThirdMalePersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Masculine , num);
    m_conjugations[selection].setConjugation(dualThirdFemalePersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Feminine , num);
    m_conjugations[selection].setConjugation(dualThirdNeutralPersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Neuter , num);

    num = KEduVocWordFlag::Plural;
    m_conjugations[selection].setConjugation(pluralFirstPersonLineEdit->text(),  KEduVocWordFlag::First , num);
    m_conjugations[selection].setConjugation(pluralSecondPersonLineEdit->text(),  KEduVocWordFlag::Second , num);
    m_conjugations[selection].setConjugation(pluralThirdMalePersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Masculine , num);
    m_conjugations[selection].setConjugation(pluralThirdFemalePersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Feminine , num);
    m_conjugations[selection].setConjugation(pluralThirdNeutralPersonLineEdit->text(),  KEduVocWordFlag::Third | KEduVocWordFlag::Neuter , num);
}

void TenseEntryPage::updateEntries()
{
    QString selection = m_lastSelection;

    KEduVocWordFlag::ConjugationNumber num = KEduVocWordFlag::Singular;
    singularFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::First , num));
    singularSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Second , num));
    singularThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine , num));
    singularThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine , num));
    singularThirdNeutralPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter , num));

    num = KEduVocWordFlag::Dual;
    dualFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::First , num));
    dualSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Second , num));
    dualThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine , num));
    dualThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine , num));
    dualThirdNeutralPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter , num));

    num = KEduVocWordFlag::Plural;
    pluralFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::First , num));
    pluralSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Second , num));
    pluralThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine , num));
    pluralThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine , num));
    pluralThirdNeutralPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter , num));
}


#include "TenseEntryPage.moc"
