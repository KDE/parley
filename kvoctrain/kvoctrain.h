/***************************************************************************

                         main part of kvoctrain

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
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


#ifndef KVOCTRAIN_H
#define KVOCTRAIN_H

#include <config.h>

#include <QPainter>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>

#include <kapplication.h>
#include <kmainwindow.h>
#include <kaction.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <krandomsequence.h>

#include "query-dialogs/QueryDlgBase.h"
#include "kvtnewstuff.h"
#include "kvttablemodel.h"
#include "kvttableview.h"
#include "kvtlanguages.h"

class QTimer;
class KLined;
class ProgressDlg;
class KEduVocDocument;
class SimpleQueryDlg;
class MCQueryDlg;
class VerbQueryDlg;
class RandomQueryDlg;
class AdjQueryDlg;
class ArtQueryDlg;
class EntryDlg;

class KLineEdit;
class KComboBox;
class QProgressBar;
class KRecentFilesAction;
class KSelectAction;

class KVTNewStuff;

/**
  * This Class is the base class for your application. It sets up the main
  * window and reads the config file as well as providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class kvoctrainView is
  * created which creates your view.
  */
class KVocTrainApp : public KMainWindow
{
  Q_OBJECT

public:
  /** construtor */
  KVocTrainApp(QWidget *parent = 0);
  /** destructor */
  ~KVocTrainApp();
  void initActions();

  /** setup the statusbar */
  void initStatusBar();
  /** setup the main document*/
  void initDoc();
  /** setup the main model*/
  void initModel();
  /** setup the main view*/
  void initView();
  /** save the app-specific options on slotAppExit or by an Options dialog */
  void saveOptions();
  /** read the app-specific options on init() or by an Options dialog */
  void readOptions();

  void saveLanguages();
  void readLanguages();

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

 signals:
  void progressChanged (KEduVocDocument *, int curr_percent);

 public slots:
  void keyPressEvent( QKeyEvent *e );
  void keyReleaseEvent( QKeyEvent *e );
  void slotModifiedDoc(bool mod);
  void slotCurrentChanged(const QModelIndex &, const QModelIndex &);
//  void slotSelectEntry (int row, int col, int key_state);

  /** edit an entry */
  void slotEditEntry(int row, int col); ///@todo get rid of
  void slotEditEntry2(const QModelIndex &);
  void slotEditCallBack(int res);

  void commitEntryDlg(bool force);
  void setDataEntryDlg (int row, int col);
  void removeEntryDlg();

  /** select an entry */
  void slotSaveSelection ();
  void slotCancelSelection ();
  void slotSelectAll();
  void slotProgress(KEduVocDocument*,int);
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
  //void slotQueryOptions();
  //void slotQueryOptions(int pageindex);
  void slotApplyPreferences();
  void slotDocProps();
  void slotDocPropsLang();
  void slotShowStatist();
  void slotCreateRandom();
  /** set up vocabulary and learning submenus */
  void aboutToShowVocabAppendLanguage();
  void aboutToShowVocabSetLanguage();
  void aboutToShowVocabRemoveLanguage();
  void aboutToShowLearn();

  /** starts random query mode */
  void slotRestartQuery();
  void slotStartTypeQuery(int col, const QString & type);
  void slotStartPropertyQuery(int col, QueryType property);
  void slotStartQuery(const QString & trans, const QString & org, bool create_new);

  void slotTimeOutRandomQuery(QueryDlgBase::Result res);
  void slotTimeOutMultipleChoice(QueryDlgBase::Result res);
  void slotTimeOutQuery(QueryDlgBase::Result res);
  void slotTimeOutType(QueryDlgBase::Result res);
  void slotTimeOutProperty(QueryDlgBase::Result res);


  void slotTimeOutBackup();

  /** open a new application window */
  void slotFileNew();
  /** open a document */
  void slotFileOpen();
  /** opens a file from the recent files menu */
  void slotFileOpenRecent(const KUrl& url);
  /** open a sample document */
  void slotFileOpenExample();
  /** download new vocabularies */
  void slotGHNS();
  void loadFileFromPath(const KUrl &, bool addRecent = true);
  /** merge a document */
  void slotFileMerge();
  /** save a document */
  void slotFileSave();
  /** save a document under a different filename*/
  void slotFileSaveAs();
  void slotFilePrint();
  void slotFileQuit();

