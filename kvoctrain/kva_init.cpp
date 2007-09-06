/***************************************************************************

                   initialisation part of kvoctrain

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

#include <QClipboard>
#include <QTimer>
#include <QLabel>
#include <QHeaderView>
#include <QSplitter>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QVBoxLayout>

#include <KTabWidget>

#include <kactioncollection.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <krecentfilesaction.h>
#include <kselectaction.h>
#include <kstatusbar.h>
#include <kstandardaction.h>
#include <ktoggleaction.h>
#include <kmenu.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kicon.h>
#include <knewstuff2/ui/knewstuffaction.h>

#include "kvoctrain.h"
#include "prefs.h"

KVocTrainApp::KVocTrainApp(QWidget *parent) : KXmlGuiWindow(parent)
{
    m_doc = 0;
    m_tableView = 0;
    m_tableModel = 0;
    m_lessonModel = 0;
    m_lessonView = 0;
    m_sortFilterModel = 0;
    m_lessonSelectionCombo = 0;
    m_searchLine = 0;
    m_mainSplitter = 0;
    m_searchWidget = 0;
    m_newStuff = 0;
    m_learningMapper = 0;
    m_pronunciationStatusBarLabel = 0;
    m_remarkStatusBarLabel = 0;
    m_typeStatusBarLabel = 0;

//  controlActive = false; ///@todo delete this variable when deleting the old lesson combo
//  m_currentLesson = 0;

    pbar = 0;

    entryDlg = 0;
    m_queryManager = 0;

    initStatusBar();
    initActions();

    readOptions();

    initModel();
    initDoc();
    initView();

    editDelete->setEnabled(m_tableModel->rowCount(QModelIndex()) > 0);

    if (Prefs::autoBackup())
        QTimer::singleShot(Prefs::backupTime() * 60 * 1000, this, SLOT(slotTimeOutBackup()));
}


void KVocTrainApp::initActions()
{
// -- FILE --------------------------------------------------

    fileNew = KStandardAction::openNew(this, SLOT(slotFileNew()), actionCollection());
    fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
    fileNew->setToolTip(fileNew->whatsThis());
    fileNew->setStatusTip(fileNew->whatsThis());

    fileOpen = KStandardAction::open(this, SLOT(slotFileOpen()), actionCollection());
    fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
    fileOpen->setToolTip(fileOpen->whatsThis());
    fileOpen->setStatusTip(fileOpen->whatsThis());

    fileOpenExample = actionCollection()->addAction("file_open_example");
    fileOpenExample->setIcon(KIcon("document-open"));
    fileOpenExample->setText(i18n("Open &Example..."));
    connect(fileOpenExample, SIGNAL(triggered(bool)), this, SLOT(slotFileOpenExample()));
    fileOpenExample->setWhatsThis(i18n("Open an example vocabulary document"));
    fileOpenExample->setToolTip(fileOpenExample->whatsThis());
    fileOpenExample->setStatusTip(fileOpenExample->whatsThis());

    fileGHNS = KNS::standardAction(i18n("Vocabularies..."), this, SLOT(slotGHNS()), actionCollection(), "file_ghns");
    fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
    fileGHNS->setToolTip(fileGHNS->whatsThis());
    fileGHNS->setStatusTip(fileGHNS->whatsThis());

    fileOpenRecent = KStandardAction::openRecent(this, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());

    fileMerge = actionCollection()->addAction("file_merge");
    fileMerge->setText(i18n("&Merge..."));
    connect(fileMerge, SIGNAL(triggered(bool)), this, SLOT(slotFileMerge()));
    fileMerge->setWhatsThis(i18n("Merge an existing vocabulary document with the current one"));
    fileMerge->setToolTip(fileMerge->whatsThis());
    fileMerge->setStatusTip(fileMerge->whatsThis());
    fileMerge->setEnabled(false); ///@todo merging files is horribly broken

    fileSave = KStandardAction::save(this, SLOT(slotFileSave()), actionCollection());
    fileSave->setWhatsThis(i18n("Save the active vocabulary document"));
    fileSave->setToolTip(fileSave->whatsThis());
    fileSave->setStatusTip(fileSave->whatsThis());

    fileSaveAs = KStandardAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
    fileSaveAs->setWhatsThis(i18n("Save the active vocabulary document with a different name"));
    fileSaveAs->setToolTip(fileSaveAs->whatsThis());
    fileSaveAs->setStatusTip(fileSaveAs->whatsThis());

    filePrint = KStandardAction::print(this, SLOT(slotFilePrint()), actionCollection());
    filePrint->setWhatsThis(i18n("Print the active vocabulary document"));
    filePrint->setToolTip(filePrint->whatsThis());
    filePrint->setStatusTip(filePrint->whatsThis());

    fileQuit = KStandardAction::quit(this, SLOT(slotFileQuit()), actionCollection());
    fileQuit->setWhatsThis(i18n("Quit KVocTrain"));
    fileQuit->setToolTip(fileQuit->whatsThis());
    fileQuit->setStatusTip(fileQuit->whatsThis());

// -- EDIT --------------------------------------------------

    editCopy = KStandardAction::copy(this, SLOT(slotEditCopy()), actionCollection());
    editCopy->setWhatsThis(i18n("Copy"));
    editCopy->setToolTip(editCopy->whatsThis());
    editCopy->setStatusTip(editCopy->whatsThis());

    editPaste = KStandardAction::paste(this, SLOT(slotEditPaste()), actionCollection());
    editPaste->setWhatsThis(i18n("Paste"));
    editPaste->setToolTip(editPaste->whatsThis());
    editPaste->setStatusTip(editPaste->whatsThis());

    editSelectAll = KStandardAction::selectAll(this, SLOT(slotSelectAll()), actionCollection());
    editSelectAll->setWhatsThis(i18n("Select all rows"));
    editSelectAll->setToolTip(editSelectAll->whatsThis());
    editSelectAll->setStatusTip(editSelectAll->whatsThis());

    editClearSelection = KStandardAction::deselect(this, SLOT(slotCancelSelection()), actionCollection());
    editClearSelection->setWhatsThis(i18n("Deselect all rows"));
    editClearSelection->setToolTip(editClearSelection->whatsThis());
    editClearSelection->setStatusTip(editClearSelection->whatsThis());

    editAppend = actionCollection()->addAction("edit_append");
    editAppend->setIcon(KIcon("insert_table_row"));
    editAppend->setText(i18n("&Add New Entry"));
    connect(editAppend, SIGNAL(triggered(bool)), this, SLOT(slotNewEntry()));
    editAppend->setShortcut(QKeySequence(Qt::Key_Insert));
    editAppend->setWhatsThis(i18n("Append a new row to the vocabulary"));
    editAppend->setToolTip(editAppend->whatsThis());
    editAppend->setStatusTip(editAppend->whatsThis());

    editDelete = actionCollection()->addAction("edit_remove_selected_area");
    editDelete->setIcon(KIcon("delete_table_row"));
    editDelete->setText(i18n("&Delete Entry"));
    connect(editDelete, SIGNAL(triggered(bool)), this, SLOT(slotDeleteEntry()));
    editDelete->setShortcut(QKeySequence(Qt::Key_Delete));
    editDelete->setWhatsThis(i18n("Delete the selected rows"));
    editDelete->setToolTip(editDelete->whatsThis());
    editDelete->setStatusTip(editDelete->whatsThis());

    editEditEntry = actionCollection()->addAction("edit_edit_selected_area");
    editEditEntry->setIcon(KIcon("edit_table_row"));
    editEditEntry->setText(i18n("&Edit Entry..."));
    connect(editEditEntry, SIGNAL(triggered(bool)), this, SLOT(slotEditEntry()));
    editEditEntry->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
    editEditEntry->setWhatsThis(i18n("Edit the entries in the selected rows"));
    editEditEntry->setToolTip(editEditEntry->whatsThis());
    editEditEntry->setStatusTip(editEditEntry->whatsThis());

    editSaveSelectedArea = actionCollection()->addAction("edit_save_selected_area");
    editSaveSelectedArea->setIcon(KIcon("document-save-as"));
    editSaveSelectedArea->setText(i18n("Save E&ntries in Current Test as..."));
    connect(editSaveSelectedArea, SIGNAL(triggered(bool)), this, SLOT(slotSaveSelection()));
    editSaveSelectedArea->setWhatsThis(i18n("Save the entries in the current test as a new vocabulary"));
    editSaveSelectedArea->setToolTip(editSaveSelectedArea->whatsThis());
    editSaveSelectedArea->setStatusTip(editSaveSelectedArea->whatsThis());

// -- LESSON --------------------------------------------------

    QAction *actionNewLesson = actionCollection()->addAction("new_lesson");
    actionNewLesson->setText(i18n("New Lesson"));
    actionNewLesson->setIcon(KIcon("edit-add"));
    actionNewLesson->setWhatsThis(i18n("Add a new lesson to your document"));
    actionNewLesson->setToolTip(actionNewLesson->whatsThis());
    actionNewLesson->setStatusTip(actionNewLesson->whatsThis());
    actionNewLesson->setStatusTip(actionNewLesson->whatsThis());

    QAction *actionRenameLesson = actionCollection()->addAction("rename_lesson");
    actionRenameLesson->setText(i18n("Rename Lesson"));
    actionRenameLesson->setIcon(KIcon("edit"));
    actionRenameLesson->setWhatsThis(i18n("Rename the selected lesson"));
    actionRenameLesson->setToolTip(actionRenameLesson->whatsThis());
    actionRenameLesson->setStatusTip(actionRenameLesson->whatsThis());
    actionRenameLesson->setStatusTip(actionRenameLesson->whatsThis());

    QAction *actionDeleteLesson = actionCollection()->addAction("delete_lesson");
    actionDeleteLesson->setText(i18n("Delete Lesson"));
    actionDeleteLesson->setIcon(KIcon("edit-delete"));
    actionDeleteLesson->setWhatsThis(i18n("Delete the selected lesson."));
    actionDeleteLesson->setToolTip(actionDeleteLesson->whatsThis());
    actionDeleteLesson->setStatusTip(actionDeleteLesson->whatsThis());
    actionDeleteLesson->setStatusTip(actionDeleteLesson->whatsThis());

    QAction *actionCheckAllLessons = actionCollection()->addAction("check_all_lessons");
    actionCheckAllLessons->setText(i18n("Select All Lessons"));
    actionCheckAllLessons->setIcon(KIcon("edit-add"));  /// @todo better icon
    actionCheckAllLessons->setWhatsThis(i18n("Select all lessons for the test."));
    actionCheckAllLessons->setToolTip(actionCheckAllLessons->whatsThis());
    actionCheckAllLessons->setStatusTip(actionCheckAllLessons->whatsThis());
    actionCheckAllLessons->setStatusTip(actionCheckAllLessons->whatsThis());

    QAction *actionCheckNoLessons = actionCollection()->addAction("check_no_lessons");
    actionCheckNoLessons->setText(i18n("Deselect All Lessons"));
    actionCheckNoLessons->setIcon(KIcon("edit-delete"));  /// @todo better icon
    actionCheckNoLessons->setWhatsThis(i18n("Remove all lessons from the test."));
    actionCheckNoLessons->setToolTip(actionCheckNoLessons->whatsThis());
    actionCheckNoLessons->setStatusTip(actionCheckNoLessons->whatsThis());
    actionCheckNoLessons->setStatusTip(actionCheckNoLessons->whatsThis());

    QAction *actionSplitLesson = actionCollection()->addAction("split_lesson");
    actionSplitLesson->setText(i18n("Split Lesson into Smaller Lessons"));
    actionSplitLesson->setIcon(KIcon("edit-copy"));  /// @todo better icon
    actionSplitLesson->setWhatsThis(i18n("Make multiple smaller lessons out of one big lesson."));
    actionSplitLesson->setToolTip(actionSplitLesson->whatsThis());
    actionSplitLesson->setStatusTip(actionSplitLesson->whatsThis());
    actionSplitLesson->setStatusTip(actionSplitLesson->whatsThis());

// -- VOCABULARY --------------------------------------------------

    vocabShowStatistics = actionCollection()->addAction("vocab_show_statistics");
    vocabShowStatistics->setIcon(KIcon("statistics"));
    vocabShowStatistics->setText(i18n("&Statistics..."));
    connect(vocabShowStatistics, SIGNAL(triggered(bool)), this, SLOT(slotShowStatistics()));
    vocabShowStatistics->setWhatsThis(i18n("Show and reset statistics for the current vocabulary"));
    vocabShowStatistics->setToolTip(vocabShowStatistics->whatsThis());
    vocabShowStatistics->setStatusTip(vocabShowStatistics->whatsThis());

    vocabCleanUp = actionCollection()->addAction("vocab_clean_up");
    vocabCleanUp->setIcon(KIcon("cleanup"));
    vocabCleanUp->setText(i18n("Remove &Duplicates"));
    connect(vocabCleanUp, SIGNAL(triggered(bool)), this, SLOT(slotCleanVocabulary()));
    vocabCleanUp->setWhatsThis(i18n("Remove duplicate entries from the vocabulary"));
    vocabCleanUp->setToolTip(vocabCleanUp->whatsThis());
    vocabCleanUp->setStatusTip(vocabCleanUp->whatsThis());


    vocabEditLanguages = actionCollection()->addAction("vocab_edit_languages");
    vocabEditLanguages->setIcon(KIcon("insert_table_col"));
    vocabEditLanguages->setText(i18n("&Edit Languages"));
    connect(vocabEditLanguages, SIGNAL(triggered()),  this, SLOT(slotEditLanguages()));


/*
    vocabSetLanguage = actionCollection()->add<KSelectAction>("vocab_set_language");
    vocabSetLanguage->setIcon(KIcon("set_language"));
    vocabSetLanguage->setText(i18n("Assign &Language"));
    connect(vocabSetLanguage->menu(),    SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabSetLanguage()));

    vocabRemoveLanguage = actionCollection()->add<KSelectAction>("vocab_remove_language");
    vocabRemoveLanguage->setIcon(KIcon("delete_table_col"));
    vocabRemoveLanguage->setText(i18n("&Remove Language"));
    connect(vocabRemoveLanguage->menu(), SIGNAL(aboutToShow()),  this, SLOT(aboutToShowVocabRemoveLanguage()));
    connect(vocabRemoveLanguage,         SIGNAL(triggered(int)), this, SLOT(slotRemoveLanguage(int)));*/

    vocabDocumentProperties = actionCollection()->addAction("vocab_document_properties");
    vocabDocumentProperties->setText(i18n("&Properties..."));
    connect(vocabDocumentProperties, SIGNAL(triggered(bool)), SLOT(slotDocumentProperties()));
    vocabDocumentProperties->setWhatsThis(i18n("Edit vocabulary properties"));
    vocabDocumentProperties->setToolTip(vocabDocumentProperties->whatsThis());
    vocabDocumentProperties->setStatusTip(vocabDocumentProperties->whatsThis());

    vocabLanguageProperties = actionCollection()->addAction("vocab_language_properties");
    vocabLanguageProperties->setText(i18n("Articles and Personal Pronouns..."));
    connect(vocabLanguageProperties, SIGNAL(triggered(bool)), this, SLOT(slotDocPropsLang()));
    vocabLanguageProperties->setWhatsThis(i18n("Edit language properties in current document"));
    vocabLanguageProperties->setToolTip(vocabLanguageProperties->whatsThis());
    vocabLanguageProperties->setStatusTip(vocabLanguageProperties->whatsThis());

    vocabShowSearchBar = actionCollection()->add<KToggleAction>("config_show_search");
    vocabShowSearchBar->setText(i18n("Show Se&arch"));
    connect(vocabShowSearchBar, SIGNAL(triggered(bool)), this, SLOT(slotConfigShowSearch()));
    vocabShowSearchBar->setWhatsThis(i18n("Toggle display of the search bar"));
    vocabShowSearchBar->setToolTip(vocabShowSearchBar->whatsThis());
    vocabShowSearchBar->setStatusTip(vocabShowSearchBar->whatsThis());

    QAction *actionShowLessonColumn = actionCollection()->addAction("config_show_lesson_column");
    actionShowLessonColumn->setText(i18n("Show Lesson Column"));
    actionShowLessonColumn->setCheckable((true));
    actionShowLessonColumn->setChecked(Prefs::tableLessonColumnVisible());


