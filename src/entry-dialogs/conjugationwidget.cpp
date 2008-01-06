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
#include <keduvocwordtype.h>
#include <KDebug>

ConjugationWidget::ConjugationWidget(QWidget *parent) : QWidget(parent)
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;

    setupUi(this);

    connect(makeVerbButton, SIGNAL(clicked()), SLOT(slotMakeVerb()));
    connect(nextTenseButton, SIGNAL(clicked()), SLOT(slotNextTense()));
    connect(tenseComboBox, SIGNAL(activated(int)), SLOT(slotTenseSelected(int)));

    singularGroupBox->setVisible(false);
    pluralGroupBox->setVisible(false);
    dualGroupBox->setVisible(false);
    makeVerbButton->setEnabled(false);

    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::First, KEduVocConjugation::Singular)]
        = singularFirstPersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::Second, KEduVocConjugation::Singular)]
        = singularSecondPersonLineEdit;

    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdMale, KEduVocConjugation::Singular)]
        = singularThirdMalePersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Singular)]
        = singularThirdFemalePersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Singular)]
        = singularThirdNeutralPersonLineEdit;

    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::First, KEduVocConjugation::Dual)]
        = dualFirstPersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::Second, KEduVocConjugation::Dual)]
        = dualSecondPersonLineEdit;

    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdMale, KEduVocConjugation::Dual)]
        = dualThirdMalePersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Dual)]
        = dualThirdFemalePersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Dual)]
        = dualThirdNeutralPersonLineEdit;

    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::First, KEduVocConjugation::Plural)]
        = pluralFirstPersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::Second, KEduVocConjugation::Plural)]
        = pluralSecondPersonLineEdit;

    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdMale, KEduVocConjugation::Plural)]
        = pluralThirdMalePersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdFemale, KEduVocConjugation::Plural)]
        = pluralThirdFemalePersonLineEdit;
    m_conjugationLineEdits[KEduVocConjugation::indexOf(KEduVocConjugation::ThirdNeutralCommon, KEduVocConjugation::Plural)]
        = pluralThirdNeutralPersonLineEdit;

    foreach(int index, m_conjugationLineEdits.keys()) {
        connect(m_conjugationLineEdits.value(index), SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    }
}


void ConjugationWidget::textChanged(const QString& text)
{
    Q_UNUSED(text);
kDebug() << "text for " << m_conjugationLineEdits.values().indexOf(qobject_cast<QLineEdit*>(sender())) << " changed";
}


void ConjugationWidget::slotTenseSelected(int sel)
{
kDebug() << "selected tense " << sel;
    saveCurrentTense();
    updateEntries();
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
    foreach(int key, m_entry->translation(m_identifier)->conjugation(tenseComboBox->currentText()).keys()) {
        m_conjugationLineEdits.value(key)->setText(m_entry->translation(m_identifier)->conjugation(tenseComboBox->currentText()).conjugation(key));
    }
}

void ConjugationWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    m_entry = entry;
    m_identifier = translation;

    if (!entry) {
        setEnabled(false);
        return;
    }
    setEnabled(true);
    if (entry->translation(translation)->wordType()
            && entry->translation(translation)->wordType()->wordType() == KEduVocWordType::Verb) {
        // if it's a verb already, hide the make verb button and start editing it
        makeVerbButton->setVisible(false);
        singularGroupBox->setVisible(true);
        pluralGroupBox->setVisible(true);
        dualGroupBox->setVisible(true);
        updateEntries();

    } else {
        makeVerbButton->setText(i18n("\"%1\" is a verb", m_entry->translation(translation)->text()));
        makeVerbButton->setEnabled(true);
        makeVerbButton->setVisible(true);
// hide the other stuff
    }
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

void ConjugationWidget::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
    tenseComboBox->clear();
    if (m_doc) {
        tenseComboBox->addItems(m_doc->tenseDescriptions());
        tenseComboBox->setCurrentIndex(0);
    }
}

void ConjugationWidget::slotNextTense()
{
    if (tenseComboBox->currentIndex() + 1 < tenseComboBox->count()) {
        tenseComboBox->setCurrentIndex(tenseComboBox->currentIndex() + 1);
    } else {
        tenseComboBox->setCurrentIndex(0);
    }
    updateEntries();
}

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

void ConjugationWidget::slotMakeVerb()
{
}


#include "conjugationwidget.moc"
