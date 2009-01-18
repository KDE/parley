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

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <keduvocdeclension.h>

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

    m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Nominative] = singular_1;

    m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Genitive] = singular_2;

    m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Dative] = singular_3;

    m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Accusative] = singular_4;

    m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Ablative] = singular_5;

    m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Locative] = singular_6;

    m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Vocative] = singular_7;


    m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Nominative] = dual_1;

    m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Genitive] = dual_2;

    m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Dative] = dual_3;

    m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Accusative] = dual_4;

    m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Ablative] = dual_5;

    m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Locative] = dual_6;

    m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Vocative] = dual_7;


    m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Nominative] = plural_1;

    m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Genitive] = plural_2;

    m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Dative] = plural_3;

    m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Accusative] = plural_4;

    m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Ablative] = plural_5;

    m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Locative] = plural_6;

    m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Vocative] = plural_7;

    foreach(int index, m_DeclensionLineEdits.keys()) {
        connect(m_DeclensionLineEdits.value(index), SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    }
}


void DeclensionWidget::textChanged(const QString& text)
{
    int valueIndex = m_DeclensionLineEdits.values().indexOf(qobject_cast<KLineEdit*>(sender()));
    int key = m_DeclensionLineEdits.keys().value(valueIndex);
    m_entry->translation(m_identifier)->declension()->setDeclension(text, (KEduVocWordFlag::Flags)key);
    emit sigModified();
}


void DeclensionWidget::updateEntries()
{
    foreach(int key, m_DeclensionLineEdits.keys()) {
        m_DeclensionLineEdits.value(key)->setText(m_entry->translation(m_identifier)->declension()->declension((KEduVocWordFlag::Flags)key).text());
    }
}

void DeclensionWidget::setTranslation(KEduVocExpression * entry, int identifier)
{
    if (!m_doc) {
        return;
    }

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
        KEduVocWordFlags wordType = entry->translation(m_identifier)->wordType()->wordType();
        if (wordType & KEduVocWordFlag::Noun) {

            // we create declensions on demand. if empty it will simply not be saved.
            // very little memory overhead, comfy to use ;)
            if (!entry->translation(m_identifier)->declension()) {
                entry->translation(m_identifier)->setDeclension(new KEduVocDeclension);
            }

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

    ///@todo allow to choose the type of noun

    // find a noun container
    KEduVocWordType* container = m_doc->wordTypeContainer()->childOfType(KEduVocWordFlag::Noun);
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
    declensionGroupBox->setVisible(true);
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

    singularFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First, KEduVocWordFlag::Singular ));
    singularSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second, KEduVocWordFlag::Singular ));
    singularThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdMale, KEduVocWordFlag::Singular ));
    singularThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdFemale, KEduVocWordFlag::Singular ));
    singularThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdNeutralCommon, KEduVocWordFlag::Singular ));

    dualFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First, KEduVocWordFlag::Dual ));
    dualSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second, KEduVocWordFlag::Dual ));
    dualThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdMale, KEduVocWordFlag::Dual ));
    dualThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdFemale, KEduVocWordFlag::Dual ));
    dualThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdNeutralCommon, KEduVocWordFlag::Dual ));

    pluralFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First, KEduVocWordFlag::Plural ));
    pluralSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second, KEduVocWordFlag::Plural ));
    pluralThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdMale, KEduVocWordFlag::Plural ));
    pluralThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdFemale, KEduVocWordFlag::Plural ));
    pluralThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::ThirdNeutralCommon, KEduVocWordFlag::Plural ));
    */
}


#include "declensionwidget.moc"
