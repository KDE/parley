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

#include <QtCore/QModelIndexList>

#include <KXmlGuiWindow>

#include "parleydocument.h"
#include "scripts/scripting/parley.h"


class QStackedWidget;
class KLineEdit;
class KActionMenu;
class ScriptManager;
class KEduVocExpression;

namespace Editor {

    class SimpleEditor;
    class TableEditor;

    class VocabularyModel;
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

    /** setup the main view*/
    void initView();

    void initDockWidgets();

    /** This will look at the lesson list and also the combo box to determine what should be displayed in the table. */
    void updateTableFilter();

    /**
     * Load enabled scripts (from configuration parleyrc)
     */
    void initScripts();

public Q_SLOTS:
    /**
     * Edit languages contained in the document.
     * This includes adding/removing languages, 
     * editing articles, personal pronouns and tenses.
     */
    void slotLanguagesChanged();

    /**
     *  Display script manager (open a new window / or Settings window)
     */
    void slotShowScriptManager();

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

    /** Insert letter from KCharSelect widget */
    void appendChar(const QChar& newChar);

    QModelIndexList selectedIndexes() const;
    
private slots:
    
    /** Make the search bar visible and focus it */
    void startSearch();

    void toggleTableEditor(bool table);
    
    void setCurrentIndex(const QModelIndex& index);
    
Q_SIGNALS:
    void currentChanged(const QModelIndex& index);
    void translationChanged(KEduVocExpression*, int);
    
private:
    ParleyMainWindow* m_mainWindow;
    VocabularyModel *m_vocabularyModel;

    /** Show a single conjugation and let the user edit it */
    ConjugationWidget *m_conjugationWidget;
    ComparisonWidget *m_comparisonWidget;
    LatexWidget *m_latexWidget;

    /// dock widgets to display lessons, word types, ...
    LessonView *m_lessonView;
    LessonModel *m_lessonModel;

    WordTypeView *m_wordTypeView;
    WordTypeModel *m_wordTypeModel;

    ScriptManager* m_scriptManager;

    ///stores all the translations of a vocabulary word
    Translator* m_translator;

    QList<QDockWidget*> m_dockWidgets;
    QList<bool> m_dockWidgetVisibility;

    QStackedWidget* editorStack;
    SimpleEditor* simpleEditor;
    TableEditor* tableEditor;
    KAction* m_toggleTableEditorAction;
    KAction *m_vocabularyColumnsDialogAction;

    friend class ::ParleyDocument;
    friend class Scripting::Parley;
    friend class ::ScriptManager;
};

}

#endif // EDITOR_H

