/***************************************************************************
    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EDITOR_H
#define EDITOR_H
#if 0

#include "parleydocument.h"

// #include "scripts/scriptmanager.h"
#include "scripts/scripting/parley.h"
// #include "scripts/translator.h"

#include <KXmlGuiWindow>
#include <KUrl>
#include <QItemSelection>
#include <QModelIndex>
#include <QList>

#define IDS_DEFAULT I18N_NOOP("Ready.")

class EntryDlg;

class KLineEdit;
class KRecentFilesAction;
class KAction;
class KActionMenu;
class QLabel;
class QDockWidget;
class VocabularyView;
class VocabularyModel;
class VocabularyFilter;
class LessonView;
class WordTypeView;
class LeitnerView;
class LessonModel;
class WordTypeModel;
class LeitnerModel;
class ConjugationWidget;
class SummaryWordWidget;
class ScriptManager;
class Translator;

/**
  * This Class is the base class for your application. It sets up the main
  * window and reads the config file as well as providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class kvoctrainView is
  * created which creates your view.
  */
class Editor : public KXmlGuiWindow
{
    Q_OBJECT

public:
    /** construtor with appName (executable name) and filename to open */
    explicit Editor(const QString& appName, const KUrl &filename = KUrl());

    /**
     * setup the action (menus etc)
     */
    void initActions();

    /** setup the statusbar */
    void initStatusBar();
    /** setup the main model*/
    void initModel();

    /** setup the main view*/
    void initView();

    /** setup the welcome screen */
    void initWelcomeScreen();

    void initDockWidgets();

    /** save the app-specific options on slotAppExit or by an Options dialog */
    void saveOptions();

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();

    /**
     * Load enabled scripts (from configuration parleyrc)
     */
    void initScripts();

    /**
     * Return the ParleyDocument member object
     * @return member m_document
     */
    ParleyDocument* parleyDocument();

public slots:
    /** Update the title bar of the main window with the current document */
    void slotUpdateWindowCaption();

    /**
     * Edit languages contained in the document.
     * This includes adding/removing languages, 
     * editing articles, personal pronouns and tenses.
     */
    void slotLanguageProperties();

    /** When quitting, ask for confirmation if the doc has not been saved */
    bool queryClose();
    /** overloaded for Message box on last window exit */
    bool queryExit();
    /** set up options */
    void slotGeneralOptions();
    void slotApplyPreferences();

    void slotCloseDocument();

    /** General doc properties like title, author etc */
    void slotDocumentProperties();
    void slotShowStatistics();

    void startPractice();
    void configurePractice();

    void slotConfigShowSearch();

    /**
     *  Display script manager (open a new window / or Settings window)
     */
    void slotShowScriptManager();

    /**
     * Show the tip of the day (force it to be shown)
     */
    void tipOfDay();

    /**
     * Show the tip of the day - the startup version that can be disabled
     */
    void startupTipOfDay();

    /**
     * Removes all grading information from the current document
     */
    void removeGrades();
    
    /**
     * Shows or hides the welcome screen.
     */
    void setShowWelcomeScreen(bool show);
    void hideWelcomeScreen();

private slots:
    void slotConfigOldPractice(bool old);

signals:
    void signalSetData( const QList<int>& entries, int currentTranslation);

protected:
    void closeEvent(QCloseEvent *event);

private:

    /**
     * Set the current doc (after creating a new one or opening a file)
     */
    void updateDocument();

    // KAction pointers to enable/disable actions
    KRecentFilesAction* m_recentFilesAction;
    KRecentFilesAction* m_downloadedFilesAction;
    KAction* m_deleteEntriesAction;
    KAction* m_vocabShowSearchBarAction;
    KActionMenu* m_vocabularyColumnsActionMenu;

    QString lastPixName;

    VocabularyModel *m_vocabularyModel;
    VocabularyView *m_vocabularyView;
    VocabularyFilter *m_vocabularyFilter;

    QWidget *m_searchWidget;

    /** Show a single conjugation and let the user edit it */
    ConjugationWidget *m_conjugationWidget;
    SummaryWordWidget *m_summaryWordWidget;

    /** m_document is the current vocabulary document. */
    ParleyDocument   *m_document;

    /// dock widgets to display lessons, word types, ...
    LessonView *m_lessonView;
    LessonModel *m_lessonModel;

    WordTypeView *m_wordTypeView;
    WordTypeModel *m_wordTypeModel;

    LeitnerView *m_leitnerView;
    LeitnerModel *m_leitnerModel;

    KLineEdit           *m_searchLine;

    EntryDlg            *m_entryDlg;

    QLabel              *m_pronunciationStatusBarLabel;
    QLabel              *m_remarkStatusBarLabel;
    QLabel              *m_typeStatusBarLabel;

    friend class ParleyDocument;
    friend class Scripting::Parley;
    friend class ScriptManager;
    friend class Translator;

    /// the name of the executable
    QString m_appName;

    //Script Manager
    ScriptManager* m_scriptManager;

    ///stores all the translations of a vocabulary word
    Translator* m_translator;

    QList<QDockWidget*> m_dockWidgets;
    QList<bool> m_dockWidgetVisibility;
};

#endif 0
#endif // EDITOR_H

