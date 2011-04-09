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



#ifndef SIMPLEEDITOR_H
#define SIMPLEEDITOR_H

#include <QItemDelegate>

#include "ui_simpleeditor.h"

class KEduVocExpression;
class KEduVocDocument;
class QDataWidgetMapper;
class QTreeView;

namespace Editor {
class WordTypeModel;
class VocabularyModel;

class SimpleEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleEditor(VocabularyModel* model, QWidget* parent = 0);
    void setDocument(KEduVocDocument* doc);
    
Q_SIGNALS:
//     void translationChanged(KEduVocExpression*, int);
    void nextEntry();
    void previousEntry();

public Q_SLOTS:
    void setCurrentIndex(const QModelIndex& index);

private Q_SLOTS:
    void modelReset();
    void mapperIndexChanged(int);
    void userSetIndex();
    void addNewEntry();

protected:
    virtual bool eventFilter(QObject* object, QEvent* event);
    
private:
    /**
    * Fill the word types list with the available word types of the KEduVocDocument
    */
    void setCurrentWordType(KEduVocExpression *entry, int translation);
    void clear();
    
    KEduVocDocument *m_doc;
    VocabularyModel* m_vocabularyModel;
    QDataWidgetMapper *m_mapper;
    WordTypeModel *m_wordTypeModel;
    QTreeView *m_wordTypeView;
    
    Ui::Editor* ui;    
};


class SummaryWordDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SummaryWordDelegate(QObject *parent = 0);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

}

#endif
