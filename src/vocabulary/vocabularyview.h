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

class KEduVocExpression;
class KEduVocDocument;

class KActionMenu;
class KAction;
class KUrl;

class Translator;

namespace Editor {
    class EditorWindow;
    class VocabularyFilter;
    class VocabularyDelegate;

class VocabularyView : public QTableView
{
    Q_OBJECT
public:
    VocabularyView(QWidget* parent);
    virtual ~VocabularyView() {}

    KActionMenu* columnsActionMenu();

    void setModel(VocabularyFilter * model);

    /** Save column visibility settings */
    void saveColumnVisibility() const;

    QModelIndexList selectedIndexes() const;

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

    void setTranslator(Translator* translator);

    void checkSpelling();

    /**
    * Show the vocabulary columns dialog to enable or disable the columns in the view
    */
    void slotShowVocabularyColumnsDialog();

private slots:
    void slotSelectionChanged(const QItemSelection&, const QItemSelection&);

    void continueSpelling();
    void misspelling(const QString &word, int start);
    void spellingReplace(const QString& oldWord, int start, const QString &newWord);

private:
    void selectIndex(const QModelIndex &index);
    // trap enter presses at the end of the document to add a new entry instead of moving to the first cell
    bool eventFilter(QObject *obj, QEvent *event);
    virtual void setModel(QAbstractItemModel *model) { Q_UNUSED(model) }

    VocabularyFilter* m_model;
    VocabularyDelegate* m_vocabularyDelegate;
    KEduVocDocument *m_doc;

    int m_spellColumn;
    int m_spellRow;
    Sonnet::BackgroundChecker *m_spellChecker;
    Sonnet::Dialog *m_spellDialog;
};

}

#endif