  /** put the marked text/object into the clipboard*/
  void slotEditCopy();
  void slotSmartSearchClip();
  void slotAppendRow();
  void slotRemoveRow();
  void slotEditRow();
  /** paste the clipboard into the document*/
  void slotEditPaste();

  /** change the status message to text */
  void slotStatusMsg(const QString &text);
  /** change the status message of the whole statusbar temporary */
  void slotStatusHelpMsg(const QString &text);

public:
  void removeProgressBar();
  void prepareProgressBar();
  void fillLessonBox();
  void loadDocProps();
  void saveDocProps(KEduVocDocument *);
  QList<int> csvOrder();

private:
  void createNewDocument();

  // KAction pointers to enable/disable actions
  KAction* fileNew;
  KAction* fileOpen;
  KAction* fileOpenExample;
  KAction* fileGHNS;
  KRecentFilesAction* fileOpenRecent;
  KAction* fileMerge;
  KAction* fileSave;
  KAction* fileSaveAs;
  KAction* filePrint;
  KAction* fileQuit;

  KAction* editCopy;
  KAction* editPaste;
  KAction* editSelectAll;
  KAction* editClearSelection;
  KAction* editSearchFromClipboard;
  KAction* editAppend;
  KAction* editEditEntry;
  KAction* editDelete;
  KAction* editSaveSelectedArea;

  KAction* vocabShowStatistics;
  KAction* vocabAssignLessons;
  KAction* vocabCleanUp;
  KSelectAction* vocabAppendLanguage;
  KSelectAction* vocabSetLanguage;
  KSelectAction* vocabRemoveLanguage;
  KAction* vocabDocumentProperties;
  KAction* vocabLanguageProperties;
  KAction* vocabLessons;
  KAction* vocabSearch;

  //KAction* learningResumeQuery;
  //KAction* learningResumeMultipleChoice;

  QAction* configToolbar;
  KAction* configNotifications;
  KAction* configApp;
  //KAction* configQueryOptions;

  QString lastPixName;

  QMenu *learn_menu;

  /** m_tableView is the main widget which is the table that represents your working area. */
  KVTTableView *m_tableView;

  /** m_doc represents your vocabulary document. It keeps
    * information such as filename and does the serialization of your files.
    */
  KEduVocDocument *m_doc;

  KVTTableModel *m_tableModel;

  QList<QueryEntryRef> random_expr1;
  QList<QueryEntryRef> random_expr2;
  // Vectors for use in Leitner style learning. There is no
  // correct_0_times, we simply reuse random_expr1.
  QList<QueryEntryRef> correct_1_times;
  QList<QueryEntryRef> correct_2_times;
  QList<QueryEntryRef> correct_3_times;
  QuerySelection queryList;
  QTimer          *btimer;
  int              random_query_nr;
  bool             querymode;
  bool             querying;
  QString          def_lang;
  QMenu            *header_m;
  int              act_query_col;
  QString          act_query_trans;
  QString          act_query_org;
  KVTLanguages     m_languages;
  int              searchpos;
  QString          m_textToFind;
  KLineEdit       *searchLine;
  KComboBox       *m_lessonsComboBox;
  int              m_currentLesson;
  QueryManager     querymanager;
  bool             controlActive;
  ProgressDlg     *pdlg;
  QProgressBar    *pbar;
  QLabel          *vslide_label;
  QLabel          *pron_label;
  QLabel          *rem_label;
  QLabel          *type_label;
  SimpleQueryDlg  *simpleQueryDlg;
  EntryDlg        *entryDlg;
  MCQueryDlg      *mcQueryDlg;
  VerbQueryDlg    *verbQueryDlg;
  RandomQueryDlg  *randomQueryDlg;
  AdjQueryDlg     *adjQueryDlg;
  ArtQueryDlg     *artQueryDlg;
  int              num_queryTimeout;
  int              query_cycle;
  int              query_num;
  int              query_startnum;
  QueryType        queryType;
  KRandomSequence  random;
  KVTNewStuff     *m_newStuff;
  QVBoxLayout     *m_topLayout;
};

#endif // KVOCTRAIN_H

