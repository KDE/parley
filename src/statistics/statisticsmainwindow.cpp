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
    :KXmlGuiWindow(parent)
    ,m_mainWindow(parent)
    ,m_doc(doc)
    ,m_conjugationOptions(0)
{
    // KXmlGui
    setXMLFile("statisticsui.rc");
    setObjectName("Statistics");

    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    m_ui = new Ui::StatisticsMainWindow;
    m_ui->setupUi(mainWidget);
    m_ui->caption->setText(i18nc("caption for an overview of the grades for a document", "Statistics for \"%1\"", m_doc->title()));
    m_statisticsModel = new StatisticsModel(this);

    setDocument(doc);
    initActions();
    initPracticeModeSelection();
    initLanguages();

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

void StatisticsMainWindow::setDocument(KEduVocDocument* doc)
{
    m_doc = doc;
    m_statisticsModel->setDocument(doc);
    m_ui->lessonStatistics->setModel(m_statisticsModel);
    m_ui->lessonStatistics->expandToDepth(0);
}

void StatisticsMainWindow::initActions()
{
    ParleyActions::create(ParleyActions::EnterEditMode, m_mainWindow, SLOT(showEditor()), actionCollection());
    ParleyActions::create(ParleyActions::StartPractice, m_mainWindow, SLOT(startPractice()), actionCollection());
    ParleyActions::create(ParleyActions::ConfigurePractice, m_mainWindow, SLOT(configurePractice()), actionCollection());

    m_ui->startPracticeButton->setText(i18n("Start Practice..."));
    m_ui->startPracticeButton->setIcon(KIcon("practice-start"));
    connect(m_ui->startPracticeButton, SIGNAL(clicked()), m_mainWindow, SLOT(startPractice()));
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
    kDebug() << "init languages: " << Prefs::questionLanguage() << Prefs::solutionLanguage();
    const int totalNumLanguages = m_doc->identifierCount();
    if (Prefs::questionLanguage() >= totalNumLanguages || Prefs::solutionLanguage() >= totalNumLanguages
            || Prefs::solutionLanguage() == Prefs::questionLanguage()) {
        Prefs::setQuestionLanguage(0);
        Prefs::setSolutionLanguage(1);
        kDebug() << "Invalid language selection.";
    }
    for ( int i = 0; i < totalNumLanguages-1; i++ ) {
        for (int j = i+1; j < totalNumLanguages; j++) {
            QListWidgetItem* item = new QListWidgetItem(
                i18nc("pair of two languages that the user chooses to practice", "%1 to %2",
                m_doc->identifier(i).name(), m_doc->identifier(j).name()));
            item->setData(Qt::UserRole, i);
            item->setData(Qt::UserRole+1, j);
            m_ui->languageList->addItem(item);

            if (i == Prefs::questionLanguage() && j == Prefs::solutionLanguage()) {
                m_ui->languageList->setCurrentItem(item);
            }

            QListWidgetItem* item2 = new QListWidgetItem(
                i18nc("pair of two languages that the user chooses to practice", "%1 to %2",
                m_doc->identifier(j).name(), m_doc->identifier(i).name()));
            item2->setData(Qt::UserRole, j);
            item2->setData(Qt::UserRole+1, i);
            m_ui->languageList->addItem(item2);

            if (j == Prefs::questionLanguage() && i == Prefs::solutionLanguage()) {
                m_ui->languageList->setCurrentItem(item2);
            }
        }
    }
    connect(m_ui->languageList, SIGNAL(currentRowChanged(int)), SLOT(languagesChanged()));
    m_ui->languageList->sortItems();
    languagesChanged();
}

void StatisticsMainWindow::languagesChanged()
{
    int questionLanguage;
    int solutionLangauge;
    QListWidgetItem* current = m_ui->languageList->currentItem();
    questionLanguage = current->data(Qt::UserRole).toInt();
    solutionLangauge = current->data(Qt::UserRole+1).toInt();
    Prefs::setQuestionLanguage(questionLanguage);
    Prefs::setSolutionLanguage(solutionLangauge);
    emit languagesChanged(questionLanguage, solutionLangauge);
    //m_ui->lessonStatistics->showGrades(current->data(Qt::UserRole).toInt(), current->data(Qt::UserRole+1).toInt());
    kDebug() << "set languages: " << current->data(Qt::UserRole).toInt() << current->data(Qt::UserRole+1).toInt();
}

void StatisticsMainWindow::practiceModeSelected(int mode)
{
    Prefs::setPracticeMode(static_cast<Prefs::EnumPracticeMode::type>(mode));
    kDebug() << "mode: " << mode << Prefs::practiceMode();

    showConjugationOptions(mode == Prefs::EnumPracticeMode::ConjugationPractice);
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
        connect(this, SIGNAL(languagesChanged(int,int)), m_conjugationOptions, SLOT(setLanguages(int,int)));
        m_conjugationOptions->setLanguages(Prefs::questionLanguage(), Prefs::solutionLanguage());
    }
    m_conjugationOptions->setVisible(visible);
}

void StatisticsMainWindow::configurePractice()
{
    ConfigurePracticeDialog dialog(m_doc, this, "practice settings",  Prefs::self());
    dialog.exec();
}

#include "statisticsmainwindow.moc"