// -- LEARNING --------------------------------------------------

    learningResumeQuery = actionCollection()->addAction("learning_resume");
    learningResumeQuery->setText(i18n("Resume Written Test"));
    learningResumeQuery->setIcon(KIcon("run_query"));
    connect(learningResumeQuery, SIGNAL(triggered(bool)), this, SLOT(slotResumeQuery()));
    learningResumeQuery->setWhatsThis(i18n("Resumes written test with existing selection"));
    learningResumeQuery->setToolTip(learningResumeQuery->whatsThis());
    learningResumeQuery->setStatusTip(learningResumeQuery->whatsThis());

    learningResumeMultipleChoice = actionCollection()->addAction("learning_resume_mc");
    learningResumeMultipleChoice->setText(i18n("Resume &Multiple Choice"));
    learningResumeMultipleChoice->setIcon(KIcon("run_multi"));
    connect(learningResumeMultipleChoice, SIGNAL(triggered(bool)), this, SLOT(slotResumeQueryMC()));
    learningResumeMultipleChoice->setWhatsThis(i18n("Resumes multiple choice with existing selection"));
    learningResumeMultipleChoice->setToolTip(learningResumeMultipleChoice->whatsThis());
    learningResumeMultipleChoice->setStatusTip(learningResumeMultipleChoice->whatsThis());

