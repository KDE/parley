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

#include <kxmlguiwindow.h>
#include <kaction.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <krandomsequence.h>

#include "query-dialogs/kvtquerymanager.h"
#include "kvttablemodel.h"
#include "kvtsortfiltermodel.h"
#include "kvttableview.h"
#include "kvtlanguages.h"
#include "kvtlessonmodel.h"
#include "kvtlessonview.h"

#define IDS_DEFAULT I18N_NOOP("Ready.")



//ugly :( make a nice enum in KVTQuery
#define START_QUERY                 1
#define START_MULTIPLE              2
#define START_ARTICLE               3
#define START_VERB                  4
#define START_ADJECTIVE             5
#define START_ADVERB                6
#define START_SYNONYM               7
#define START_ANTONYM               8
#define START_EXAMPLE               9
#define START_PARAPHRASE           10



class QProgressBar;
class QSplitter;

class KLineEdit;
class KComboBox;
class KRecentFilesAction;
class KSelectAction;

class KEduVocDocument;

class EntryDlg;
class KVTNewStuff;

/**
  * This Class is the base class for your application. It sets up the main
  * window and reads the config file as well as providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class kvoctrainView is
  * created which creates your view.
  */
class KVocTrainApp : public KXmlGuiWindow
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

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();
    /** Make sure, the lesson is visible - if combo is set to
      1. all, no problem
      2. in query -> if current not in query change combo?
      3. current -> make the lesson current */
    void makeLessonVisibleInTable(int lessonIndex);

    /** saves the window properties for each open window during session end to the session config file, including saving the currently
    * opened file by a temporary filename provided by KApplication.
    * @see KXmlGuiWindow#saveProperties
    */
    virtual void saveProperties(KConfigGroup &);
    /** reads the session config file and restores the application's state including the last opened files and documents by reading the
    * temporary files saved by saveProperties()
    * @see KXmlGuiWindow#readProperties
    */
    virtual void readProperties(const KConfigGroup &);

signals:
    void progressChanged(KEduVocDocument *, int curr_percent);

public slots:
    void keyPressEvent(QKeyEvent *e);
//  void keyReleaseEvent( QKeyEvent *e );
    void slotModifiedDoc(bool mod);
    void slotCurrentChanged(const QModelIndex &, const QModelIndex &);
//  void slotSelectEntry (int row, int col, int key_state);

    /** edit an entry */
    void slotEditEntry(int row, int col); ///@todo get rid of
    void slotEditEntry2(const QModelIndex &);
    void slotEditCallBack(int res);

    void setDataEntryDlg(int row, int col);
    void removeEntryDlg();

    /** select an entry */
    void slotSaveSelection();
    void slotCancelSelection();
    void slotSelectAll();
    void slotProgress(KEduVocDocument*,int);

    // was never used: void slotCreateLesson(int header);
    void slotCleanVocabulary();
    // not used: void slotChooseLesson(int id);
    /** selection of a lesson from the lesson list on the left */
    void slotCurrentLessonChanged(int currentLesson);
    /** selection of which lessons should be in the table to edit */
    void slotLessonSelectionComboChanged(int index);
    /** the lessons in query were changed */
    void slotLessonCheckboxesChanged(const QModelIndex &, const QModelIndex &);

    /** append language to vocabulary - creates a new column */
    void slotAppendLanguage(int index);
    /** assign language to vocabulary column - changes the column title */
    void slotAssignLanguage(QAction *);
    /** @todo Frederik - if possible replace slotAssignLanguage with this simpler one */
    void slotAssignLanguage2(int column, int languageIndex);    /** remove language from vocabulary */
    void slotRemoveLanguage(int index);
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
    /** set up vocabulary and learning submenus */
    void aboutToShowVocabAppendLanguage();
    void aboutToShowVocabSetLanguage();
    /** create the remove language menu */
    void aboutToShowVocabRemoveLanguage();
    void aboutToShowLearn();

    /** starts query mode */
    void slotLearningMapperTriggered(const QString &);

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

    /** Create a new entry */
    void slotNewEntry();

    /** Delete an entry */
    void slotDeleteEntry();

    void slotEditRow();
    /** paste the clipboard into the document*/
    void slotEditPaste();

    /** change the status message to text */
    void slotStatusMsg(const QString &text);
    /** change the status message of the whole statusbar temporary */
    void slotStatusHelpMsg(const QString &text);

    void slotConfigShowSearch();

    void slotResumeQuery() { m_queryManager->slotResumeQuery(); }
    void slotResumeQueryMC() { m_queryManager->slotResumeQueryMC(); }

public:
    void removeProgressBar();
    void prepareProgressBar();
    void loadDocProps();
    void saveDocProps(KEduVocDocument *);

private:
    /** used for the query slotStartXYZ - to check if query is not empty */
    bool queryIsEmpty();
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
    QAction* vocabShowSearchBar;

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
    KVTLessonView *m_lessonView;

    /** Divides the main window to have the lessons and the table. */
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

    KVTLanguageList      m_languages;

    QString              m_textToFind;

    KLineEdit           *m_searchLine;

    QueryManager        *m_queryManager;
    bool                 controlActive;

    EntryDlg            *entryDlg;

    QProgressBar        *pbar;
    QLabel              *m_pronunciationStatusBarLabel;
    QLabel              *m_remarkStatusBarLabel;
    QLabel              *m_typeStatusBarLabel;
    KVTNewStuff         *m_newStuff;
};

#endif // KVOCTRAIN_H

