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

#include "vocabulary/vocabularymodel.h"
#include "vocabulary/containerview.h"
#include "vocabulary/lessonview.h"
#include "vocabulary/wordtypeview.h"
#include "vocabulary/containermodel.h"
#include "vocabulary/lessonmodel.h"
#include "vocabulary/wordtypemodel.h"

#include "simpleeditor.h"
#include "tableeditor.h"

#include "multiplechoicewidget.h"
#include "comparisonwidget.h"
#include "conjugationwidget.h"
#include "declensionwidget.h"
#include "imagechooserwidget.h"
#include "audiowidget.h"
#include "browserwidget.h"
#include "synonymwidget.h"
#include "latexwidget.h"

#include "settings/parleyprefs.h"
#include "prefs.h"

#include "scripts/scriptdialog.h"
#include "scripts/translator.h"

#include "parleyactions.h"
#include <KActionCollection>
#include <KAction>
#include <KToggleAction>
#include <KActionMenu>
#include <KCharSelect>

#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QStackedWidget>

#include "modeltest/modeltest.h"

using namespace Editor;

EditorWindow::EditorWindow(ParleyMainWindow* parent)
    :KXmlGuiWindow(parent), m_mainWindow(parent)
{
    // KXmlGui
    setXMLFile("editorui.rc");
    setObjectName("Editor");

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    initView();

    initDockWidgets();
    initActions();
    initScripts();

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);

    connect(parent->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)), this, SLOT(updateDocument(KEduVocDocument*)));
    connect(parent->parleyDocument(), SIGNAL(languagesChanged()), this, SLOT(slotLanguagesChanged()));
    connect(parent->parleyDocument(), SIGNAL(statesNeedSaving()), this, SLOT(saveState()));
    connect(parent, SIGNAL(preferencesChanged()), tableEditor, SLOT(applyPrefs()));
}

EditorWindow::~EditorWindow()
{
    saveState();
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
}

void EditorWindow::updateDocument(KEduVocDocument *doc)
{
    m_vocabularyModel->setDocument(doc);
    simpleEditor->setDocument(doc);
    tableEditor->setDocument(doc);

    m_lessonModel->setDocument(doc);
    m_wordTypeModel->setDocument(doc);

    m_conjugationWidget->setDocument(doc);
    m_comparisonWidget->setDocument(doc);

    if (!m_mainWindow->parleyDocument()->document()) {
        return;
    }

    // expand the root items
    m_lessonView->expandToDepth(0);
    m_wordTypeView->expandToDepth(0);

    connect(m_mainWindow->parleyDocument()->document(), SIGNAL(docModified(bool)), m_mainWindow, SLOT(slotUpdateWindowCaption()));

    // FIXME that latex thingy
//     connect(m_vocabularyView->selectionModel(),
//                 SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
//             m_latexWidget, SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));

    setCaption(m_mainWindow->parleyDocument()->document()->url().fileName(), false);

    m_mainWindow->slotUpdateWindowCaption();
}


void EditorWindow::initDockWidgets()
{
// Lesson dock
    QDockWidget *lessonDockWidget = new QDockWidget(i18n("Lessons"), this);
    lessonDockWidget->setObjectName("LessonDock");
    m_lessonView = new LessonView(this);
    lessonDockWidget->setWidget(m_lessonView);
    addDockWidget(Qt::LeftDockWidgetArea, lessonDockWidget);
    m_dockWidgets.append(lessonDockWidget);
    actionCollection()->addAction("show_lesson_dock", lessonDockWidget->toggleViewAction());

    m_lessonModel = new LessonModel(this);
    m_lessonView->setModel(m_lessonModel);
    m_lessonView->setToolTip(i18n("Right click to add, delete, or rename lessons. \n"
            "With the checkboxes you can select which lessons you want to practice. \n"
            "Only checked lessons [x] will be asked in the tests!"));

    connect(m_lessonView, SIGNAL(selectedLessonChanged(KEduVocLesson*)),
        m_vocabularyModel, SLOT(setLesson(KEduVocLesson*)));

    connect(m_lessonView, SIGNAL(signalShowContainer(KEduVocContainer*)),
        m_vocabularyModel, SLOT(showContainer(KEduVocContainer*)));

    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_lessonView, SLOT(setTranslation(KEduVocExpression*, int)));


