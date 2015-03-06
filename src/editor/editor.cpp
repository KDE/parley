/***************************************************************************
    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "editor.h"

#include "../config-parley.h"

// Qt models on top of the KEduVocDocument

#include "vocabularyfilter.h"

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

//KEduVoc
#include <keduvocvocabularymodel.h>
#include <keduvoccontainermodel.h>
#include <keduvoclessonmodel.h>
#include <keduvocwordclassmodel.h>

#include <KActionCollection>
#include <KToggleAction>
#include <KActionMenu>
#include <KCharSelect>

#include <QtCore/QTimer>
#include <QtCore/QSignalMapper>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>
#include <QtWidgets/QStackedWidget>
#include <QScrollArea>

using namespace Editor;

EditorWindow::EditorWindow(ParleyMainWindow* parent)
    : KXmlGuiWindow(parent), m_mainWindow(parent)
{
    // KXmlGui
    setXMLFile("editorui.rc");
    setObjectName("Editor");

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    initView();
    initModel();

    initDockWidgets();
    initActions();

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);

    connect(parent->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)),
            this,                     SLOT(updateDocument(KEduVocDocument*)));
    connect(parent->parleyDocument(), SIGNAL(languagesChanged()),
            this,                     SLOT(slotLanguagesChanged()));
    connect(parent->parleyDocument(), SIGNAL(statesNeedSaving()), this, SLOT(saveState()));
    connect(parent, SIGNAL(preferencesChanged()), this, SLOT(applyPrefs()));

    QTimer::singleShot(0, this, SLOT(initScripts()));
}

EditorWindow::~EditorWindow()
{
    saveState();
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
}

void EditorWindow::updateDocument(KEduVocDocument *doc)
{

    m_vocabularyView->setDocument(doc);
    m_vocabularyModel->setDocument(doc);

    m_lessonModel->setDocument(doc);
    m_wordTypeModel->setDocument(doc);

    m_summaryWordWidget->slotDocumentChanged(doc);
    m_inflectionWidget->setDocument(doc);
    m_comparisonWidget->setDocument(doc);
    m_synonymWidget->setDocument(doc);
    m_antonymWidget->setDocument(doc);
    m_falseFriendWidget->setDocument(doc);

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
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            m_summaryWordWidget, SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(m_vocabularyView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            m_latexWidget, SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));

    m_spellCheckMenu->menu()->clear();
    for (int i = 0; i < doc->identifierCount(); ++i) {
        QAction* languageSpellCheck = new QAction(doc->identifier(i).name(), m_spellCheckMenu->menu());
        m_spellCheckMenu->menu()->addAction(languageSpellCheck);
        m_spellCheckMapper->setMapping(languageSpellCheck, i);
        connect(languageSpellCheck, SIGNAL(triggered()), m_spellCheckMapper, SLOT(map()));
    }
}


void EditorWindow::initDockWidgets()
{
    // Lesson dockwidget
    QDockWidget *lessonDockWidget = new QDockWidget(i18n("Units"), this);
    lessonDockWidget->setObjectName("LessonDock");
    m_lessonView = new LessonView(this);
    lessonDockWidget->setWidget(m_lessonView);
    addDockWidget(Qt::LeftDockWidgetArea, lessonDockWidget);
    m_dockWidgets.append(lessonDockWidget);
    actionCollection()->addAction("show_units_dock", lessonDockWidget->toggleViewAction());

    m_lessonModel = new KEduVocLessonModel(this);
    m_lessonView->setModel(m_lessonModel);
    m_lessonView->setToolTip(i18n("Right click to add, delete, or rename units. \n"
                                  "With the checkboxes you can select which units you want to practice. \n"
                                  "Only checked units [x] will be asked in the practice!"));

    connect(m_lessonView, SIGNAL(selectedLessonChanged(KEduVocLesson*)),
            m_vocabularyModel, SLOT(setLesson(KEduVocLesson*)));

    connect(m_lessonView, SIGNAL(signalShowContainer(KEduVocContainer*)),
            m_vocabularyModel, SLOT(showContainer(KEduVocContainer*)));

    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_lessonView, SLOT(setTranslation(KEduVocExpression*, int)));


    // Word classes dock widget
    QDockWidget* wordTypeDockWidget = new QDockWidget(i18n("Word Types"), this);
    wordTypeDockWidget->setObjectName("WordTypeDock");
    m_wordTypeView = new WordTypeView(this);
    wordTypeDockWidget->setWidget(m_wordTypeView);
    addDockWidget(Qt::LeftDockWidgetArea, wordTypeDockWidget);
    m_dockWidgets.append(wordTypeDockWidget);

    m_wordTypeModel = new KEduVocWordClassModel(this);
    wordTypeDockWidget->setVisible(false);
    actionCollection()->addAction("show_wordtype_dock", wordTypeDockWidget->toggleViewAction());

///@todo test, should be fixed with the lesson one though
///@todo remove before release
//     new ModelTest(m_wordTypeModel, this);

    m_wordTypeView->setModel(m_wordTypeModel);

    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_wordTypeView, SLOT(setTranslation(KEduVocExpression*, int)));

// Inflections
    QDockWidget *inflectionDock = new QDockWidget(i18n("Inflection (verbs, adjectives, nouns)"), this);
    inflectionDock->setObjectName("InflectionDock");
    m_inflectionWidget = new InflectionWidget(this);
    QScrollArea *inflectionScrollArea = new QScrollArea(this);
    inflectionScrollArea->setWidgetResizable(true);
    inflectionScrollArea->setWidget(m_inflectionWidget);
    inflectionDock->setWidget(inflectionScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, inflectionDock);
    m_dockWidgets.append(inflectionDock);
    actionCollection()->addAction("show_inflection_dock", inflectionDock->toggleViewAction());
    connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)),
            m_inflectionWidget, SLOT(setDocument(KEduVocDocument*)));
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_inflectionWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Comparison forms
    QDockWidget *comparisonDock = new QDockWidget(i18n("Comparison forms"), this);
    comparisonDock->setObjectName("ComparisonDock");
    m_comparisonWidget = new ComparisonWidget(this);
    QScrollArea *comparisonScrollArea = new QScrollArea(this);
    comparisonScrollArea->setWidgetResizable(true);
    comparisonScrollArea->setWidget(m_comparisonWidget);
    comparisonDock->setWidget(comparisonScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, comparisonDock);
    m_dockWidgets.append(comparisonDock);
    actionCollection()->addAction("show_comparison_dock", comparisonDock->toggleViewAction());
    comparisonDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_comparisonWidget, SLOT(setTranslation(KEduVocExpression*, int)));
    tabifyDockWidget(comparisonDock,inflectionDock);

// Multiple choice
    QDockWidget *multipleChoiceDock = new QDockWidget(i18n("Multiple Choice"), this);
    multipleChoiceDock->setObjectName("MultipleChoiceDock");
    MultipleChoiceWidget *multipleChoiceWidget = new MultipleChoiceWidget(this);
    QScrollArea *multipleChoiceScrollArea = new QScrollArea(this);
    multipleChoiceScrollArea->setWidgetResizable(true);
    multipleChoiceScrollArea->setWidget(multipleChoiceWidget);
    multipleChoiceDock->setWidget(multipleChoiceScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, multipleChoiceDock);
    m_dockWidgets.append(multipleChoiceDock);
    actionCollection()->addAction("show_multiplechoice_dock", multipleChoiceDock->toggleViewAction());
    multipleChoiceDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            multipleChoiceWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Synonym (and the same for antonym and false friends)
    QDockWidget *synonymDock = new QDockWidget(i18n("Synonyms"), this);
    synonymDock->setObjectName("SynonymDock");
    m_synonymWidget = new SynonymWidget(SynonymWidget::Synonym, this);
    QScrollArea *synonymScrollArea = new QScrollArea(this);
    synonymScrollArea->setWidgetResizable(true);
    synonymScrollArea->setWidget(m_synonymWidget);
    synonymDock->setWidget(synonymScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, synonymDock);
    m_dockWidgets.append(synonymDock);
    actionCollection()->addAction("show_synonym_dock", synonymDock->toggleViewAction());
    synonymDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_synonymWidget, SLOT(setTranslation(KEduVocExpression*, int)));

    QDockWidget *antonymDock = new QDockWidget(i18n("Antonyms"), this);
    antonymDock->setObjectName("AntonymDock");
    m_antonymWidget = new SynonymWidget(SynonymWidget::Antonym, this);
    QScrollArea *antonymScrollArea = new QScrollArea(this);
    antonymScrollArea->setWidgetResizable(true);
    antonymScrollArea->setWidget(m_antonymWidget);
    antonymDock->setWidget(antonymScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, antonymDock);
    m_dockWidgets.append(antonymDock);
    actionCollection()->addAction("show_antonym_dock", antonymDock->toggleViewAction());
    antonymDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_antonymWidget, SLOT(setTranslation(KEduVocExpression*, int)));
    tabifyDockWidget(synonymDock,antonymDock);

    QDockWidget *falseFriendDock = new QDockWidget(i18n("False Friends"), this);
    falseFriendDock->setObjectName("FalseFriendDock");
    m_falseFriendWidget = new SynonymWidget(SynonymWidget::FalseFriend, this);
    QScrollArea *falseFriendScrollArea = new QScrollArea(this);
    falseFriendScrollArea->setWidgetResizable(true);
    falseFriendScrollArea->setWidget(m_falseFriendWidget);
    falseFriendDock->setWidget(falseFriendScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, falseFriendDock);
    m_dockWidgets.append(falseFriendDock);
    actionCollection()->addAction("show_falsefriend_dock", falseFriendDock->toggleViewAction());
    falseFriendDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_falseFriendWidget, SLOT(setTranslation(KEduVocExpression*, int)));
    tabifyDockWidget(antonymDock,falseFriendDock);

// Pronunciation symbols - Use KCharSelect
    QDockWidget *charSelectDock = new QDockWidget(i18n("Phonetic Symbols"), this);
    charSelectDock->setObjectName("IPADock");
    KCharSelect *charSelectWidget = new KCharSelect(this, 0, KCharSelect::SearchLine | KCharSelect::BlockCombos | KCharSelect::CharacterTable);
    charSelectWidget->setCurrentChar(0x0250);
    QScrollArea *charSelectScrollArea = new QScrollArea(this);
    charSelectScrollArea->setWidgetResizable(true);
    charSelectScrollArea->setWidget(charSelectWidget);
    charSelectDock->setWidget(charSelectScrollArea);
    addDockWidget(Qt::BottomDockWidgetArea, charSelectDock);
    m_dockWidgets.append(charSelectDock);
    actionCollection()->addAction("show_pronunciation_dock", charSelectDock->toggleViewAction());
    charSelectDock->setVisible(false);
    connect(charSelectWidget, SIGNAL(charSelected(const QChar &)), m_vocabularyView, SLOT(appendChar(const QChar &)));

// Image
    QDockWidget *imageDock = new QDockWidget(i18n("Image"), this);
    imageDock->setObjectName("ImageDock");
    ImageChooserWidget *imageChooserWidget = new ImageChooserWidget(this);
    QScrollArea *imageScrollArea = new QScrollArea(this);
    imageScrollArea->setWidgetResizable(true);
    imageScrollArea->setWidget(imageChooserWidget);
    imageDock->setWidget(imageScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, imageDock);
    m_dockWidgets.append(imageDock);
    actionCollection()->addAction("show_image_dock", imageDock->toggleViewAction());
    imageDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            imageChooserWidget, SLOT(setTranslation(KEduVocExpression*, int)));
    tabifyDockWidget(multipleChoiceDock,imageDock);

// Summary word
    QDockWidget *summaryDock = new QDockWidget(i18n("Summary"), this);
    summaryDock->setObjectName("SummaryDock");
    m_summaryWordWidget = new SummaryWordWidget(m_vocabularyFilter, m_mainWindow->parleyDocument()->document(), this);
    QScrollArea *summaryScrollArea = new QScrollArea(this);
    summaryScrollArea->setWidgetResizable(true);
    summaryScrollArea->setWidget(m_summaryWordWidget);
    summaryDock->setWidget(summaryScrollArea);
    addDockWidget(Qt::BottomDockWidgetArea, summaryDock);
    actionCollection()->addAction("show_summary_dock", summaryDock->toggleViewAction());
    summaryDock->setVisible(false);
    m_dockWidgets.append(summaryDock);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_summaryWordWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Sound
    QDockWidget *audioDock = new QDockWidget(i18n("Sound"), this);
    audioDock->setObjectName("AudioDock");
    AudioWidget *audioWidget = new AudioWidget(this);
    QScrollArea *audioScrollArea = new QScrollArea(this);
    audioScrollArea->setWidgetResizable(true);
    audioScrollArea->setWidget(audioWidget);
    audioDock->setWidget(audioScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, audioDock);
    m_dockWidgets.append(audioDock);
    actionCollection()->addAction("show_audio_dock", audioDock->toggleViewAction());
    audioDock->setVisible(false);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            audioWidget, SLOT(setTranslation(KEduVocExpression*, int)));
    tabifyDockWidget(imageDock,audioDock);

// browser
    QDockWidget *browserDock = new QDockWidget(i18n("Internet"), this);
    browserDock->setObjectName("BrowserDock");
    //TinyWebBrowser *browserWidget = new TinyWebBrowser(this);
    //BrowserWidget *htmlPart = new BrowserWidget(browserDock);
    QScrollArea *browserScrollArea = new QScrollArea(this);
    browserScrollArea->setWidgetResizable(true);
    //browserScrollArea->setWidget(htmlPart);
    browserDock->setWidget(browserScrollArea);
    addDockWidget(Qt::BottomDockWidgetArea, browserDock);
    m_dockWidgets.append(browserDock);
    actionCollection()->addAction("show_browser_dock", browserDock->toggleViewAction());
    browserDock->setVisible(false);
    //    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
    //            htmlPart, SLOT(setTranslation(KEduVocExpression*, int)));
    tabifyDockWidget(summaryDock,browserDock);

// LaTeX
    QDockWidget *latexDock = new QDockWidget(i18n("LaTeX"), this);
    latexDock->setObjectName("LatexDock");
    m_latexWidget = new LatexWidget(m_vocabularyFilter, m_mainWindow->parleyDocument()->document(), this);
    QScrollArea *latexScrollArea = new QScrollArea(this);
    latexScrollArea->setWidgetResizable(true);
    latexScrollArea->setWidget(m_latexWidget);
    latexDock->setWidget(latexScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, latexDock);
    actionCollection()->addAction("show_latex_dock", latexDock->toggleViewAction());
    latexDock->setVisible(false);
    m_dockWidgets.append(latexDock);
    connect(m_vocabularyView, SIGNAL(translationChanged(KEduVocExpression*, int)),
            m_latexWidget, SLOT(setTranslation(KEduVocExpression*, int)));
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
    m_spellCheckMapper = new QSignalMapper(this);
    connect(m_spellCheckMapper, SIGNAL(mapped(int)), m_vocabularyView, SLOT(checkSpelling(int)));

    ParleyActions::create(ParleyActions::ToggleShowSublessons, m_vocabularyModel, SLOT(showEntriesOfSubcontainers(bool)), actionCollection());
    ParleyActions::create(ParleyActions::AutomaticTranslation, m_vocabularyModel, SLOT(Prefs::setAutomaticTranslation(bool)), actionCollection());
    ParleyActions::create(ParleyActions::StartPractice, m_mainWindow, SLOT(showPracticeConfiguration()), actionCollection());
    actionCollection()->action("practice_start")->setText(i18n("Practice"));
    actionCollection()->action("practice_start")->setToolTip(i18n("Practice"));
    ParleyActions::create(ParleyActions::ConfigurePractice, m_mainWindow, SLOT(configurePractice()), actionCollection());
    ParleyActions::create(ParleyActions::ToggleSearchBar, this, SLOT(slotConfigShowSearch()), actionCollection());
    ParleyActions::create(ParleyActions::SearchVocabulary, this, SLOT(startSearch()), actionCollection());
    ParleyActions::create(ParleyActions::ShowScriptManager, this, SLOT(slotShowScriptManager()), actionCollection());
    ParleyActions::create(ParleyActions::LanguagesProperties, m_mainWindow->parleyDocument(), SLOT(languageProperties()), actionCollection());
    ParleyActions::createUploadAction(m_mainWindow->parleyDocument(), SLOT(uploadFile()), actionCollection());

    new EditorWindowAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/AddWithTranslation", this);
}

void EditorWindow::addWordWithTranslation(const QStringList &w)
{
    KEduVocExpression *kexpr = new KEduVocExpression(w);

    m_vocabularyModel->appendEntry(kexpr);
}

void EditorWindow::initModel()
{
    m_vocabularyModel = new KEduVocVocabularyModel(Prefs::showSublessonentries(), this);
    m_vocabularyFilter = new VocabularyFilter(this);
    m_vocabularyFilter->setSourceModel(m_vocabularyModel);
    m_vocabularyView->setFilter(m_vocabularyFilter);

//    connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyModel, SLOT(setDocument(KEduVocDocument*)));
//    connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyView, SLOT(setDocument(KEduVocDocument*)));
    connect(m_searchLine, SIGNAL(textChanged(const QString&)), m_vocabularyFilter, SLOT(setSearchString(const QString&)));
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
    layout->setMargin(0);
    layout->addWidget(label);
    layout->addWidget(m_searchLine);

///@todo     centralWidget()-> delete layout
    QVBoxLayout * rightLayout = new QVBoxLayout();
    rightLayout->setMargin(0);
    rightLayout->addWidget(m_searchWidget);
    m_searchWidget->setVisible(Prefs::showSearch());

    m_vocabularyView = new VocabularyView(this);
    rightLayout->addWidget(m_vocabularyView, 1, 0);

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
