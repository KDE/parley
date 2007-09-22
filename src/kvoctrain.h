/***************************************************************************

                         main part of Parley

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include <KXmlGuiWindow>
#include <KUrl>
#include <QItemSelection>
#include <QModelIndex>

#define IDS_DEFAULT I18N_NOOP("Ready.")

class KVTTableModel;
class KVTSortFilterModel;
class KVTTableView;
class KVTLessonModel;
class KVTLessonView;
class QueryManager;
class EntryDlg;
class KVTNewStuff;

class KEduVocDocument;
class KLineEdit;
class KComboBox;
class KMessageBox;
class KRecentFilesAction;
class KAction;
class QProgressBar;
class QSplitter;
class QLabel;

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

    void removeProgressBar();
    void prepareProgressBar();

signals:
    void progressChanged(KEduVocDocument *, int curr_percent);

public slots:
    void keyPressEvent(QKeyEvent *e);
//  void keyReleaseEvent( QKeyEvent *e );
    void slotModifiedDoc(bool mod);

    /** select an entry */
    void slotSaveSelection();
    void slotCancelSelection();
    void slotSelectAll();
    void slotProgress(KEduVocDocument*,int);

    void slotCleanVocabulary();

    /** append language to vocabulary - creates a new column */
    void slotEditLanguages();

    bool queryClose();
    /** overloaded for Message box on last window exit */
    bool queryExit();
    /** set up options */
    void slotGeneralOptionsPage(int index);
    void slotGeneralOptions();
    void slotApplyPreferences();
    void slotLanguageProperties();
    void slotDocumentProperties();
    void slotDocPropsLang();
    void slotShowStatistics();

    void startPractice();
    void configurePractice();

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

    /** Cut an entry */
    void slotCutEntry();

    /** Delete an entry */
    void slotDeleteEntry();


    /** paste the clipboard into the document*/
    void slotEditPaste();

    /** change the status message to text */
    void slotStatusMsg(const QString &text);
    /** change the status message of the whole statusbar temporary */
    void slotStatusHelpMsg(const QString &text);

    void slotConfigShowSearch();

    /**
     * Close the entry dialog
     */
    void removeEntryDlg();

private slots:
    /**
     * To update the statusbar and the entry dialog
     * @param
     * @param
     */
    void slotCurrentChanged(const QModelIndex &, const QModelIndex &);
    /**
     * To update the entry dialog
     * @param
     * @param
     */
    void slotSelectionChanged(const QItemSelection &, const QItemSelection &);

    /**
     * To update e.g. context menu entries when the table view changed
     */
    void slotCurrentLessonChanged();

    /** edit an entry */
    void slotEditEntry();

private:
    /**
     * Start the wizard to help set up the doc.
     */
    void newDocumentWizard();
    /**
     * Add some pre defined types and usages.
     */
    void initializeDefaultGrammar();
    /**
     * When starting the first time, create some entries to get started.
     */
    void createExampleEntries();

    ///@todo these are refered to somewhere. should probably be avoidable.
    // KAction pointers to enable/disable actions
    KRecentFilesAction* fileOpenRecent;
    KAction* editDelete;
    KAction* vocabShowSearchBar;

    QString lastPixName;

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

    QString              m_textToFind;

    KLineEdit           *m_searchLine;

    QueryManager        *m_queryManager;
//     bool                 controlActive;

    EntryDlg            *entryDlg;

    QProgressBar        *pbar;
    QLabel              *m_pronunciationStatusBarLabel;
    QLabel              *m_remarkStatusBarLabel;
    QLabel              *m_typeStatusBarLabel;
    KVTNewStuff         *m_newStuff;
};

#endif // KVOCTRAIN_H