// Word types dock
    QDockWidget* wordTypeDockWidget = new QDockWidget(i18n("Word Types"), this);
    wordTypeDockWidget->setObjectName( "WordTypeDock" );
    m_wordTypeView = new WordTypeView(this);
    wordTypeDockWidget->setWidget(m_wordTypeView);
    addDockWidget( Qt::LeftDockWidgetArea, wordTypeDockWidget );
    m_dockWidgets.append(wordTypeDockWidget);

    m_wordTypeModel = new WordTypeModel(this);
    wordTypeDockWidget->setVisible(false);
    actionCollection()->addAction("show_wordtype_dock", wordTypeDockWidget->toggleViewAction());

    m_wordTypeView->setModel(m_wordTypeModel);

    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_wordTypeView, SLOT(setTranslation(KEduVocExpression*, int)));

// Conjugations
    QDockWidget *conjugationDock = new QDockWidget(i18n("Conjugation"), this);
    conjugationDock->setObjectName("ConjugationDock");
    m_conjugationWidget = new ConjugationWidget(this);
    conjugationDock->setWidget(m_conjugationWidget);
    addDockWidget(Qt::RightDockWidgetArea, conjugationDock);
    m_dockWidgets.append(conjugationDock);
    conjugationDock->setVisible(false);
    actionCollection()->addAction("show_conjugation_dock", conjugationDock->toggleViewAction());
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_conjugationWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Declensions
//     QDockWidget *declensionDock = new QDockWidget(i18n("Declension"), this);
//     declensionDock->setObjectName("DeclensionDock");
//     DeclensionWidget *declensionWidget = new DeclensionWidget(this);
//     declensionDock->setWidget(declensionWidget);
//     addDockWidget(Qt::RightDockWidgetArea, declensionDock);
//     m_dockWidgets.append(declensionDock);
//     actionCollection()->addAction("show_declension_dock", declensionDock->toggleViewAction());
//     declensionDock->setVisible(false);
//     connect(m_mainWindow->parleyDocument(), SIGNAL(documentChanged(KEduVocDocument*)),
//             declensionWidget, SLOT(setDocument(KEduVocDocument*)));
//     connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
//             declensionWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Comparison forms
    QDockWidget *comparisonDock = new QDockWidget(i18n("Comparison forms"), this);
    comparisonDock->setObjectName("ComparisonDock");
    m_comparisonWidget = new ComparisonWidget(this);
    comparisonDock->setWidget(m_comparisonWidget);
    addDockWidget(Qt::RightDockWidgetArea, comparisonDock);
    m_dockWidgets.append(comparisonDock);
    actionCollection()->addAction("show_comparison_dock", comparisonDock->toggleViewAction());
    comparisonDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
        m_comparisonWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Multiple choice
    QDockWidget *multipleChoiceDock = new QDockWidget(i18n("Multiple Choice"), this);
    multipleChoiceDock->setObjectName("MultipleChoiceDock");
    MultipleChoiceWidget *multipleChoiceWidget = new MultipleChoiceWidget(this);
    multipleChoiceDock->setWidget(multipleChoiceWidget);
    addDockWidget(Qt::RightDockWidgetArea, multipleChoiceDock);
    m_dockWidgets.append(multipleChoiceDock);
    actionCollection()->addAction("show_multiplechoice_dock", multipleChoiceDock->toggleViewAction());
    multipleChoiceDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
        multipleChoiceWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Synonym (and the same for antonym and false friends)
    QDockWidget *synonymDock = new QDockWidget(i18n("Synonyms"), this);
    synonymDock->setObjectName("SynonymDock");
    SynonymWidget *synonymWidget = new SynonymWidget(SynonymWidget::Synonym, this);
    synonymDock->setWidget(synonymWidget);
    addDockWidget(Qt::RightDockWidgetArea, synonymDock);
    m_dockWidgets.append(synonymDock);
    actionCollection()->addAction("show_synonym_dock", synonymDock->toggleViewAction());
    synonymDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
            synonymWidget, SLOT(setTranslation(KEduVocExpression*, int)));

    QDockWidget *antonymDock = new QDockWidget(i18n("Antonyms"), this);
    antonymDock->setObjectName("AntonymDock");
    SynonymWidget *antonymWidget = new SynonymWidget(SynonymWidget::Antonym, this);
    antonymDock->setWidget(antonymWidget);
    addDockWidget(Qt::RightDockWidgetArea, antonymDock);
    m_dockWidgets.append(antonymDock);
    actionCollection()->addAction("show_antonym_dock", antonymDock->toggleViewAction());
    antonymDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
            antonymWidget, SLOT(setTranslation(KEduVocExpression*, int)));

    QDockWidget *falseFriendDock = new QDockWidget(i18n("False Friends"), this);
    falseFriendDock->setObjectName("FalseFriendDock");
    SynonymWidget *falseFriendWidget = new SynonymWidget(SynonymWidget::FalseFriend, this);
    falseFriendDock->setWidget(falseFriendWidget);
    addDockWidget(Qt::RightDockWidgetArea, falseFriendDock);
    m_dockWidgets.append(falseFriendDock);
    actionCollection()->addAction("show_falsefriend_dock", falseFriendDock->toggleViewAction());
    falseFriendDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
            falseFriendWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Pronunciation symbols - Use KCharSelect
    QDockWidget *charSelectDock = new QDockWidget(i18n("Phonetic Symbols"), this);
    charSelectDock->setObjectName("IPADock");
    KCharSelect *charSelectWidget = new KCharSelect(this, 0, KCharSelect::SearchLine | KCharSelect::BlockCombos | KCharSelect::CharacterTable);
    charSelectWidget->setCurrentChar(0x0250);
    charSelectDock->setWidget(charSelectWidget);
    addDockWidget(Qt::BottomDockWidgetArea, charSelectDock);
    m_dockWidgets.append(charSelectDock);
    actionCollection()->addAction("show_pronunciation_dock", charSelectDock->toggleViewAction());
    charSelectDock->setVisible(false);
    connect(charSelectWidget, SIGNAL(charSelected(const QChar &)), this, SLOT(appendChar(const QChar &)));

