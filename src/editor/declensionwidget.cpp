/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2011 Jan Gerrit Marker <jangerrit@weiler-marker.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "declensionwidget.h"

#include <KEduVocDocument>
#include <KEduVocExpression>
#include <KEduVocWordtype>

#include <KMessageBox>
#include <KLocalizedString>

using namespace Editor;

DeclensionWidget::DeclensionWidget(QWidget *parent) : QWidget(parent)
{
    m_doc = 0;
    m_entry = 0;
    m_identifier = -1;

    setupUi(this);

    numberSelection->setItemData(0, KEduVocWordFlag::Singular);
    numberSelection->setItemData(1, KEduVocWordFlag::Dual);
    numberSelection->setItemData(2, KEduVocWordFlag::Plural);
    connect(numberSelection, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DeclensionWidget::updateEntries);

    connect(nextButton, &QPushButton::clicked, this, &DeclensionWidget::nextNumber);

    setupLineEdits();

    foreach(int index, m_DeclensionLineEdits.keys()) {
        connect(m_DeclensionLineEdits.value(index), &QLineEdit::textChanged, this, &DeclensionWidget::textChanged);
    }
}


void DeclensionWidget::textChanged(const QString& text)
{
    int valueIndex = m_DeclensionLineEdits.values().indexOf(qobject_cast<QLineEdit*>(sender()));
    int key = m_DeclensionLineEdits.keys().value(valueIndex) | currentAdditionalWordFlag();
    m_entry->translation(m_identifier)->declension()->setDeclension(text, (KEduVocWordFlag::Flags)key);
    emit sigModified();
}


void DeclensionWidget::updateEntries()
{
    foreach(int key, m_DeclensionLineEdits.keys()) {
        m_DeclensionLineEdits.value(key)->setText(m_entry->translation(m_identifier)->declension()->declension((KEduVocWordFlag::Flags)(key | currentAdditionalWordFlag())).text());
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
    }

    if (!entry) {
        setEnabled(false);
        return;
    }

    setupLineEdits();

    setEnabled(true);
    if (entry->translation(m_identifier)->wordType()) {
        KEduVocWordFlags wordType = entry->translation(m_identifier)->wordType()->wordType();
        if (wordType & KEduVocWordFlag::Noun || wordType & KEduVocWordFlag::Adjective) {

            // we create declensions on demand. if empty it will simply not be saved.
            // very little memory overhead, comfy to use ;)
            if (!entry->translation(m_identifier)->declension()) {
                entry->translation(m_identifier)->setDeclension(new KEduVocDeclension);
            }

            updateEntries();
        }
    }
}

void DeclensionWidget::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
}

int DeclensionWidget::currentAdditionalWordFlag()
{
    if (m_entry->translation(m_identifier)->wordType()->wordType() & KEduVocWordFlag::Noun) {
        ///@todo easier and better way to get gender?
        if (m_entry->translation(m_identifier)->wordType()->wordType() & KEduVocWordFlag::Feminine)
            return KEduVocWordFlag::Feminine;
        if (m_entry->translation(m_identifier)->wordType()->wordType() & KEduVocWordFlag::Masculine)
            return KEduVocWordFlag::Masculine;
        if (m_entry->translation(m_identifier)->wordType()->wordType() & KEduVocWordFlag::Neuter)
            return KEduVocWordFlag::Neuter;
    }

    return numberSelection->itemData(numberSelection->currentIndex()).toInt();
}

void DeclensionWidget::nextNumber()
{
    int newIndex = numberSelection->currentIndex() + 1;
    if (newIndex >= numberSelection->count())
        newIndex = 0;

    numberSelection->setCurrentIndex(newIndex);
}

