/***************************************************************************

    Copyright 2008-2010 Frederik Gladhorn <gladhorn@kde.org>
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "statisticsmainwindow.h"

#include <QtCore/qsignalmapper.h>

#include <KLocale>
#include <KConfig>
#include <KGlobal>
#include <KActionCollection>

#include <keduvocdocument.h>
#include "configure-practice/configurepracticedialog.h"
#include "lessonstatistics.h"
#include "statisticsmodel.h"
#include "parleymainwindow.h"
#include "parleyactions.h"

#include "prefs.h"


#include "ui_statisticsmainwindow.h"
#include "prefs.h"
#include "conjugationoptions.h"

using namespace Editor;

StatisticsMainWindow::StatisticsMainWindow(KEduVocDocument* doc, ParleyMainWindow* parent)
    : KXmlGuiWindow(parent)
    , m_mainWindow(parent)
    , m_doc(doc)
    , m_conjugationOptions(0)
{
    // KXmlGui
    setXMLFile("statisticsui.rc");
    setObjectName("Statistics");

    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    m_ui = new Ui::StatisticsMainWindow;
    m_ui->setupUi(mainWidget);
    //m_ui->caption->setText(i18nc("caption for an overview of the confidence levels for a document"
    //                             "Statistics for \"%1\"", m_doc->title()));
    m_statisticsModel = new StatisticsModel(this);
    //DatabaseManager *dm = new DatabaseManager();
    //dm->createDB();

    setDocument(doc);
    initActions();
    initPracticeModeSelection();
    initLanguages();
    initPracticeMode();

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);
}

StatisticsMainWindow::~StatisticsMainWindow()
{
    if (m_conjugationOptions) {
        m_conjugationOptions->updateSettings();
    }
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
    delete m_ui;
}

void StatisticsMainWindow::syncConfig()
{
    //kDebug() << "save tenses";
    if (m_conjugationOptions) {
        m_conjugationOptions->updateSettings();
    }
}

void StatisticsMainWindow::setDocument(KEduVocDocument* doc)
{
    m_doc = doc;
    m_statisticsModel->setDocument(doc);
    m_ui->lessonStatistics->setModel(m_statisticsModel);
    m_ui->lessonStatistics->expandAll();
    m_ui->lessonStatistics->resizeColumnToContents(0);
}

void StatisticsMainWindow::initActions()
{
    ParleyActions::create(ParleyActions::EnterEditMode, m_mainWindow, SLOT(showEditor()), actionCollection());
    ParleyActions::create(ParleyActions::StartPractice, m_mainWindow, SLOT(startPractice()), actionCollection());
    ParleyActions::create(ParleyActions::ConfigurePractice, m_mainWindow, SLOT(configurePractice()), actionCollection());
}

void StatisticsMainWindow::initPracticeModeSelection()
{
    switch (Prefs::practiceMode()) {
    case Prefs::EnumPracticeMode::FlashCardsPractice:
        m_ui->flashCard->setChecked(true);
        break;
    case Prefs::EnumPracticeMode::MixedLettersPractice:
        m_ui->mixedLetters->setChecked(true);
        break;
    case Prefs::EnumPracticeMode::MultipleChoicePractice:
        m_ui->multipleChoice->setChecked(true);
        break;
    case Prefs::EnumPracticeMode::WrittenPractice:
        m_ui->written->setChecked(true);
        break;
    case Prefs::EnumPracticeMode::ExampleSentencesPractice:
        m_ui->exampleSentence->setChecked(true);
        break;
    case Prefs::EnumPracticeMode::GenderPractice:
        m_ui->gender->setChecked(true);
        break;
    case Prefs::EnumPracticeMode::ComparisonPractice:
        m_ui->comparisonForms->setChecked(true);
        break;
    case Prefs::EnumPracticeMode::ConjugationPractice:
        m_ui->conjugations->setChecked(true);
        showConjugationOptions();
        break;
    default:
        break;
    }
    QSignalMapper* mapper = new QSignalMapper(this);
    mapper->setMapping(m_ui->flashCard, Prefs::EnumPracticeMode::FlashCardsPractice);
    mapper->setMapping(m_ui->mixedLetters, Prefs::EnumPracticeMode::MixedLettersPractice);
    mapper->setMapping(m_ui->multipleChoice, Prefs::EnumPracticeMode::MultipleChoicePractice);
    mapper->setMapping(m_ui->written, Prefs::EnumPracticeMode::WrittenPractice);
    mapper->setMapping(m_ui->exampleSentence, Prefs::EnumPracticeMode::ExampleSentencesPractice);
    mapper->setMapping(m_ui->gender, Prefs::EnumPracticeMode::GenderPractice);
    mapper->setMapping(m_ui->comparisonForms, Prefs::EnumPracticeMode::ComparisonPractice);
    mapper->setMapping(m_ui->conjugations, Prefs::EnumPracticeMode::ConjugationPractice);
    connect(m_ui->flashCard, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(m_ui->mixedLetters, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(m_ui->multipleChoice, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(m_ui->written, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(m_ui->exampleSentence, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(m_ui->gender, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(m_ui->comparisonForms, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(m_ui->conjugations, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(practiceModeSelected(int)));
}

void StatisticsMainWindow::initLanguages()
{
    //kDebug() << "init languages: " << Prefs::learningLanguage() << Prefs::knownLanguage();
    const int totalNumLanguages = m_doc->identifierCount();

    if ( Prefs::knownLanguage() < 0 || totalNumLanguages <= Prefs::knownLanguage() ) {
        Prefs::setKnownLanguage(0);
    }
    if ( Prefs::learningLanguage() < 0 || totalNumLanguages <= Prefs::learningLanguage() ) {
        Prefs::setLearningLanguage(0);
    }

    if (Prefs::knownLanguage() >= totalNumLanguages
        || Prefs::learningLanguage() >= totalNumLanguages
        || Prefs::learningLanguage() == Prefs::knownLanguage())
    {
        Prefs::setKnownLanguage(0);
        Prefs::setLearningLanguage(1);
        //kDebug() << "Invalid language selection.";
    }

    // Insert data into the comboboxes.
    for (int i = 0; i < totalNumLanguages; ++i) {
        m_ui->learnedLanguage->insertItem(i, m_doc->identifier(i).name());
        m_ui->knownLanguage->insertItem(i, m_doc->identifier(i).name());
    }
    m_ui->learnedLanguage->setCurrentIndex(Prefs::learningLanguage());
    m_ui->knownLanguage->setCurrentIndex(Prefs::knownLanguage());

    connect(m_ui->learnedLanguage, SIGNAL(currentIndexChanged(int)), SLOT(languagesChanged()));
    connect(m_ui->knownLanguage, SIGNAL(currentIndexChanged(int)), SLOT(languagesChanged()));

    languagesChanged();
}

void StatisticsMainWindow::languagesChanged()
{
    int knownLanguage = m_ui->knownLanguage->currentIndex();
    int learningLanguage = m_ui->learnedLanguage->currentIndex();
    Prefs::setLearningLanguage(learningLanguage);
    Prefs::setKnownLanguage(knownLanguage);

    emit languagesChanged(knownLanguage, learningLanguage);

    updateVisibleColumns();
}

void StatisticsMainWindow::initPracticeMode()
{
    m_ui->practiceDirection->insertItem(0, i18n("Known to Learning"));
    m_ui->practiceDirection->insertItem(1, i18n("Learning to Known"));
    m_ui->practiceDirection->insertItem(2, i18n("Mixed Directions"));
    //m_ui->practiceDirection->insertItem(3, i18n("Mixed Directions with Sound"));

    if (Prefs::practiceDirection() < 0 || 3 < Prefs::practiceDirection())
        Prefs::setPracticeDirection(Prefs::EnumPracticeDirection::MixedDirectionsWordsOnly);

    m_ui->practiceDirection->setCurrentIndex(Prefs::practiceDirection());

    connect(m_ui->practiceDirection, SIGNAL(currentIndexChanged(int)),
            this,                    SLOT(practiceDirectionChanged(int)));
}

void StatisticsMainWindow::practiceModeSelected(int mode)
{
    Prefs::setPracticeMode(static_cast<Prefs::EnumPracticeMode::type>(mode));
    //kDebug() << "mode: " << mode << Prefs::practiceMode();

    showConjugationOptions(mode == Prefs::EnumPracticeMode::ConjugationPractice);
}

void StatisticsMainWindow::practiceDirectionChanged(int mode)
{
    //kDebug() << "new practice direction:" << mode;
    Prefs::setPracticeDirection(static_cast<Prefs::EnumPracticeDirection::type>(mode));
}

void StatisticsMainWindow::updateVisibleColumns()
{
    bool isHidden;
    for (int i = ContainerModel::FirstDataColumn; i < m_ui->lessonStatistics->header()->count(); i++) {
        int iLang = i - ContainerModel::FirstDataColumn;

        switch (Prefs::practiceDirection()) {
        case Prefs::EnumPracticeDirection::LearningToKnown:
            isHidden = iLang != Prefs::knownLanguage();
            break;
        case Prefs::EnumPracticeDirection::MixedDirectionsWordsOnly:
        case Prefs::EnumPracticeDirection::MixedDirectionsWithSound:
            isHidden = iLang != Prefs::knownLanguage() && iLang !=  Prefs::learningLanguage();
            break;
        case Prefs::EnumPracticeDirection::KnownToLearning:
        // Use KnownToLearning as default.
        default:
            isHidden = iLang != Prefs::learningLanguage();
            break;
        }

        m_ui->lessonStatistics->setColumnHidden(i, isHidden);
    }
}

void StatisticsMainWindow::showConjugationOptions(bool visible)
{
    if (!m_conjugationOptions && !visible) {
        return;
    }

    if (!m_conjugationOptions) {
        m_conjugationOptions = new ConjugationOptions(m_doc, m_ui->modeSpecificOptions);
        QHBoxLayout* layout = new QHBoxLayout(m_ui->modeSpecificOptions);
        layout->setMargin(0);
        layout->addWidget(m_conjugationOptions);
        connect(this, SIGNAL(languagesChanged(int, int)), m_conjugationOptions, SLOT(setLanguages(int, int)));
        m_conjugationOptions->setLanguages(Prefs::knownLanguage(), Prefs::learningLanguage());
    }
    m_conjugationOptions->setVisible(visible);
}

void StatisticsMainWindow::configurePractice()
{
    ConfigurePracticeDialog dialog(m_doc, this, "practice settings",  Prefs::self());
    dialog.exec();
}

#include "statisticsmainwindow.moc"