// Image
    QDockWidget *imageDock = new QDockWidget(i18n("Image"), this);
    imageDock->setObjectName("ImageDock");
    ImageChooserWidget *imageChooserWidget = new ImageChooserWidget(this);
    imageDock->setWidget(imageChooserWidget);
    addDockWidget(Qt::RightDockWidgetArea, imageDock);
    m_dockWidgets.append(imageDock);
    actionCollection()->addAction("show_image_dock", imageDock->toggleViewAction());
    imageDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
        imageChooserWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// Sound
    QDockWidget *audioDock = new QDockWidget(i18n("Sound"), this);
    audioDock->setObjectName("AudioDock");
    AudioWidget *audioWidget = new AudioWidget(this);
    audioDock->setWidget(audioWidget);
    addDockWidget(Qt::RightDockWidgetArea, audioDock);
    m_dockWidgets.append(audioDock);
    actionCollection()->addAction("show_audio_dock", audioDock->toggleViewAction());
    audioDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
        audioWidget, SLOT(setTranslation(KEduVocExpression*, int)));

// browser
    QDockWidget *browserDock = new QDockWidget(i18n("Internet"), this);
    browserDock->setObjectName("BrowserDock");
    //TinyWebBrowser *browserWidget = new TinyWebBrowser(this);
    BrowserWidget *htmlPart = new BrowserWidget(browserDock);
    browserDock->setWidget(htmlPart);
    addDockWidget(Qt::RightDockWidgetArea, browserDock);
    m_dockWidgets.append(browserDock);
    actionCollection()->addAction("show_browser_dock", browserDock->toggleViewAction());
    browserDock->setVisible(false);
    connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
            htmlPart, SLOT(setTranslation(KEduVocExpression*, int)));

// LaTeX
//     QDockWidget *latexDock = new QDockWidget(i18n("LaTeX"), this);
//     latexDock->setObjectName("LatexDock");
//     m_latexWidget = new LatexWidget(m_vocabularyFilter, m_mainWindow->parleyDocument()->document(), this);
//     latexDock->setWidget(m_latexWidget);
//     addDockWidget(Qt::RightDockWidgetArea, latexDock);
//     actionCollection()->addAction("show_latex_dock", latexDock->toggleViewAction());
//     latexDock->setVisible(false);
//     m_dockWidgets.append(latexDock);
//     connect(this, SIGNAL(translationChanged(KEduVocExpression*, int)),
//             m_latexWidget, SLOT(setTranslation(KEduVocExpression*, int)));

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
    m_toggleTableEditorAction = ParleyActions::create(ParleyActions::TableEditor, this, SLOT(toggleTableEditor(bool)), actionCollection());
    ParleyActions::create(ParleyActions::RemoveGrades, this, SLOT(removeGrades()), actionCollection());
