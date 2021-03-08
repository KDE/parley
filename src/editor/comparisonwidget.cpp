/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "comparisonwidget.h"

#include <KEduVocWordtype>
#include <KEduVocTranslation>
#include <KEduVocExpression>
#include <KEduVocDocument>
#include <KMessageBox>
#include <KLocalizedString>

using namespace Editor;

ComparisonWidget::ComparisonWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    m_doc = 0;

    connect(makeAdjectiveButton, &QPushButton::clicked, this, &ComparisonWidget::slotMakeAdjectiveButton);
    connect(makeAdverbButton, &QPushButton::clicked, this, &ComparisonWidget::slotMakeAdverbButton);

    connect(comparativeLineEdit, &QLineEdit::editingFinished, this, &ComparisonWidget::slotComparativeChanged);
    connect(superlativeLineEdit, &QLineEdit::editingFinished, this, &ComparisonWidget::slotSuperlativeChanged);
}

void ComparisonWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    comparativeLineEdit->setText(QString());
    superlativeLineEdit->setText(QString());
    comparativeLineEdit->setEnabled(false);
    superlativeLineEdit->setEnabled(false);

    if (entry) {
        m_translation = entry->translation(translation);
    } else {
        m_translation = 0;
    }

    if (m_translation) {
        absoluteEntryLabel->setText(m_translation->text());
        setEnabled(true);

        if (m_translation->wordType()) {
            if (m_translation->wordType()->wordType() & KEduVocWordFlag::Adjective
                    || m_translation->wordType()->wordType() & KEduVocWordFlag::Adverb) {
                comparativeLineEdit->setEnabled(true);
                superlativeLineEdit->setEnabled(true);

                comparativeLineEdit->setText(m_translation->comparativeForm().text());
                superlativeLineEdit->setText(m_translation->superlativeForm().text());
            }
        }
    } else {
        setEnabled(false);
        absoluteEntryLabel->setText(QString());
    }
}

void ComparisonWidget::slotMakeAdjectiveButton()
{
    if (!m_doc) {
        return;
    }

    // find an adjective container
    KEduVocWordType* container = m_doc->wordTypeContainer()->childOfType(KEduVocWordFlag::Adjective);
    if (container) {
        m_translation->setWordType(container);
        comparativeLineEdit->setEnabled(true);
        superlativeLineEdit->setEnabled(true);
        comparativeLineEdit->setFocus();
    } else {
        ///@todo better message
        KMessageBox::information(this, i18n("Could not determine word type of adjectives"));
    }
}

void ComparisonWidget::slotMakeAdverbButton()
{
    if (!m_doc) {
        return;
    }

    // find an adverb container
    KEduVocWordType* container = m_doc->wordTypeContainer()->childOfType(KEduVocWordFlag::Adverb);
    if (container) {
        m_translation->setWordType(container);
        comparativeLineEdit->setEnabled(true);
        superlativeLineEdit->setEnabled(true);
        comparativeLineEdit->setFocus();
    } else {
        ///@todo better message
        KMessageBox::information(this, i18n("Could not determine word type of adverbs"));
    }
}

void ComparisonWidget::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
}

void ComparisonWidget::slotComparativeChanged()
{
    m_translation->setComparativeForm(comparativeLineEdit->text());
}

void ComparisonWidget::slotSuperlativeChanged()
{
    m_translation->setSuperlativeForm(superlativeLineEdit->text());
}
