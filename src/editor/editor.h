/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2004-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-FileCopyrightText: 2008 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EDITOR_H
#define EDITOR_H

#include "parleydocument.h"

#include "scripts/scripting/parley.h"

#include <KXmlGuiWindow>

class QLineEdit;
class ScriptManager;

///@brief contains objects for the editor main window

// Models around the Collection / KEduVocDocument
class LessonModel;
class VocabularyModel;
class VocabularyFilter;
class WordClassModel;

namespace Editor
{
class VocabularyView;
class LessonView;
class WordTypeView;
class InflectionWidget;
class ComparisonWidget;
class SummaryWordWidget;
class LatexWidget;
class SynonymWidget;

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
    void updateDocument(const std::shared_ptr<KEduVocDocument> &doc);

    /**
     * DBus method for adding words by external apps
     */
    void addWordWithTranslation(const QStringList &w);

private slots:

    /** Make the search bar visible and focus it */
    void startSearch();

signals:
    void signalSetData(const QList<int>& entries, int currentTranslation);

private:
    ParleyMainWindow  *m_mainWindow;

    QAction* m_vocabShowSearchBarAction;

    VocabularyModel *m_vocabularyModel;
    VocabularyView *m_vocabularyView;
    VocabularyFilter *m_vocabularyFilter;

    QLineEdit *m_searchLine;
    QWidget *m_searchWidget;
    QAction *m_spellCheckMenu;

    /** Show a single conjugation and let the user edit it */
    InflectionWidget *m_inflectionWidget;
    SummaryWordWidget *m_summaryWordWidget;
    ComparisonWidget *m_comparisonWidget;
    LatexWidget *m_latexWidget;
    SynonymWidget *m_synonymWidget;
    SynonymWidget *m_antonymWidget;
    SynonymWidget *m_falseFriendWidget;

    /// dock widgets to display lessons, word types, ...
    LessonView *m_lessonView;
    LessonModel *m_lessonModel;

    WordClassModel *m_wordTypeModel;
    WordTypeView *m_wordTypeView;

    ScriptManager* m_scriptManager;

    ///stores all the translations of a vocabulary word
    Translator* m_translator;

    QList<QDockWidget*> m_dockWidgets;
    QList<bool> m_dockWidgetVisibility;

    friend class ::ParleyDocument;
    friend class Scripting::Parley;
    friend class ::ScriptManager;
};

}

#endif // EDITOR_H