//FIXME
    //    ParleyActions::create(ParleyActions::CheckSpelling, m_vocabularyView, SLOT(checkSpelling()), actionCollection());
    ParleyActions::create(ParleyActions::ToggleShowSublessons, m_vocabularyModel, SLOT(showEntriesOfSubcontainers(bool)), actionCollection());
    ParleyActions::create(ParleyActions::AutomaticTranslation, m_vocabularyModel, SLOT(automaticTranslation(bool)), actionCollection());
    ParleyActions::create(ParleyActions::StartPractice, m_mainWindow, SLOT(showPracticeConfiguration()), actionCollection());
    actionCollection()->action("practice_start")->setText(i18n("Practice"));
    actionCollection()->action("practice_start")->setToolTip(i18n("Practice"));
    ParleyActions::create(ParleyActions::ConfigurePractice, m_mainWindow, SLOT(configurePractice()), actionCollection());
    ParleyActions::create(ParleyActions::SearchVocabulary, this, SLOT(startSearch()), actionCollection());
    ParleyActions::create(ParleyActions::ShowScriptManager, this, SLOT(slotShowScriptManager()), actionCollection());
    ParleyActions::create(ParleyActions::LanguagesProperties, m_mainWindow->parleyDocument(), SLOT(languageProperties()), actionCollection());
    ParleyActions::createUploadAction(m_mainWindow->parleyDocument(), SLOT(uploadFile()), actionCollection());
    
    KAction* m_appendEntryAction;
    KAction* m_deleteEntriesAction;
    KAction* m_copyAction;
    KAction* m_cutAction;
    KAction* m_pasteAction;
    KAction* m_selectAllAction;
    KAction* m_clearSelectionAction;
    
    m_appendEntryAction = new KAction(this);
    actionCollection()->addAction("edit_append", m_appendEntryAction);
    m_appendEntryAction->setIcon(KIcon("list-add-card"));
    m_appendEntryAction->setText(i18n("&Add New Entry"));
    connect(m_appendEntryAction, SIGNAL(triggered(bool)), SLOT(appendEntry()));
    m_appendEntryAction->setShortcut(QKeySequence(Qt::Key_Insert));
    m_appendEntryAction->setWhatsThis(i18n("Append a new row to the vocabulary"));
    m_appendEntryAction->setToolTip(m_appendEntryAction->whatsThis());
    m_appendEntryAction->setStatusTip(m_appendEntryAction->whatsThis());
    addAction(m_appendEntryAction);
    
    m_deleteEntriesAction = new KAction(this);
    actionCollection()->addAction("edit_remove_selected_area", m_deleteEntriesAction);
    m_deleteEntriesAction->setIcon(KIcon("list-remove-card"));
    m_deleteEntriesAction->setText(i18n("&Delete Entry"));
    connect(m_deleteEntriesAction, SIGNAL(triggered(bool)), this, SLOT(deleteSelectedEntries()));
    m_deleteEntriesAction->setShortcut(QKeySequence(Qt::Key_Delete));
    m_deleteEntriesAction->setWhatsThis(i18n("Delete the selected rows"));
    m_deleteEntriesAction->setToolTip(m_deleteEntriesAction->whatsThis());
    m_deleteEntriesAction->setStatusTip(m_deleteEntriesAction->whatsThis());
    addAction(m_deleteEntriesAction);
    
    QAction* separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    
    m_copyAction = KStandardAction::copy(this, SLOT(slotEditCopy()), actionCollection());
    m_copyAction->setWhatsThis(i18n("Copy"));
    m_copyAction->setToolTip(m_copyAction->whatsThis());
    m_copyAction->setStatusTip(m_copyAction->whatsThis());
    addAction(m_copyAction);
    
    m_cutAction = KStandardAction::cut(this, SLOT(slotCutEntry()), actionCollection());
    m_cutAction->setWhatsThis(i18n("Cut"));
    m_cutAction->setToolTip(m_cutAction->whatsThis());
    m_cutAction->setStatusTip(m_cutAction->whatsThis());
    addAction(m_cutAction);
    
    m_pasteAction = KStandardAction::paste(this, SLOT(slotEditPaste()), actionCollection());
    m_pasteAction->setWhatsThis(i18n("Paste"));
    m_pasteAction->setToolTip(m_pasteAction->whatsThis());
    m_pasteAction->setStatusTip(m_pasteAction->whatsThis());
    addAction(m_pasteAction);
    
    m_selectAllAction = KStandardAction::selectAll(this, SLOT(selectAll()), actionCollection());
    m_selectAllAction->setWhatsThis(i18n("Select all rows"));
    m_selectAllAction->setToolTip(m_selectAllAction->whatsThis());
    m_selectAllAction->setStatusTip(m_selectAllAction->whatsThis());
    
    m_clearSelectionAction = KStandardAction::deselect(this, SLOT(clearSelection()), actionCollection());
    m_clearSelectionAction->setWhatsThis(i18n("Deselect all rows"));
    m_clearSelectionAction->setToolTip(m_clearSelectionAction->whatsThis());
    m_clearSelectionAction->setStatusTip(m_clearSelectionAction->whatsThis());
    
    // vocabulary columns dialog
    m_vocabularyColumnsDialogAction = new KAction(this);
    actionCollection()->addAction("show_vocabulary_columns_dialog", m_vocabularyColumnsDialogAction);
    m_vocabularyColumnsDialogAction->setIcon(KIcon("view-file-columns"));
    m_vocabularyColumnsDialogAction->setText(i18n("Vocabulary Columns..."));
    m_vocabularyColumnsDialogAction->setWhatsThis(i18n("Toggle display of individual vocabulary columns"));
    m_vocabularyColumnsDialogAction->setToolTip(m_vocabularyColumnsDialogAction->whatsThis());
    m_vocabularyColumnsDialogAction->setStatusTip(m_vocabularyColumnsDialogAction->whatsThis());
    tableEditor->addActionToHeader(m_vocabularyColumnsDialogAction);
    addAction(m_vocabularyColumnsDialogAction);
    connect(m_vocabularyColumnsDialogAction, SIGNAL(triggered(bool)), tableEditor, SLOT(slotShowVocabularyColumnsDialog()));
}

