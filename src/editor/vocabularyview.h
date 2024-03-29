/*
    SPDX-FileCopyrightText: 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef VOCABULARYVIEW_H
#define VOCABULARYVIEW_H

#include <memory>
#include <sonnet/dialog.h>

// Qt
#include <QMap>
#include <QTableView>

// KDE
class KActionMenu;
class QAction;

// KEduVocDocument
class KEduVocExpression;
class KEduVocDocument;

// parley / collection
class VocabularyFilter;

class Translator;

namespace Editor
{
class EditorWindow;
class VocabularyDelegate;

class VocabularyView : public QTableView
{
    Q_OBJECT
public:
    explicit VocabularyView(EditorWindow *parent);
    ~VocabularyView() override
    {
    }

    KActionMenu *columnsActionMenu();

    void setFilter(VocabularyFilter *model);

    /** Save column visibility settings */
    void saveColumnVisibility() const;

    QModelIndexList getSelectedIndexes() const;

public Q_SLOTS:
    void reset() override;
    void appendEntry();

    /** Delete the currently selected entries */
    void deleteSelectedEntries(bool askConfirmation = true);

    /** put the marked text/object into the clipboard*/
    void slotEditCopy();

    /** Cut an entry */
    void slotCutEntry();

    /** paste the clipboard into the document*/
    void slotEditPaste();

    /**
     * Append a char to the last edited entry (used to add the phonetic stuff).
     * @param c the char
     */
    void appendChar(const QChar &c);

    void setDocument(const std::shared_ptr<KEduVocDocument> &doc);

    void checkSpelling(int language);

    /**
     * Show the vocabulary columns dialog to enable or disable the columns in the view
     */
    void slotShowVocabularyColumnsDialog();

Q_SIGNALS:
    void translationChanged(KEduVocExpression *, int);

private Q_SLOTS:
    void slotCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void slotSelectionChanged(const QItemSelection &, const QItemSelection &);

    void continueSpelling();
    void misspelling(const QString &word, int start);
    void spellingReplace(const QString &oldWord, int start, const QString &newWord);

private:
    void selectIndex(const QModelIndex &index);
    // trap enter presses at the end of the document to add a new entry instead of moving to the first cell
    bool eventFilter(QObject *obj, QEvent *event) override;

    // Make this private to avoid inadvertent use. Instead use setFilter() which is public.
    void setModel(QAbstractItemModel *model) override
    {
        Q_UNUSED(model)
    }

    QAction *m_appendEntryAction{nullptr};
    QAction *m_deleteEntriesAction{nullptr};
    QAction *m_copyAction{nullptr};
    QAction *m_cutAction{nullptr};
    QAction *m_pasteAction{nullptr};
    QAction *m_selectAllAction{nullptr};
    QAction *m_clearSelectionAction{nullptr};

    VocabularyFilter *m_model{nullptr};
    VocabularyDelegate *m_vocabularyDelegate{nullptr};
    std::shared_ptr<KEduVocDocument> m_doc;

    int m_spellColumn{0};
    int m_spellRow{0};
    Sonnet::BackgroundChecker *m_spellChecker{nullptr};
    Sonnet::Dialog *m_spellDialog{nullptr};
};

}

#endif
