/***************************************************************************

    $Id$

                         main part of kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.6  2001/10/20 12:07:44  arnold
    removed aboutdialog files

    Revision 1.5  2001/10/20 00:58:26  waba
    * Selection fixes
    * Compile fixes

    Revision 1.4  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.3  2001/10/13 21:17:59  arnold
    tested and fixed changes from previous cvs update

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:36:34  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#ifndef KVOCTRAIN_H
#define KVOCTRAIN_H 
 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include files for QT
#include <qpainter.h>
#include <qcombobox.h>

// include files for KDE 
#include <kapp.h> 
#include <kmainwindow.h>
#include <kaccel.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kiconloader.h>

#include <kmessagebox.h>

// application specific includes
#include "kvoctrainview.h"
#include "kv_resource.h"
#include "langset.h"
#include "QueryManager.h"
#include "queryoption-dialogs/GroupOptPage.h"

class QTimer;
class KLined;
class ProgressDlg;
class kvoctrainDoc;
class QueryDlgBase;

class KLineEdit;
class KComboBox;
class KProgress;

/**
  * This Class is the base class for your application. It sets up the main
  * window and reads the config file as well as providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class kvoctrainView is
  * created which creates your view.
  */
class kvoctrainApp : public KMainWindow 
{
  Q_OBJECT

public:
  /** construtor */
  kvoctrainApp(const QString &name = QString::null);
  /** destructor */
  ~kvoctrainApp();
  /** returns true, if running in query mode now */
  bool isQueryMode() const
    { return querymode; }
  /** initMenuBar creates the menu_bar and inserts the menuitems */
  void initMenuBar();
  /** this creates the toolbars. Change the toobar look and add new toolbars in this
    * function */
  void initToolBar();
  /** setup the statusbar */
  void initStatusBar();
  void clearStatusBar();
  /** setup the mainview*/
  void initView(const QString &name);
  /** save the app-specific options on slotAppExit or by an Options dialog */
  void saveOptions(bool all);
  /** read the app-specific options on init() or by an Options dialog */
  void readOptions();
  /** saves the window properties for each open window during session end to the session config file, including saving the currently
  * opened file by a temporary filename provided by KApplication.
  * @see KMainWindow#saveProperties
  */
  virtual void saveProperties(KConfig* );
  /** reads the session config file and restores the application's state including the last opened files and documents by reading the
  * temporary files saved by saveProperties()
  * @see KMainWindow#readProperties
  */
  virtual void readProperties(KConfig* );

  /** enable menuentries/toolbar items */
  void enableCommand(int id_);
  /** disable menuentries/toolbar items */
  void disableCommand(int id_);
  void keyPressEvent( QKeyEvent *e );
  void keyReleaseEvent( QKeyEvent *e );
  /** add a opened file to the recent file list and update recent_file_menu
		*/
  void addRecentFile(const QString &file);

  /** Whether the user has selected a range of items. **/
  bool hasSelection();
  
 signals:
  void progressChanged (kvoctrainDoc *, int curr_percent);

 public slots:
  void slotModifiedDoc(bool mod);

  /** edit an entry */
  bool slotEditEntry(int row, int col);

  /** select an entry */
  void slotSaveSelection ();
  void slotCancelSelection ();
  void slotSelectAll();
  void slotProgress(kvoctrainDoc*,int);
  void slotResumeSearch(const QString&);
  void slotSearchNext();
  void slotInitSearch();
  void slotCreateLesson(int header);
  void slotCleanVocabulary ();
  void slotChooseLesson(int id);
//  void slotSearchClip();
  void slotSetHeaderProp (int header_and_id);
  void slotHeaderCallBack (int cmd_and_id);
  void slotHeaderStatus (int cmd_and_id);
  void slotHeaderMenu(int header, int x, int y);
  void slotCurrentCellChanged(int row, int col);
  void invokeHelp();
  /** append language to table */
  void slotAppendLang(int header);
  /** exit query mode */
  void slotStopQuery(bool show_view);
  bool queryClose();
  /** overloaded for Message box on last window exit */
  bool queryExit();
  /** set up options */
  void slotGeneralOptionsPage(int index);
  void slotGeneralOptions();
  void slotQueryOptions();
  void slotSaveOptions();
  void slotDocProps();
  void slotDocPropsLang();
  void slotShowStatist();
  void slotCreateRandom();
  /** setup edit submenu */
  void aboutToShowEdit();
  void aboutToShowOptions();
  void aboutToShowVocabulary();
  void aboutToShowLearn();
  /** setup file submenu */
  void aboutToShowFile();
  /** switch argument for slot selection by menu or toolbar ID */
  void commandCallback(int id_);
  /** switch argument for Statusbar help entries on slot selection */
  void statusCallback(int id_);
  /** starts random query mode */
  void slotRestartQuery();
  void slotStartTypeQuery(int col, QString type);
  void slotStartPropertyQuery(int col, QueryType property);
  void slotStartQuery(QString trans, QString org);

