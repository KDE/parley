/***************************************************************************

                   initialisation part of kvoctrain

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <kactioncollection.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <krecentfilesaction.h>
#include <kselectaction.h>
#include <kstatusbar.h>
#include <kstandardaction.h>
#include <kmenu.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kicon.h>

#include "kvoctrain.h"
#include "common-dialogs/ProgressDlg.h"
#include "prefs.h"

KVocTrainApp::KVocTrainApp(QWidget *parent) : KMainWindow(parent)
{
  m_doc = 0;
  m_tableView = 0;
  m_tableModel = 0;
  m_lessonModel = 0;
  m_sortFilterModel = 0;
  btimer = 0;
  querymode = false;
  controlActive = false;
  m_currentLesson = 0;
  searchpos = 0;
  vslide_label = 0;
  pron_label = 0;
  rem_label = 0;
  type_label = 0;
  pdlg = 0;
  pbar = 0;
  m_newStuff = 0;

  simpleQueryDlg = 0;
  mcQueryDlg = 0;
  verbQueryDlg = 0;
  randomQueryDlg = 0;
  adjQueryDlg = 0;
  artQueryDlg = 0;
  entryDlg = 0;
  learningMapper = 0;

  initStatusBar();
  initActions();

  readOptions();

  initModel();
  initDoc();
  initView();

  editDelete->setEnabled(m_tableModel->rowCount(QModelIndex()) > 0);

  querying = false;
  btimer = new QTimer(this);
  btimer->setSingleShot(true);
  connect(btimer, SIGNAL(timeout()), this, SLOT(slotTimeOutBackup()));
  if (Prefs::autoBackup())
    btimer->start(Prefs::backupTime() * 60 * 1000);
}


void KVocTrainApp::initActions()
{
  fileNew = KStandardAction::openNew(this, SLOT(slotFileNew()), actionCollection());
  fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
  fileNew->setToolTip(fileNew->whatsThis());
  fileNew->setStatusTip(fileNew->whatsThis());

  fileOpen = KStandardAction::open(this, SLOT(slotFileOpen()), actionCollection());
  fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
  fileOpen->setToolTip(fileOpen->whatsThis());
  fileOpen->setStatusTip(fileOpen->whatsThis());

  fileOpenExample = actionCollection()->addAction("file_open_example");
  fileOpenExample->setIcon(KIcon("fileopen"));
  fileOpenExample->setText(i18n("Open &Example..."));
  connect(fileOpenExample, SIGNAL(triggered(bool)), this, SLOT(slotFileOpenExample()));
  fileOpenExample->setWhatsThis(i18n("Open an example vocabulary document"));
  fileOpenExample->setToolTip(fileOpen->whatsThis());
  fileOpenExample->setStatusTip(fileOpen->whatsThis());

  fileGHNS = actionCollection()->addAction("file_ghns");
  fileGHNS->setIcon(KIcon("knewstuff"));
  fileGHNS->setText(i18n("&Get New Vocabularies..."));
  connect(fileGHNS, SIGNAL(triggered(bool)), this, SLOT(slotGHNS()));
  fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
  fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
  fileGHNS->setToolTip(fileGHNS->whatsThis());
  fileGHNS->setStatusTip(fileGHNS->whatsThis());

  fileOpenRecent = KStandardAction::openRecent(this, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());

  fileMerge = actionCollection()->addAction("file_merge");
  fileMerge->setText(i18n("&Merge..."));
  connect(fileMerge, SIGNAL(triggered(bool)), this, SLOT(slotFileMerge()));
  fileMerge->setWhatsThis(i18n("Merge an existing vocabulary document with the current one"));
  fileMerge->setToolTip(fileOpen->whatsThis());
  fileMerge->setStatusTip(fileOpen->whatsThis());

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

  editSearchFromClipboard =  KStandardAction::find(this, SLOT(slotSmartSearchClip()), actionCollection());
  editSearchFromClipboard->setWhatsThis(i18n("Search for the clipboard contents in the vocabulary"));
  editSearchFromClipboard->setToolTip(editSearchFromClipboard->whatsThis());
  editSearchFromClipboard->setStatusTip(editSearchFromClipboard->whatsThis());

  editAppend = actionCollection()->addAction("edit_append");
  editAppend->setIcon(KIcon("insert_table_row"));
  editAppend->setText(i18n("&Append New Entry"));
  connect(editAppend, SIGNAL(triggered(bool)), this, SLOT(slotAppendRow()));
  editAppend->setShortcut(QKeySequence(Qt::Key_Insert));
  editAppend->setWhatsThis(i18n("Append a new row to the vocabulary"));
  editAppend->setToolTip(editAppend->whatsThis());
  editAppend->setStatusTip(editAppend->whatsThis());

  editDelete = actionCollection()->addAction("edit_remove_selected_area");
  editDelete->setIcon(KIcon("delete_table_row"));
  editDelete->setText(i18n("&Delete Entry"));
  connect(editDelete, SIGNAL(triggered(bool)), this, SLOT(slotRemoveRow()));
  editDelete->setShortcut(QKeySequence(Qt::Key_Delete));
  editDelete->setWhatsThis(i18n("Delete the selected rows"));
  editDelete->setToolTip(editDelete->whatsThis());
  editDelete->setStatusTip(editDelete->whatsThis());

  editEditEntry = actionCollection()->addAction("edit_edit_selected_area");
  editEditEntry->setIcon(KIcon("edit_table_row"));
  editEditEntry->setText(i18n("&Edit Entry..."));
  connect(editEditEntry, SIGNAL(triggered(bool)), this, SLOT(slotEditRow()));
  editEditEntry->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
  editEditEntry->setWhatsThis(i18n("Edit the entries in the selected rows"));
  editEditEntry->setToolTip(editEditEntry->whatsThis());
  editEditEntry->setStatusTip(editEditEntry->whatsThis());

  editSaveSelectedArea = actionCollection()->addAction("edit_save_selected_area");
  editSaveSelectedArea->setIcon(KIcon("filesaveas"));
  editSaveSelectedArea->setText(i18n("Save E&ntries in Query As..."));
  connect(editSaveSelectedArea, SIGNAL(triggered(bool)), this, SLOT(slotSaveSelection()));
  editSaveSelectedArea->setWhatsThis(i18n("Save the entries in the query as a new vocabulary"));
  editSaveSelectedArea->setToolTip(editSaveSelectedArea->whatsThis());
  editSaveSelectedArea->setStatusTip(editSaveSelectedArea->whatsThis());

  vocabShowStatistics = actionCollection()->addAction("vocab_show_statistics");
  vocabShowStatistics->setIcon(KIcon("statistics"));
  vocabShowStatistics->setText(i18n("&Statistics..."));
  connect(vocabShowStatistics, SIGNAL(triggered(bool)), this, SLOT(slotShowStatistics()));
  vocabShowStatistics->setWhatsThis(i18n("Show and reset statistics for the current vocabulary"));
  vocabShowStatistics->setToolTip(vocabShowStatistics->whatsThis());
  vocabShowStatistics->setStatusTip(vocabShowStatistics->whatsThis());

  vocabAssignLessons = actionCollection()->addAction("vocab_assign_lessons");
  vocabAssignLessons->setIcon(KIcon("rand_less"));
  vocabAssignLessons->setText(i18n("Assign L&essons..."));
  connect(vocabAssignLessons, SIGNAL(triggered(bool)), this, SLOT(slotCreateRandom()));
  vocabAssignLessons->setWhatsThis(i18n("Create random lessons for unassigned entries"));
  vocabAssignLessons->setToolTip(vocabAssignLessons->whatsThis());
  vocabAssignLessons->setStatusTip(vocabAssignLessons->whatsThis());

  vocabCleanUp = actionCollection()->addAction("vocab_clean_up");
  vocabCleanUp->setIcon(KIcon("cleanup"));
  vocabCleanUp->setText(i18n("Remove &Duplicates"));
  connect(vocabCleanUp, SIGNAL(triggered(bool)), this, SLOT(slotCleanVocabulary()));
  vocabCleanUp->setWhatsThis(i18n("Remove duplicate entries from the vocabulary"));
  vocabCleanUp->setToolTip(vocabCleanUp->whatsThis());
  vocabCleanUp->setStatusTip(vocabCleanUp->whatsThis());

  vocabAppendLanguage = actionCollection()->add<KSelectAction>("vocab_append_language");
  vocabAppendLanguage->setIcon(KIcon("insert_table_col"));
  vocabAppendLanguage->setText(i18n("&Append Language"));
  connect(vocabAppendLanguage->menu(), SIGNAL(aboutToShow()),  this, SLOT(aboutToShowVocabAppendLanguage()));
  connect(vocabAppendLanguage,         SIGNAL(triggered(int)), this, SLOT(slotAppendLanguage(int)));

  vocabSetLanguage = actionCollection()->add<KSelectAction>("vocab_set_language");
  vocabSetLanguage->setIcon(KIcon("set_language"));
  vocabSetLanguage->setText(i18n("Assign &Language"));
  connect(vocabSetLanguage->menu(),    SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabSetLanguage()));

  vocabRemoveLanguage = actionCollection()->add<KSelectAction>("vocab_remove_language");
  vocabRemoveLanguage->setIcon(KIcon("delete_table_col"));
  vocabRemoveLanguage->setText(i18n("&Remove Language"));
  connect(vocabRemoveLanguage->menu(), SIGNAL(aboutToShow()),  this, SLOT(aboutToShowVocabRemoveLanguage()));
  connect(vocabRemoveLanguage,         SIGNAL(triggered(int)), this, SLOT(slotRemoveLanguage(int)));

  vocabDocumentProperties = actionCollection()->addAction("vocab_document_properties");
  vocabDocumentProperties->setText(i18n("&Properties..."));
  connect(vocabDocumentProperties, SIGNAL(triggered(bool)), SLOT(slotDocumentProperties()));
  vocabDocumentProperties->setWhatsThis(i18n("Edit vocabulary properties"));
  vocabDocumentProperties->setToolTip(vocabAppendLanguage->whatsThis());
  vocabDocumentProperties->setStatusTip(vocabAppendLanguage->whatsThis());

  vocabLanguageProperties = actionCollection()->addAction("vocab_language_properties");
  vocabLanguageProperties->setText(i18n("Lan&guage Properties..."));
  connect(vocabLanguageProperties, SIGNAL(triggered(bool)), this, SLOT(slotDocPropsLang()));
  vocabLanguageProperties->setWhatsThis(i18n("Edit language properties in current document"));
  vocabLanguageProperties->setToolTip(vocabLanguageProperties->whatsThis());
  vocabLanguageProperties->setStatusTip(vocabLanguageProperties->whatsThis());

  m_lessonsComboBox = new KComboBox(this);
  m_lessonsComboBox->setMinimumWidth(160);
  connect(m_lessonsComboBox, SIGNAL(highlighted(int)), this, SLOT(slotChooseLesson(int)));
  m_lessonsComboBox->setFocusPolicy(Qt::NoFocus);

  vocabLessons = actionCollection()->addAction("vocab_lessons");
  vocabLessons->setText(i18n("Lessons"));
  qobject_cast<KAction*>(vocabLessons)->setDefaultWidget(m_lessonsComboBox);
  vocabLessons->setWhatsThis(i18n("Choose current lesson"));
  vocabLessons->setToolTip(vocabLessons->whatsThis());
  vocabLessons->setStatusTip(vocabLessons->whatsThis());

  searchLine = new KLineEdit(this);
  searchLine->setFocusPolicy(Qt::ClickFocus);
  connect(searchLine, SIGNAL(returnPressed()), this, SLOT(slotSearchNext()));
  connect(searchLine, SIGNAL(textChanged(const QString&)), this, SLOT(slotResumeSearch(const QString&)));

  vocabSearch = actionCollection()->addAction("vocab_search");
  vocabSearch->setText(i18n("Smart Search"));
  qobject_cast<KAction*>(vocabSearch)->setDefaultWidget(searchLine);
  vocabSearch->setWhatsThis(i18n("Search vocabulary for specified text "));
  vocabSearch->setToolTip(vocabSearch->whatsThis());
  vocabSearch->setStatusTip(vocabSearch->whatsThis());

  learningResumeQuery = actionCollection()->addAction("learning_resume");
  learningResumeQuery->setText(i18n("Resume &Query"));
  learningResumeQuery->setIcon(KIcon("run_query"));
  connect(learningResumeQuery, SIGNAL(triggered(bool)), this, SLOT(slotResumeQuery()));
  learningResumeQuery->setWhatsThis(i18n("Resumes random query with existing selection"));
  learningResumeQuery->setToolTip(learningResumeQuery->whatsThis());
  learningResumeQuery->setStatusTip(learningResumeQuery->whatsThis());

  learningResumeMultipleChoice = actionCollection()->addAction("learning_resume_mc");
  learningResumeMultipleChoice->setText(i18n("Resume &Multiple Choice"));
  learningResumeMultipleChoice->setIcon(KIcon("run_multi"));
  connect(learningResumeMultipleChoice, SIGNAL(triggered(bool)), this, SLOT(slotResumeQueryMC()));
  learningResumeMultipleChoice->setWhatsThis(i18n("Resumes multiple choice with existing selection"));
  learningResumeMultipleChoice->setToolTip(learningResumeMultipleChoice->whatsThis());
  learningResumeMultipleChoice->setStatusTip(learningResumeMultipleChoice->whatsThis());

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
}


void KVocTrainApp::initStatusBar()
{
  type_label = new QLabel(statusBar());
  type_label->setFrameStyle(QFrame::NoFrame);
  statusBar()->addWidget(type_label, 150);

  pron_label = new QLabel(statusBar());
  pron_label->setFrameStyle(QFrame::NoFrame);
  pron_label->setFont(Prefs::iPAFont());
  statusBar()->addWidget(pron_label, 200);

  rem_label = new QLabel(statusBar());
  rem_label->setFrameStyle(QFrame::NoFrame);
  statusBar()->addWidget(rem_label, 150);
}


void KVocTrainApp::initDoc()
{
  if (fileOpenRecent->actions().count() > 0  && fileOpenRecent->action(0)->isEnabled()){
    fileOpenRecent->action(0)->trigger();
  }
  else
    createNewDocument();
}

void KVocTrainApp::initModel()
{
  m_lessonModel = new KVTLessonModel(this);
  m_tableModel = new KVTTableModel(this);
  m_tableModel->setLanguages(m_languages);
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
  boxLayout->addWidget(m_lessonView);
  // Get the lessons form vocab document
  m_lessonModel->setDocument(m_doc);
  // I need to initialize the lessons with the model as well...
  m_lessonView->setModel(m_lessonModel);
  m_lessonView->setToolTip(i18n("Double click to rename a lesson. With the checkboxes you can select which lessons you want to be tested. Only checked lessons [x] will be asked!"));

  // Here the user selects whether he wants all lessons in the table, or the current one or the ones in query
  m_lessonSelectionCombo = new KComboBox();
  boxLayout->addWidget(m_lessonSelectionCombo);
  m_lessonSelectionCombo->insertItem(KVocTrainApp::currentLesson, i18n("Edit current lesson"));
  m_lessonSelectionCombo->insertItem(KVocTrainApp::queryLessons, i18n("Edit lessons in test"));
  m_lessonSelectionCombo->insertItem(KVocTrainApp::allLessons, i18n("Edit all lessons"));
  m_lessonSelectionCombo->setToolTip(i18n("Here you select which lessons you want to edit on the right."));

  m_buttonNewLesson = new QPushButton(i18n("New lesson"), left);
  m_buttonNewLesson->setToolTip(i18n("Click here to create a new lesson. You can then type its name directly in the list."));
  boxLayout->addWidget(m_buttonNewLesson);
  connect(m_buttonNewLesson, SIGNAL(clicked()), m_lessonView, SLOT(slotCreateNewLesson()));

  /// New lesson selected
  connect(m_lessonView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotCurrentLessonChanged(const QModelIndex &, const QModelIndex &)));
  /// Rename lesson (?)
  connect(m_lessonModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotCurrentLessonChanged(const QModelIndex &, const QModelIndex &)));
  /// Combo box - what will be displayed at the left
  connect(m_lessonSelectionCombo, SIGNAL(activated(int)), this, SLOT(slotLessonSelectionComboChanged(int)));
  // is this necessary?
  connect(m_lessonModel, SIGNAL(modelReset()), m_lessonView, SLOT(slotModelReset()));

  //slotCurrentChanged(m_lessonView->currentIndex(), m_lessonView->currentIndex());

  /// @todo does something like this make sense here?
  //connect(m_lessonView, SIGNAL(newCurrentLesson()), this, SLOT(slotChooseLesson(int)));

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
  QSplitter *splitter = new QSplitter(centralWidget());
  topLayout->addWidget(splitter);
  /// List of lessons
  splitter->addWidget(initLessonList(centralWidget()));
  /// Table view
  m_tableView = new KVTTableView(centralWidget());
  m_tableView->setFrameStyle(QFrame::NoFrame);
  splitter->addWidget(m_tableView);
  /// Filter proxy
  m_sortFilterModel= new KVTSortFilterModel(this);
  m_sortFilterModel->setSourceModel(m_tableModel);

  m_tableView->setModel(m_sortFilterModel);
  m_tableView->setColumnWidth(0, qvariant_cast<QSize>(m_tableModel->headerData(0, Qt::Horizontal, Qt::SizeHintRole)).width());
  m_tableView->setColumnWidth(1, qvariant_cast<QSize>(m_tableModel->headerData(1, Qt::Horizontal, Qt::SizeHintRole)).width());
  m_tableView->setColumnWidth(2, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
  m_tableView->setColumnWidth(3, qvariant_cast<QSize>(m_tableModel->headerData(2, Qt::Horizontal, Qt::SizeHintRole)).width());
  m_tableView->horizontalHeader()->setResizeMode(KV_COL_MARK, QHeaderView::Fixed);
  int cc = Prefs::currentCol();
  int cr = Prefs::currentRow();
  if (cc <= KV_COL_LESS)
    cc = KV_COL_ORG;

  m_tableView->setCurrentIndex(m_tableModel->index(cr, cc));
  m_tableView->setSortingEnabled(m_doc->isSortingEnabled());

  setCaption(m_doc->url().fileName(), false);
  connect(m_tableModel, SIGNAL(modelReset()), m_tableView, SLOT(slotModelReset()));
  connect(m_tableView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotCurrentChanged(const QModelIndex &, const QModelIndex &)));
  slotCurrentChanged(m_tableView->currentIndex(), m_tableView->currentIndex());
  m_doc->setModified(false); ///@todo doc being modified at startup is due to resize code. Needs to be improved.

  /// set filter order to get usefull default sorting (ascending within lesson)
  m_sortFilterModel->sort(KV_COL_LESS, Qt::AscendingOrder);


  /// @todo Make the size relation between left and table sensible. Save the size maybe???
  // Well I have no clue how this works !?!? But it is better than the default.
  QList <int> sizes;
  sizes << 10 << 400;
  splitter->setSizes(sizes);
}
