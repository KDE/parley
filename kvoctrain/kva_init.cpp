/***************************************************************************

                   initialisation part of kvoctrain

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004-2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <kactioncollection.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <krecentfilesaction.h>
#include <kselectaction.h>
#include <kstatusbar.h>
#include <kstdaction.h>
#include <kmenu.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kicon.h>

#include "kvoctrain.h"
#include "common-dialogs/ProgressDlg.h"
#include "prefs.h"

KVocTrainApp::KVocTrainApp(QWidget *parent, const char *name)
: KMainWindow(parent, name)
{
  doc = 0;
  view = 0;
  header_m = 0;
  btimer = 0;
  querymode = false;
  shiftActive = false;
  altActive = false;
  controlActive = false;
  act_lesson = 0;
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

  initStatusBar();
  initActions();

  readOptions();

  initDoc();
  initView();

  int cc = Prefs::currentCol();
  int cr = Prefs::currentRow();
  if (cc <= KV_COL_LESS)
    cc = KV_COL_LESS+1;

  view->getTable()->updateContents(cr, cc);
  view->getTable()->clearSelection();
  view->getTable()->selectRow(cr);

  editRemoveSelectedArea->setEnabled(view->getTable()->numRows() > 0);

  querying = false;
  btimer = new QTimer( this );
  connect( btimer, SIGNAL(timeout()), this, SLOT(slotTimeOutBackup()) );
  if (Prefs::autoBackup())
    btimer->start(Prefs::backupTime() * 60 * 1000, true);
}


void KVocTrainApp::initActions()
{
  fileNew = KStdAction::openNew(this, SLOT(slotFileNew()), actionCollection());
  fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
  fileNew->setToolTip(fileNew->whatsThis());

  fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
  fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
  fileOpen->setToolTip(fileOpen->whatsThis());

  fileOpenExample = new KAction(KIcon("fileopen"), i18n("Open &Example..."), actionCollection(), "file_open_example");
  connect(fileOpenExample, SIGNAL(triggered(bool)), SLOT(slotFileOpenExample()));
  fileOpen->setWhatsThis(i18n("Open a vocabulary document"));
  fileOpen->setToolTip(fileOpen->whatsThis());

  fileGHNS = new KAction(KIcon("knewstuff"), i18n("&Get New Vocabularies..."), actionCollection(), "file_ghns");
  connect(fileGHNS, SIGNAL(triggered(bool)), SLOT(slotGHNS()));
  fileGHNS->setShortcut(QKeySequence(Qt::Key_Control+Qt::Key_G));
  fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
  fileGHNS->setToolTip(fileGHNS->whatsThis());

  fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());

  fileMerge = new KAction(i18n("&Merge..."), actionCollection(), "file_merge");
  connect(fileMerge, SIGNAL(triggered(bool)), SLOT(slotFileMerge()));
  fileMerge->setWhatsThis(i18n("Merge an existing vocabulary document with the current one"));
  fileMerge->setToolTip(fileOpen->whatsThis());

  fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
  fileSave->setWhatsThis(i18n("Save the active vocabulary document"));
  fileSave->setToolTip(fileSave->whatsThis());

  fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
  fileSaveAs->setWhatsThis(i18n("Save the active vocabulary document with a different name"));
  fileSaveAs->setToolTip(fileSaveAs->whatsThis());

  filePrint = KStdAction::print(this, SLOT(slotFilePrint()), actionCollection());
  filePrint->setWhatsThis(i18n("Print the active vocabulary document"));
  filePrint->setToolTip(filePrint->whatsThis());

  fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
  fileQuit->setWhatsThis(i18n("Quit KVocTrain"));
  fileQuit->setToolTip(fileQuit->whatsThis());

  editCopy = KStdAction::copy(this, SLOT(slotEditCopy()), actionCollection());
  editCopy->setWhatsThis(i18n("Copy"));
  editCopy->setToolTip(editCopy->whatsThis());

  editPaste = KStdAction::paste(this, SLOT(slotEditPaste()), actionCollection());
  editPaste->setWhatsThis(i18n("Paste"));
  editPaste->setToolTip(editPaste->whatsThis());

  editSelectAll = KStdAction::selectAll(this, SLOT(slotSelectAll()), actionCollection());
  editSelectAll->setWhatsThis(i18n("Select all rows"));
  editSelectAll->setToolTip(editSelectAll->whatsThis());

  editClearSelection = KStdAction::deselect(this, SLOT(slotCancelSelection()), actionCollection());
  editClearSelection->setWhatsThis(i18n("Deselect all rows"));
  editClearSelection->setToolTip(editClearSelection->whatsThis());

  editSearchFromClipboard =  KStdAction::find(this, SLOT(slotSmartSearchClip()), actionCollection());
  editSearchFromClipboard->setWhatsThis(i18n("Search for the clipboard contents in the vocabulary"));
  editSearchFromClipboard->setToolTip(editSearchFromClipboard->whatsThis());

  editAppend = new KAction(KIcon("insert_table_row"), i18n("&Append New Entry"), actionCollection(), "edit_append");
  connect(editAppend, SIGNAL(triggered(bool)), SLOT(slotAppendRow()));
  editAppend->setShortcut(QKeySequence(Qt::Key_Insert));
  editAppend->setWhatsThis(i18n("Append a new row to the vocabulary"));
  editAppend->setToolTip(editAppend->whatsThis());

  editEditSelectedArea = new KAction(KIcon("edit_table_row"), i18n("&Edit Selected Area..."), actionCollection(), "edit_edit_selected_area");
  connect(editEditSelectedArea, SIGNAL(triggered(bool)), SLOT(slotEditRow()));
  editEditSelectedArea->setShortcut(QKeySequence(Qt::Key_Control+Qt::Key_Return));
  editEditSelectedArea->setWhatsThis(i18n("Edit the entries in the selected rows"));
  editEditSelectedArea->setToolTip(editEditSelectedArea->whatsThis());

  editRemoveSelectedArea = new KAction(KIcon("delete_table_row"), i18n("&Remove Selected Area"), actionCollection(), "edit_remove_selected_area");
  connect(editRemoveSelectedArea, SIGNAL(triggered(bool)), SLOT(slotRemoveRow()));
  editRemoveSelectedArea->setShortcut(QKeySequence(Qt::Key_Delete));
  editRemoveSelectedArea->setWhatsThis(i18n("Delete the selected rows"));
  editRemoveSelectedArea->setToolTip(editRemoveSelectedArea->whatsThis());

  editSaveSelectedArea = new KAction(KIcon("filesaveas"), i18n("Save E&ntries in Query As..."), actionCollection(),"edit_save_selected_area");
  connect(editSaveSelectedArea, SIGNAL(triggered(bool)), SLOT(slotSaveSelection()));
  editSaveSelectedArea->setWhatsThis(i18n("Save the entries in the query as a new vocabulary"));
  editSaveSelectedArea->setToolTip(editSaveSelectedArea->whatsThis());

  vocabShowStatistics = new KAction(KIcon("statistics"), i18n("Show &Statistics"), actionCollection(), "vocab_show_statistics");
  connect(vocabShowStatistics, SIGNAL(triggered(bool)), SLOT(slotShowStatist()));
  vocabShowStatistics->setWhatsThis(i18n("Show statistics for the current vocabulary"));
  vocabShowStatistics->setToolTip(vocabShowStatistics->whatsThis());

  vocabAssignLessons = new KAction(KIcon("rand_less"), i18n("Assign L&essons..."), actionCollection(), "vocab_assign_lessons");
  connect(vocabAssignLessons, SIGNAL(triggered(bool)), SLOT(slotCreateRandom()));
  vocabAssignLessons->setWhatsThis(i18n("Create random lessons with unassigned entries"));
  vocabAssignLessons->setToolTip(vocabAssignLessons->whatsThis());

  vocabCleanUp = new KAction(KIcon("cleanup"), i18n("&Clean Up"), actionCollection(), "vocab_clean_up");
  connect(vocabCleanUp, SIGNAL(triggered(bool)), SLOT(slotCleanVocabulary()));
  vocabCleanUp->setWhatsThis(i18n("Remove entries with same content from vocabulary"));
  vocabCleanUp->setToolTip(vocabCleanUp->whatsThis());

  vocabAppendLanguage = new KSelectAction(i18n("&Append Language"), "insert_table_col", KShortcut(), actionCollection(), "vocab_append_language");
  connect(vocabAppendLanguage->menu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabAppendLanguage()));
  connect (vocabAppendLanguage->menu(), SIGNAL(activated(int)), this, SLOT(slotAppendLang(int)));
  connect (vocabAppendLanguage->menu(), SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  vocabSetLanguage = new KSelectAction(i18n("Set &Language"), "set_language", KShortcut(), actionCollection(), "vocab_set_language");
  connect(vocabSetLanguage->menu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabSetLanguage()));

  vocabRemoveLanguage = new KSelectAction(i18n("&Remove Language"), "delete_table_col", KShortcut(), actionCollection(), "vocab_remove_language");
  connect(vocabRemoveLanguage->menu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabRemoveLanguage()));
  connect(vocabRemoveLanguage->menu(), SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect(vocabRemoveLanguage->menu(), SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  vocabDocumentProperties = new KAction(i18n("Document &Properties"), actionCollection(), "vocab_document_properties");
  connect(vocabDocumentProperties, SIGNAL(triggered(bool)), SLOT(slotDocProps()));
  vocabDocumentProperties->setWhatsThis(i18n("Edit document properties"));
  vocabDocumentProperties->setToolTip(vocabAppendLanguage->whatsThis());

  vocabLanguageProperties = new KAction(i18n("Lan&guage Properties"), actionCollection(), "vocab_language_properties");
  connect(vocabLanguageProperties, SIGNAL(triggered(bool)), SLOT(slotDocPropsLang()));
  vocabLanguageProperties->setWhatsThis(i18n("Edit language properties in current document"));
  vocabLanguageProperties->setToolTip(vocabSetLanguage->whatsThis());

  lessons = new KComboBox(this);
  lessons->setMinimumWidth(160);
  connect(lessons, SIGNAL(highlighted(int)), this, SLOT(slotChooseLesson(int)));
  lessons->setFocusPolicy(Qt::NoFocus);

  vocabLessons = new KAction(i18n("Lessons"), actionCollection(), "vocab_lessons");
  vocabLessons->setDefaultWidget( lessons );
  vocabLessons->setWhatsThis(i18n("Choose current lesson"));
  vocabLessons->setToolTip(vocabLessons->whatsThis());

  searchLine = new KLineEdit(this);
  searchLine->setFocusPolicy(Qt::ClickFocus);
  connect (searchLine, SIGNAL(returnPressed()), this, SLOT(slotSearchNext()));
  connect (searchLine, SIGNAL(textChanged(const QString&)), this, SLOT(slotResumeSearch(const QString&)));

  vocabSearch = new KAction(i18n("Smart Search"), actionCollection(), "vocab_search");
  vocabSearch->setDefaultWidget( searchLine );
#ifdef __GNUC__
#warning "kde4: porting vocabSearch->setAutoSized(true);"
#endif
  //vocabSearch->setAutoSized(true);
  vocabSearch->setWhatsThis(i18n("Search vocabulary for specified text "));
  vocabSearch->setToolTip(vocabSearch->whatsThis());
  /*
  learningResumeQuery = new KAction(i18n("Resume &Query..."), "run_query", 0, this, SLOT(slotRestartQuery()), actionCollection(),"learning_resume_query");
  //learningResumeQuery->setWhatsThis(i18n(""));
  learningResumeQuery->setToolTip(learningResumeQuery->whatsThis());

  learningResumeMultipleChoice = new KAction(i18n("&Resume Multiple Choice..."), "run_multi", 0, this, SLOT(slotRestartQuery()), actionCollection(),"learning_resume_multiple_choice");
  //learningResumeMultipleChoice->setWhatsThis(i18n(""));
  learningResumeMultipleChoice->setToolTip(learningResumeMultipleChoice->whatsThis());
  */
  configApp = KStdAction::preferences(this, SLOT( slotGeneralOptions()), actionCollection());
  configApp->setWhatsThis(i18n("Show the configuration dialog"));
  configApp->setToolTip(configApp->whatsThis());

  /*configQueryOptions = new KAction(i18n("Configure &Query..."), "configure_query", 0, this, SLOT(slotQueryOptions()), actionCollection(),"config_query_options");
  configQueryOptions->setWhatsThis(i18n("Show the query configuration dialog"));
  configQueryOptions->setToolTip(configQueryOptions->whatsThis());*/
