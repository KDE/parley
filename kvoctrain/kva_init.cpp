/***************************************************************************

                   initialisation part of kvoctrain

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004-2005 Peter Hedlund <peter@peterandlinda.com>

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

#include "kvoctrain.h"

#include "common-dialogs/ProgressDlg.h"

#include <qclipboard.h>
#include <qtimer.h>

#include <klineedit.h>
#include <kcombobox.h>
#include <kstatusbar.h>
#include <kpopupmenu.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <klocale.h>

#include "prefs.h"

kvoctrainApp::kvoctrainApp(QWidget *parent, const char *name)
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

  configSaveOptions->setEnabled(!autosaveopts);
  editRemoveSelectedArea->setEnabled(view->getTable()->numRows() > 0);

  querying = false;
  btimer = new QTimer( this );
  connect( btimer, SIGNAL(timeout()), this, SLOT(slotTimeOutBackup()) );
  if (backupTime > 0)
    btimer->start(backupTime, TRUE);
}


void kvoctrainApp::initActions()
{
  fileNew = KStdAction::openNew(this, SLOT(slotFileNew()), actionCollection());
  fileNew->setWhatsThis(i18n("Creates a new blank vocabulary document"));
  fileNew->setToolTip(fileNew->whatsThis());

  fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
  fileOpen->setWhatsThis(i18n("Opens an existing vocabulary document"));
  fileOpen->setToolTip(fileOpen->whatsThis());

  fileOpenExample = new KAction(i18n("Open &Example..."), "fileopen", 0, this, SLOT(slotFileOpenExample()), actionCollection(), "file_open_example");
  fileOpen->setWhatsThis(i18n("Opens an example vocabulary document"));
  fileOpen->setToolTip(fileOpen->whatsThis());

  fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());

  fileMerge = new KAction(i18n("&Merge..."), 0, 0, this, SLOT(slotFileMerge()), actionCollection(), "file_merge");
  fileMerge->setWhatsThis(i18n("Merges an existing vocabulary document with the current one"));
  fileMerge->setToolTip(fileOpen->whatsThis());

  fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
  fileSave->setWhatsThis(i18n("Saves the active vocabulary document"));
  fileSave->setToolTip(fileSave->whatsThis());

  fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
  fileSaveAs->setWhatsThis(i18n("Saves the active vocabulary document with a different name"));
  fileSaveAs->setToolTip(fileSaveAs->whatsThis());

  fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
  fileQuit->setWhatsThis(i18n("Quits KVocTrain"));
  fileQuit->setToolTip(fileQuit->whatsThis());

  editCopy = KStdAction::copy(this, SLOT(slotEditCopy()), actionCollection());
  editCopy->setWhatsThis(i18n("Copies the text from the selected cells and places it on the clipboard"));
  editCopy->setToolTip(editCopy->whatsThis());

  editPaste = KStdAction::paste(this, SLOT(slotEditPaste()), actionCollection());
  editPaste->setWhatsThis(i18n("Pastes previously cut or copied text from the clipboard into the selected cells"));
  editPaste->setToolTip(editPaste->whatsThis());

  editSelectAll = KStdAction::selectAll(this, SLOT(slotSelectAll()), actionCollection()); // KAction(i18n("Se&lect All"), 0, "CTRL+A", this, SLOT(slotSelectAll()), actionCollection(),"edit_select_all");
  editSelectAll->setWhatsThis(i18n("Selects all rows"));
  editSelectAll->setToolTip(editSelectAll->whatsThis());

  editClearSelection = KStdAction::deselect(this, SLOT(slotCancelSelection()), actionCollection());
  editClearSelection->setWhatsThis(i18n("Deselects all rows"));
  editClearSelection->setToolTip(editClearSelection->whatsThis());

  editSearchFromClipboard =  KStdAction::find(this, SLOT(slotSmartSearchClip()), actionCollection());
  editSearchFromClipboard->setWhatsThis(i18n("Searches for clipboard content in the vocabulary"));
  editSearchFromClipboard->setToolTip(editSearchFromClipboard->whatsThis());

  editAppend = new KAction(i18n("&Append New Entry"), "insert_table_row", "Insert", this, SLOT(slotAppendRow()), actionCollection(),"edit_append");
  editAppend->setWhatsThis(i18n("Appends a new row to the vocabulary"));
  editAppend->setToolTip(editAppend->whatsThis());

  editEditSelectedArea = new KAction(i18n("&Edit Selected Area..."), "edit_table_row", "Ctrl+Return", this, SLOT(slotEditRow()), actionCollection(),"edit_edit_selected_area");
  editEditSelectedArea->setWhatsThis(i18n("Edits the entries in the selected rows"));
  editEditSelectedArea->setToolTip(editEditSelectedArea->whatsThis());

  editRemoveSelectedArea = new KAction(i18n("&Remove Selected Area"), "delete_table_row", "Delete", this, SLOT(slotRemoveRow()), actionCollection(),"edit_remove_selected_area");
  editRemoveSelectedArea->setWhatsThis(i18n("Deletes the selected rows"));
  editRemoveSelectedArea->setToolTip(editRemoveSelectedArea->whatsThis());

  editSaveSelectedArea = new KAction(i18n("Save E&ntries in Query As..."), KGlobal::iconLoader()->loadIcon("filesaveas", KIcon::Small), 0, this, SLOT(slotSaveSelection()), actionCollection(),"edit_save_selected_area");
  editSaveSelectedArea->setWhatsThis(i18n("Saves the entries in the query as a new vocabulary"));
  editSaveSelectedArea->setToolTip(editSaveSelectedArea->whatsThis());

  vocabShowStatistics = new KAction(i18n("Show &Statistics..."), "statistics", 0, this, SLOT(slotShowStatist()), actionCollection(),"vocab_show_statistics");
  vocabShowStatistics->setWhatsThis(i18n("Shows statistics for the current vocabulary"));
  vocabShowStatistics->setToolTip(vocabShowStatistics->whatsThis());

  vocabAssignLessons = new KAction(i18n("Assign L&essons..."), "rand_less", 0, this, SLOT(slotCreateRandom()), actionCollection(),"vocab_assign_lessons");
  vocabAssignLessons->setWhatsThis(i18n("Creates random lessons with unassigned entries"));
  vocabAssignLessons->setToolTip(vocabAssignLessons->whatsThis());

  vocabCleanUp = new KAction(i18n("&Clean Up"), "cleanup", 0, this, SLOT(slotCleanVocabulary()), actionCollection(),"vocab_clean_up");
  vocabCleanUp->setWhatsThis(i18n("Removes entries with same content from vocabulary"));
  vocabCleanUp->setToolTip(vocabCleanUp->whatsThis());

  vocabAppendLanguage = new KSelectAction(i18n("&Append Language"), "insert_table_col", 0, actionCollection(), "vocab_append_language");
  connect(vocabAppendLanguage->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabAppendLanguage()));
  connect (vocabAppendLanguage->popupMenu(), SIGNAL(activated(int)), this, SLOT(slotAppendLang(int)));
  connect (vocabAppendLanguage->popupMenu(), SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  vocabSetLanguage = new KSelectAction(i18n("Set &Language"), "set_language", 0, actionCollection(), "vocab_set_language");
  connect(vocabSetLanguage->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabSetLanguage()));

  vocabRemoveLanguage = new KSelectAction(i18n("&Remove Language"), "delete_table_col", 0, actionCollection(), "vocab_remove_language");
  connect(vocabRemoveLanguage->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabRemoveLanguage()));
  connect(vocabRemoveLanguage->popupMenu(), SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect(vocabRemoveLanguage->popupMenu(), SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  vocabDocumentProperties = new KAction(i18n("Document &Properties..."), 0, 0, this, SLOT(slotDocProps()), actionCollection(), "vocab_document_properties");
  vocabDocumentProperties->setWhatsThis(i18n("Edits document properties"));
  vocabDocumentProperties->setToolTip(vocabAppendLanguage->whatsThis());

  vocabLanguageProperties = new KAction(i18n("Lan&guage Properties..."), 0, 0, this, SLOT(slotDocPropsLang()), actionCollection(), "vocab_language_properties");
  vocabLanguageProperties->setWhatsThis(i18n("Edits language properties in current document"));
  vocabLanguageProperties->setToolTip(vocabSetLanguage->whatsThis());

  lessons = new KComboBox(this);
  lessons->setMinimumWidth(160);
  connect(lessons, SIGNAL(highlighted(int)), this, SLOT(slotChooseLesson(int)));
  lessons->setFocusPolicy(QWidget::NoFocus);

  vocabLessons = new KWidgetAction(lessons, i18n("Lessons"), 0, this, 0, actionCollection(), "vocab_lessons");
  vocabLessons->setWhatsThis(i18n("Choose current lesson"));
  vocabLessons->setToolTip(vocabLessons->whatsThis());

  searchLine = new KLineEdit(this);
  searchLine->setFocusPolicy(QWidget::ClickFocus);
  connect (searchLine, SIGNAL(returnPressed()), this, SLOT(slotSearchNext()));
  connect (searchLine, SIGNAL(textChanged(const QString&)), this, SLOT(slotResumeSearch(const QString&)));

  vocabSearch = new KWidgetAction(searchLine, i18n("Smart Search"), 0, this, 0, actionCollection(), "vocab_search");
  vocabSearch->setAutoSized(true);
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
  configApp->setWhatsThis(i18n("Shows the configuration dialog"));
  configApp->setToolTip(configApp->whatsThis());

  configQueryOptions = new KAction(i18n("Configure &Query..."), "configure_query", 0, this, SLOT(slotQueryOptions()), actionCollection(),"config_query_options");
  configQueryOptions->setWhatsThis(i18n("Shows the query configuration dialog"));
  configQueryOptions->setToolTip(configQueryOptions->whatsThis());

  configSaveOptions =  KStdAction::saveOptions(this, SLOT(slotSaveOptions()), actionCollection());
  configSaveOptions->setWhatsThis(i18n("Saves current configuration"));
  configSaveOptions->setToolTip(configSaveOptions->whatsThis());

  actionCollection()->setHighlightingEnabled(true);
  connect(actionCollection(), SIGNAL(actionStatusText(const QString &)), this, SLOT(slotStatusHelpMsg(const QString &)));
  //connect(actionCollection(), SIGNAL(actionHighlighted(KAction *, bool)), this, SLOT(slotActionHighlighted(KAction *, bool)));

  //setupGUI(ToolBar|Keys|StatusBar|Save|Create, "/home/kdedev/src/kde/kdeedu/kvoctrain/kvoctrain/kvoctrainui.rc");
  setupGUI();

  configToolbar = actionCollection()->action("options_configure_toolbars");
  configToolbar->setWhatsThis(i18n("Toggles display of the toolbars"));
  configToolbar->setToolTip(configToolbar->whatsThis());

  learn_menu = (QPopupMenu*) child( "learning", "KPopupMenu" );
  connect(learn_menu, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect(learn_menu, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));
  connect(learn_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowLearn()));
}


void kvoctrainApp::initStatusBar()
{
  type_label = new QLabel (i18n(PREFIX_Type), statusBar());
  type_label->setFrameStyle(QFrame::NoFrame);
  statusBar()->addWidget(type_label, 150);

  pron_label = new QLabel (i18n(PREFIX_Pronunce), statusBar());
  pron_label->setFrameStyle(QFrame::NoFrame);
  pron_label->setFont(ipafont);
  statusBar()->addWidget(pron_label, 200);

  rem_label = new QLabel (i18n(PREFIX_Remark), statusBar());
  rem_label->setFrameStyle(QFrame::NoFrame);
  statusBar()->addWidget(rem_label, 150);
}


void kvoctrainApp::clearStatusBar()
{
  statusBar()->removeWidget(rem_label);
  delete rem_label;
  rem_label = 0;

  statusBar()->removeWidget(pron_label);
  delete pron_label;
  pron_label = 0;

  statusBar()->removeWidget(type_label);
  delete type_label;
  type_label = 0;

/*
  statusBar()->removeWidget(pbar);
  delete pbar;
  pbar = 0;
*/
}


void kvoctrainApp::initDoc( )
{
  if (fileOpenRecent->items().count() /*recent_files.count()*/ > 0)
    doc = new kvoctrainDoc(this, fileOpenRecent->items()[0] /*KURL(recent_files[0])*/, separator, &paste_order);
  else
    doc = new kvoctrainDoc(this, KURL(""), separator, &paste_order);

  loadDocProps(doc);
  if (doc->numLangs() == 0)
    doc->appendLang("en");
  connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
  doc->setModified(false);
}


void kvoctrainApp::initView()
{
  view = new kvoctrainView(doc, langset, gradecols, this);
  view->setResizer (header_resizer);
  view->getTable()->setFont(tablefont);
  setCentralWidget(view);
  slotStatusMsg(IDS_DEFAULT);
}
