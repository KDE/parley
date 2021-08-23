/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-FileCopyrightText: 2008 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "editor.h"

#include <config-parley.h>

// Qt models on top of the KEduVocDocument
#include "containermodel.h"
#include "lessonmodel.h"
#include "vocabularymodel.h"
#include "vocabularyfilter.h"
#include "wordclassmodel.h"

// Views
#include "vocabularyview.h"
#include "containerview.h"
#include "lessonview.h"
#include "wordtypeview.h"

#include "multiplechoicewidget.h"
#include "comparisonwidget.h"
#include "inflectionwidget.h"
#include "imagechooserwidget.h"
#include "audiowidget.h"
#include "browserwidget.h"
#include "synonymwidget.h"
#include "summarywordwidget.h"
#include "latexwidget.h"

#include "settings/parleyprefs.h"
#include "prefs.h"

#include "scripts/scriptdialog.h"
#include "scripts/translator.h"

#include "parleyactions.h"
#include "parleyadaptor.h"

#include <KActionCollection>
#include <KToggleAction>
#include <KActionMenu>
#include <KCharSelect>

#include <QTimer>
#include <QDockWidget>
#include <QHeaderView>
#include <QMenu>
#include <QStackedWidget>
#include <QScrollArea>

using namespace Editor;

EditorWindow::EditorWindow(ParleyMainWindow* parent)
    : KXmlGuiWindow(parent), m_mainWindow(parent)
{
    // KXmlGui
    setXMLFile(QStringLiteral("editorui.rc"));
    setObjectName(QStringLiteral("Editor"));

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    initView();
    initModel();

    initDockWidgets();
    initActions();

    KConfigGroup cfg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), objectName());
    applyMainWindowSettings(cfg);

    connect(parent->parleyDocument(), &ParleyDocument::documentChanged,
            this,                     &EditorWindow::updateDocument);
    connect(parent->parleyDocument(), &ParleyDocument::languagesChanged,
            this,                     &EditorWindow::slotLanguagesChanged);
    connect(parent->parleyDocument(), SIGNAL(statesNeedSaving()), this, SLOT(saveState()));
    connect(parent, &ParleyMainWindow::preferencesChanged, this, &EditorWindow::applyPrefs);

    QTimer::singleShot(0, this, &EditorWindow::initScripts);
}

EditorWindow::~EditorWindow()
{
    saveState();
    KConfigGroup cfg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), objectName());
    saveMainWindowSettings(cfg);
}

void EditorWindow::updateDocument(const std::shared_ptr<KEduVocDocument> &doc)
{

    m_vocabularyView->setDocument(doc);
    m_vocabularyModel->setDocument(doc.get());

    m_lessonModel->setDocument(doc);
    m_wordTypeModel->setDocument(doc);

    m_summaryWordWidget->slotDocumentChanged(doc);
    m_inflectionWidget->setDocument(doc);
    m_comparisonWidget->setDocument(doc.get());
    m_synonymWidget->setDocument(doc.get());
    m_antonymWidget->setDocument(doc.get());
    m_falseFriendWidget->setDocument(doc.get());

    if (!m_mainWindow->parleyDocument()->document()) {
        return;
    }

    if (!doc) {
        return;
    }

    // expand the root items
    m_lessonView->expandToDepth(0);
    m_wordTypeView->expandToDepth(0);

    connect(m_vocabularyView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            m_summaryWordWidget, &SummaryWordWidget::slotSelectionChanged);
    connect(m_vocabularyView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            m_latexWidget, &LatexWidget::slotSelectionChanged);

    m_spellCheckMenu->menu()->clear();
    for (int i = 0; i < doc->identifierCount(); ++i) {
        QAction* languageSpellCheck = new QAction(doc->identifier(i).name(), m_spellCheckMenu->menu());
        m_spellCheckMenu->menu()->addAction(languageSpellCheck);
        connect(languageSpellCheck, &QAction::triggered,
                this, [=] {m_vocabularyView->checkSpelling(i);});
    }
}


