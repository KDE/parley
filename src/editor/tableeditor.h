/***************************************************************************
 * 
 *    Copyright 2011 Frederik Gladhorn <gladhorn@kde.org>
 *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <QWidget>
#include <QtCore/QModelIndexList>

#include "ui_tableeditor.h"

class KLineEdit;

class KEduVocDocument;
class KEduVocExpression;

namespace Editor {
    class VocabularyView;
    class VocabularyFilter;
    class VocabularyModel;

class TableEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TableEditor(VocabularyModel* model, QWidget* parent = 0);
    void focusSearch();
    void setDocument(KEduVocDocument* doc);
    void saveState();

    void addActionToHeader(QAction* action);

    // FIXME this should be part of a common interface between all editors
    // the simple editor can simply return the current item
    QModelIndexList selectedIndexes() const;

public Q_SLOTS:
    void applyPrefs();
    void slotShowVocabularyColumnsDialog();

Q_SIGNALS:
//     void translationChanged(KEduVocExpression*, int);

    // the index as source model index
    void currentChanged(const QModelIndex& index);

private Q_SLOTS:
    void currentIndexChanged(const QModelIndex& index);

private:
    VocabularyModel *m_vocabularyModel;
    VocabularyFilter *m_vocabularyFilter;    

    Ui::TableEditor* ui;
};
}

#endif
