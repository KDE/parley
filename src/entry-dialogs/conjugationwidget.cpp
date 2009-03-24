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
    connect(tenseComboBox->lineEdit(), SIGNAL(editingFinished()), SLOT(tenseEditingFinished()));

    showMakeVerbWidgets();
    makeVerbButton->setEnabled(false);

    m_conjugationLineEdits[KEduVocWordFlag::First | KEduVocWordFlag::Singular]
        = singularFirstPersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Second | KEduVocWordFlag::Singular]
        = singularSecondPersonLineEdit;

    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Singular]
        = singularThirdMalePersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Singular]
        = singularThirdFemalePersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Singular]
        = singularThirdNeutralPersonLineEdit;

    m_conjugationLineEdits[KEduVocWordFlag::First | KEduVocWordFlag::Dual]
        = dualFirstPersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Second | KEduVocWordFlag::Dual]
        = dualSecondPersonLineEdit;

    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Dual]
        = dualThirdMalePersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Dual]
        = dualThirdFemalePersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Dual]
        = dualThirdNeutralPersonLineEdit;

    m_conjugationLineEdits[KEduVocWordFlag::First | KEduVocWordFlag::Plural]
        = pluralFirstPersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Second | KEduVocWordFlag::Plural]
        = pluralSecondPersonLineEdit;

    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Plural]
        = pluralThirdMalePersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Plural]
        = pluralThirdFemalePersonLineEdit;
    m_conjugationLineEdits[KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Plural]
        = pluralThirdNeutralPersonLineEdit;

    foreach(const KEduVocWordFlags &index, m_conjugationLineEdits.keys()) {
        connect(m_conjugationLineEdits.value(index), SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
    }
}


void ConjugationWidget::textChanged(const QString& text)
{
    int valueIndex = m_conjugationLineEdits.values().indexOf(qobject_cast<KLineEdit*>(sender()));
    int key = m_conjugationLineEdits.keys().value(valueIndex);
    m_entry->translation(m_identifier)->conjugation(tenseComboBox->currentText()).setConjugation(text, (KEduVocWordFlag::Flags)key);
}


void ConjugationWidget::slotTenseSelected(int sel)
{
    Q_UNUSED(sel);
    updateEntries();
}


void ConjugationWidget::updateEntries()
{
    foreach(int key, m_conjugationLineEdits.keys()) {
        m_conjugationLineEdits.value((KEduVocWordFlag::Flags)key)->setText(m_entry->translation(m_identifier)->conjugation(tenseComboBox->currentText()).conjugation((KEduVocWordFlag::Flags)key).text());
    }
}

void ConjugationWidget::setTranslation(KEduVocExpression * entry, int identifier)
{
    tenseComboBox->clear();
    tenseComboBox->completionObject()->clear();

    m_entry = entry;
//     if (m_identifier != identifier) {
        m_identifier = identifier;
//         updateVisiblePersons();
//     }

    if (!m_doc || !entry) {
        setEnabled(false);
        showMakeVerbWidgets();
        makeVerbButton->setEnabled(false);
        return;
    }

    // init tenses per language
    tenseComboBox->addItems(m_doc->identifier(identifier).tenseList());
    tenseComboBox->completionObject()->insertItems(m_doc->identifier(identifier).tenseList());
    tenseComboBox->setCurrentIndex(0);

    setEnabled(true);
    if (entry->translation(m_identifier)->wordType()
            && entry->translation(m_identifier)->wordType()->wordType() & KEduVocWordFlag::Verb) {
        // if it's a verb already, hide the make verb button and start editing it
        showConjugationEditWidgets();
        updateEntries();
        updateVisiblePersons();
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
    tenseComboBox->completionObject()->clear();
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
    KEduVocWordType* container = m_doc->wordTypeContainer()->childOfType(KEduVocWordFlag::Verb);
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

    singularFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First | KEduVocWordFlag::Singular ));
    singularSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second | KEduVocWordFlag::Singular ));
    singularThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine| KEduVocWordFlag::Singular ));
    singularThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine| KEduVocWordFlag::Singular ));
    singularThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter| KEduVocWordFlag::Singular ));

    dualFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First| KEduVocWordFlag::Dual ));
    dualSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second| KEduVocWordFlag::Dual ));
    dualThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine| KEduVocWordFlag::Dual ));
    dualThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine| KEduVocWordFlag::Dual ));
    dualThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter| KEduVocWordFlag::Dual ));

    pluralFirstPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::First| KEduVocWordFlag::Plural ));
    pluralSecondPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Second| KEduVocWordFlag::Plural ));
    pluralThirdMalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Masculine| KEduVocWordFlag::Plural ));
    pluralThirdFemalePersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Feminine| KEduVocWordFlag::Plural ));
    pluralThirdNeutralPersonLabel->setText(pron.personalPronoun( KEduVocWordFlag::Third | KEduVocWordFlag::Neuter| KEduVocWordFlag::Plural ));
}

void ConjugationWidget::tenseEditingFinished()
{
    const QStringList& oldTenses = m_doc->identifier(m_identifier).tenseList();
    if (!oldTenses.contains(tenseComboBox->currentText())) {
        // add a new tense
        m_doc->identifier(m_identifier).setTense(oldTenses.count(), tenseComboBox->currentText());
        // put it into the completion
        tenseComboBox->completionObject()->addItem(tenseComboBox->currentText());
    }
}


#include "conjugationwidget.moc"
