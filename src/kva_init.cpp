/***************************************************************************

                   initialisation part of Parley

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "parley.h"
#include "prefs.h"

#include "kvttablemodel.h"
#include "kvtsortfiltermodel.h"
#include "kvttableview.h"
#include "vocabulary/vocabularymodel.h"
#include "vocabulary/vocabularyview.h"
#include "vocabulary/vocabularydelegate.h"
#include "vocabulary/lessonview.h"

#include "entry-dialogs/EntryDlg.h"
#include "entry-dialogs/wordtypewidget.h"

#include <KActionCollection>
#include <KActionMenu>
#include <KLineEdit>
#include <KComboBox>
#include <KRecentFilesAction>
#include <KStatusBar>
#include <KToggleAction>
#include <KLocale>
#include <KDialog>
#include <knewstuff2/ui/knewstuffaction.h>

#include <QClipboard>
#include <QTimer>
#include <QLabel>
#include <QHeaderView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QDockWidget>

void ParleyApp::initActions()
{
// -- FILE --------------------------------------------------
    KAction* fileNew = KStandardAction::openNew(m_document, SLOT(slotFileNew()), actionCollection());
    fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
    fileNew->setToolTip(fileNew->whatsThis());
    fileNew->setStatusTip(fileNew->whatsThis());

    KAction* fileOpen = KStandardAction::open(m_document, SLOT(slotFileOpen()), actionCollection());
    fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
    fileOpen->setToolTip(fileOpen->whatsThis());
    fileOpen->setStatusTip(fileOpen->whatsThis());

    KAction* fileOpenExample = new KAction(this);
    actionCollection()->addAction("file_open_example", fileOpenExample);
    fileOpenExample->setIcon(KIcon("document-open"));
    fileOpenExample->setText(i18n("Open &Example..."));
    connect(fileOpenExample, SIGNAL(triggered(bool)), m_document, SLOT(openExample()));
    fileOpenExample->setWhatsThis(i18n("Open an example vocabulary document"));
    fileOpenExample->setToolTip(fileOpenExample->whatsThis());
    fileOpenExample->setStatusTip(fileOpenExample->whatsThis());

    KAction* fileGHNS = KNS::standardAction(i18n("Vocabularies..."), m_document, SLOT(slotGHNS()), actionCollection(), "file_ghns");
    fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
    fileGHNS->setToolTip(fileGHNS->whatsThis());
    fileGHNS->setStatusTip(fileGHNS->whatsThis());

    m_recentFilesAction = KStandardAction::openRecent(m_document, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());

    KAction* fileMerge = new KAction(this);
    actionCollection()->addAction("file_merge", fileMerge);
    fileMerge->setText(i18n("&Merge..."));
    connect(fileMerge, SIGNAL(triggered(bool)), m_document, SLOT(slotFileMerge()));
    fileMerge->setWhatsThis(i18n("Merge an existing vocabulary document with the current one"));
    fileMerge->setToolTip(fileMerge->whatsThis());
    fileMerge->setStatusTip(fileMerge->whatsThis());
    fileMerge->setEnabled(false); ///@todo merging files is horribly broken

    KAction* fileSave = KStandardAction::save(m_document, SLOT(save()), actionCollection());
    fileSave->setWhatsThis(i18n("Save the active vocabulary document"));
    fileSave->setToolTip(fileSave->whatsThis());
    fileSave->setStatusTip(fileSave->whatsThis());

    KAction* fileSaveAs = KStandardAction::saveAs(m_document, SLOT(saveAs()), actionCollection());
    fileSaveAs->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    fileSaveAs->setWhatsThis(i18n("Save the active vocabulary document with a different name"));
    fileSaveAs->setToolTip(fileSaveAs->whatsThis());
    fileSaveAs->setStatusTip(fileSaveAs->whatsThis());

    KAction* filePrint = KStandardAction::print(m_document, SLOT(printFile()), actionCollection());
    filePrint->setWhatsThis(i18n("Print the active vocabulary document"));
    filePrint->setToolTip(filePrint->whatsThis());
    filePrint->setStatusTip(filePrint->whatsThis());

    KAction* fileProperties = new KAction(this);
    actionCollection()->addAction("file_properties", fileProperties);
    fileProperties->setText(i18n("&Properties..."));
    connect(fileProperties, SIGNAL(triggered(bool)), SLOT(slotDocumentProperties()));
    fileProperties->setIcon(KIcon("document-properties"));
    fileProperties->setWhatsThis(i18n("Edit document properties"));
    fileProperties->setToolTip(fileProperties->whatsThis());
    fileProperties->setStatusTip(fileProperties->whatsThis());


    KAction* fileQuit = KStandardAction::quit(this, SLOT(slotFileQuit()), actionCollection());
    fileQuit->setWhatsThis(i18n("Quit Parley"));
    fileQuit->setToolTip(fileQuit->whatsThis());
    fileQuit->setStatusTip(fileQuit->whatsThis());

// -- EDIT --------------------------------------------------

    KAction* editCopy = KStandardAction::copy(this, SLOT(slotEditCopy()), actionCollection());
    editCopy->setWhatsThis(i18n("Copy"));
    editCopy->setToolTip(editCopy->whatsThis());
    editCopy->setStatusTip(editCopy->whatsThis());

    KAction* editCut = KStandardAction::cut(this, SLOT(slotCutEntry()), actionCollection());
    editCut->setWhatsThis(i18n("Copy"));
    editCut->setToolTip(editCut->whatsThis());
    editCut->setStatusTip(editCut->whatsThis());

    KAction* editPaste = KStandardAction::paste(this, SLOT(slotEditPaste()), actionCollection());
    editPaste->setWhatsThis(i18n("Paste"));
    editPaste->setToolTip(editPaste->whatsThis());
    editPaste->setStatusTip(editPaste->whatsThis());


    KAction* editLanguages =new KAction(this);
    actionCollection()->addAction("edit_languages", editLanguages);
    editLanguages->setIcon(KIcon("preferences-desktop-locale"));
    editLanguages->setText(i18n("&Languages..."));
    connect(editLanguages, SIGNAL(triggered()),  this, SLOT(slotEditLanguages()));
    ///@todo tooltip

    KAction* editGramar = new KAction(this);
    actionCollection()->addAction("edit_grammar", editGramar);
    editGramar->setText(i18n("&Grammar..."));
    editGramar->setWhatsThis(i18n("Edit language properties (types, tenses and usages)."));
    editGramar->setToolTip(editGramar->whatsThis());
    editGramar->setStatusTip(editGramar->whatsThis());
    connect(editGramar, SIGNAL(triggered(bool)), SLOT(slotLanguageProperties()));


    KAction* editSelectAll = KStandardAction::selectAll(this, SLOT(slotSelectAll()), actionCollection());
    editSelectAll->setWhatsThis(i18n("Select all rows"));
    editSelectAll->setToolTip(editSelectAll->whatsThis());
    editSelectAll->setStatusTip(editSelectAll->whatsThis());

    KAction* editClearSelection = KStandardAction::deselect(this, SLOT(slotCancelSelection()), actionCollection());
    editClearSelection->setWhatsThis(i18n("Deselect all rows"));
    editClearSelection->setToolTip(editClearSelection->whatsThis());
    editClearSelection->setStatusTip(editClearSelection->whatsThis());

    KAction* editAppend = new KAction(this);
    actionCollection()->addAction("edit_append", editAppend);
    editAppend->setIcon(KIcon("list-add-card"));
    editAppend->setText(i18n("&Add New Entry"));
    connect(editAppend, SIGNAL(triggered(bool)), this, SLOT(slotNewEntry()));
    editAppend->setShortcut(QKeySequence(Qt::Key_Insert));
    editAppend->setWhatsThis(i18n("Append a new row to the vocabulary"));
    editAppend->setToolTip(editAppend->whatsThis());
    editAppend->setStatusTip(editAppend->whatsThis());

    m_deleteEntriesAction = new KAction(this);
    actionCollection()->addAction("edit_remove_selected_area", m_deleteEntriesAction);
    m_deleteEntriesAction->setIcon(KIcon("list-remove-card"));
    m_deleteEntriesAction->setText(i18n("&Delete Entry"));
    connect(m_deleteEntriesAction, SIGNAL(triggered(bool)), this, SLOT(slotDeleteEntry()));
    m_deleteEntriesAction->setShortcut(QKeySequence(Qt::Key_Delete));
    m_deleteEntriesAction->setWhatsThis(i18n("Delete the selected rows"));
    m_deleteEntriesAction->setToolTip(m_deleteEntriesAction->whatsThis());
    m_deleteEntriesAction->setStatusTip(m_deleteEntriesAction->whatsThis());

    KAction* editEditEntry = new KAction(this);
    actionCollection()->addAction("edit_edit_selected_area", editEditEntry);
    editEditEntry->setIcon(KIcon("document-properties-card"));
    editEditEntry->setText(i18n("&Edit Entry..."));
    connect(editEditEntry, SIGNAL(triggered(bool)), this, SLOT(slotEditEntry()));
    editEditEntry->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
    editEditEntry->setWhatsThis(i18n("Edit the entries in the selected rows"));
    editEditEntry->setToolTip(editEditEntry->whatsThis());
    editEditEntry->setStatusTip(editEditEntry->whatsThis());

//     KAction* editSaveSelectedArea = new KAction(this);
//      actionCollection()->addAction("edit_save_selected_area", editSaveSelectedArea);
//     editSaveSelectedArea->setIcon(KIcon("document-save-as"));
//     editSaveSelectedArea->setText(i18n("Save E&ntries in Current Test as..."));
//     connect(editSaveSelectedArea, SIGNAL(triggered(bool)), this, SLOT(slotSaveSelection()));
//     editSaveSelectedArea->setWhatsThis(i18n("Save the entries in the current test as a new vocabulary"));
//     editSaveSelectedArea->setToolTip(editSaveSelectedArea->whatsThis());
//     editSaveSelectedArea->setStatusTip(editSaveSelectedArea->whatsThis());
//     ///@todo enable when/if the corresponding function is rewritten
//     editSaveSelectedArea->setEnabled(false);

// -- LESSON --------------------------------------------------

    KAction *actionNewLesson = new KAction(this);
    actionCollection()->addAction("new_lesson", actionNewLesson);
    actionNewLesson->setText(i18n("New Lesson"));
    actionNewLesson->setIcon(KIcon("lesson-add"));
    actionNewLesson->setWhatsThis(i18n("Add a new lesson to your document"));
    actionNewLesson->setToolTip(actionNewLesson->whatsThis());
    actionNewLesson->setStatusTip(actionNewLesson->whatsThis());
    actionNewLesson->setStatusTip(actionNewLesson->whatsThis());

    KAction *actionRenameLesson = new KAction(this);
    actionCollection()->addAction("rename_lesson", actionRenameLesson);
    actionRenameLesson->setText(i18n("Rename Lesson"));
    actionRenameLesson->setIcon(KIcon("edit-rename"));
    actionRenameLesson->setWhatsThis(i18n("Rename the selected lesson"));
    actionRenameLesson->setToolTip(actionRenameLesson->whatsThis());
    actionRenameLesson->setStatusTip(actionRenameLesson->whatsThis());
    actionRenameLesson->setStatusTip(actionRenameLesson->whatsThis());

    KAction *actionDeleteLesson = new KAction(this);
    actionCollection()->addAction("delete_lesson", actionDeleteLesson);
    actionDeleteLesson->setText(i18n("Delete Lesson"));
    actionDeleteLesson->setIcon(KIcon("lesson-remove"));
    actionDeleteLesson->setWhatsThis(i18n("Delete the selected lesson."));
    actionDeleteLesson->setToolTip(actionDeleteLesson->whatsThis());
    actionDeleteLesson->setStatusTip(actionDeleteLesson->whatsThis());
    actionDeleteLesson->setStatusTip(actionDeleteLesson->whatsThis());

    KAction *actionCheckAllLessons = new KAction(this);
    actionCollection()->addAction("check_all_lessons", actionCheckAllLessons);
    actionCheckAllLessons->setText(i18n("Select All Lessons"));
    actionCheckAllLessons->setIcon(KIcon("edit-select-all"));
    actionCheckAllLessons->setWhatsThis(i18n("Select all lessons for the test."));
    actionCheckAllLessons->setToolTip(actionCheckAllLessons->whatsThis());
    actionCheckAllLessons->setStatusTip(actionCheckAllLessons->whatsThis());
    actionCheckAllLessons->setStatusTip(actionCheckAllLessons->whatsThis());

    KAction *actionCheckNoLessons = new KAction(this);
    actionCollection()->addAction("check_no_lessons", actionCheckNoLessons);
    actionCheckNoLessons->setText(i18n("Deselect All Lessons"));
    actionCheckNoLessons->setIcon(KIcon("edit-clear"));
    actionCheckNoLessons->setWhatsThis(i18n("Remove all lessons from the test."));
    actionCheckNoLessons->setToolTip(actionCheckNoLessons->whatsThis());
    actionCheckNoLessons->setStatusTip(actionCheckNoLessons->whatsThis());
    actionCheckNoLessons->setStatusTip(actionCheckNoLessons->whatsThis());

    KAction *actionSplitLesson = new KAction(this);
    actionCollection()->addAction("split_lesson", actionSplitLesson);
    actionSplitLesson->setText(i18n("Split Lesson into Smaller Lessons"));
    actionSplitLesson->setIcon(KIcon("edit-copy"));  /// @todo better icon
    actionSplitLesson->setWhatsThis(i18n("Make multiple smaller lessons out of one big lesson."));
    actionSplitLesson->setToolTip(actionSplitLesson->whatsThis());
    actionSplitLesson->setStatusTip(actionSplitLesson->whatsThis());
    actionSplitLesson->setStatusTip(actionSplitLesson->whatsThis());

// -- VOCABULARY --------------------------------------------------

    KAction* vocabCleanUp = new KAction(this);
    actionCollection()->addAction("vocab_clean_up", vocabCleanUp);
    vocabCleanUp->setIcon(KIcon("edit-clear"));
    vocabCleanUp->setText(i18n("Remove &Duplicates"));
    connect(vocabCleanUp, SIGNAL(triggered(bool)), this, SLOT(slotCleanVocabulary()));
    vocabCleanUp->setWhatsThis(i18n("Remove duplicate entries from the vocabulary"));
    vocabCleanUp->setToolTip(vocabCleanUp->whatsThis());
    vocabCleanUp->setStatusTip(vocabCleanUp->whatsThis());

// -- PRACTICE --------------------------------------------------

    KAction* configurePractice = new KAction(this);
    configurePractice->setText(i18n("Configure Practice..."));
    configurePractice->setIcon(KIcon("practice-setup"));
    configurePractice->setWhatsThis(i18n("Set up and start a test"));
    configurePractice->setToolTip(configurePractice->whatsThis());
    configurePractice->setStatusTip(configurePractice->whatsThis());
    actionCollection()->addAction("practice_configure", configurePractice);
    connect(configurePractice, SIGNAL(triggered(bool)), SLOT(configurePractice()));

    KAction* startPractice = new KAction(this);
    startPractice->setText(i18n("Start Practice..."));
    startPractice->setIcon(KIcon("practice-start"));
    startPractice->setWhatsThis(i18n("Start a test with the last settings"));
    startPractice->setToolTip(startPractice->whatsThis());
    startPractice->setStatusTip(startPractice->whatsThis());
    actionCollection()->addAction("practice_start", startPractice);
    connect(startPractice, SIGNAL(triggered(bool)), SLOT(startPractice()));

    KAction* showStatistics = new KAction(this);
    actionCollection()->addAction("show_statistics", showStatistics);
    showStatistics->setIcon(KIcon("view-statistics"));
    showStatistics->setText(i18n("&Statistics..."));
    connect(showStatistics, SIGNAL(triggered(bool)), this, SLOT(slotShowStatistics()));
    showStatistics->setWhatsThis(i18n("Show and reset statistics for the current vocabulary"));
    showStatistics->setToolTip(showStatistics->whatsThis());
    showStatistics->setStatusTip(showStatistics->whatsThis());


// -- SETTINGS --------------------------------------------------

    KAction* configApp = KStandardAction::preferences(this, SLOT(slotGeneralOptions()), actionCollection());
    configApp->setWhatsThis(i18n("Show the configuration dialog"));
    configApp->setToolTip(configApp->whatsThis());
    configApp->setStatusTip(configApp->whatsThis());

    KAction* configToolbar = new KAction(this);
    actionCollection()->addAction("options_configure_toolbars", configToolbar);
    configToolbar->setWhatsThis(i18n("Toggle display of the toolbars"));
    configToolbar->setToolTip(configToolbar->whatsThis());
    configToolbar->setStatusTip(configToolbar->whatsThis());

    m_vocabShowSearchBarAction = actionCollection()->add<KToggleAction>("config_show_search");
    m_vocabShowSearchBarAction->setText(i18n("Show Se&arch"));
    connect(m_vocabShowSearchBarAction, SIGNAL(triggered(bool)), this, SLOT(slotConfigShowSearch()));
    m_vocabShowSearchBarAction->setWhatsThis(i18n("Toggle display of the search bar"));
    m_vocabShowSearchBarAction->setToolTip(m_vocabShowSearchBarAction->whatsThis());
    m_vocabShowSearchBarAction->setStatusTip(m_vocabShowSearchBarAction->whatsThis());

    KAction *actionShowLessonColumn = new KAction(this);
    actionCollection()->addAction("config_show_lesson_column", actionShowLessonColumn);
    actionShowLessonColumn->setText(i18n("Show Lesson Column"));
    actionShowLessonColumn->setCheckable((true));
    actionShowLessonColumn->setChecked(Prefs::tableLessonColumnVisible());

    KAction *actionShowActiveColumn = new KAction(this);
    actionCollection()->addAction("config_show_active_column", actionShowActiveColumn);
    actionShowActiveColumn->setText(i18n("Show Active Entry Column"));
    actionShowActiveColumn->setCheckable((true));
    actionShowActiveColumn->setChecked(Prefs::tableActiveColumnVisible());

    m_vocabularyColumnsActionMenu = new KActionMenu(this);
    actionCollection()->addAction("show_vocabulary_columns_menu", m_vocabularyColumnsActionMenu);
    m_vocabularyColumnsActionMenu->setText(i18n("Vocabulary Columns"));
    m_vocabularyColumnsActionMenu->setWhatsThis(i18n("Toggle display of individual vocabulary columns"));
    m_vocabularyColumnsActionMenu->setToolTip(m_vocabularyColumnsActionMenu->whatsThis());
    m_vocabularyColumnsActionMenu->setStatusTip(m_vocabularyColumnsActionMenu->whatsThis());


// -- ONLY ON RIGHT CLICK - HEADER SO FAR -------------------------------------
    KAction *actionRestoreNativeOrder = new KAction(this);
    actionCollection()->addAction("restore_native_order", actionRestoreNativeOrder);
    actionRestoreNativeOrder->setText(i18n("Restore Native Order"));
}


void ParleyApp::initStatusBar()
{
    m_typeStatusBarLabel = new QLabel(statusBar());
    m_typeStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    statusBar()->addWidget(m_typeStatusBarLabel, 150);

    m_pronunciationStatusBarLabel = new QLabel(statusBar());
    m_pronunciationStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    m_pronunciationStatusBarLabel->setFont(Prefs::iPAFont());
    statusBar()->addWidget(m_pronunciationStatusBarLabel, 200);

    m_remarkStatusBarLabel = new QLabel(statusBar());
    m_remarkStatusBarLabel->setFrameStyle(QFrame::NoFrame);
    statusBar()->addWidget(m_remarkStatusBarLabel, 150);
}


void ParleyApp::initModel()
{
//     m_tableModel = new KVTTableModel(this);
//     m_sortFilterModel= new KVTSortFilterModel(this);
//     m_sortFilterModel->setSourceModel(m_tableModel);
//     m_tableView->setModel(m_sortFilterModel);

//     connect(m_searchLine, SIGNAL(textChanged(const QString&)), m_sortFilterModel, SLOT(slotSearch(const QString&)));

    m_vocabularyModel = new VocabularyModel(this);
    m_vocabularyView->setModel(m_vocabularyModel);

    connect(m_document, SIGNAL(documentChanged(KEduVocDocument*)), m_vocabularyModel, SLOT(setDocument(KEduVocDocument*)));


}


/**
 * This initializes the main widgets and table.
 */