// -- SETTINGS --------------------------------------------------

    configApp = KStandardAction::preferences(this, SLOT(slotGeneralOptions()), actionCollection());
    configApp->setWhatsThis(i18n("Show the configuration dialog"));
    configApp->setToolTip(configApp->whatsThis());
    configApp->setStatusTip(configApp->whatsThis());

    if (!initialGeometrySet())
        resize(QSize(550, 400).expandedTo(minimumSizeHint()));
    setupGUI(ToolBar | Keys | StatusBar | Create);
    setAutoSaveSettings();

    configToolbar = actionCollection()->action("options_configure_toolbars");
    configToolbar->setWhatsThis(i18n("Toggle display of the toolbars"));
    configToolbar->setToolTip(configToolbar->whatsThis());
    configToolbar->setStatusTip(configToolbar->whatsThis());

    learningMenu = findChild<KMenu *>("learning");
    connect(learningMenu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowLearn()));

// -- ONLY ON RIGHT CLICK - HEADER SO FAR -------------------------------------
    QAction *actionRestoreNativeOrder = actionCollection()->addAction("restore_native_order");
    actionRestoreNativeOrder->setText(i18n("Restore Native Order"));
}


void KVocTrainApp::initStatusBar()
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


void KVocTrainApp::initDoc()
{
    if (fileOpenRecent->actions().count() > 0
        && fileOpenRecent->action(
            fileOpenRecent->actions().count()-1)->isEnabled() )
    {
        fileOpenRecent->action(fileOpenRecent->actions().count()-1)->trigger();
    } else {
        // this is probably the first time we start.
        m_doc = new KEduVocDocument();

        m_lessonModel->setDocument(m_doc);
        if (m_lessonView) {
            m_lessonView->setModel(m_lessonModel);
            m_lessonView->initializeSelection();
        }
        if (m_tableView) {
            m_tableView->adjustContent();
            m_tableView->setColumnHidden(KV_COL_LESS, !Prefs::tableLessonColumnVisible());
        }

        m_tableModel->setDocument(m_doc);
        initializeDefaultGrammar();
        createExampleEntries();


        connect(m_doc, SIGNAL(docModified(bool)), this, SLOT(slotModifiedDoc(bool)));

        loadDocProps();

        m_tableModel->setDocument(m_doc);
    }
}

