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
  header_m = 0;
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
  connect( btimer, SIGNAL(timeout()), this, SLOT(slotTimeOutBackup()) );
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

  fileOpenExample = new KAction(KIcon("fileopen"), i18n("Open &Example..."), actionCollection(), "file_open_example");
  connect(fileOpenExample, SIGNAL(triggered(bool)), SLOT(slotFileOpenExample()));
  fileOpenExample->setWhatsThis(i18n("Open an example vocabulary document"));
  fileOpenExample->setToolTip(fileOpen->whatsThis());
  fileOpenExample->setStatusTip(fileOpen->whatsThis());

  fileGHNS = new KAction(KIcon("knewstuff"), i18n("&Get New Vocabularies..."), actionCollection(), "file_ghns");
  connect(fileGHNS, SIGNAL(triggered(bool)), SLOT(slotGHNS()));
  fileGHNS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
  fileGHNS->setWhatsThis(i18n("Downloads new vocabularies"));
  fileGHNS->setToolTip(fileGHNS->whatsThis());
  fileGHNS->setStatusTip(fileGHNS->whatsThis());

  fileOpenRecent = KStandardAction::openRecent(this, SLOT(slotFileOpenRecent(const KUrl&)), actionCollection());

  fileMerge = new KAction(i18n("&Merge..."), actionCollection(), "file_merge");
  connect(fileMerge, SIGNAL(triggered(bool)), SLOT(slotFileMerge()));
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

  editAppend = new KAction(KIcon("insert_table_row"), i18n("&Append New Entry"), actionCollection(), "edit_append");
  connect(editAppend, SIGNAL(triggered(bool)), SLOT(slotAppendRow()));
  editAppend->setShortcut(QKeySequence(Qt::Key_Insert));
  editAppend->setWhatsThis(i18n("Append a new row to the vocabulary"));
  editAppend->setToolTip(editAppend->whatsThis());
  editAppend->setStatusTip(editAppend->whatsThis());

  editDelete = new KAction(KIcon("delete_table_row"), i18n("&Delete Entry"), actionCollection(), "edit_remove_selected_area");
  connect(editDelete, SIGNAL(triggered(bool)), SLOT(slotRemoveRow()));
  editDelete->setShortcut(QKeySequence(Qt::Key_Delete));
  editDelete->setWhatsThis(i18n("Delete the selected rows"));
  editDelete->setToolTip(editDelete->whatsThis());
  editDelete->setStatusTip(editDelete->whatsThis());

  editEditEntry = new KAction(KIcon("edit_table_row"), i18n("&Edit Entry..."), actionCollection(), "edit_edit_selected_area");
  connect(editEditEntry, SIGNAL(triggered(bool)), SLOT(slotEditRow()));
  editEditEntry->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return));
  editEditEntry->setWhatsThis(i18n("Edit the entries in the selected rows"));
  editEditEntry->setToolTip(editEditEntry->whatsThis());
  editEditEntry->setStatusTip(editEditEntry->whatsThis());

  editSaveSelectedArea = new KAction(KIcon("filesaveas"), i18n("Save E&ntries in Query As..."), actionCollection(),"edit_save_selected_area");
  connect(editSaveSelectedArea, SIGNAL(triggered(bool)), SLOT(slotSaveSelection()));
  editSaveSelectedArea->setWhatsThis(i18n("Save the entries in the query as a new vocabulary"));
  editSaveSelectedArea->setToolTip(editSaveSelectedArea->whatsThis());
  editSaveSelectedArea->setStatusTip(editSaveSelectedArea->whatsThis());

  vocabShowStatistics = new KAction(KIcon("statistics"), i18n("Show &Statistics"), actionCollection(), "vocab_show_statistics");
  connect(vocabShowStatistics, SIGNAL(triggered(bool)), SLOT(slotShowStatistics()));
  vocabShowStatistics->setWhatsThis(i18n("Show statistics for the current vocabulary"));
  vocabShowStatistics->setToolTip(vocabShowStatistics->whatsThis());
  vocabShowStatistics->setStatusTip(vocabShowStatistics->whatsThis());

  vocabAssignLessons = new KAction(KIcon("rand_less"), i18n("Assign L&essons..."), actionCollection(), "vocab_assign_lessons");
  connect(vocabAssignLessons, SIGNAL(triggered(bool)), SLOT(slotCreateRandom()));
  vocabAssignLessons->setWhatsThis(i18n("Create random lessons with unassigned entries"));
  vocabAssignLessons->setToolTip(vocabAssignLessons->whatsThis());
  vocabAssignLessons->setStatusTip(vocabAssignLessons->whatsThis());

  vocabCleanUp = new KAction(KIcon("cleanup"), i18n("&Clean Up"), actionCollection(), "vocab_clean_up");
  connect(vocabCleanUp, SIGNAL(triggered(bool)), SLOT(slotCleanVocabulary()));
  vocabCleanUp->setWhatsThis(i18n("Remove entries with same content from vocabulary"));
  vocabCleanUp->setToolTip(vocabCleanUp->whatsThis());
  vocabCleanUp->setStatusTip(vocabCleanUp->whatsThis());

  vocabAppendLanguage = new KSelectAction(KIcon("insert_table_col"), i18n("&Append Language"),actionCollection(), "vocab_append_language");
  connect(vocabAppendLanguage->menu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabAppendLanguage()));
  connect (vocabAppendLanguage->menu(), SIGNAL(activated(int)), this, SLOT(slotAppendLang(int)));
  connect (vocabAppendLanguage->menu(), SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  vocabSetLanguage = new KSelectAction(KIcon("set_language"), i18n("Set &Language"), actionCollection(), "vocab_set_language");
  connect(vocabSetLanguage->menu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabSetLanguage()));

  vocabRemoveLanguage = new KSelectAction(KIcon("delete_table_col"), i18n("&Remove Language"), actionCollection(), "vocab_remove_language");
  connect(vocabRemoveLanguage->menu(), SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabRemoveLanguage()));
  connect(vocabRemoveLanguage->menu(), SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect(vocabRemoveLanguage->menu(), SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  vocabDocumentProperties = new KAction(i18n("Document &Properties"), actionCollection(), "vocab_document_properties");
  connect(vocabDocumentProperties, SIGNAL(triggered(bool)), SLOT(slotDocProps()));
  vocabDocumentProperties->setWhatsThis(i18n("Edit document properties"));
  vocabDocumentProperties->setToolTip(vocabAppendLanguage->whatsThis());
  vocabDocumentProperties->setStatusTip(vocabAppendLanguage->whatsThis());

  vocabLanguageProperties = new KAction(i18n("Lan&guage Properties"), actionCollection(), "vocab_language_properties");
  connect(vocabLanguageProperties, SIGNAL(triggered(bool)), SLOT(slotDocPropsLang()));
  vocabLanguageProperties->setWhatsThis(i18n("Edit language properties in current document"));
  vocabLanguageProperties->setToolTip(vocabSetLanguage->whatsThis());
  vocabLanguageProperties->setStatusTip(vocabSetLanguage->whatsThis());

  m_lessonsComboBox = new KComboBox(this);
  m_lessonsComboBox->setMinimumWidth(160);
  connect(m_lessonsComboBox, SIGNAL(highlighted(int)), this, SLOT(slotChooseLesson(int)));
  m_lessonsComboBox->setFocusPolicy(Qt::NoFocus);

  vocabLessons = new KAction(i18n("Lessons"), actionCollection(), "vocab_lessons");
  vocabLessons->setDefaultWidget(m_lessonsComboBox);
  vocabLessons->setWhatsThis(i18n("Choose current lesson"));
  vocabLessons->setToolTip(vocabLessons->whatsThis());
  vocabLessons->setStatusTip(vocabLessons->whatsThis());

  searchLine = new KLineEdit(this);
  searchLine->setFocusPolicy(Qt::ClickFocus);
  connect(searchLine, SIGNAL(returnPressed()), this, SLOT(slotSearchNext()));
  connect(searchLine, SIGNAL(textChanged(const QString&)), this, SLOT(slotResumeSearch(const QString&)));

  vocabSearch = new KAction(i18n("Smart Search"), actionCollection(), "vocab_search");
  vocabSearch->setDefaultWidget( searchLine );
  vocabSearch->setWhatsThis(i18n("Search vocabulary for specified text "));
  vocabSearch->setToolTip(vocabSearch->whatsThis());
  vocabSearch->setStatusTip(vocabSearch->whatsThis());

  configApp = KStandardAction::preferences(this, SLOT( slotGeneralOptions()), actionCollection());
  configApp->setWhatsThis(i18n("Show the configuration dialog"));
  configApp->setToolTip(configApp->whatsThis());
  configApp->setStatusTip(configApp->whatsThis());

  if (!initialGeometrySet())
    resize( QSize(550, 400).expandedTo(minimumSizeHint()));
  setupGUI(ToolBar | Keys | StatusBar | Create);
  setAutoSaveSettings();

  configToolbar = actionCollection()->action("options_configure_toolbars");
  configToolbar->setWhatsThis(i18n("Toggle display of the toolbars"));
  configToolbar->setToolTip(configToolbar->whatsThis());
  configToolbar->setStatusTip(configToolbar->whatsThis());

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
  if (fileOpenRecent->actions().count() > 0  && fileOpenRecent->action(0)->isEnabled()){
    fileOpenRecent->action(0)->trigger();
  }
  else 
    createNewDocument();
}

void KVocTrainApp::initModel()
{
  m_tableModel = new KVTTableModel(this);
  m_tableModel->setLanguages(m_languages);
}

void KVocTrainApp::initView()
{
  QWidget * mainWidget = new QWidget(this);
  setCentralWidget(mainWidget);
  m_topLayout = new QVBoxLayout(mainWidget);
  m_topLayout->setMargin(0);
  m_topLayout->setSpacing(KDialog::spacingHint());
  m_tableView = new KVTTableView(centralWidget());
  m_tableView->setFrameStyle(QFrame::NoFrame);
  m_topLayout->addWidget(m_tableView);

  m_tableView->setModel(m_tableModel);
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

  setCaption(m_doc->URL().fileName(), false);
  connect(m_tableModel, SIGNAL(modelReset()), m_tableView, SLOT(slotModelReset()));
  connect(m_tableView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotCurrentChanged(const QModelIndex &, const QModelIndex &)));
  slotCurrentChanged(m_tableView->currentIndex(), m_tableView->currentIndex());
  m_doc->setModified(false); ///@todo doc being modified at startup is due to resize code. Needs to be improved.
}