void DeclensionWidget::setupLineEdits()
{
    m_DeclensionLineEdits.clear();

    if (m_entry == 0 || !m_entry->translation(m_identifier) || !m_entry->translation(m_identifier)->wordType()
            || m_entry->translation(m_identifier)->wordType()->wordType() & KEduVocWordFlag::Noun) {
        label_3->setText(i18n("Singular"));
        label_4->setText(i18n("Dual"));
        label_5->setText(i18n("Plural"));

        /* Hide the GUI elements which are needed to switch the number */
        for (int i = 0; i < numberLayout->count(); ++i) {
            numberLayout->itemAt(i)->widget()->hide();
        }

        m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Nominative] = masculine_1;
        m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Genitive] = masculine_2;
        m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Dative] = masculine_3;
        m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Accusative] = masculine_4;
        m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Ablative] = masculine_5;
        m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Locative] = masculine_6;
        m_DeclensionLineEdits[KEduVocWordFlag::Singular | KEduVocWordFlag::Vocative] = masculine_7;

        m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Nominative] = feminine_1;
        m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Genitive] = feminine_2;
        m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Dative] = feminine_3;
        m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Accusative] = feminine_4;
        m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Ablative] = feminine_5;
        m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Locative] = feminine_6;
        m_DeclensionLineEdits[KEduVocWordFlag::Dual | KEduVocWordFlag::Vocative] = feminine_7;

        m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Nominative] = neuter_1;
        m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Genitive] = neuter_2;
        m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Dative] = neuter_3;
        m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Accusative] = neuter_4;
        m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Ablative] = neuter_5;
        m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Locative] = neuter_6;
        m_DeclensionLineEdits[KEduVocWordFlag::Plural | KEduVocWordFlag::Vocative] = neuter_7;
    } else {
        label_3->setText(i18n("Masculine"));
        label_4->setText(i18n("Feminine"));
        label_5->setText(i18n("Neuter"));

        /* Show the GUI elements which are needed to switch the number */
        for (int i = 0; i < numberLayout->count(); ++i) {
            numberLayout->itemAt(i)->widget()->show();
        }

        m_DeclensionLineEdits[KEduVocWordFlag::Masculine | KEduVocWordFlag::Nominative] = masculine_1;
        m_DeclensionLineEdits[KEduVocWordFlag::Masculine | KEduVocWordFlag::Genitive] = masculine_2;
        m_DeclensionLineEdits[KEduVocWordFlag::Masculine | KEduVocWordFlag::Dative] = masculine_3;
        m_DeclensionLineEdits[KEduVocWordFlag::Masculine | KEduVocWordFlag::Accusative] = masculine_4;
        m_DeclensionLineEdits[KEduVocWordFlag::Masculine | KEduVocWordFlag::Ablative] = masculine_5;
        m_DeclensionLineEdits[KEduVocWordFlag::Masculine | KEduVocWordFlag::Locative] = masculine_6;
        m_DeclensionLineEdits[KEduVocWordFlag::Masculine | KEduVocWordFlag::Vocative] = masculine_7;

        m_DeclensionLineEdits[KEduVocWordFlag::Feminine | KEduVocWordFlag::Nominative] = feminine_1;
        m_DeclensionLineEdits[KEduVocWordFlag::Feminine | KEduVocWordFlag::Genitive] = feminine_2;
        m_DeclensionLineEdits[KEduVocWordFlag::Feminine | KEduVocWordFlag::Dative] = feminine_3;
        m_DeclensionLineEdits[KEduVocWordFlag::Feminine | KEduVocWordFlag::Accusative] = feminine_4;
        m_DeclensionLineEdits[KEduVocWordFlag::Feminine | KEduVocWordFlag::Ablative] = feminine_5;
        m_DeclensionLineEdits[KEduVocWordFlag::Feminine | KEduVocWordFlag::Locative] = feminine_6;
        m_DeclensionLineEdits[KEduVocWordFlag::Feminine | KEduVocWordFlag::Vocative] = feminine_7;

        m_DeclensionLineEdits[KEduVocWordFlag::Neuter | KEduVocWordFlag::Nominative] = neuter_1;
        m_DeclensionLineEdits[KEduVocWordFlag::Neuter | KEduVocWordFlag::Genitive] = neuter_2;
        m_DeclensionLineEdits[KEduVocWordFlag::Neuter | KEduVocWordFlag::Dative] = neuter_3;
        m_DeclensionLineEdits[KEduVocWordFlag::Neuter | KEduVocWordFlag::Accusative] = neuter_4;
        m_DeclensionLineEdits[KEduVocWordFlag::Neuter | KEduVocWordFlag::Ablative] = neuter_5;
        m_DeclensionLineEdits[KEduVocWordFlag::Neuter | KEduVocWordFlag::Locative] = neuter_6;
        m_DeclensionLineEdits[KEduVocWordFlag::Neuter | KEduVocWordFlag::Vocative] = neuter_7;
    }
}