void EditorWindow::initDockWidgets()
{
    // Lesson dockwidget
    QDockWidget *lessonDockWidget = new QDockWidget(i18n("Units"), this);
    lessonDockWidget->setObjectName(QStringLiteral("LessonDock"));
    m_lessonView = new LessonView(this);
    lessonDockWidget->setWidget(m_lessonView);
    addDockWidget(Qt::LeftDockWidgetArea, lessonDockWidget);
    m_dockWidgets.append(lessonDockWidget);
    actionCollection()->addAction(QStringLiteral("show_units_dock"), lessonDockWidget->toggleViewAction());

    m_lessonModel = new LessonModel(this);
    m_lessonView->setModel(m_lessonModel);
    m_lessonView->setToolTip(i18n("Right click to add, delete, or rename units. \n"
                                  "With the checkboxes you can select which units you want to practice. \n"
                                  "Only checked units [x] will be asked in the practice!"));

    connect(m_lessonView, &LessonView::selectedLessonChanged,
            m_vocabularyModel, &VocabularyModel::setLesson);

    connect(m_lessonView, &LessonView::signalShowContainer,
            m_vocabularyModel, &VocabularyModel::showContainer);

    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_lessonView, &LessonView::setTranslation);


    // Word classes dock widget
    QDockWidget* wordTypeDockWidget = new QDockWidget(i18n("Word Types"), this);
    wordTypeDockWidget->setObjectName(QStringLiteral("WordTypeDock"));
    m_wordTypeView = new WordTypeView(this);
    wordTypeDockWidget->setWidget(m_wordTypeView);
    addDockWidget(Qt::LeftDockWidgetArea, wordTypeDockWidget);
    m_dockWidgets.append(wordTypeDockWidget);

    m_wordTypeModel = new WordClassModel(this);
    wordTypeDockWidget->setVisible(false);
    actionCollection()->addAction(QStringLiteral("show_wordtype_dock"), wordTypeDockWidget->toggleViewAction());

///@todo test, should be fixed with the lesson one though
///@todo remove before release
//     new ModelTest(m_wordTypeModel, this);

    m_wordTypeView->setModel(m_wordTypeModel);

    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_wordTypeView, &WordTypeView::setTranslation);

// Inflections
    QDockWidget *inflectionDock = new QDockWidget(i18n("Inflection (verbs, adjectives, nouns)"), this);
    inflectionDock->setObjectName(QStringLiteral("InflectionDock"));
    m_inflectionWidget = new InflectionWidget(this);
    QScrollArea *inflectionScrollArea = new QScrollArea(this);
    inflectionScrollArea->setWidgetResizable(true);
    inflectionScrollArea->setWidget(m_inflectionWidget);
    inflectionDock->setWidget(inflectionScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, inflectionDock);
    m_dockWidgets.append(inflectionDock);
    actionCollection()->addAction(QStringLiteral("show_inflection_dock"), inflectionDock->toggleViewAction());
    connect(m_mainWindow->parleyDocument(), &ParleyDocument::documentChanged,
            m_inflectionWidget, &InflectionWidget::setDocument);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_inflectionWidget, &InflectionWidget::setTranslation);

// Comparison forms
    QDockWidget *comparisonDock = new QDockWidget(i18n("Comparison forms"), this);
    comparisonDock->setObjectName(QStringLiteral("ComparisonDock"));
    m_comparisonWidget = new ComparisonWidget(this);
    QScrollArea *comparisonScrollArea = new QScrollArea(this);
    comparisonScrollArea->setWidgetResizable(true);
    comparisonScrollArea->setWidget(m_comparisonWidget);
    comparisonDock->setWidget(comparisonScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, comparisonDock);
    m_dockWidgets.append(comparisonDock);
    actionCollection()->addAction(QStringLiteral("show_comparison_dock"), comparisonDock->toggleViewAction());
    comparisonDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_comparisonWidget, &ComparisonWidget::setTranslation);
    tabifyDockWidget(comparisonDock,inflectionDock);

// Multiple choice
    QDockWidget *multipleChoiceDock = new QDockWidget(i18n("Multiple Choice"), this);
    multipleChoiceDock->setObjectName(QStringLiteral("MultipleChoiceDock"));
    MultipleChoiceWidget *multipleChoiceWidget = new MultipleChoiceWidget(this);
    QScrollArea *multipleChoiceScrollArea = new QScrollArea(this);
    multipleChoiceScrollArea->setWidgetResizable(true);
    multipleChoiceScrollArea->setWidget(multipleChoiceWidget);
    multipleChoiceDock->setWidget(multipleChoiceScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, multipleChoiceDock);
    m_dockWidgets.append(multipleChoiceDock);
    actionCollection()->addAction(QStringLiteral("show_multiplechoice_dock"), multipleChoiceDock->toggleViewAction());
    multipleChoiceDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            multipleChoiceWidget, &MultipleChoiceWidget::setTranslation);

