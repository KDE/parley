/***************************************************************************

    $Id$

                   initialisation part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

 ***************************************************************************/

/*** content ************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include <kvoctraincore.h>
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
#include <kconfig.h>

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

  ///////////////////////////////////////////////////////////////////
  // read the config file options
  readOptions();

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initMenuBar();
  initToolBar();
  initStatusBar();
  initView(name);
  setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));

  int cc = Prefs::currentCol();
  int cr = Prefs::currentRow();
  if (cc <= KV_COL_LESS)
    cc = KV_COL_LESS+1;

  view->getTable()->updateContents(cr, cc);

  ///////////////////////////////////////////////////////////////////
  // enable bars dependent on config file setups
  if (!bViewToolbar)
    toolBar()->hide();
  if (!bViewStatusbar)
    statusBar()->hide();

  toolBar()->setBarPos(tool_bar_pos);

  ///////////////////////////////////////////////////////////////////
  // disable menu and toolbar items at startup

  querying = false;
  btimer = new QTimer( this );
  connect( btimer, SIGNAL(timeout()), this, SLOT(slotTimeOutBackup()) );
  if (backupTime > 0)
    btimer->start(backupTime, TRUE);
}


void kvoctrainApp::enableCommand(int id_)
{
  ///////////////////////////////////////////////////////////////////
  // enable menu and toolbar functions by their ID's
  menuBar()->setItemEnabled(id_,true);
  toolBar()->setItemEnabled(id_,true);
}


void kvoctrainApp::disableCommand(int id_)
{
  ///////////////////////////////////////////////////////////////////
  // disable menu and toolbar functions by their ID's
  menuBar()->setItemEnabled(id_,false);
  toolBar()->setItemEnabled(id_,false);
}


void kvoctrainApp::initMenuBar()
{

  header_m = 0;

  ///////////////////////////////////////////////////////////////////
  // MENUBAR
  ///////////////////////////////////////////////////////////////////
  // menuBar entry file_menu
  file_menu = new QPopupMenu();
  file_menu->insertItem(QPixmap(locate("data", "kvoctrain/new.xpm")),
                        i18n("&New"), ID_FILE_NEW );

  file_menu->insertItem(KGlobal::iconLoader()->loadIcon("fileopen", KIcon::Small),
                        i18n("&Open..."), ID_FILE_OPEN );

  file_menu->insertItem(i18n("&Open Example..."), ID_FILE_OPEN_XMP );

  recent_files_menu = new QPopupMenu();
  connect(recent_files_menu, SIGNAL(activated(int)),   SLOT(slotFileOpenRecent(int)) );
  connect(recent_files_menu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
  file_menu->insertItem(i18n("Open &Recent"), recent_files_menu, ID_FILE_OPEN_RECENT);
  QString accel;
  for (uint i = 0 ; i < recent_files.count(); i++){
    accel.setNum (i);
    accel.insert (0, "&");
    accel += "  ";
    accel += recent_files[i];
    recent_files_menu->insertItem(accel, (i << 16) | ID_FILE_OPEN_RECENT, i);
  }

  file_menu->insertSeparator();
  file_menu->insertItem(QPixmap(locate("data", "kvoctrain/merge.xpm")), i18n("&Merge..."), ID_FILE_MERGE );

  file_menu->insertSeparator();
  file_menu->insertItem(KGlobal::iconLoader()->loadIcon("filesave", KIcon::Small) ,i18n("&Save"), ID_FILE_SAVE );
  file_menu->insertItem(SmallIcon("filesaveas"),i18n("Save &As..."), ID_FILE_SAVE_AS );
  file_menu->insertSeparator();
  file_menu->insertItem(KGlobal::iconLoader()->loadIcon("exit", KIcon::Small), i18n("&Quit"), ID_FILE_QUIT );

  // file_menu key accelerators
  file_menu->setAccel(CTRL+Key_O, ID_FILE_OPEN);
  file_menu->setAccel(CTRL+Key_S, ID_FILE_SAVE);
  file_menu->setAccel(CTRL+Key_Q, ID_FILE_QUIT);
  connect( file_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowFile()));

  ///////////////////////////////////////////////////////////////////
  // menuBar entry edit_menu
  edit_menu = new QPopupMenu();
  edit_menu->insertItem(KGlobal::iconLoader()->loadIcon("editcopy", KIcon::Small), i18n("&Copy"), ID_EDIT_COPY );
  edit_menu->insertItem(KGlobal::iconLoader()->loadIcon("editpaste", KIcon::Small), i18n("&Paste"), ID_EDIT_PASTE );
  edit_menu->insertItem(i18n("Se&lect All"), ID_SEL_ALL );
  edit_menu->insertItem(i18n("Clear Selec&tion"), ID_CLR_SEL );
  edit_menu->insertSeparator();
  edit_menu->insertItem(KGlobal::iconLoader()->loadIcon("find", KIcon::Small), i18n("&Search From Clipboard"), ID_SEARCH_CLIP );
  edit_menu->insertSeparator();
  edit_menu->insertItem(QPixmap(locate("data", "kvoctrain/append-row.xpm")), i18n("&Append New Entry"), ID_APPEND_ROW );
  edit_menu->insertItem(QPixmap(locate("data", "kvoctrain/edit-row.xpm")), i18n("&Edit Selected Area..."), ID_EDIT_ROW );
  edit_menu->insertItem(QPixmap(locate("data", "kvoctrain/delete-row.xpm")), i18n("&Remove Selected Area"), ID_REMOVE_ROW );
  edit_menu->insertItem(KGlobal::iconLoader()->loadIcon("filesave", KIcon::Small), i18n("Sa&ve Selected Area..."), ID_SAVE_ROW );
  connect( edit_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowEdit()));
  connect(QApplication::clipboard(),SIGNAL(dataChanged()), this,SLOT(clipboardChanged()));
  clipboardChanged();

  //edit_menu key accelerators
  edit_menu->setAccel(CTRL+Key_C, ID_EDIT_COPY);
  edit_menu->setAccel(CTRL+Key_V, ID_EDIT_PASTE);
  edit_menu->setAccel(CTRL+Key_F, ID_SEARCH_CLIP);
  edit_menu->setAccel(Key_Insert, ID_APPEND_ROW);
  edit_menu->setAccel(Key_Delete, ID_REMOVE_ROW);
  edit_menu->setAccel(Key_Return, ID_EDIT_ROW);
  edit_menu->setAccel(Key_Enter, ID_EDIT_ROW);
  edit_menu->setAccel(CTRL+Key_A, ID_SEL_ALL);
  edit_menu->setAccel(CTRL+Key_Delete, ID_CLR_SEL);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry voc_menu
  voc_menu = new QPopupMenu();
  voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/statist.xpm")), i18n("Show &Statistics"), ID_SHOW_STAT );
  voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/rand-less.xpm")), i18n("Assign &Lessons..."), ID_RAND_CREATE );
  voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/cleanup.xpm")), i18n("&Clean Up"), ID_CLEANUP );
  voc_menu->insertSeparator();
  QPopupMenu *add_m = new QPopupMenu();
  voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/append-col.xpm")), i18n("&Append Language"), add_m, ID_APPEND_LANG );
  QPopupMenu *set_m = new QPopupMenu();
  voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/flags.xpm")), i18n("Set &Language"), set_m, ID_SET_LANG );
  QPopupMenu *remove_m = new QPopupMenu();
  voc_menu->insertItem(QPixmap(locate("data", "kvoctrain/delete-col.xpm")), i18n("&Remove Language"), remove_m, ID_REMOVE_LANG);

  voc_menu->insertSeparator();
  voc_menu->insertItem(i18n("Document &Properties"), ID_DOC_PROPS );
  voc_menu->insertItem(i18n("Lan&guage Properties"), ID_DOC_PROPS_LANG );
  connect( voc_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabulary()));

  ///////////////////////////////////////////////////////////////////
  // menuBar entry learn_menu
  learn_menu = new QPopupMenu();
  connect( learn_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowLearn()));

  opts_menu = new QPopupMenu();
  opts_menu->setCheckable(true);
  opts_menu->insertItem(i18n("Tool&bar"), ID_VIEW_TOOLBAR);
  opts_menu->insertItem(i18n("St&atusbar"), ID_VIEW_STATUSBAR );
  opts_menu->insertItem(i18n("&Inline Editing"), ID_VIEW_INLINE );
  opts_menu->insertSeparator();
  opts_menu->insertItem(QPixmap(locate("data", "kvoctrain/gen-conf.xpm")), i18n("General &Options..."), ID_GENERAL_OPTIONS );
  opts_menu->insertItem(QPixmap(locate("data", "kvoctrain/query-conf.xpm")), i18n("&Query Options..."), ID_QUERY_OPTIONS );
  opts_menu->insertItem(QPixmap(locate("data", "kvoctrain/conf-save.xpm")),i18n("&Save Options"), ID_SAVE_OPTIONS );
  connect( opts_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowOptions()));

  opts_menu->setItemChecked(ID_VIEW_TOOLBAR, bViewToolbar);
  opts_menu->setItemChecked(ID_VIEW_STATUSBAR, bViewStatusbar);
  opts_menu->setItemChecked(ID_VIEW_INLINE, inline_edit);

  ///////////////////////////////////////////////////////////////////
  // MENUBAR CONFIGURATION
  // set menuBar() the current menuBar and the position due to config file

  menuBar()->insertItem(i18n("&File"), file_menu);

  menuBar()->insertItem(i18n("&Edit"), edit_menu);

//  menuBar()->insertItem(i18n("&View"), view_menu);

  menuBar()->insertItem(i18n("Vo&cabulary"), voc_menu);

  menuBar()->insertItem(i18n("&Learning"), learn_menu);

  menuBar()->insertItem(i18n("&Options"), opts_menu);

  help_menu = helpMenu();
  menuBar()->insertSeparator();
  menuBar()->insertItem(i18n("&Help"), help_menu);

  ///////////////////////////////////////////////////////////////////
  // CONNECT THE SUBMENU SLOTS WITH SIGNALS

  CONNECT_CMD(file_menu);
  CONNECT_CMD(edit_menu);
  connect (learn_menu, SIGNAL(activated(int)), this, SLOT(slotHeaderCallBack(int)));
  connect (learn_menu, SIGNAL(highlighted(int)), this, SLOT(slotHeaderStatus(int)));

  CONNECT_CMD(opts_menu);
  CONNECT_CMD(voc_menu);
}


void kvoctrainApp::initToolBar()
{

  ///////////////////////////////////////////////////////////////////
  // TOOLBAR
  // set toolBar() the current toolBar and the position due to config file

  toolBar()->insertButton(KGlobal::iconLoader()->loadIcon("fileopen", KIcon::Toolbar), ID_FILE_OPEN, true, i18n("Open File"));
  file_open_popup= new QPopupMenu();
  QString accel;
  for (uint i = 0 ; i < recent_files.count(); i++){
    accel.setNum (i);
    accel.insert (0, "&");
    accel += "  ";
    accel += recent_files[i];
    file_open_popup->insertItem(accel, (i << 16) | ID_FILE_OPEN_RECENT, i);
  }
  connect(file_open_popup, SIGNAL(activated(int)), SLOT(slotFileOpenRecent(int)));
  toolBar()->setDelayedPopup(ID_FILE_OPEN, file_open_popup);

  toolBar()->insertButton(KGlobal::iconLoader()->loadIcon("filesave", KIcon::Toolbar), ID_FILE_SAVE, true, i18n("Save File"));
  toolBar()->insertSeparator();
  toolBar()->insertButton(KGlobal::iconLoader()->loadIcon("editcopy", KIcon::Toolbar), ID_EDIT_COPY, true, i18n("Copy"));
  toolBar()->insertButton(KGlobal::iconLoader()->loadIcon("editpaste", KIcon::Toolbar), ID_EDIT_PASTE, true, i18n("Paste"));

  toolBar()->insertSeparator();
  QStringList slist;
  toolBar()->insertCombo (slist, ID_TLESSON, false,
                          SIGNAL(highlighted(int)), this, SLOT(slotChooseLesson(int)),
                          true, i18n("Choose current lesson"), 160);
  lessons = toolBar()->getCombo(ID_TLESSON);
  lessons->setFocusPolicy(QWidget::NoFocus);

  toolBar()->insertSeparator();
  toolBar()->insertButton(KGlobal::iconLoader()->loadIcon("find", KIcon::Toolbar), ID_SEARCH_CLIP, true, i18n("Search From Clipboard"));
  toolBar()->insertLined ("", ID_TSEARCH, SIGNAL(textChanged(const QString&)), this, SLOT(slotResumeSearch(const QString&)), true, i18n("Smart Search"));
  searchLine = toolBar()->getLined (ID_TSEARCH);
  searchLine->setFocusPolicy(QWidget::ClickFocus);
  toolBar()->setItemAutoSized (ID_TSEARCH, true);
  connect (searchLine, SIGNAL(returnPressed()), this, SLOT(slotSearchNext()));
  toolBar()->insertSeparator();

  ///////////////////////////////////////////////////////////////////
  // INSERT YOUR APPLICATION SPECIFIC TOOLBARS HERE -e.g. tool_bar_1:
  // add functionality for new created toolbars in:
  // enableCommand, disableCommand, in the menuBar() and an additional function slotViewToolbar_1
  // for that also create a bViewToolbar_1 and a KConfig entry (see Constructor).
  // Also update resource values and commands 


  ///////////////////////////////////////////////////////////////////
  // CONNECT THE TOOLBAR SLOTS WITH SIGNALS - add new created toolbars
  CONNECT_TOOLBAR(toolBar());

}


void kvoctrainApp::initStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  //STATUSBAR

/*
  QLabel *label = new QLabel ("", statusBar());
  label->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  statusBar()->insertWidget(label, 50, ID_STATUS_MSG );
*/
  type_label = new QLabel (i18n(PREFIX_Type), statusBar());
  type_label->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  statusBar()->addWidget(type_label, 150);

  pron_label = new QLabel (i18n(PREFIX_Pronunce), statusBar());
  pron_label->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  pron_label->setFont(ipafont);
  statusBar()->addWidget(pron_label, 200);

  rem_label = new QLabel (i18n(PREFIX_Remark), statusBar());
  rem_label->setFrameStyle( QFrame::Panel | QFrame::Sunken );
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


