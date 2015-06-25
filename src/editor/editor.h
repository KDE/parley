/***************************************************************************
    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
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
#include "parleymainwindow.h"

#include "scripts/scripting/parley.h"

#include <keduvocvocabularymodel.h>
#include <keduvocvocabularyfilter.h>

#include <KXmlGuiWindow>

class QLineEdit;
class ScriptManager;

///@brief contains objects for the editor main window

// Models around the Collection / KEduVocDocument
class KEduVocLessonModel;
class KEduVocVocabularyModel;
class KEduVocVocabularyFilter;
class KEduVocWordClassModel;

namespace Editor
{
class KEduVocVocabularyView;
class KEduVocLessonView;
class KEduVocWordTypeView;
class KEduVocInflectionWidget;
class KEduVocComparisonWidget;
class KEduVocSummaryWordWidget;
class KEduVocLatexWidget;
class KEduVocSynonymWidget;

class EditorWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(ParleyMainWindow* parent);
    ~EditorWindow();

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

public slots:
    /**
     * Load enabled scripts (from configuration parleyrc)
     */
    void initScripts();

    /**
     * Edit languages contained in the document.
     * This includes adding/removing languages,
     * editing articles, personal pronouns and tenses.
     */
    void slotLanguagesChanged();

    void slotConfigShowSearch();

    /**
     *  Display script manager (open a new window / or Settings window)
     */
    void slotShowScriptManager();

    void applyPrefs();

    /**
     * Removes all grading information from the current document
     */
    void removeGrades();

    /** when closing the application, save the editor's state */
    void saveState();

    /**
     * Set the current doc (after creating a new one or opening a file)
     */
    void updateDocument(KEduVocDocument *doc);

    /**
     * DBus method for adding words by external apps
     */
    void addWordWithTranslation(const QStringList &w);

    /**
      * Set/Unset Automatic Translation in Prefs
      */
    void setAutomaticTranslation(bool v);

    /**
     * Set m_vocabularyView->m_spellCheck
     */
    void setSpellCheck( int language );

private slots:

    /** Make the search bar visible and focus it */
    void startSearch();

signals:
    void signalSetData(const QList<int>& entries, int currentTranslation);

private:
    ParleyMainWindow  *m_mainWindow;

    QAction* m_vocabShowSearchBarAction;

    KEduVocVocabularyModel *m_vocabularyModel;
    KEduVocVocabularyView *m_vocabularyView;
    KEduVocVocabularyFilter *m_vocabularyFilter;

    QLineEdit *m_searchLine;
    QWidget *m_searchWidget;
    QAction *m_spellCheckMenu;

    /** Show a single conjugation and let the user edit it */
    KEduVocInflectionWidget *m_inflectionWidget;
    KEduVocSummaryWordWidget *m_summaryWordWidget;
    KEduVocComparisonWidget *m_comparisonWidget;
    KEduVocLatexWidget *m_latexWidget;
    KEduVocSynonymWidget *m_synonymWidget;
    KEduVocSynonymWidget *m_antonymWidget;
    KEduVocSynonymWidget *m_falseFriendWidget;

    /// dock widgets to display lessons, word types, ...
    KEduVocLessonView *m_lessonView;
    KEduVocLessonModel *m_lessonModel;

    KEduVocWordClassModel *m_wordTypeModel;
    KEduVocWordTypeView *m_wordTypeView;

    ScriptManager* m_scriptManager;

    ///stores all the translations of a vocabulary word
    KEduVocTranslator* m_translator;

    QList<QDockWidget*> m_dockWidgets;
    QList<bool> m_dockWidgetVisibility;

    friend class ::ParleyDocument;
    friend class Scripting::Parley;
    friend class ::ScriptManager;
};

}

#endif // EDITOR_H
