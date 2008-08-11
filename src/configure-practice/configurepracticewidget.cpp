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
#include "ui_writtenpracticeoptionswidget.h"
#include "ui_multiplechoiceoptionswidget.h"
#include "ui_flashcardsoptionswidget.h"

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
    m_bilingual = true;

    setupUi(this);

    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        LanguageSettings currentSettings(m_doc->identifier(i).locale());
        currentSettings.readConfig();
        QString icon = currentSettings.icon();
        if (icon.isEmpty()) {
            icon = QString("set-language");
        }
        LanguageFromList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );
    }

    connect(LanguageFromList, SIGNAL(currentRowChanged(int)), SLOT(fromLanguageSelected(int)));
    LanguageFromList->setCurrentRow(Prefs::questionLanguage());

    OptionsGroupBox->setEnabled( false );
    m_optionsStackedLayout = new QStackedLayout(OptionsGroupBox);
    OptionsGroupBox->setLayout(m_optionsStackedLayout);

    // the widgets have to be inserted in order of the enum because insert appends if the index is too great
    m_optionsStackedLayout->insertWidget(Empty, new QLabel(i18n("No options"), OptionsGroupBox));

    // add the written practice ui to the stacked widget
    QWidget* writtenContainer = new QWidget(OptionsGroupBox);
    Ui::WrittenPracticeOptionsWidget writtenPracticeUi;
    writtenPracticeUi.setupUi(writtenContainer);
    m_optionsStackedLayout->insertWidget(WrittenPractice, writtenContainer);

    // add the written practice ui to the stacked widget
    QWidget* multipleChoiceContainer = new QWidget(OptionsGroupBox);
    Ui::MultipleChoiceOptionsWidget multipleChoiceUi;
    multipleChoiceUi.setupUi(multipleChoiceContainer);
    m_optionsStackedLayout->insertWidget(MultipleChoice, multipleChoiceContainer);


    // add the conjugation ui to the stacked widget
    if ( !m_doc->tenseDescriptions().isEmpty() ) {
        QWidget* conjugationContainer = new QWidget(OptionsGroupBox);
        Ui::ConjugationOptionsWidget conjugationUi;
        conjugationUi.setupUi(conjugationContainer);
        m_optionsStackedLayout->insertWidget(Conjugation, conjugationContainer);
        m_tenseListWidget = conjugationUi.tenseSelectionTreeWidget;
        setupTenses();
    } else {
        m_tenseListWidget = 0;
        QLabel* tenseHint = new QLabel(OptionsGroupBox);
        tenseHint->setText(i18n("To practice conjugations set up tenses in the \"Edit\" -> \"Grammar\" options and add the conjugation forms to your vocabulary."));
        tenseHint->setWordWrap(true);
        m_optionsStackedLayout->insertWidget(Conjugation, tenseHint);
    }


    // add the comparison ui to the stacked widget
    QWidget* comparisonContainer = new QWidget(OptionsGroupBox);
    Ui::ComparisonOptionsWidget comparisonUi;
    comparisonUi.setupUi(comparisonContainer);
    m_optionsStackedLayout->insertWidget(Comparison, comparisonContainer);

    // add the flashcard ui
    QWidget* flashcardsContainer = new QWidget(OptionsGroupBox);
    Ui::FlashCardsOptionsWidget flashcardsUi;
    flashcardsUi.setupUi(flashcardsContainer);
    m_optionsStackedLayout->insertWidget(FlashCards, flashcardsContainer);

    // this is preset in the ui. thus the button is not toggled when setting up
    if ( WrittenRadio->isChecked() ) {
        writtenRadioToggled(true);
    }

    connect(AntonymRadio, SIGNAL(toggled(bool)), SLOT(otherRadioToggled(bool)));
    connect(ArticleRadio, SIGNAL(toggled(bool)), SLOT(otherRadioToggled(bool)));
    connect(ExampleRadio, SIGNAL(toggled(bool)), SLOT(otherRadioToggled(bool)));
    connect(MixedLettersRadio, SIGNAL(toggled(bool)), SLOT(otherRadioToggled(bool)));
    connect(MultipleChoiceRadio, SIGNAL(toggled(bool)), SLOT(otherRadioToggled(bool)));
    connect(ParaphraseRadio, SIGNAL(toggled(bool)), SLOT(otherRadioToggled(bool)));
    connect(SynonymRadio, SIGNAL(toggled(bool)), SLOT(otherRadioToggled(bool)));

    connect(FlashCardsRadio, SIGNAL(toggled(bool)), SLOT(flashCardsRadioToggled(bool)));
    connect(WrittenRadio, SIGNAL(toggled(bool)), SLOT(writtenRadioToggled(bool)));
    connect(ComparisonRadio, SIGNAL(toggled(bool)), SLOT(comparisonRadioToggled(bool)));
    connect(ConjugationRadio, SIGNAL(toggled(bool)), SLOT(conjugationRadioToggled(bool)));
    connect(MultipleChoiceRadio, SIGNAL(toggled(bool)), SLOT(multipleChoiceRadioToggled(bool)));


    // mono/bilingual - practice effects one/two languages - change the language selection accordingly
    connect(WrittenRadio, SIGNAL(toggled(bool)), SLOT(bilingualPracticeSelected(bool)));
    connect(MixedLettersRadio, SIGNAL(toggled(bool)), SLOT(bilingualPracticeSelected(bool)));
    connect(MultipleChoiceRadio, SIGNAL(toggled(bool)), SLOT(bilingualPracticeSelected(bool)));
    connect(FlashCardsRadio, SIGNAL(toggled(bool)), SLOT(bilingualPracticeSelected(bool)));


    connect(ArticleRadio, SIGNAL(toggled(bool)), SLOT(monolingualPracticeSelected(bool)));
    connect(ExampleRadio, SIGNAL(toggled(bool)), SLOT(monolingualPracticeSelected(bool)));
    connect(ParaphraseRadio, SIGNAL(toggled(bool)), SLOT(monolingualPracticeSelected(bool)));
    connect(SynonymRadio, SIGNAL(toggled(bool)), SLOT(monolingualPracticeSelected(bool)));
    connect(AntonymRadio, SIGNAL(toggled(bool)), SLOT(bilingualPracticeSelected(bool)));
    connect(ComparisonRadio, SIGNAL(toggled(bool)), SLOT(monolingualPracticeSelected(bool)));
    connect(ConjugationRadio, SIGNAL(toggled(bool)), SLOT(monolingualPracticeSelected(bool)));
}