// Synonym (and the same for antonym and false friends)
    QDockWidget *synonymDock = new QDockWidget(i18n("Synonyms"), this);
    synonymDock->setObjectName(QStringLiteral("SynonymDock"));
    m_synonymWidget = new SynonymWidget(SynonymWidget::Synonym, this);
    QScrollArea *synonymScrollArea = new QScrollArea(this);
    synonymScrollArea->setWidgetResizable(true);
    synonymScrollArea->setWidget(m_synonymWidget);
    synonymDock->setWidget(synonymScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, synonymDock);
    m_dockWidgets.append(synonymDock);
    actionCollection()->addAction(QStringLiteral("show_synonym_dock"), synonymDock->toggleViewAction());
    synonymDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_synonymWidget, &SynonymWidget::setTranslation);

    QDockWidget *antonymDock = new QDockWidget(i18n("Antonyms"), this);
    antonymDock->setObjectName(QStringLiteral("AntonymDock"));
    m_antonymWidget = new SynonymWidget(SynonymWidget::Antonym, this);
    QScrollArea *antonymScrollArea = new QScrollArea(this);
    antonymScrollArea->setWidgetResizable(true);
    antonymScrollArea->setWidget(m_antonymWidget);
    antonymDock->setWidget(antonymScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, antonymDock);
    m_dockWidgets.append(antonymDock);
    actionCollection()->addAction(QStringLiteral("show_antonym_dock"), antonymDock->toggleViewAction());
    antonymDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_antonymWidget, &SynonymWidget::setTranslation);
    tabifyDockWidget(synonymDock,antonymDock);

    QDockWidget *falseFriendDock = new QDockWidget(i18n("False Friends"), this);
    falseFriendDock->setObjectName(QStringLiteral("FalseFriendDock"));
    m_falseFriendWidget = new SynonymWidget(SynonymWidget::FalseFriend, this);
    QScrollArea *falseFriendScrollArea = new QScrollArea(this);
    falseFriendScrollArea->setWidgetResizable(true);
    falseFriendScrollArea->setWidget(m_falseFriendWidget);
    falseFriendDock->setWidget(falseFriendScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, falseFriendDock);
    m_dockWidgets.append(falseFriendDock);
    actionCollection()->addAction(QStringLiteral("show_falsefriend_dock"), falseFriendDock->toggleViewAction());
    falseFriendDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_falseFriendWidget, &SynonymWidget::setTranslation);
    tabifyDockWidget(antonymDock,falseFriendDock);

// Pronunciation symbols - Use KCharSelect
    QDockWidget *charSelectDock = new QDockWidget(i18n("Phonetic Symbols"), this);
    charSelectDock->setObjectName(QStringLiteral("IPADock"));
    KCharSelect *charSelectWidget = new KCharSelect(this, 0, KCharSelect::SearchLine | KCharSelect::BlockCombos | KCharSelect::CharacterTable);
    charSelectWidget->setCurrentChar(0x0250);
    QScrollArea *charSelectScrollArea = new QScrollArea(this);
    charSelectScrollArea->setWidgetResizable(true);
    charSelectScrollArea->setWidget(charSelectWidget);
    charSelectDock->setWidget(charSelectScrollArea);
    addDockWidget(Qt::BottomDockWidgetArea, charSelectDock);
    m_dockWidgets.append(charSelectDock);
    actionCollection()->addAction(QStringLiteral("show_pronunciation_dock"), charSelectDock->toggleViewAction());
    charSelectDock->setVisible(false);
    connect(charSelectWidget, &KCharSelect::charSelected, m_vocabularyView, &VocabularyView::appendChar);

// Image
    QDockWidget *imageDock = new QDockWidget(i18n("Image"), this);
    imageDock->setObjectName(QStringLiteral("ImageDock"));
    ImageChooserWidget *imageChooserWidget = new ImageChooserWidget(this);
    QScrollArea *imageScrollArea = new QScrollArea(this);
    imageScrollArea->setWidgetResizable(true);
    imageScrollArea->setWidget(imageChooserWidget);
    imageDock->setWidget(imageScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, imageDock);
    m_dockWidgets.append(imageDock);
    actionCollection()->addAction(QStringLiteral("show_image_dock"), imageDock->toggleViewAction());
    imageDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            imageChooserWidget, &ImageChooserWidget::setTranslation);
    tabifyDockWidget(multipleChoiceDock,imageDock);

