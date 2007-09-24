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
    connect(singularThirdNeuterPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));

    connect(pluralFirstPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralSecondPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralThirdMalePersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralThirdFemalePersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    connect(pluralThirdNeuterPersonLineEdit, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
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

    singularFirstPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::First, KEduVocConjugation::Singular ));
    singularSecondPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::Second, KEduVocConjugation::Singular ));
    singularThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular ));
    singularThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular ));
    singularThirdNeuterPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Singular ));

    dualFirstPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::First, KEduVocConjugation::Dual ));
    dualSecondPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::Second, KEduVocConjugation::Dual ));
    dualThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual ));
    dualThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual ));
    dualThirdNeuterPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Dual ));

    pluralFirstPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::First, KEduVocConjugation::Plural ));
    pluralSecondPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::Second, KEduVocConjugation::Plural ));
    pluralThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural ));
    pluralThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural ));
    pluralThirdNeuterPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdNeuterCommon, KEduVocConjugation::Plural ));

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
        singularThirdNeuterPersonLabel->setVisible(true);
        singularThirdNeuterPersonLineEdit->setVisible(true);
        pluralThirdNeuterPersonLabel->setVisible(true);
        pluralThirdNeuterPersonLineEdit->setVisible(true);
    } else {
        bool neuterExists = m_doc->identifier(m_currentTranslation).personalPronouns().neuterExists();
        singularThirdNeuterPersonLabel->setVisible(neuterExists);
        singularThirdNeuterPersonLineEdit->setVisible(neuterExists);
        pluralThirdNeuterPersonLabel->setVisible(neuterExists);
        pluralThirdNeuterPersonLineEdit->setVisible(neuterExists);
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

    KEduVocConjugation::ConjugationNumber num = KEduVocConjugation::Singular;

    m_conjugations[selection].setConjugation(singularFirstPersonLineEdit->text(),  KEduVocConjugation::First , num);
    m_conjugations[selection].setConjugation(singularSecondPersonLineEdit->text(),  KEduVocConjugation::Second , num);
    m_conjugations[selection].setConjugation(singularThirdMalePersonLineEdit->text(),  KEduVocConjugation::ThirdMale , num);
    m_conjugations[selection].setConjugation(singularThirdFemalePersonLineEdit->text(),  KEduVocConjugation::ThirdFemale , num);
    m_conjugations[selection].setConjugation(singularThirdNeuterPersonLineEdit->text(),  KEduVocConjugation::ThirdNeuterCommon , num);

    num = KEduVocConjugation::Dual;
    m_conjugations[selection].setConjugation(dualFirstPersonLineEdit->text(),  KEduVocConjugation::First , num);
    m_conjugations[selection].setConjugation(dualSecondPersonLineEdit->text(),  KEduVocConjugation::Second , num);
    m_conjugations[selection].setConjugation(dualThirdMalePersonLineEdit->text(),  KEduVocConjugation::ThirdMale , num);
    m_conjugations[selection].setConjugation(dualThirdFemalePersonLineEdit->text(),  KEduVocConjugation::ThirdFemale , num);
    m_conjugations[selection].setConjugation(dualThirdNeuterPersonLineEdit->text(),  KEduVocConjugation::ThirdNeuterCommon , num);

    num = KEduVocConjugation::Plural;
    m_conjugations[selection].setConjugation(pluralFirstPersonLineEdit->text(),  KEduVocConjugation::First , num);
    m_conjugations[selection].setConjugation(pluralSecondPersonLineEdit->text(),  KEduVocConjugation::Second , num);
    m_conjugations[selection].setConjugation(pluralThirdMalePersonLineEdit->text(),  KEduVocConjugation::ThirdMale , num);
    m_conjugations[selection].setConjugation(pluralThirdFemalePersonLineEdit->text(),  KEduVocConjugation::ThirdFemale , num);
    m_conjugations[selection].setConjugation(pluralThirdNeuterPersonLineEdit->text(),  KEduVocConjugation::ThirdNeuterCommon , num);
}

void TenseEntryPage::updateEntries()
{
    QString selection = m_lastSelection;

    KEduVocConjugation::ConjugationNumber num = KEduVocConjugation::Singular;
    singularFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::First , num));
    singularSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::Second , num));
    singularThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdMale , num));
    singularThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdFemale , num));
    singularThirdNeuterPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdNeuterCommon , num));

    num = KEduVocConjugation::Dual;
    dualFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::First , num));
    dualSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::Second , num));
    dualThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdMale , num));
    dualThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdFemale , num));
    dualThirdNeuterPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdNeuterCommon , num));

    num = KEduVocConjugation::Plural;
    pluralFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::First , num));
    pluralSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::Second , num));
    pluralThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdMale , num));
    pluralThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdFemale , num));
    pluralThirdNeuterPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdNeuterCommon , num));
}


#include "TenseEntryPage.moc"
