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
#include <KXMLGUIClient>
#include <QDockWidget>
#include <QWidget>

class QLineEdit;

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
class AudioWidget;
class InflectionWidget;
class ComparisonWidget;
class SummaryWordWidget;
class LatexWidget;
class SynonymWidget;

class EditorWindow : public QWidget, public KXMLGUIClient
{
    Q_OBJECT

public:
    explicit EditorWindow(ParleyMainWindow *parent);
    ~EditorWindow() override;

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

    VocabularyView *vocabularyView() const
    {
        return m_vocabularyView;
    }

public Q_SLOTS:
    /**
     * Edit languages contained in the document.
     * This includes adding/removing languages,
     * editing articles, personal pronouns and tenses.
     */
    void slotLanguagesChanged();

    void slotConfigShowSearch();

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

private Q_SLOTS:

    /** Make the search bar visible and focus it */
    void startSearch();

Q_SIGNALS:
    void signalSetData(const QList<int> &entries, int currentTranslation);

private:
    ParleyMainWindow *m_mainWindow{nullptr};

    QAction *m_vocabShowSearchBarAction{nullptr};

    VocabularyModel *m_vocabularyModel{nullptr};
    VocabularyView *m_vocabularyView{nullptr};
    VocabularyFilter *m_vocabularyFilter{nullptr};

    QLineEdit *m_searchLine{nullptr};
    QWidget *m_searchWidget{nullptr};
    QAction *m_spellCheckMenu{nullptr};

    /** Show a single conjugation and let the user edit it */
    InflectionWidget *m_inflectionWidget{nullptr};
    SummaryWordWidget *m_summaryWordWidget{nullptr};
    ComparisonWidget *m_comparisonWidget{nullptr};
    LatexWidget *m_latexWidget{nullptr};
    SynonymWidget *m_synonymWidget{nullptr};
    SynonymWidget *m_antonymWidget{nullptr};
    SynonymWidget *m_falseFriendWidget{nullptr};
    AudioWidget *m_audioWidget{nullptr};

    /// dock widgets to display lessons, word types, ...
    LessonView *m_lessonView{nullptr};
    LessonModel *m_lessonModel{nullptr};

    WordClassModel *m_wordTypeModel{nullptr};
    WordTypeView *m_wordTypeView{nullptr};

    QList<QDockWidget *> m_dockWidgets;
    QList<bool> m_dockWidgetVisibility;

    friend class ::ParleyDocument;
};

}

#endif // EDITOR_H