// Summary word
    QDockWidget *summaryDock = new QDockWidget(i18n("Summary"), this);
    summaryDock->setObjectName(QStringLiteral("SummaryDock"));
    m_summaryWordWidget = new SummaryWordWidget(m_vocabularyFilter, m_mainWindow->parleyDocument()->document(), this);
    QScrollArea *summaryScrollArea = new QScrollArea(this);
    summaryScrollArea->setWidgetResizable(true);
    summaryScrollArea->setWidget(m_summaryWordWidget);
    summaryDock->setWidget(summaryScrollArea);
    addDockWidget(Qt::BottomDockWidgetArea, summaryDock);
    actionCollection()->addAction(QStringLiteral("show_summary_dock"), summaryDock->toggleViewAction());
    summaryDock->setVisible(false);
    m_dockWidgets.append(summaryDock);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_summaryWordWidget, &SummaryWordWidget::setTranslation);

// Sound
    QDockWidget *audioDock = new QDockWidget(i18n("Sound"), this);
    audioDock->setObjectName(QStringLiteral("AudioDock"));
    AudioWidget *audioWidget = new AudioWidget(this);
    QScrollArea *audioScrollArea = new QScrollArea(this);
    audioScrollArea->setWidgetResizable(true);
    audioScrollArea->setWidget(audioWidget);
    audioDock->setWidget(audioScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, audioDock);
    m_dockWidgets.append(audioDock);
    actionCollection()->addAction(QStringLiteral("show_audio_dock"), audioDock->toggleViewAction());
    audioDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            audioWidget, &AudioWidget::setTranslation);
    tabifyDockWidget(imageDock,audioDock);

// browser
    QDockWidget *browserDock = new QDockWidget(i18n("Internet"), this);
    browserDock->setObjectName(QStringLiteral("BrowserDock"));
    BrowserWidget *htmlPart = new BrowserWidget(browserDock);
    QScrollArea *browserScrollArea = new QScrollArea(this);
    browserScrollArea->setWidgetResizable(true);
    browserScrollArea->setWidget(htmlPart);
    browserDock->setWidget(browserScrollArea);
    addDockWidget(Qt::BottomDockWidgetArea, browserDock);
    m_dockWidgets.append(browserDock);
    actionCollection()->addAction(QStringLiteral("show_browser_dock"), browserDock->toggleViewAction());
    browserDock->setVisible(false);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            htmlPart, &BrowserWidget::setTranslation);
    tabifyDockWidget(summaryDock,browserDock);

// LaTeX
    QDockWidget *latexDock = new QDockWidget(i18n("LaTeX"), this);
    latexDock->setObjectName(QStringLiteral("LatexDock"));
    m_latexWidget = new LatexWidget(m_vocabularyFilter, m_mainWindow->parleyDocument()->document().get(), this);
    QScrollArea *latexScrollArea = new QScrollArea(this);
    latexScrollArea->setWidgetResizable(true);
    latexScrollArea->setWidget(m_latexWidget);
    latexDock->setWidget(latexScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, latexDock);
    actionCollection()->addAction(QStringLiteral("show_latex_dock"), latexDock->toggleViewAction());
    latexDock->setVisible(false);
    m_dockWidgets.append(latexDock);
    connect(m_vocabularyView, &VocabularyView::translationChanged,
            m_latexWidget, &LatexWidget::setTranslation);
    tabifyDockWidget(audioDock,latexDock);

// Grades
//     QDockWidget *gradeDock = new QDockWidget(i18n("Grade"), this);
//     gradeDock->setObjectName("gradeDock");
//     QLabel *gradeWidget = new QLabel("grade placeholder", this);
//     gradeDock->setWidget(gradeWidget);
//     addDockWidget(Qt::RightDockWidgetArea, gradeDock);
//     connect(this, SIGNAL(signalSetData(KEduVocTranslation*)), m_declensionWidget, SLOT(setTranslation(KEduVocTranslation*)));

}

