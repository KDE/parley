/***************************************************************************

    Copyright 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VOCABULARYVIEW_H
#define VOCABULARYVIEW_H

#include <sonnet/dialog.h>

#include <QTableView>
#include <QMap>

class VocabularyFilter;
class VocabularyDelegate;
class KEduVocExpression;
class KEduVocDocument;

class KActionMenu;
class KAction;
class ParleyApp;
class KUrl;

class VocabularyView : public QTableView
{
    Q_OBJECT
public:
    VocabularyView(ParleyApp *parent);
    KActionMenu* columnsActionMenu();

    void setModel(VocabularyFilter * model);

    /** Save column visibility settings */
    void saveColumnVisibility(const KUrl & kurl) const;

public slots:
    void reset();
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
    void appendChar(const QChar & c);

    void setDocument(KEduVocDocument * doc);

    void checkSpelling();

signals:
    void translationChanged(KEduVocExpression*, int);

private slots:
    void slotCurrentChanged(const QModelIndex& current, const QModelIndex& previous);

    void slotSelectionChanged(const QItemSelection&, const QItemSelection&);

    void slotToggleColumn(bool show);

    void continueSpelling();

    void misspelling(const QString &word, int start);
    void spellingReplace(const QString& oldWord, int start, const QString &newWord);
    
    // for the header view workaround
    void headerViewWorkaround();

private:
    void selectIndex(const QModelIndex &index);

    QMap <KAction*, int> m_columnActionMap;
    KActionMenu * m_vocabularyColumnsActionMenu;
    // for the header view workaround
    QMap <int, bool> m_workaroundColumnVisibilityMap;

    KAction* m_appendEntryAction;
    KAction* m_deleteEntriesAction;
    KAction* m_copyAction;
    KAction* m_cutAction;
    KAction* m_pasteAction;
    KAction* m_selectAllAction;
    KAction* m_clearSelectionAction;

    VocabularyFilter* m_model;
    VocabularyDelegate* m_vocabularyDelegate;
    KEduVocDocument *m_doc;

    int spellcheckRow;
    int spellcheckColumn;
    Sonnet::BackgroundChecker *spellingChecker;
    Sonnet::Dialog *spellingDialog;
};

#endif