void ConfigurePracticeWidget::updateSettings()
{
    Prefs::setQuestionLanguage(LanguageFromList->currentRow());
    if (m_bilingual) {
        Prefs::setSolutionLanguage(LanguageToList->currentItem()->data(Qt::UserRole).toInt());
    } else {
        Prefs::setSolutionLanguage(LanguageFromList->currentRow());
    }

    if ( m_tenseListWidget ) {
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
}

void ConfigurePracticeWidget::fromLanguageSelected(int identifierFromIndex)
{
    LanguageToList->clear();
    for ( int i = 0; i < m_doc->identifierCount(); i++ ) {
        if ( i != identifierFromIndex ) {

            LanguageSettings currentSettings(m_doc->identifier(i).locale());
            currentSettings.readConfig();
            QString icon = currentSettings.icon();
            if (icon.isEmpty()) {
                icon = QString("set-language");
            }
            LanguageToList->addItem( new QListWidgetItem( KIcon(icon), m_doc->identifier(i).name() ) );

            LanguageToList->item(LanguageToList->count()-1)->setData(Qt::UserRole, i);
            if ( i == Prefs::solutionLanguage() ) {
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
    LanguageFromList->setCurrentRow(Prefs::questionLanguage());
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
        //LanguageFromList->currentRow() == Prefs::questionLanguage() &&
        //    toRow == Prefs::solutionLanguage();

        /// @todo tenses
}

bool ConfigurePracticeWidget::isDefault()
{
    ///@todo language selection and tenses
    return true;
//         LanguageFromList->currentRow() == 0 &&
//         LanguageToList->currentRow() == 1;
}

void ConfigurePracticeWidget::writtenRadioToggled(bool checked)
{
    if ( checked ) {
        OptionsGroupBox->setEnabled(true);
        m_optionsStackedLayout->setCurrentIndex(WrittenPractice);
    }
}


void ConfigurePracticeWidget::multipleChoiceRadioToggled(bool checked)
{
    if ( checked ) {
        OptionsGroupBox->setEnabled(true);
        m_optionsStackedLayout->setCurrentIndex(MultipleChoice);
    }
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


void ConfigurePracticeWidget::flashCardsRadioToggled(bool checked)
{
    if ( checked ) {
        OptionsGroupBox->setEnabled(true);
        m_optionsStackedLayout->setCurrentIndex(FlashCards);
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

    foreach ( const QString &tenseName, m_doc->tenseDescriptions() ) {
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
    ///@todo emit changed when checkstate changed
}

void ConfigurePracticeWidget::monolingualPracticeSelected(bool selected)
{
    if (selected) {
        LanguageToList->setEnabled(false);
        m_bilingual = false;
    }
}

void ConfigurePracticeWidget::bilingualPracticeSelected(bool selected)
{
    if (selected) {
        LanguageToList->setEnabled(true);
        m_bilingual = true;
    }
}

#include "configurepracticewidget.moc"
