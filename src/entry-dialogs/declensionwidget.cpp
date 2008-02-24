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

#include "declensionwidget.h"

#include <keduvocdeclension.h>

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <KDebug>
#include <KMessageBox>

DeclensionWidget::DeclensionWidget(QWidget *parent) : QWidget(parent)
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;

    setupUi(this);

    connect(makeNounButton, SIGNAL(clicked()), SLOT(slotMakeNoun()));

    showMakeNounWidgets();
    makeNounButton->setEnabled(false);

//     m_DeclensionLineEdits[KEduVocDeclension::indexOf(KEduVocDeclension::First, KEduVocDeclension::Singular)]
//         = singularFirstPersonLineEdit;
//     m_DeclensionLineEdits[KEduVocDeclension::indexOf(KEduVocDeclension::Second, KEduVocDeclension::Singular)]
//         = singularSecondPersonLineEdit;

//     foreach(int index, m_DeclensionLineEdits.keys()) {
//         connect(m_DeclensionLineEdits.value(index), SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
//     }
}


void DeclensionWidget::textChanged(const QString& text)
{
//     int valueIndex = m_DeclensionLineEdits.values().indexOf(qobject_cast<QLineEdit*>(sender()));
//     int key = m_DeclensionLineEdits.keys().value(valueIndex);
//     m_entry->translation(m_identifier)->Declension(tenseComboBox->currentText()).setDeclension(text, key);
}


void DeclensionWidget::updateEntries()
{
//     foreach(int key, m_DeclensionLineEdits.keys()) {
//         m_DeclensionLineEdits.value(key)->setText(m_entry->translation(m_identifier)->Declension(tenseComboBox->currentText()).Declension(key));
//     }
}

void DeclensionWidget::setTranslation(KEduVocExpression * entry, int identifier)
{
    m_entry = entry;
    if (m_identifier != identifier) {
        m_identifier = identifier;
        updateVisiblePersons();
    }

    if (!entry) {
        setEnabled(false);
        showMakeNounWidgets();
        makeNounButton->setEnabled(false);
        return;
    }

    setEnabled(true);
    if (entry->translation(m_identifier)->wordType()) {
        KEduVocWordType::EnumWordType wordType = entry->translation(m_identifier)->wordType()->wordType();
        if (wordType == KEduVocWordType::Noun
            || wordType == KEduVocWordType::NounMale) {
        // if it's a noun already, hide the make noun button and start editing it
        showDeclensionEditWidgets();
        updateEntries();
        }
    } else {
        makeNounButton->setEnabled(true);
        showMakeNounWidgets();
        makeNounButton->setText(i18n("\"%1\" is a noun", m_entry->translation(m_identifier)->text()));
// hide the other stuff
    }
}

void DeclensionWidget::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
}

void DeclensionWidget::slotMakeNoun()
{
    if(!m_doc) {
        return;
    }

    // find a noun container
    KEduVocWordType* container = m_doc->wordTypeContainer()->childOfType(KEduVocWordType::Noun);
    if (container) {
        m_entry->translation(m_identifier)->setWordType(container);
        showDeclensionEditWidgets();
    } else {
        ///@todo better message
        KMessageBox::information(this, i18n("Could not determine word type of nouns"));
    }
    setTranslation(m_entry, m_identifier);
}

void DeclensionWidget::showMakeNounWidgets()
{
    makeNounButton->setVisible(true);

    declensionGroupBox->setVisible(false);
}

void DeclensionWidget::showDeclensionEditWidgets()
{
    makeNounButton->setVisible(false);
}

void DeclensionWidget::updateVisiblePersons()
{
    /*
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

    singularFirstPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::First, KEduVocDeclension::Singular ));
    singularSecondPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::Second, KEduVocDeclension::Singular ));
    singularThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdMale, KEduVocDeclension::Singular ));
    singularThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdFemale, KEduVocDeclension::Singular ));
    singularThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdNeutralCommon, KEduVocDeclension::Singular ));

    dualFirstPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::First, KEduVocDeclension::Dual ));
    dualSecondPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::Second, KEduVocDeclension::Dual ));
    dualThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdMale, KEduVocDeclension::Dual ));
    dualThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdFemale, KEduVocDeclension::Dual ));
    dualThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdNeutralCommon, KEduVocDeclension::Dual ));

    pluralFirstPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::First, KEduVocDeclension::Plural ));
    pluralSecondPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::Second, KEduVocDeclension::Plural ));
    pluralThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdMale, KEduVocDeclension::Plural ));
    pluralThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdFemale, KEduVocDeclension::Plural ));
    pluralThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocDeclension::ThirdNeutralCommon, KEduVocDeclension::Plural ));
    */
}


#include "declensionwidget.moc"
