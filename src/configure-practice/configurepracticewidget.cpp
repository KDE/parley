/***************************************************************************

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "configurepracticewidget.h"

#include "ui_comparisonoptionswidget.h"
#include "ui_conjugationoptionswidget.h"

#include "languagesettings.h"
#include "documentsettings.h"
#include "prefs.h"

#include <keduvocdocument.h>

#include <KStandardDirs>
#include <KLocale>
#include <QStackedLayout>
#include <QLabel>
#include <QtDBus>

/**
 *
 * @param parent
 */
ConfigurePracticeWidget::ConfigurePracticeWidget(KEduVocDocument* doc, QWidget * parent)
    : QWidget(parent)
{
    m_doc = doc;
    setupUi(this);

    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        LanguageSettings currentSettings(m_doc->identifier(i).locale());
        currentSettings.readConfig();
        QString icon = currentSettings.icon();
        LanguageFromList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );
    }

    connect(LanguageFromList, SIGNAL(currentRowChanged(int)), this, SLOT(fromLanguageSelected(int)));

    connect(ComparisonRadio, SIGNAL(toggled(bool)), this, SLOT(comparisonRadioToggled(bool)));
    connect(ConjugationRadio, SIGNAL(toggled(bool)), this, SLOT(conjugationRadioToggled(bool)));

    connect(AntonymRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));
    connect(ArticleRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));
    connect(ExampleRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));
    connect(MixedLettersRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));
    connect(MultipleChoiceRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));
    connect(ParaphraseRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));
    connect(SynonymRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));
    connect(WrittenRadio, SIGNAL(toggled(bool)), this, SLOT(otherRadioToggled(bool)));

//     connect(WrittenRadio, )

///@todo connect the Radios to disable the option group

    LanguageFromList->setCurrentRow(Prefs::fromIdentifier());

    OptionsGroupBox->setEnabled( false );
    m_optionsStackedLayout = new QStackedLayout(OptionsGroupBox);
    OptionsGroupBox->setLayout(m_optionsStackedLayout);


    // the widgets have to be inserted in order of the enum because insert appends if the index is too great
    m_optionsStackedLayout->insertWidget(Empty, new QLabel(i18n("No options"), OptionsGroupBox));

    // add the conjugation ui to the stacked widget
    QWidget* conjugationContainer = new QWidget(OptionsGroupBox);
    Ui::ConjugationOptionsWidget conjugationUi;
    conjugationUi.setupUi(conjugationContainer);
    m_optionsStackedLayout->insertWidget(Conjugation, conjugationContainer);
    m_tenseListWidget = conjugationUi.tenseSelectionTreeWidget;
    setupTenses();

    // add the comparison ui to the stacked widget
    QWidget* comparisonContainer = new QWidget(OptionsGroupBox);
    Ui::ComparisonOptionsWidget comparisonUi;
    comparisonUi.setupUi(comparisonContainer);
    m_optionsStackedLayout->insertWidget(Comparison, comparisonContainer);
}


void ConfigurePracticeWidget::updateSettings()
{
    Prefs::setFromIdentifier(LanguageFromList->currentRow());
    Prefs::setToIdentifier(LanguageToList->currentItem()->data(Qt::UserRole).toInt());

    QTreeWidgetItem* parentItem = m_tenseListWidget->invisibleRootItem();
    QStringList activeTenses;

    for ( int i = 0; i < parentItem->childCount(); i++ ) {
        QTreeWidgetItem* tenseItem = parentItem->child(i);
        if ( tenseItem->checkState(0) == Qt::Checked ) {
            activeTenses.append(tenseItem->text(0));
        }
    }

    DocumentSettings documentSettings(m_doc->url().url());
    documentSettings.setConjugationTenses(activeTenses);
    documentSettings.writeConfig();
}

void ConfigurePracticeWidget::fromLanguageSelected(int identifierFromIndex)
{
    LanguageToList->clear();
    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        if ( i != identifierFromIndex ) {

            LanguageSettings currentSettings(m_doc->identifier(i).locale());
            currentSettings.readConfig();
            QString icon = currentSettings.icon();
            LanguageToList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );

            LanguageToList->item(LanguageToList->count()-1)->setData(Qt::UserRole, i);
            if ( i == Prefs::toIdentifier() ) {
                LanguageToList->setCurrentRow(i);
            }
        }
    }
    if ( LanguageToList->currentRow() < 0 ) {
        LanguageToList->setCurrentRow(0);
    }
}

void ConfigurePracticeWidget::updateWidgets()
{
    LanguageFromList->setCurrentRow(Prefs::fromIdentifier());
}

bool ConfigurePracticeWidget::hasChanged()
{
    kDebug()<< "ConfigurePracticeWidget::hasChanged()";
    int toRow = LanguageToList->currentRow();
    if ( toRow <= LanguageFromList->currentRow() ) {
        toRow++;
    }
    ///@todo implement so that the current settings are respected
    return false;
        //LanguageFromList->currentRow() == Prefs::fromIdentifier() &&
        //    toRow == Prefs::toIdentifier();
}

bool ConfigurePracticeWidget::isDefault()
{
    ///@todo language selection
    return true;
//         LanguageFromList->currentRow() == 0 &&
//         LanguageToList->currentRow() == 1;
}

void ConfigurePracticeWidget::comparisonRadioToggled(bool checked)
{
    if ( checked ) {
        OptionsGroupBox->setEnabled(true);
        m_optionsStackedLayout->setCurrentIndex(Comparison);
    }
}

void ConfigurePracticeWidget::conjugationRadioToggled(bool checked)
{
    if ( checked ) {
        OptionsGroupBox->setEnabled(true);
        m_optionsStackedLayout->setCurrentIndex(Conjugation);
    }
}

void ConfigurePracticeWidget::otherRadioToggled(bool checked)
{
    if ( checked ) {
        OptionsGroupBox->setEnabled(false);
        m_optionsStackedLayout->setCurrentIndex(Empty);
    }
}


void ConfigurePracticeWidget::setupTenses()
{
    DocumentSettings currentSettings(m_doc->url().url());
    currentSettings.readConfig();
    QStringList activeTenses = currentSettings.conjugationTenses();
    QTreeWidgetItem* tenseItem;

    foreach ( QString tenseName, m_doc->tenseDescriptions() ) {
        tenseItem = new QTreeWidgetItem(m_tenseListWidget);
        tenseItem->setText(0, tenseName);
        if ( activeTenses.contains( tenseName ) ) {
            tenseItem->setCheckState(0, Qt::Checked);
        } else {
            tenseItem->setCheckState(0, Qt::Unchecked);
        }
        tenseItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        m_tenseListWidget->addTopLevelItem( tenseItem );
    }
}

#include "configurepracticewidget.moc"