void EditorWindow::initActions()
{
    ParleyActions::create(ParleyActions::RemoveGrades, this, SLOT(removeGrades()), actionCollection());
    m_spellCheckMenu = ParleyActions::create(ParleyActions::CheckSpelling, 0, "", actionCollection());
    m_spellCheckMenu->setMenu(new QMenu(this));

    ParleyActions::create(ParleyActions::ToggleShowSublessons, m_vocabularyModel, SLOT(showEntriesOfSubcontainers(bool)), actionCollection());
    ParleyActions::create(ParleyActions::AutomaticTranslation, m_vocabularyModel, SLOT(automaticTranslation(bool)), actionCollection());
    ParleyActions::create(ParleyActions::StartPractice, m_mainWindow, SLOT(showPracticeConfiguration()), actionCollection());
    actionCollection()->action(QStringLiteral("practice_start"))->setText(i18n("Practice"));
    actionCollection()->action(QStringLiteral("practice_start"))->setToolTip(i18n("Practice"));
    ParleyActions::create(ParleyActions::ConfigurePractice, m_mainWindow, SLOT(configurePractice()), actionCollection());
    ParleyActions::create(ParleyActions::ToggleSearchBar, this, SLOT(slotConfigShowSearch()), actionCollection());
    ParleyActions::create(ParleyActions::SearchVocabulary, this, SLOT(startSearch()), actionCollection());
    ParleyActions::create(ParleyActions::ShowScriptManager, this, SLOT(slotShowScriptManager()), actionCollection());
    ParleyActions::create(ParleyActions::LanguagesProperties, m_mainWindow->parleyDocument(), SLOT(languageProperties()), actionCollection());

    new EditorWindowAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(QStringLiteral("/AddWithTranslation"), this);
}

void EditorWindow::addWordWithTranslation(const QStringList &w)
{
    KEduVocExpression *kexpr = new KEduVocExpression(w);

    m_vocabularyModel->appendEntry(kexpr);
}

void EditorWindow::initModel()
{
    m_vocabularyModel = new VocabularyModel(this);
    m_vocabularyFilter = new VocabularyFilter(this);
    m_vocabularyFilter->setSourceModel(m_vocabularyModel);
    m_vocabularyView->setFilter(m_vocabularyFilter);

//    connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyModel, SLOT(setDocument(KEduVocDocument*)));
//    connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyView, SLOT(setDocument(KEduVocDocument*)));
    connect(m_searchLine, &QLineEdit::textChanged, m_vocabularyFilter, &VocabularyFilter::setSearchString);
}

/**
 * This initializes the main widgets and table.
 */
void EditorWindow::initView()
{
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout *topLayout = new QVBoxLayout(mainWidget);

    m_searchLine = new QLineEdit(this);
    m_searchLine->show();
    m_searchLine->setFocusPolicy(Qt::ClickFocus);
    m_searchLine->setClearButtonEnabled(true);
    m_searchLine->setPlaceholderText(i18n("Enter search terms here"));
    m_searchLine->setToolTip(i18n("Search your vocabulary"));

    QLabel *label = new QLabel(i18n("S&earch:"), this);
    label->setBuddy(m_searchLine);
    label->show();

    m_searchWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(m_searchWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(label);
    layout->addWidget(m_searchLine);

///@todo     centralWidget()-> delete layout
    QVBoxLayout * rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->addWidget(m_searchWidget);
    m_searchWidget->setVisible(Prefs::showSearch());

    m_vocabularyView = new VocabularyView(this);
    rightLayout->addWidget(m_vocabularyView, 1);

    topLayout->addLayout(rightLayout);
}

void EditorWindow::slotConfigShowSearch()
{
    m_searchWidget->setVisible(m_searchWidget->isHidden());
    Prefs::setShowSearch(m_searchWidget->isVisible());
}

void EditorWindow::startSearch()
{
    m_searchWidget->setVisible(true);
    m_searchLine->setFocus();
}

void EditorWindow::slotShowScriptManager()
{
    ScriptDialog * dialog = new ScriptDialog(m_scriptManager);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void EditorWindow::applyPrefs()
{
    m_vocabularyView->reset();
}

void EditorWindow::removeGrades()
{
    m_mainWindow->parleyDocument()->document()->lesson()->resetGrades(-1, KEduVocContainer::Recursive);
}

void EditorWindow::initScripts()
{
    m_scriptManager = new ScriptManager(this);

    m_vocabularyView->setTranslator(m_scriptManager->translator());

    //Load scripts
    m_scriptManager->loadScripts();
}

void EditorWindow::saveState()
{
    m_vocabularyView->saveColumnVisibility();
}

void EditorWindow::slotLanguagesChanged()
{
    m_vocabularyModel->resetLanguages();
}
