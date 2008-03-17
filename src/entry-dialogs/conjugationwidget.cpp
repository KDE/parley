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
#include <KMessageBox>

ConjugationWidget::ConjugationWidget(QWidget *parent) : QWidget(parent)
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;

    setupUi(this);

    connect(makeVerbButton, SIGNAL(clicked()), SLOT(slotMakeVerb()));
    connect(nextTenseButton, SIGNAL(clicked()), SLOT(slotNextTense()));
    connect(tenseComboBox, SIGNAL(activated(int)), SLOT(slotTenseSelected(int)));

    showMakeVerbWidgets();
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
    int valueIndex = m_conjugationLineEdits.values().indexOf(qobject_cast<QLineEdit*>(sender()));
    int key = m_conjugationLineEdits.keys().value(valueIndex);
    m_entry->translation(m_identifier)->conjugation(tenseComboBox->currentText()).setConjugation(text, key);
}


void ConjugationWidget::slotTenseSelected(int sel)
{
    Q_UNUSED(sel);
    updateEntries();
}


void ConjugationWidget::updateEntries()
{
    foreach(int key, m_conjugationLineEdits.keys()) {
        m_conjugationLineEdits.value(key)->setText(m_entry->translation(m_identifier)->conjugation(tenseComboBox->currentText()).conjugation(key).text());
    }
}

void ConjugationWidget::setTranslation(KEduVocExpression * entry, int identifier)
{
    m_entry = entry;
    if (m_identifier != identifier) {
        m_identifier = identifier;
        updateVisiblePersons();
    }

    if (!entry) {
        setEnabled(false);
        showMakeVerbWidgets();
        makeVerbButton->setEnabled(false);
        return;
    }

    setEnabled(true);
    if (entry->translation(m_identifier)->wordType()
            && entry->translation(m_identifier)->wordType()->wordType() == KEduVocWordType::Verb) {
        // if it's a verb already, hide the make verb button and start editing it
        showConjugationEditWidgets();
        updateEntries();
    } else {
        makeVerbButton->setEnabled(true);
        showMakeVerbWidgets();
        makeVerbButton->setText(i18n("\"%1\" is a verb", m_entry->translation(m_identifier)->text()));
// hide the other stuff
    }
}

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

void ConjugationWidget::slotMakeVerb()
{
    if(!m_doc) {
        return;
    }

    // find an adverb container
    KEduVocWordType* container = m_doc->wordTypeContainer()->childOfType(KEduVocWordType::Verb);
    if (container) {
        m_entry->translation(m_identifier)->setWordType(container);
        showConjugationEditWidgets();
    } else {
        ///@todo better message
        KMessageBox::information(this, i18n("Could not determine word type of verbs"));
    }
    setTranslation(m_entry, m_identifier);
}

void ConjugationWidget::showMakeVerbWidgets()
{
    makeVerbButton->setVisible(true);
// tense selection stuff
    singularGroupBox->setVisible(false);
    pluralGroupBox->setVisible(false);
    dualGroupBox->setVisible(false);
}

void ConjugationWidget::showConjugationEditWidgets()
{
    makeVerbButton->setVisible(false);
}

void ConjugationWidget::updateVisiblePersons()
{
    if (m_identifier < 0) {
        singularGroupBox->setVisible(false);
        pluralGroupBox->setVisible(false);
        dualGroupBox->setVisible(false);
        return;
    }

    singularGroupBox->setVisible(true);
    pluralGroupBox->setVisible(true);
    dualGroupBox->setVisible( m_doc->identifier(m_identifier).personalPronouns().dualExists() );

    bool maleFemaleDifferent = m_doc->identifier(m_identifier).personalPronouns().maleFemaleDifferent();

    singularThirdMalePersonLabel->setVisible(maleFemaleDifferent);
    singularThirdMalePersonLineEdit->setVisible(maleFemaleDifferent);
    singularThirdFemalePersonLabel->setVisible(maleFemaleDifferent);
    singularThirdFemalePersonLineEdit->setVisible(maleFemaleDifferent);

    dualThirdMalePersonLabel->setVisible(maleFemaleDifferent);
    dualThirdMalePersonLineEdit->setVisible(maleFemaleDifferent);
    dualThirdFemalePersonLabel->setVisible(maleFemaleDifferent);
    dualThirdFemalePersonLineEdit->setVisible(maleFemaleDifferent);

    pluralThirdMalePersonLabel->setVisible(maleFemaleDifferent);
    pluralThirdMalePersonLineEdit->setVisible(maleFemaleDifferent);
    pluralThirdFemalePersonLabel->setVisible(maleFemaleDifferent);
    pluralThirdFemalePersonLineEdit->setVisible(maleFemaleDifferent);

    if ( !maleFemaleDifferent ) {
        singularThirdNeutralPersonLabel->setVisible(true);
        singularThirdNeutralPersonLineEdit->setVisible(true);
        dualThirdNeutralPersonLabel->setVisible(true);
        dualThirdNeutralPersonLineEdit->setVisible(true);
        pluralThirdNeutralPersonLabel->setVisible(true);
        pluralThirdNeutralPersonLineEdit->setVisible(true);
    } else {
        bool neutralExists = m_doc->identifier(m_identifier).personalPronouns().neutralExists();
        singularThirdNeutralPersonLabel->setVisible(neutralExists);
        singularThirdNeutralPersonLineEdit->setVisible(neutralExists);
        dualThirdNeutralPersonLabel->setVisible(neutralExists);
        dualThirdNeutralPersonLineEdit->setVisible(neutralExists);
        pluralThirdNeutralPersonLabel->setVisible(neutralExists);
        pluralThirdNeutralPersonLineEdit->setVisible(neutralExists);
    }

    // set up the personal pronouns
    KEduVocPersonalPronoun pron = m_doc->identifier(m_identifier).personalPronouns();

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
}


#include "conjugationwidget.moc"
