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

#include <QPainter>
#include <QLabel>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QTreeView>
#include <QPushButton>
#include <QSignalMapper>

#include <kmainwindow.h>
#include <kaction.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <krandomsequence.h>

#include "query-dialogs/QueryDlgBase.h"
#include "kvtnewstuff.h"
#include "kvttablemodel.h"
#include "kvtsortfiltermodel.h"
#include "kvttableview.h"
#include "kvtlanguages.h"
#include "kvtlessonmodel.h"
#include "kvtlessonview.h"

#define IDS_DEFAULT I18N_NOOP("Ready.")

class QTimer;
class QProgressBar;
class QSplitter;

class KLineEdit;
class KComboBox;
class KRecentFilesAction;
class KSelectAction;

class ProgressDlg;
class KEduVocDocument;
class SimpleQueryDlg;
class MCQueryDlg;
class VerbQueryDlg;
class RandomQueryDlg;
class AdjQueryDlg;
class ArtQueryDlg;
class EntryDlg;
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
  /** setup the lesson list and its buttons */
  QWidget* initLessonList(QWidget *parent);
  /** setup the main view*/
  void initView();
  /** save the app-specific options on slotAppExit or by an Options dialog */
  void saveOptions();
  /** read the app-specific options on init() or by an Options dialog */
  void readOptions();

  void saveLanguages();
  void readLanguages();

  /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
  void updateTableFilter();

  /** saves the window properties for each open window during session end to the session config file, including saving the currently
  * opened file by a temporary filename provided by KApplication.
  * @see KMainWindow#saveProperties
  */
  virtual void saveProperties(KConfigGroup & );
  /** reads the session config file and restores the application's state including the last opened files and documents by reading the
  * temporary files saved by saveProperties()
  * @see KMainWindow#readProperties
  */
  virtual void readProperties(const KConfigGroup & );

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
  /** Use this to filter out stuff - access from search line. */
  void slotSearch(const QString&);
  //void slotSearchNext();
  //void slotInitSearch();
  void slotCreateLesson(int header);
  void slotCleanVocabulary ();
  void slotChooseLesson(int id);
  /** selection of a lesson from the lesson list on the left */
  void slotCurrentLessonChanged(const QModelIndex &, const QModelIndex &);
  /** selection of which lessons should be in the table to edit */
  void slotLessonSelectionComboChanged(int index);


  /** append language to vocabulary */
  void slotAppendLanguage(int index);
  /** assign language to vocabulary column */
  void slotAssignLanguage(QAction *);
  /** remove language from vocabulary */
  void slotRemoveLanguage(int index);
  /** exit query mode */
  void slotStopQuery(bool show_view);
  bool queryClose();
  /** overloaded for Message box on last window exit */
  bool queryExit();
  /** set up options */
  void slotGeneralOptionsPage(int index);
  void slotGeneralOptions();
  void slotApplyPreferences();
  void slotDocumentProperties();
  void slotDocPropsLang();
  void slotShowStatistics();
  void slotCreateRandom();
  /** set up vocabulary and learning submenus */
  void aboutToShowVocabAppendLanguage();
  void aboutToShowVocabSetLanguage();
  void aboutToShowVocabRemoveLanguage();
  void aboutToShowLearn();

  /** starts random query mode */
  void slotLearningMapperTriggered(const QString &);
  void slotResumeQuery();
  void slotResumeQueryMC();
  void slotRestartQuery();
  void slotStartTypeQuery(int col, const QString & type);
  void slotStartPropertyQuery(int col, KVTQuery::QueryType property);
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
//  void slotSmartSearchClip();
  void slotAppendRow();
  void slotRemoveRow();
  void slotEditRow();
  /** paste the clipboard into the document*/
  void slotEditPaste();

  /** change the status message to text */
  void slotStatusMsg(const QString &text);
  /** change the status message of the whole statusbar temporary */
  void slotStatusHelpMsg(const QString &text);

  void slotConfigShowSearch();

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
  QAction* fileNew;
  QAction* fileOpen;
  QAction* fileOpenExample;
  QAction* fileGHNS;
  KRecentFilesAction* fileOpenRecent;
  QAction* fileMerge;
  QAction* fileSave;
  QAction* fileSaveAs;
  QAction* filePrint;
  QAction* fileQuit;

  QAction* editCopy;
  QAction* editPaste;
  QAction* editSelectAll;
  QAction* editClearSelection;

  QAction* editAppend;
  QAction* editEditEntry;
  QAction* editDelete;
  QAction* editSaveSelectedArea;

  QAction* vocabShowStatistics;
  QAction* vocabAssignLessons;
  QAction* vocabCleanUp;
  KSelectAction* vocabAppendLanguage;
  KSelectAction* vocabSetLanguage;
  KSelectAction* vocabRemoveLanguage;
  QAction* vocabDocumentProperties;
  QAction* vocabLanguageProperties;
  QAction* vocabLessons;
  QAction* vocabSearch;

  QAction* learningResumeQuery;
  QAction* learningResumeMultipleChoice;

  QAction* configToolbar;
  QAction* configNotifications;
  QAction* configApp;

  QString lastPixName;

  QMenu *learningMenu;
  QSignalMapper *m_learningMapper;