/**
 * This initializes the main widgets and table.
 */
void EditorWindow::initView()
{
    m_vocabularyModel = new VocabularyModel(this);
    tableEditor = new TableEditor(m_vocabularyModel, this);
    simpleEditor = new SimpleEditor(m_vocabularyModel, this);
    
    connect(tableEditor, SIGNAL(currentChanged(QModelIndex)),
            SLOT(setCurrentIndex(QModelIndex)));
    connect(simpleEditor, SIGNAL(translationChanged(KEduVocExpression*,int)),
            SIGNAL(translationChanged(KEduVocExpression*,int)));

    connect(this, SIGNAL(currentChanged(QModelIndex)),
        simpleEditor, SLOT(setCurrentIndex(QModelIndex)));

    editorStack = new QStackedWidget(this);
    setCentralWidget(editorStack);

    editorStack->addWidget(tableEditor);
    editorStack->addWidget(simpleEditor);
    editorStack->setCurrentIndex(Prefs::editorTable()?0:1);
}

void EditorWindow::setCurrentIndex(const QModelIndex& index)
{
    emit currentChanged(index);
    
    if (!index.isValid()) {
        emit translationChanged(0, 0);
    }
    KEduVocExpression* entry;
    entry = m_vocabularyModel->data(index, VocabularyModel::EntryRole).value<KEduVocExpression*>();
    emit translationChanged(entry, VocabularyModel::translation(index.column()));
}

void EditorWindow::toggleTableEditor(bool table)
{
    m_vocabularyColumnsDialogAction->setVisible(table);
    editorStack->setCurrentIndex(table?0:1);
    Prefs::setEditorTable(table);

    // FIXME hide/show actions here if they are table specific
}

void EditorWindow::startSearch()
{
    m_toggleTableEditorAction->setChecked(true);
    toggleTableEditor(true);
    tableEditor->focusSearch();
}

void EditorWindow::slotShowScriptManager() {
    ScriptDialog * dialog = new ScriptDialog(m_scriptManager);
    dialog->show();
}

void EditorWindow::removeGrades()
{
    m_mainWindow->parleyDocument()->document()->lesson()->resetGrades(-1, KEduVocContainer::Recursive);
}

void EditorWindow::initScripts()
{
    // FIXME
//     m_scriptManager = new ScriptManager(this);
//     m_vocabularyView->setTranslator(m_scriptManager->translator());
//     //Load scripts
//     m_scriptManager->loadScripts();
}

void EditorWindow::saveState()
{
    tableEditor->saveState();
}

void EditorWindow::slotLanguagesChanged()
{
    m_vocabularyModel->resetLanguages();
}

void EditorWindow::appendChar(const QChar& newChar)
{
        // FIXME
        qWarning() << "Implement me: EditorWindow::appendChar";
}

QModelIndexList EditorWindow::selectedIndexes() const
{
    return tableEditor->selectedIndexes();
}


#include "editor.moc"