void kvoctrainApp::initView(const QString &name)
{ 
  view = 0;

  pdlg = new ProgressDlg (QString(), QString(),
                kapp->makeStdCaption("Loading vocabulary file"));
  pdlg->show();

  kvoctrainExpr::setPixmap(kvoctrainExpr::ExprInQuery,
                           QPixmap(locate("data", "kvoctrain/entry-in-query.png")));
  kvoctrainExpr::setPixmap(kvoctrainExpr::ExprInactive,
                           QPixmap(locate("data", "kvoctrain/entry-inactive.png")));

  if (!name.isEmpty()) {
    doc = new kvoctrainDoc(this, name, separator, &paste_order);
    if (doc) {
      addRecentFile(name);
    }
  }
  else {
    if (recent_files.count() > 0)
      doc = new kvoctrainDoc(this, recent_files[0], separator, &paste_order);
    else
      doc = new kvoctrainDoc(this, "", separator, &paste_order);
  }
  removeProgressBar();
  loadDocProps(doc);
  if (doc->numLangs() == 0)
    doc->appendLang("en");

  view = new kvoctrainView(doc, langset, gradecols, this);
  connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
  doc->setModified(false);

  view->setResizer (header_resizer);
  view->getTable()->setFont(tablefont);
  setCentralWidget(view);

  ////////////////////////////////////////////////////////////////////
  // prepare dock window in panel
  slotStatusMsg(IDS_DEFAULT);
}