void ParleyApp::initView()
{
    // Parent of all
    QWidget * mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout *topLayout = new QVBoxLayout(mainWidget);
    topLayout->setMargin(KDialog::marginHint());
    topLayout->setSpacing(KDialog::spacingHint());

    m_searchLine = new KLineEdit(this);
    m_searchLine->show();
    m_searchLine->setFocusPolicy(Qt::ClickFocus);
    m_searchLine->setClearButtonShown(true);
    m_searchLine->setClickMessage(i18n("Enter search terms here"));

    m_searchLine->setToolTip(i18n("Enter space-separated search terms to find words.\n\nEnter ^abc to look for words beginning with \"abc\".\nEnter abc$ to look for words ending with \"abc\".\nEnter type:verb to search for verbs."));

    QLabel *label = new QLabel(i18n("S&earch:"), this);
    label->setBuddy(m_searchLine);
    label->show();

    m_searchWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(m_searchWidget);
    layout->setSpacing(KDialog::spacingHint());
    layout->setMargin(0);
    layout->addWidget(label);
    layout->addWidget(m_searchLine);

///@todo     centralWidget()-> delete layout
    QVBoxLayout * rightLayout = new QVBoxLayout(centralWidget());
    rightLayout->setSpacing(KDialog::spacingHint());
    rightLayout->setMargin(0);
    rightLayout->addWidget(m_searchWidget);
    m_searchWidget->setVisible(Prefs::showSearch());
    m_vocabShowSearchBarAction->setChecked(Prefs::showSearch());

    // Table view
//     m_tableView = new KVTTableView(centralWidget());
//     m_tableView->setFrameStyle(QFrame::NoFrame);
//     m_tableView->setAlternatingRowColors(true);
//     rightLayout->addWidget(m_tableView, 1, 0);


    /* the new table */
    m_vocabularyView = new VocabularyView(m_vocabularyColumnsActionMenu, centralWidget());
    m_vocabularyView->setFrameStyle(QFrame::NoFrame);
    m_vocabularyView->setAlternatingRowColors(true);

    VocabularyDelegate *vocabularyDelegate = new VocabularyDelegate;
    m_vocabularyView->setItemDelegate(vocabularyDelegate);

    rightLayout->addWidget(m_vocabularyView, 1, 0);

    /* end the new table */

    topLayout->addLayout(rightLayout);

//     m_tableView->addAction(actionCollection()->action("edit_append"));
//     m_tableView->addAction(actionCollection()->action("edit_edit_selected_area"));
//     m_tableView->addAction(actionCollection()->action("edit_remove_selected_area"));
// 

    m_vocabularyView->horizontalHeader()->addAction(m_vocabularyColumnsActionMenu);


//     QAction *actionShowLessonColumn = actionCollection()->action("config_show_lesson_column");
//     m_tableView->horizontalHeader()->addAction(actionShowLessonColumn);
//     connect(actionShowLessonColumn, SIGNAL(toggled(bool)), m_tableView, SLOT(slotShowLessonColumn(bool)));
// 
//     QAction *actionShowActiveColumn = actionCollection()->action("config_show_active_column");
//     m_tableView->horizontalHeader()->addAction(actionShowActiveColumn);
//     connect(actionShowActiveColumn, SIGNAL(toggled(bool)), m_tableView, SLOT(slotShowActiveColumn(bool)));
}