//   QPushButton *m_buttonNewLesson;
//   QPushButton *m_buttonRenameLesson;
//   QPushButton *m_buttonDeleteLesson;

  /** m_editCombo selects which lessons to display in m_tableView (the main table) */
  KComboBox *m_lessonSelectionCombo;

  /** m_tableView is the main widget which is the table that represents your working area. */
  KVTTableView *m_tableView;

  /** m_lessonView is the lesson list at the left side. */
  //KVTLessonView *m_lessonView;
  KVTLessonView *m_lessonView;

  QSplitter *m_mainSplitter;

  QWidget *m_searchWidget;

  /** m_doc represents your vocabulary document. It keeps
    * information such as filename and does the serialization of your files.
    */
  KEduVocDocument *m_doc;

  /** The models to represent the data of m_doc */
  KVTTableModel       *m_tableModel;
  KVTLessonModel      *m_lessonModel;
  KVTSortFilterModel  *m_sortFilterModel;

  QueryEntryList       random_expr1;
  QueryEntryList       random_expr2;
  // Vectors for use in Leitner style learning. There is no
  // correct_0_times, we simply reuse random_expr1.
  QueryEntryList       correct_1_times;
  QueryEntryList       correct_2_times;
  QueryEntryList       correct_3_times;
  QuerySelection       queryList;

  int                  random_query_nr;
  QString              def_lang;
  int                  act_query_col;
  QString              act_query_trans;
  QString              act_query_org;
  KVTLanguages         m_languages;
  //int                  searchpos;
  QString              m_textToFind;
  /// @todo Move above table...
  KLineEdit           *m_searchLine;
  KComboBox           *m_lessonsComboBox;
  int                  m_currentLesson;
  KVTQuery             querymanager;
  bool                 controlActive;
  ProgressDlg         *pdlg;
  QProgressBar        *pbar;
  QLabel              *m_pronunciationStatusBarLabel;
  QLabel              *m_remarkStatusBarLabel;
  QLabel              *m_typeStatusBarLabel;
  SimpleQueryDlg      *simpleQueryDlg;
  EntryDlg            *entryDlg;
  MCQueryDlg          *mcQueryDlg;
  VerbQueryDlg        *verbQueryDlg;
  RandomQueryDlg      *randomQueryDlg;
  AdjQueryDlg         *adjQueryDlg;
  ArtQueryDlg         *artQueryDlg;
  int                  num_queryTimeout;
  int                  query_cycle;
  int                  query_num;
  int                  query_startnum;
  KVTQuery::QueryType  m_queryType;
  KRandomSequence      m_randomSequence;
  KVTNewStuff         *m_newStuff;
};

#endif // KVOCTRAIN_H

