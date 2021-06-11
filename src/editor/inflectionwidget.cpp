/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2011 Jan Gerrit Marker <jangerrit@weiler-marker.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include "inflectionwidget.h"

#include <KEduVocDocument>
#include <KEduVocExpression>
#include <KEduVocWordtype>

#include "declensionwidget.h"
#include "conjugationwidget.h"

#include <KLocalizedString>

#include <QDialogButtonBox>
#include <QDialog>

using namespace Editor;

InflectionWidget::InflectionWidget(QWidget* parent): QStackedWidget(parent)
{
    m_wordTypeWidget = new QWidget(this);
    QVBoxLayout *wordTypeLayout = new QVBoxLayout();

    QLabel *label = new QLabel();
    wordTypeLayout->addWidget(label);

    QPushButton *toVerb = new QPushButton(i18n("Verb"));
    toVerb->setObjectName(QStringLiteral("toVerb"));
    connect(toVerb, &QPushButton::pressed, this, &InflectionWidget::setWordType);
    wordTypeLayout->addWidget(toVerb, 1);

    QPushButton *toNoun = new QPushButton(i18n("Noun"));
    toNoun->setObjectName(QStringLiteral("toNoun"));
    connect(toNoun, &QPushButton::pressed, this, &InflectionWidget::setWordType);
    wordTypeLayout->addWidget(toNoun, 1);

    QPushButton *toAdjective = new QPushButton(i18n("Adjective"));
    toAdjective->setObjectName(QStringLiteral("toAdjective"));
    connect(toAdjective, &QPushButton::pressed, this, &InflectionWidget::setWordType);
    wordTypeLayout->addWidget(toAdjective, 1);

    wordTypeLayout->addStretch();

    m_wordTypeWidget->setDisabled(true);
    m_wordTypeWidget->setLayout(wordTypeLayout);

    m_conjugationWidget = new ConjugationWidget(this);
    m_declensionWidget = new DeclensionWidget(this);

    addWidget(m_wordTypeWidget);
    addWidget(m_conjugationWidget);
    addWidget(m_declensionWidget);
}

void InflectionWidget::setDocument(const std::shared_ptr<KEduVocDocument> &doc)
{
    m_doc = doc;
    m_conjugationWidget->setDocument(doc.get());
    m_declensionWidget->setDocument(doc.get());
}

void InflectionWidget::setTranslation(KEduVocExpression* entry, int translation)
{
    m_entry = entry;
    m_translation = translation;

    if (!entry || !entry->translation(translation)) {
        m_wordTypeWidget->setDisabled(true);
        setCurrentWidget(m_wordTypeWidget);
        return;
    }
    if (!entry->translation(translation)->wordType() ||
            !(entry->translation(translation)->wordType()->wordType() & KEduVocWordFlag::Verb
              || entry->translation(translation)->wordType()->wordType() & KEduVocWordFlag::Noun
              || entry->translation(translation)->wordType()->wordType() & KEduVocWordFlag::Adjective
             )
       ) {
        qobject_cast<QLabel*>(m_wordTypeWidget->layout()->itemAt(0)->widget())->setText(i18n("\"%1\" is a:", entry->translation(translation)->text()));
        m_wordTypeWidget->setEnabled(true);
        setCurrentWidget(m_wordTypeWidget);
        return;
    }

    m_conjugationWidget->setTranslation(entry, translation);
    m_declensionWidget->setTranslation(entry, translation);

    if (entry->translation(translation)->wordType()->wordType() & KEduVocWordFlag::Verb) {
        setCurrentWidget(m_conjugationWidget);
    } else {
        setCurrentWidget(m_declensionWidget);
    }
}

void InflectionWidget::setWordType()
{
    if (!sender()) {
        return;
    }

    KEduVocWordType* container = 0;

    if (sender()->objectName() == QLatin1String("toVerb")) {
        container = m_doc->wordTypeContainer()->childOfType(KEduVocWordFlag::Verb);
    } else if (sender()->objectName() == QLatin1String("toNoun")) {
        KEduVocWordFlags type = KEduVocWordFlag::Noun;

        QPointer<QDialogButtonBox> getGenderDialog = new QDialogButtonBox;
        getGenderDialog->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        KComboBox *genderComboBox = new KComboBox;
        genderComboBox->addItem(i18n("Masculine"), KEduVocWordFlag::Masculine);
        genderComboBox->addItem(i18n("Neuter"), KEduVocWordFlag::Neuter);
        genderComboBox->addItem(i18n("Feminine"), KEduVocWordFlag::Feminine);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget( genderComboBox );
        layout->addWidget( getGenderDialog );

        QPointer<QDialog> dialog = new QDialog( this );
        dialog->setLayout( layout );
        dialog->setWindowTitle(i18n("Please select the noun's gender"));

        connect(getGenderDialog.data(), &QDialogButtonBox::accepted, dialog.data(), &QDialog::accept);
        connect(getGenderDialog.data(), &QDialogButtonBox::rejected, dialog.data(), &QDialog::reject);

        if (dialog->exec() == QDialog::Accepted) {
            type = (KEduVocWordFlags)(type | genderComboBox->itemData(genderComboBox->currentIndex()).toInt());
        }

        delete dialog;

        container = m_doc->wordTypeContainer()->childOfType(type);
    } else if (sender()->objectName() == QLatin1String("toAdjective")) {
        container = m_doc->wordTypeContainer()->childOfType(KEduVocWordFlag::Adjective);
    }

    if (container) {
        m_entry->translation(m_translation)->setWordType(container);
        setTranslation(m_entry, m_translation);
    }
}
