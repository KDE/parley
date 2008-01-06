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

#include "conjugationwidget.h"

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <KDebug>

ConjugationWidget::ConjugationWidget(QWidget *parent) : QWidget(parent)
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;

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

/*
void ConjugationWidget::setData(int row, int col)
{
    m_currentRow = row;
    m_currentTranslation = col;

    if ( m_currentTranslation < 0 ) {
        kDebug() << "Invalid identifier for ConjugationWidget" << m_currentTranslation;
        return;
    }

    KEduVocPersonalPronoun pron = m_doc->identifier(m_currentTranslation).personalPronouns();

    singularFirstPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::First, KEduVocConjugation::Singular ));
    singularSecondPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::Second, KEduVocConjugation::Singular ));
    singularThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular ));
    singularThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular ));
    singularThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Singular ));

    dualFirstPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::First, KEduVocConjugation::Dual ));
    dualSecondPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::Second, KEduVocConjugation::Dual ));
    dualThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual ));
    dualThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual ));
    dualThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Dual ));

    pluralFirstPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::First, KEduVocConjugation::Plural ));
    pluralSecondPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::Second, KEduVocConjugation::Plural ));
    pluralThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural ));
    pluralThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural ));
    pluralThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Plural ));

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
*/

void ConjugationWidget::textChanged(const QString& text)
{
    Q_UNUSED(text);
}


void ConjugationWidget::slotTenseSelected(int sel)
{
kDebug() << "selected tense " << sel;
    saveCurrentTense();
    updateEntries();
}


void ConjugationWidget::slotNextConj()
{
/*
    QString tense = tensebox->currentText();
    int newIndex = m_doc->tenseDescriptions().indexOf(tense) +1;
    if ( newIndex >= m_doc->tenseDescriptions().count() ) {
        newIndex = 0;
    }
    QString newTense = m_doc->tenseDescriptions().value(newIndex);
    int tenseboxIndex = tensebox->findText(newTense);
    tensebox->setCurrentIndex(tenseboxIndex);
    slotTenseSelected(tenseboxIndex);
*/
}


void ConjugationWidget::saveCurrentTense()
{
/*
    QString selection = m_lastSelection;

    KEduVocConjugation::ConjugationNumber num = KEduVocConjugation::Singular;

    m_conjugations[selection].setConjugation(singularFirstPersonLineEdit->text(),  KEduVocConjugation::First , num);
    m_conjugations[selection].setConjugation(singularSecondPersonLineEdit->text(),  KEduVocConjugation::Second , num);
    m_conjugations[selection].setConjugation(singularThirdMalePersonLineEdit->text(),  KEduVocConjugation::ThirdMale , num);
    m_conjugations[selection].setConjugation(singularThirdFemalePersonLineEdit->text(),  KEduVocConjugation::ThirdFemale , num);
    m_conjugations[selection].setConjugation(singularThirdNeutralPersonLineEdit->text(),  KEduVocConjugation::ThirdNeutralCommon , num);

    num = KEduVocConjugation::Dual;
    m_conjugations[selection].setConjugation(dualFirstPersonLineEdit->text(),  KEduVocConjugation::First , num);
    m_conjugations[selection].setConjugation(dualSecondPersonLineEdit->text(),  KEduVocConjugation::Second , num);
    m_conjugations[selection].setConjugation(dualThirdMalePersonLineEdit->text(),  KEduVocConjugation::ThirdMale , num);
    m_conjugations[selection].setConjugation(dualThirdFemalePersonLineEdit->text(),  KEduVocConjugation::ThirdFemale , num);
    m_conjugations[selection].setConjugation(dualThirdNeutralPersonLineEdit->text(),  KEduVocConjugation::ThirdNeutralCommon , num);

    num = KEduVocConjugation::Plural;
    m_conjugations[selection].setConjugation(pluralFirstPersonLineEdit->text(),  KEduVocConjugation::First , num);
    m_conjugations[selection].setConjugation(pluralSecondPersonLineEdit->text(),  KEduVocConjugation::Second , num);
    m_conjugations[selection].setConjugation(pluralThirdMalePersonLineEdit->text(),  KEduVocConjugation::ThirdMale , num);
    m_conjugations[selection].setConjugation(pluralThirdFemalePersonLineEdit->text(),  KEduVocConjugation::ThirdFemale , num);
    m_conjugations[selection].setConjugation(pluralThirdNeutralPersonLineEdit->text(),  KEduVocConjugation::ThirdNeutralCommon , num);
*/
}

void ConjugationWidget::updateEntries()
{
/*
    QString selection = m_lastSelection;

    KEduVocConjugation::ConjugationNumber num = KEduVocConjugation::Singular;
    singularFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::First , num));
    singularSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::Second , num));
    singularThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdMale , num));
    singularThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdFemale , num));
    singularThirdNeutralPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdNeutralCommon , num));

    num = KEduVocConjugation::Dual;
    dualFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::First , num));
    dualSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::Second , num));
    dualThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdMale , num));
    dualThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdFemale , num));
    dualThirdNeutralPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdNeutralCommon , num));

    num = KEduVocConjugation::Plural;
    pluralFirstPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::First , num));
    pluralSecondPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::Second , num));
    pluralThirdMalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdMale , num));
    pluralThirdFemalePersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdFemale , num));
    pluralThirdNeutralPersonLineEdit->setText(m_conjugations[selection].conjugation( KEduVocConjugation::ThirdNeutralCommon , num));
*/
}

void ConjugationWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    m_entry = entry;
    m_identifier = translation;
}

void ConjugationWidget::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
}


#include "conjugationwidget.moc"
