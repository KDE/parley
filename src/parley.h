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

#ifndef PARLEY_H
#define PARLEY_H

#include "parleydocument.h"

#include <KXmlGuiWindow>
#include <KUrl>
#include <QItemSelection>
#include <QModelIndex>

#define IDS_DEFAULT I18N_NOOP("Ready.")

class PracticeManager;
class EntryDlg;
class LessonDockWidget;
class WordTypeWidget;

class KEduVocDocument;
class KLineEdit;
class KComboBox;
class KMessageBox;
class KRecentFilesAction;
class KAction;
class KActionMenu;
class QProgressBar;
class QSplitter;
class QLabel;
class VocabularyView;
class VocabularyModel;

/**
  * This Class is the base class for your application. It sets up the main
  * window and reads the config file as well as providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class kvoctrainView is
  * created which creates your view.
  */
class ParleyApp : public KXmlGuiWindow
{
    Q_OBJECT

public:
    /** construtor with filename to open */
    ParleyApp(const KUrl &filename = KUrl());
    /** construtor */
//     ParleyApp();

    /** destructor */
    ~ParleyApp();
    void initActions();

    /** setup the statusbar */
    void initStatusBar();
    /** setup the main model*/
    void initModel();

    /** setup the main view*/
    void initView();

    void initDockWidgets();

    /** save the app-specific options on slotAppExit or by an Options dialog */
    void saveOptions();

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();


public slots:
//     void keyPressEvent(QKeyEvent *e);
//  void keyReleaseEvent( QKeyEvent *e );
    void slotUpdateWindowCaption();

    /** select an entry */
    void slotCancelSelection();
    void slotSelectAll();

//     void slotCleanVocabulary();

    /** append language to vocabulary - creates a new column */
    void slotEditLanguages();

    bool queryClose();
    /** overloaded for Message box on last window exit */
    bool queryExit();
    /** set up options */
    void slotGeneralOptions();
    void slotApplyPreferences();
    void slotLanguageProperties();
    void slotDocumentProperties();
    void slotShowStatistics();

    void startPractice();
    void configurePractice();

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
//     void slotStatusMsg(const QString &text);
    /** change the status message of the whole statusbar temporary */
//     void slotStatusHelpMsg(const QString &text);

    void slotConfigShowSearch();

    /**
     * Close the entry dialog
     */
    void removeEntryDlg();

signals:
    void signalSetData( const QList<int>& entries, int currentTranslation);

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
    void slotCurrentLessonChanged(int newLesson);

    /** edit an entry */
    void slotEditEntry();

private:

    /**
     * Set the current doc (after creating a new one or opening a file)
     */
    void updateDocument();

    /**
     * When the selection changes, the editors need to be notified
     */
    void updateEditWidgets();

    // KAction pointers to enable/disable actions
    KRecentFilesAction* m_recentFilesAction;
    KAction* m_deleteEntriesAction;
    KAction* m_vocabShowSearchBarAction;
    KActionMenu* m_vocabularyColumnsActionMenu;

    QString lastPixName;

    VocabularyModel *m_vocabularyModel;
    VocabularyView *m_vocabularyView;

    QWidget *m_searchWidget;

    /** m_document is the current vocabulary document. */
    ParleyDocument   *m_document;

    /// dock widgets to display lessons, word types, ...
    LessonDockWidget *m_lessonDockWidget;
//     WordTypeWidget   *m_wordTypeWidget;

    /** The models to represent the data of m_doc */
//     KVTTableModel       *m_tableModel;
//     KVTSortFilterModel  *m_sortFilterModel;

    KLineEdit           *m_searchLine;

    EntryDlg            *m_entryDlg;

    QLabel              *m_pronunciationStatusBarLabel;
    QLabel              *m_remarkStatusBarLabel;
    QLabel              *m_typeStatusBarLabel;

    friend class LessonDockWidget;
    friend class ParleyDocument;
};

#endif // PARLEY_H

