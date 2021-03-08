/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "conjugationwidget.h"

#include <KEduVocDocument>
#include <KEduVocExpression>
#include <KEduVocWordtype>
#include <KMessageBox>

using namespace Editor;

ConjugationWidget::ConjugationWidget(QWidget *parent) : QWidget(parent)
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;

    setupUi(this);

    connect(nextTenseButton, &QPushButton::clicked, this, &ConjugationWidget::slotNextTense);
    connect(tenseComboBox, static_cast<void (KComboBox::*)(int)>(&KComboBox::activated), this, &ConjugationWidget::slotTenseSelected);
    connect(tenseComboBox->lineEdit(), SIGNAL(editingFinished()), SLOT(tenseEditingFinished()));

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

    foreach(const KEduVocWordFlags & index, m_conjugationLineEdits.keys()) {
        connect(m_conjugationLineEdits.value(index), &QLineEdit::textChanged, this, &ConjugationWidget::textChanged);
    }
}


void ConjugationWidget::textChanged(const QString& text)
{
    int valueIndex = m_conjugationLineEdits.values().indexOf(qobject_cast<QLineEdit*>(sender()));
    int key = m_conjugationLineEdits.keys().value(valueIndex);
    KEduVocTranslation *translation = m_entry->translation(m_identifier);
    KEduVocConjugation conjugation = translation->getConjugation(tenseComboBox->currentText());
    conjugation.setConjugation(text, (KEduVocWordFlag::Flags)key);
    translation->setConjugation(tenseComboBox->currentText(), conjugation);
}


void ConjugationWidget::slotTenseSelected(int sel)
{
    Q_UNUSED(sel);
    updateEntries();
}


void ConjugationWidget::updateEntries()
{
    m_lastTenseSelection = tenseComboBox->currentText();
    KEduVocConjugation conjugation = m_entry->translation(m_identifier)->getConjugation(m_lastTenseSelection);
    foreach(KEduVocWordFlags flags, m_conjugationLineEdits.keys()) {
        QString text;
        if (conjugation.keys().contains(flags)) {
            text = conjugation.conjugation(flags).text();
        }

        m_conjugationLineEdits[flags]->setText(text);
    }
}

void ConjugationWidget::setTranslation(KEduVocExpression * entry, int identifier)
{
    tenseComboBox->clear();
    tenseComboBox->completionObject()->clear();

    m_entry = entry;
    m_identifier = identifier;

    if (!m_doc || !entry) {
        setEnabled(false);
        return;
    }

    // init tenses per language
    tenseComboBox->addItems(m_doc->identifier(identifier).tenseList());
    tenseComboBox->completionObject()->insertItems(m_doc->identifier(identifier).tenseList());
    if (tenseComboBox->contains(m_lastTenseSelection)) {
        tenseComboBox->setCurrentItem(m_lastTenseSelection);
    } else {
        tenseComboBox->setCurrentIndex(0);
    }

    setEnabled(true);
    if (entry->translation(m_identifier)->wordType()
            && entry->translation(m_identifier)->wordType()->wordType() & KEduVocWordFlag::Verb) {
        updateVisiblePersons();
        updateEntries();
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

void ConjugationWidget::updateVisiblePersons()
{
    if (m_identifier < 0) {
        showWidgets(false, false, false, false, false, false, false);
        return;
    }

    bool dualVisible = m_doc->identifier(m_identifier).personalPronouns().dualExists();
    bool maleFemaleDifferent = m_doc->identifier(m_identifier).personalPronouns().maleFemaleDifferent();
    bool neutralExists = m_doc->identifier(m_identifier).personalPronouns().neutralExists();

    showWidgets(true, true, dualVisible, true, maleFemaleDifferent, maleFemaleDifferent, neutralExists || (!maleFemaleDifferent));

    // set up the personal pronouns
    KEduVocPersonalPronoun pron = m_doc->identifier(m_identifier).personalPronouns();

    singularFirstPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::First | KEduVocWordFlag::Singular));
    singularSecondPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Second | KEduVocWordFlag::Singular));
    singularThirdMalePersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Singular));
    singularThirdFemalePersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Singular));
    singularThirdNeutralPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Singular));

    dualFirstPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::First | KEduVocWordFlag::Dual));
    dualSecondPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Second | KEduVocWordFlag::Dual));
    dualThirdMalePersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Dual));
    dualThirdFemalePersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Dual));
    dualThirdNeutralPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Dual));

    pluralFirstPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::First | KEduVocWordFlag::Plural));
    pluralSecondPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Second | KEduVocWordFlag::Plural));
    pluralThirdMalePersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Masculine | KEduVocWordFlag::Plural));
    pluralThirdFemalePersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Feminine | KEduVocWordFlag::Plural));
    pluralThirdNeutralPersonLabel->setText(pron.personalPronoun(KEduVocWordFlag::Third | KEduVocWordFlag::Neuter | KEduVocWordFlag::Plural));
}

void ConjugationWidget::showWidgets(bool tenses, bool singular, bool dual, bool plural, bool maleVisible, bool femaleVisible, bool neuterVisible)
{
    tenselabel->setVisible(tenses);
    tenseComboBox->setVisible(tenses);
    nextTenseButton->setVisible(tenses);

    singularLabel->setVisible(singular);
    singularFirstPersonLabel->setVisible(singular);
    singularFirstPersonLineEdit->setVisible(singular);
    singularSecondPersonLabel->setVisible(singular);
    singularSecondPersonLineEdit->setVisible(singular);

    singularThirdMalePersonLabel->setVisible(singular && maleVisible);
    singularThirdMalePersonLineEdit->setVisible(singular && maleVisible);
    singularThirdFemalePersonLabel->setVisible(singular && femaleVisible);
    singularThirdFemalePersonLineEdit->setVisible(singular && femaleVisible);
    singularThirdNeutralPersonLabel->setVisible(singular && neuterVisible);
    singularThirdNeutralPersonLineEdit->setVisible(singular && neuterVisible);

    dualLabel->setVisible(dual);
    dualFirstPersonLabel->setVisible(dual);
    dualFirstPersonLineEdit->setVisible(dual);
    dualSecondPersonLabel->setVisible(dual);
    dualSecondPersonLineEdit->setVisible(dual);

    dualThirdMalePersonLabel->setVisible(dual && maleVisible);
    dualThirdMalePersonLineEdit->setVisible(dual && maleVisible);
    dualThirdFemalePersonLabel->setVisible(dual && femaleVisible);
    dualThirdFemalePersonLineEdit->setVisible(dual && femaleVisible);
    dualThirdNeutralPersonLabel->setVisible(dual && neuterVisible);
    dualThirdNeutralPersonLineEdit->setVisible(dual && neuterVisible);

    pluralLabel->setVisible(plural);
    pluralFirstPersonLabel->setVisible(plural);
    pluralFirstPersonLineEdit->setVisible(plural);
    pluralSecondPersonLabel->setVisible(plural);
    pluralSecondPersonLineEdit->setVisible(plural);

    pluralThirdMalePersonLabel->setVisible(plural && maleVisible);
    pluralThirdMalePersonLineEdit->setVisible(plural && maleVisible);
    pluralThirdFemalePersonLabel->setVisible(plural && femaleVisible);
    pluralThirdFemalePersonLineEdit->setVisible(plural && femaleVisible);
    pluralThirdNeutralPersonLabel->setVisible(plural && neuterVisible);
    pluralThirdNeutralPersonLineEdit->setVisible(plural && neuterVisible);
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
