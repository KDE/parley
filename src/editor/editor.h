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

#include "parleydocument.h"

#include "scripts/scripting/parley.h"

#include <KXmlGuiWindow>
#include <KUrl>
#include <QItemSelection>
#include <QModelIndex>
#include <QList>

#define IDS_DEFAULT I18N_NOOP("Ready.")

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
class Translator; ///@todo remove unneccessary items

class Editor : public KXmlGuiWindow
{
    Q_OBJECT

public:
    Editor(ParleyMainWindow* parent);

    /**
     * setup the action (menus etc)
     */
    void initActions();

    /** setup the main model*/
    void initModel();

    /** setup the main view*/
    void initView();

    void initDockWidgets();

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();

    /**
     * Load enabled scripts (from configuration parleyrc)
     */
    void initScripts();

    /**
     * Return the top-level main window
     */
    ParleyMainWindow* mainWindow();

public slots:

    /**
     * Edit languages contained in the document.
     * This includes adding/removing languages, 
     * editing articles, personal pronouns and tenses.
     */
    void slotLanguageProperties();

    void startPractice();

    void slotConfigShowSearch();

    /**
     *  Display script manager (open a new window / or Settings window)
     */
    void slotShowScriptManager();

    void setTableFont(const QFont& font);

private slots:
    void slotConfigOldPractice(bool old);

signals:
    void signalSetData( const QList<int>& entries, int currentTranslation);

private:

    /**
     * Set the current doc (after creating a new one or opening a file)
     */
    void updateDocument();

    ParleyMainWindow* m_mainWindow;
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

    /// dock widgets to display lessons, word types, ...
    LessonView *m_lessonView;
    LessonModel *m_lessonModel;

    WordTypeView *m_wordTypeView;
    WordTypeModel *m_wordTypeModel;

    LeitnerView *m_leitnerView;
    LeitnerModel *m_leitnerModel;

    KLineEdit           *m_searchLine;

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

#endif // EDITOR_H

