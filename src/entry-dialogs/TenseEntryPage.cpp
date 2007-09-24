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



    ///@todo enable connect
//     connect(b_next, SIGNAL(clicked()), SLOT(slotNextConj()));
//     connect(tensebox, SIGNAL(activated(int)), SLOT(slotTenseSelected(int)));
//
//     connect(thirdN_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdNPluralChanged(const QString&)));
//     connect(thirdN_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdNSingularChanged(const QString&)));
//     connect(thirdM_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdMPluralChanged(const QString&)));
//     connect(thirdM_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdMSingularChanged(const QString&)));
//     connect(thirdF_plural, SIGNAL(textChanged(const QString&)), SLOT(thirdFPluralChanged(const QString&)));
//     connect(thirdF_singular, SIGNAL(textChanged(const QString&)), SLOT(thirdFSingularChanged(const QString&)));
//     connect(second_plural, SIGNAL(textChanged(const QString&)), SLOT(secondPluralChanged(const QString&)));
//     connect(second_singular, SIGNAL(textChanged(const QString&)), SLOT(secondSingularChanged(const QString&)));
//     connect(first_plural, SIGNAL(textChanged(const QString&)), SLOT(firstPluralChanged(const QString&)));
//     connect(first_singular, SIGNAL(textChanged(const QString&)), SLOT(firstSingularChanged(const QString&)));

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

    ///@todo hide dual conjugations for now
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
    slotTenseSelected(0);

    setModified(false);
}


void TenseEntryPage::textChanged(const QString& s)
{
    setModified(true);
}


void TenseEntryPage::slotTenseSelected(int sel)
{
    ///@todo save entries in m_conjugations!


    QString selection = m_doc->tenseDescriptions().value(sel);

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