#ifdef __GNUC__
#warning "kde4: porting actionCollection()->setHighlightingEnabled(true);"
#endif
  //actionCollection()->setHighlightingEnabled(true);
  connect(actionCollection(), SIGNAL(actionStatusText(const QString &)), this, SLOT(slotStatusHelpMsg(const QString &)));
  //connect(actionCollection(), SIGNAL(actionHighlighted(KAction *, bool)), this, SLOT(slotActionHighlighted(KAction *, bool)));

  if (!initialGeometrySet())
      resize( QSize(550, 400).expandedTo(minimumSizeHint()));
  setupGUI(ToolBar | Keys | StatusBar | Create);
  setAutoSaveSettings();

  configToolbar = actionCollection()->action("options_configure_toolbars");
  configToolbar->setWhatsThis(i18n("Toggle display of the toolbars"));
  configToolbar->setToolTip(configToolbar->whatsThis());

  learn_menu = (QMenu *) child( "learning", "KMenu" );
  connect(learn_menu, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect(learn_menu, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
  connect(learn_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowLearn()));
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
  if (fileOpenRecent->items().count() > 0){
    doc = new KEduVocDocument(this);
    doc->setURL(KUrl(fileOpenRecent->items()[0]));
  }
  else
    doc = new KEduVocDocument(this);

  loadDocProps(doc);
  if (doc->numIdentifiers() == 0)
    doc->appendIdentifier("en");
  connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
  doc->setModified(false);
}


void KVocTrainApp::initView()
{
  view = new kvoctrainView(doc, langset, this);
  setCentralWidget(view);
  slotStatusMsg(IDS_DEFAULT);
}
