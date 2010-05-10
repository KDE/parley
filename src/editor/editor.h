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

#include "scripts/scripting/parley.h"

#include <KXmlGuiWindow>

class KLineEdit;
class KActionMenu;
class ScriptManager;

namespace Editor {
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
    class ComparisonWidget;
    class SummaryWordWidget;
    class LatexWidget;

class EditorWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    EditorWindow(ParleyMainWindow* parent);
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

    /**
     * Load enabled scripts (from configuration parleyrc)
     */
    void initScripts();

public slots:
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
    
private slots:
    
    /** Make the search bar visible and focus it */
    void startSearch();

signals:
    void signalSetData( const QList<int>& entries, int currentTranslation);

private:
    ParleyMainWindow* m_mainWindow;

    KAction* m_vocabShowSearchBarAction;

    VocabularyModel *m_vocabularyModel;
    VocabularyView *m_vocabularyView;
    VocabularyFilter *m_vocabularyFilter;

    KLineEdit *m_searchLine;
    QWidget *m_searchWidget;

    /** Show a single conjugation and let the user edit it */
    ConjugationWidget *m_conjugationWidget;
    SummaryWordWidget *m_summaryWordWidget;
    ComparisonWidget *m_comparisonWidget;
    LatexWidget *m_latexWidget;

    /// dock widgets to display lessons, word types, ...
    LessonView *m_lessonView;
    LessonModel *m_lessonModel;

    WordTypeView *m_wordTypeView;
    WordTypeModel *m_wordTypeModel;

    LeitnerView *m_leitnerView;
    LeitnerModel *m_leitnerModel;


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