  void slotTimeOutRandomQuery();
  void slotTimeOutMultipleChoice();
  void slotTimeOutQuery();
  void slotTimeOutType();
  void slotTimeOutProperty();

  void slotTimeOutBackup();

  /** open a new application window */
  void slotFileNew();
  /** open a document */
  void slotFileOpen();
  /** merge a document */
  void slotFileMerge();
  /** save a document */
  void slotFileSave();
  /** save a document under a different filename*/
  void slotFileSaveAs();
  void slotFileQuit();
  /* disconnect clipboard */
  void clipboardChanged();
  /** put the marked text/object into the clipboard*/
  void slotEditCopy();
  void slotSmartSearchClip();
  void slotAppendRow();
  void slotRemoveRow();
  void slotEditRow();
  /** paste the clipboard into the document*/
  void slotEditPaste();
  /** toggle the toolbar*/
  void slotViewToolBar();
  /** toggle the statusbar*/
  void slotViewStatusBar();
  /** change the status message to text */
  void slotStatusMsg(const QString &text);
  /** change the status message of the whole statusbar temporary */
  void slotStatusHelpMsg(const QString &text);
  /** opens a file from the recent files menu */
  void slotFileOpenRecent(int id_);
public:
  void removeProgressBar ();
  void prepareProgressBar ();
  void fillLessonBox(kvoctrainDoc *the_doc);
  void loadDocProps(kvoctrainDoc *the_doc);
  void saveDocProps(kvoctrainDoc *);
  static QString generateCaption (const QString &title, bool force_kde2 = false);
  /** exits the application */
  static vector<int> getCsvOrder(kvoctrainDoc *doc, QStringList *paste_order);

//  void doSM();

private:
  /** the recent file menu containing the last five opened files */
  QPopupMenu *recent_files_menu;
  /** contains the recently used filenames */
  QStringList recent_files;
  QString lastPixName;
  /** file_menu contains all items of the menubar entry "File" */
  QPopupMenu *file_menu;
  /** opts_menu contains all items of the menubar entry "Optins" */
  QPopupMenu *opts_menu;
  /** edit_menu contains all items of the menubar entry "Edit" */
  QPopupMenu *edit_menu;
  QPopupMenu *learn_menu;
  /** view_menu contains all items of the menubar entry "View" */
//  QPopupMenu *view_menu;
  /** voc_menu contains all items of the menubar entry "Vocabulary" */
  QPopupMenu *voc_menu;
  /** help_menu contains all items of the menubar entry "Help" */
  QPopupMenu *help_menu;
  /** view is the main widget which represents your working area. The View
    * class should handle all events of the view widget.  It is kept empty so
    * you can create your view according to your application's needs by
    * changing the view class. */
  kvoctrainView *view;
  /** doc represents your actual document and is created only once. It keeps
    * information such as filename and does the serialization of your files.
    */
  kvoctrainDoc *doc;
  /** flag if toolbar is visible or not. Used for kconfig and checking the
    * view-menu entry view toolbar. bViewStatusbar does the same for the
    * statusbar.
    */
  bool bViewToolbar;
  bool bViewStatusbar;
  /** used for KConfig to store and restore menubar position. Same with
    * tool_bar_pos. If more menubars or toolbars are created, you should add
    * positions as well and implement them in saveOptions() and readOptions().
    */
  KToolBar::BarPosition  tool_bar_pos;

  friend class kvoctrainView;

  vector<QueryEntryRef> random_expr1, random_expr2;
  QuerySelection queryList;

  QTimer          *qtimer;
  QTimer          *btimer;
  int              maxqueryTime;
  int              backupTime;
  bool             smartAppend;
  bool             querymode;
  bool             querying;
  bool             swap_querydir;
  bool             expire;
  bool             block;
  QString          def_lang;
  QString          separator;
  QPopupMenu*      file_open_popup;
  QPopupMenu      *header_m;
  int              act_query_col;
  QString          act_query_trans;
  QString          act_query_org;
  LangSet          langset;
  int              searchpos;
  QString          searchstr;
  KLineEdit       *searchLine;
  KComboBox       *lessons;
  int              act_lesson;
  QStringList      paste_order;
  bool             useCurrent;
  int              entriesPerLesson;
  QFont            tablefont;
  QueryManager     querymanager;
  bool             controlActive;
  bool             shiftActive;
  bool             altActive;
  GradeCols        gradecols;
  ProgressDlg     *pdlg;
  KProgress       *pbar;
  QLabel          *vslide_label;
  QLabel          *pron_label;
  QLabel          *rem_label;
  QLabel          *type_label;
  QueryDlgBase    *queryDlg;
  int              num_queryTimeout;
  int              query_cycle;
  int              query_num;
  int              query_startnum;
  vector<PreSetting>  presettings;
  QueryType        queryType;
  kvq_timeout_t    type_querytimeout;
  int              showcounter;
  bool             autosaveopts;
};
 
#endif // KVOCTRAIN_H

