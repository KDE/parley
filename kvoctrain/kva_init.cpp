/***************************************************************************

    $Id$

                   initialisation part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

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
#include "compat_2x.h"
#include "common-dialogs/ProgressDlg.h"

#include <qclipboard.h>
#include <qprogressbar.h>
#include <kstdaccel.h>
#include <klineedit.h>
#include <kcombobox.h>

kvoctrainApp::kvoctrainApp(const QString name)
{
  tagCount = 0;
  tagLastRow = -1;
  tagLastCol = -1;
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
  queryDlg = 0;
  mAboutDialog = 0;

  setCaption(kvoctrainApp::generateCaption(""));

  ///////////////////////////////////////////////////////////////////
  // read the config file options
  readOptions();

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initMenuBar();
  initToolBar();
  initStatusBar();
  initView(name);
  setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));

  KConfig *config = EA_KappGetConfig;
  config->setGroup(CFG_GENERAL);
  int cc = config->readNumEntry(CFG_CUR_COL, KV_COL_ORG);
  int cr = config->readNumEntry(CFG_CUR_ROW, 0);
  if (cc <= KV_COL_LESS)
    cc = KV_COL_LESS+1;

  view->getTable()->setCurrentRow (cr, cc);
  view->getTable()->updateViewPort();
  slotCellMoved(cr, cc, 0);
  tagLastRow = view->getTable()->currentRow();
  tagLastCol = view->getTable()->currentCol();

  ///////////////////////////////////////////////////////////////////
  // enable bars dependend on config file setups
  if (!bViewToolbar)
    enableToolBar(KToolBar::Hide,0);
  if (!bViewStatusbar)
    enableStatusBar(KStatusBar::Hide);

  toolBar()->setBarPos(tool_bar_pos);

  ///////////////////////////////////////////////////////////////////
  // disable menu and toolbar items at startup

  querying = false;
  qtimer = new QTimer( this );
  btimer = new QTimer( this );
  connect( btimer, SIGNAL(timeout()), this, SLOT(slotTimeOutBackup()) );
  if (backupTime != 0)
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

  used_Menus.clear();
  header_m = 0;

  ///////////////////////////////////////////////////////////////////
  // MENUBAR  
  ///////////////////////////////////////////////////////////////////
  // menuBar entry file_menu
  file_menu = new QPopupMenu();
  file_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/new.xpm")),
                        i18n("&New"), ID_FILE_NEW );
  file_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/open.xpm")),
                        i18n("&Open..."), ID_FILE_OPEN );

  recent_files_menu = new QPopupMenu();
  connect( recent_files_menu, SIGNAL(activated(int)), SLOT(slotFileOpenRecent(int)) );
  file_menu->insertItem(i18n("Open Recen&t.."), recent_files_menu, ID_FILE_OPEN_RECENT);
  QString accel;
  for (uint i = 0 ; i < recent_files.count(); i++){
    accel.setNum (i);
    accel.insert (0, "&");
    accel += "  ";
    accel += recent_files[i];
    recent_files_menu->insertItem(accel, i);
  }

  file_menu->insertSeparator();
  file_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/merge.xpm")), i18n("&Merge..."), ID_FILE_MERGE );

  file_menu->insertSeparator();
  file_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/save.xpm")) ,i18n("&Save"), ID_FILE_SAVE );
  file_menu->insertItem(i18n("Save &as..."), ID_FILE_SAVE_AS );
  file_menu->insertSeparator();
  file_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/exit.xpm")), i18n("E&xit"), ID_FILE_QUIT );

  // file_menu key accelerators
  file_menu->setAccel(CTRL+Key_O, ID_FILE_OPEN);
  file_menu->setAccel(CTRL+Key_S, ID_FILE_SAVE);
  file_menu->setAccel(CTRL+Key_Q, ID_FILE_QUIT);
  connect( file_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowFile()));

  ///////////////////////////////////////////////////////////////////
  // menuBar entry edit_menu
  edit_menu = new QPopupMenu();
  edit_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/copy.xpm")), i18n("&Copy"), ID_EDIT_COPY );
  edit_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/paste.xpm")), i18n("&Paste"), ID_EDIT_PASTE );
  edit_menu->insertItem(i18n("Se&lect all"), ID_SEL_ALL );
  edit_menu->insertItem(i18n("Clear selec&tion"), ID_CLR_SEL );
  edit_menu->insertSeparator();
  edit_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/searchclip.xpm")), i18n("&Search from clipboard"), ID_SEARCH_CLIP );
  edit_menu->insertSeparator();
  edit_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/append-row.xpm")), i18n("&Append new entry"), ID_APPEND_ROW );
  edit_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/edit-row.xpm")), i18n("&Edit selected area"), ID_EDIT_ROW );
  edit_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/delete-row.xpm")), i18n("&Remove selected area"), ID_REMOVE_ROW );
  edit_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/save.xpm")), i18n("Sa&ve selected area.."), ID_SAVE_ROW );
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
  voc_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/statist.xpm")), i18n("Show &statistics"), ID_SHOW_STAT );
  voc_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/rand-less.xpm")), i18n("Assign &lessons"), ID_RAND_CREATE );
  voc_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/cleanup.xpm")), i18n("&Clean up"), ID_CLEANUP );
  voc_menu->insertSeparator();
  QPopupMenu *add_m = new QPopupMenu();
  voc_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/append-col.xpm")), i18n("&Append language"), add_m, ID_APPEND_LANG );
  QPopupMenu *remove_m = new QPopupMenu();
  voc_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/delete-col.xpm")), i18n("&Remove language"), remove_m, ID_REMOVE_LANG);

  voc_menu->insertSeparator();
  voc_menu->insertItem(i18n("Document &properties"), ID_DOC_PROPS );
  voc_menu->insertItem(i18n("Lan&guage properties"), ID_DOC_PROPS_LANG );
  connect( voc_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowVocabulary()));

  ///////////////////////////////////////////////////////////////////
  // menuBar entry learn_menu
  learn_menu = new QPopupMenu();
  connect( learn_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowLearn()));

  opts_menu = new QPopupMenu();
  opts_menu->setCheckable(true);
  opts_menu->insertItem(i18n("Tool&bar"), ID_VIEW_TOOLBAR);
  opts_menu->insertItem(i18n("St&atusbar"), ID_VIEW_STATUSBAR );
  opts_menu->insertSeparator();
  opts_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/gen-conf.xpm")), i18n("General &options"), ID_GENERAL_OPTIONS );
  opts_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/query-conf.xpm")), i18n("&Query options"), ID_QUERY_OPTIONS );
  opts_menu->insertItem(QPixmap(EA_KDEDATADIR("", "kvoctrain/conf-save.xpm")),i18n("&Save options"), ID_SAVE_OPTIONS );
  connect( opts_menu, SIGNAL(aboutToShow()), this, SLOT(aboutToShowOptions()));

  opts_menu->setItemChecked(ID_VIEW_TOOLBAR, bViewToolbar);
  opts_menu->setItemChecked(ID_VIEW_STATUSBAR, bViewStatusbar);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry help_menu
  help_menu = new QPopupMenu();

  help_menu = customHelpMenu();
  help_menu->setId( 0, 0);
  help_menu->setId( 5, 5 );
  help_menu->setId( 6, 6 );
  help_menu->connectItem( 5, this, SLOT(showAboutDialog()) );
  help_menu->removeItem( 0 );

  help_menu->insertItem(i18n("&Contents"), this, SLOT(invokeHelp()),
		        Key_F1, 0, 0 );

  help_menu->changeItem ( QPixmap (EA_KDEDATADIR("",  "kvoctrain/contents.xpm" )),
			   help_menu->text(0),
			   0 ); 
  help_menu->changeItem ( QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )),
			   help_menu->text(5),
			   5 );
  help_menu->changeItem ( QPixmap (EA_KDEDATADIR("",  "kvoctrain/kde.xpm" )),
			   help_menu->text(6),
			   6 );


  ///////////////////////////////////////////////////////////////////
  // MENUBAR CONFIGURATION
  // set menuBar() the current menuBar and the position due to config file

  menuBar()->insertItem(i18n("&File"), file_menu);
  used_Menus.push_back(i18n("&File"));

  menuBar()->insertItem(i18n("&Edit"), edit_menu);
  used_Menus.push_back(i18n("&Edit"));

//  menuBar()->insertItem(i18n("&View"), view_menu);
//used_Menus.push_back(i18n("&View"));

  menuBar()->insertItem(i18n("Vo&cabulary"), voc_menu);
  used_Menus.push_back(i18n("Vo&cabulary"));

  menuBar()->insertItem(i18n("&Learning"), learn_menu);
  used_Menus.push_back(i18n("&Learning"));

  menuBar()->insertItem(i18n("&Options"), opts_menu);
  used_Menus.push_back(i18n("&Options"));

  ///////////////////////////////////////////////////////////////////
  // INSERT YOUR APPLICATION SPECIFIC MENUENTRIES HERE

  menuBar()->insertSeparator();
  menuBar()->insertItem(i18n("&Help"), help_menu);

  ///////////////////////////////////////////////////////////////////
  // CONNECT THE SUBMENU SLOTS WITH SIGNALS

  CONNECT_CMD(file_menu);
  CONNECT_CMD(edit_menu);
//  CONNECT_CMD(view_menu);
//  CONNECT_CMD(learn_menu);
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


  toolBar()->insertButton(QPixmap(EA_KDEDATADIR("", "kvoctrain/open.xpm")), ID_FILE_OPEN, true, i18n("Open File"));
  file_open_popup= new QPopupMenu();
  QString accel;
  for (uint i = 0 ; i < recent_files.count(); i++){
    accel.setNum (i);
    accel.insert (0, "&");
    accel += "  ";
    accel += recent_files[i];
    file_open_popup->insertItem(accel, i);
  }
  connect(file_open_popup, SIGNAL(activated(int)), SLOT(slotFileOpenRecent(int)));
  toolBar()->setDelayedPopup(ID_FILE_OPEN, file_open_popup);

  toolBar()->insertButton(QPixmap(EA_KDEDATADIR("", "kvoctrain/save.xpm")), ID_FILE_SAVE, true, i18n("Save File"));
  toolBar()->insertSeparator();
  toolBar()->insertButton(QPixmap(EA_KDEDATADIR("", "kvoctrain/copy.xpm")), ID_EDIT_COPY, true, i18n("Copy"));
  toolBar()->insertButton(QPixmap(EA_KDEDATADIR("", "kvoctrain/paste.xpm")), ID_EDIT_PASTE, true, i18n("Paste"));

  toolBar()->insertSeparator();
  QStrList slist;
  toolBar()->insertCombo (&slist, ID_TLESSON, false,
                          SIGNAL(highlighted(int)), this, SLOT(slotChooseLesson(int)),
                          true, i18n("Choose current lesson"), 160);
  lessons = toolBar()->getCombo(ID_TLESSON);
  lessons->setFocusPolicy(QWidget::NoFocus);

  toolBar()->insertSeparator();
  toolBar()->insertButton(QPixmap(EA_KDEDATADIR("", "kvoctrain/searchclip.xpm")), ID_SEARCH_CLIP, true, i18n("Search from clipboard"));
  toolBar()->insertLined ("", ID_TSEARCH, SIGNAL(textChanged(const QString&)), this, SLOT(slotResumeSearch(const QString&)), true, i18n("Smart search"));
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
  KApplication *app=kapp;
  view = 0;

  pdlg = new ProgressDlg (QString(), QString(),
                kvoctrainApp::generateCaption("Loading vocabulary file"));
  pdlg->show();

  if (name != "") {
    doc = new kvoctrainDoc(this, name, separator, &paste_order);
    if (doc) {
      connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
      addRecentFile(name);
      slotModifiedDoc(false);
    }
  }
  else {
    if (recent_files.count() > 0)
      doc = new kvoctrainDoc(this, recent_files[0], separator, &paste_order);
    else
      doc = new kvoctrainDoc(this, "", separator, &paste_order);
    if (doc)
      connect (doc, SIGNAL (docModified(bool)), this, SLOT(slotModifiedDoc(bool)));
    slotModifiedDoc(false);
  }
  removeProgressBar();

  loadDocProps(doc);

  if (doc->numLangs() == 0)
    doc->appendLang("en");
  if (!doc->getTitle().isEmpty())
    setCaption(kvoctrainApp::generateCaption(doc->getTitle()), doc->isModified());
  else
    setCaption(kvoctrainApp::generateCaption(""), doc->isModified());

  view = new kvoctrainView(app, doc, langset, gradecols, this);

  view->setResizer (header_resizer);
  view->getTable()->setFont(tablefont);
//  view->getTable()->updateViewPort();
  connect( view, SIGNAL(vSliderTrack(int)),
           this, SLOT(slotVSliderTrackInfo (int)));
  connect( view, SIGNAL(vSliderPressed(bool, int)),
           this, SLOT(slotVSliderPressed (bool, int)));
  setView(view);
  connect(view->getTable(), SIGNAL(cellMoved(int, int, int)),
          this, SLOT(slotTagEntry(int, int, int)) );

  ////////////////////////////////////////////////////////////////////
  // prepare dock window in panel
  slotStatusMsg(IDS_DEFAULT);

  KConfig *config = EA_KappGetConfig;
  config->setGroup(CFG_GENERAL);
  bool first_time = config->readNumEntry(CFG_FIRST_TIME, 1);
  if (first_time)
   /* showAboutDialog() */ ;
}