void KVocTrainApp::initModel()
{
    m_lessonModel = new KVTLessonModel(this);
    m_tableModel = new KVTTableModel(this);
    m_sortFilterModel= new KVTSortFilterModel(this);
    m_sortFilterModel->setSourceModel(m_tableModel);
}

/**
  * Initialize the lesson list.
  */
QWidget* KVocTrainApp::initLessonList(QWidget *parent)
{
    // Widget to get a boxLayout
    QWidget *left = new QWidget(parent);
    // box layout for the left side
    QVBoxLayout *boxLayout = new QVBoxLayout(left);
    boxLayout->setMargin(0);
    boxLayout->setSpacing(KDialog::spacingHint());

    // This contains the lessons for now
    m_lessonView = new KVTLessonView(left);
    // To make the treeview appear like a listview
    m_lessonView->setRootIsDecorated(false);
    // Get the lessons form vocab document
    m_lessonModel->setDocument(m_doc);
    // I need to initialize the lessons with the model as well...
    m_lessonView->setModel(m_lessonModel);
    m_lessonView->setToolTip(i18n("Right click to add, delete, or rename lessons. \n"
                                  "With the checkboxes you can select which lessons you want to practice. \n"
                                  "Only checked lessons [x] will be asked in the tests!"));

    // Here the user selects whether he wants all lessons in the table, or the current one or the ones in query
    m_lessonSelectionCombo = new KComboBox();
    m_lessonSelectionCombo->addItem(i18n("Edit current lesson"));
    m_lessonSelectionCombo->addItem(i18n("Edit lessons in test"));
    m_lessonSelectionCombo->addItem(i18n("Edit all lessons"));
    m_lessonSelectionCombo->setToolTip(i18n("Select which lessons should be displayed for editing to the right."));

    boxLayout->addWidget(m_lessonSelectionCombo);
    boxLayout->addWidget(m_lessonView);

    /// New lesson selected
    connect(m_lessonView, SIGNAL(signalCurrentLessonChanged(int)), m_sortFilterModel, SLOT(slotCurrentLessonChanged(int)));
    /** this is a little general, but at least we get notified of the changes */
    connect(m_lessonModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), m_sortFilterModel, SLOT(slotLessonsInQueryChanged()));

    connect(m_lessonSelectionCombo, SIGNAL(currentIndexChanged(int)), m_sortFilterModel, SLOT(setLessonSelection(int)));
    connect(m_lessonModel, SIGNAL(modelReset()), m_lessonView, SLOT(slotModelReset()));

    m_lessonSelectionCombo->setCurrentIndex(Prefs::lessonEditingSelection());


    m_lessonView->initializeSelection();

    connect(actionCollection()->action("new_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotCreateNewLesson()));
    connect(actionCollection()->action("rename_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotRenameLesson()));
    connect(actionCollection()->action("delete_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotDeleteLesson()));
    connect(actionCollection()->action("check_all_lessons"), SIGNAL(triggered()), m_lessonView, SLOT(slotCheckAllLessons()));
    connect(actionCollection()->action("check_no_lessons"), SIGNAL(triggered()), m_lessonView, SLOT(slotCheckNoLessons()));
    connect(actionCollection()->action("split_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotSplitLesson()));

    m_lessonView->addAction(actionCollection()->action("new_lesson"));
    m_lessonView->addAction(actionCollection()->action("rename_lesson"));  m_lessonView->addAction(actionCollection()->action("delete_lesson"));
    /// @todo add a separator here
    m_lessonView->addAction(actionCollection()->action("check_all_lessons")); m_lessonView->addAction(actionCollection()->action("check_no_lessons"));
    /// @todo add a separator here
    m_lessonView->addAction(actionCollection()->action("split_lesson"));

    return left;
}


/**
 * This initializes the main widgets, splitter and table.
 */
void KVocTrainApp::initView()
{
    /// Parent of all
    QWidget * mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);
    QVBoxLayout *topLayout = new QVBoxLayout(mainWidget);
    topLayout->setMargin(KDialog::marginHint());
    topLayout->setSpacing(KDialog::spacingHint());
    /// Splitter to divide lessons and table.
    m_mainSplitter = new QSplitter(centralWidget());
    topLayout->addWidget(m_mainSplitter);
    /// List of lessons
    m_mainSplitter->addWidget(initLessonList(centralWidget()));

    m_searchLine = new KLineEdit(this);
    m_searchLine->show();
    m_searchLine->setFocusPolicy(Qt::ClickFocus);
    m_searchLine->setClearButtonShown(true);
    m_searchLine->setClickMessage(i18n("Enter search terms here"));
    connect(m_searchLine, SIGNAL(textChanged(const QString&)), m_sortFilterModel, SLOT(slotSearch(const QString&)));
    m_searchLine->setToolTip(i18n("Enter space-seperated search terms to find words.\n\nEnter ^abc to look for words beginning with \"abc\".\nEnter abc$ to look for words ending with \"abc\".\nEnter type:verb to search for verbs."));

    QLabel *label = new QLabel(i18n("S&earch:"), this);
    label->setBuddy(m_searchLine);
    label->show();

    m_searchWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(m_searchWidget);
    layout->setSpacing(KDialog::spacingHint());
    layout->setMargin(0);
    layout->addWidget(label);
    layout->addWidget(m_searchLine);

    QWidget * rightWidget = new QWidget(this);
    QVBoxLayout * rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setSpacing(KDialog::spacingHint());
    rightLayout->setMargin(0);
    rightLayout->addWidget(m_searchWidget);
    m_searchWidget->setVisible(Prefs::showSearch());
    vocabShowSearchBar->setChecked(Prefs::showSearch());

    /// Table view
    m_tableView = new KVTTableView(centralWidget());
    m_tableView->setFrameStyle(QFrame::NoFrame);
    m_tableView->setAlternatingRowColors(true);
    rightLayout->addWidget(m_tableView, 1, 0);

    m_mainSplitter->addWidget(rightWidget);
    /// Filter proxy

    m_tableView->setModel(m_sortFilterModel);
    m_tableView->setColumnWidth(0, qvariant_cast<QSize>(m_tableModel->headerData(0, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->setColumnWidth(1, qvariant_cast<QSize>(m_tableModel->headerData(1, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->setColumnWidth(2, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->setColumnWidth(3, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
    m_tableView->horizontalHeader()->setResizeMode(KV_COL_MARK, QHeaderView::Fixed);
    int currentColumn = Prefs::currentCol();
    int currentRow = Prefs::currentRow();
    if (currentColumn <= KV_COL_LESS) {
        currentColumn = KV_COL_TRANS;
    }

    // always operate from m_sortFilterModel
    m_tableView->setCurrentIndex(m_sortFilterModel->mapFromSource(m_tableModel->index(currentRow, currentColumn)));

    setCaption(m_doc->url().fileName(), false);

    //connect(m_tableModel, SIGNAL(modelAboutToBeReset()), m_tableView, SLOT(slotModelAboutToBeReset()));

///@todo are these disconnected, once the model is changed? they probably never get reconnected.
    connect(m_tableModel, SIGNAL(modelReset()), m_tableView, SLOT(slotModelReset()));
    connect(m_tableView, SIGNAL(appendEntry()), m_tableModel, SLOT(appendEntry()));

    // selection changes (the entry dialog needs these)
    connect(m_tableView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(slotCurrentChanged(const QModelIndex &, const QModelIndex &)));

    connect(m_tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));


    slotCurrentChanged(m_tableView->currentIndex(), m_tableView->currentIndex());

    m_tableView->addAction(actionCollection()->action("edit_append"));
    m_tableView->addAction(actionCollection()->action("edit_edit_selected_area"));
    m_tableView->addAction(actionCollection()->action("edit_remove_selected_area"));


    m_tableView->setColumnHidden(KV_COL_LESS, !Prefs::tableLessonColumnVisible());
    QAction *actionShowLessonColumn = actionCollection()->action("config_show_lesson_column");
    m_tableView->horizontalHeader()->addAction(actionShowLessonColumn);
    connect(actionShowLessonColumn, SIGNAL(toggled(bool)), m_tableView, SLOT(slotShowLessonColumn(bool)));

    QAction * actionRestoreNativeOrder = actionCollection()->action("restore_native_order");
    m_tableView->horizontalHeader()->addAction(actionRestoreNativeOrder);
    connect(actionRestoreNativeOrder, SIGNAL(triggered()), m_sortFilterModel, SLOT(restoreNativeOrder()));

//     /* Begin tabs... */
//     KTabWidget *tabWidget = new KTabWidget(centralWidget());
//     tabWidget->addTab(rightWidget, "Edit vocabulary");
//
//     QPushButton *button = new QPushButton("Resume query");
//     connect(button, SIGNAL(clicked()), this, SLOT(slotResumeQuery()));
//     tabWidget->addTab(button, "Query");
//
//     m_mainSplitter->addWidget(tabWidget);
//     /* End tabs - comment out these lines to get the nomal behavior. */

///@todo initialize m_sortFilterModel if it is not!!
// m_sortFilterModel->setLessonSelection(Prefs::LessonEditingSelection);
m_sortFilterModel->clear();

    m_mainSplitter->setSizes(Prefs::mainWindowSplitter());
    m_doc->setModified(false);
    m_sortFilterModel->restoreNativeOrder();
}

