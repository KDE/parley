/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 Javier Goday <jgoday@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SUMMARYWORDWIDGET_H
#define SUMMARYWORDWIDGET_H

#include "ui_summarywordwidget.h"

#include <QItemDelegate>
#include <QWidget>

class QTreeView;
class QDataWidgetMapper;
class QItemSelection;
class QModelIndex;
class KEduVocDocument;
class KEduVocExpression;

class VocabularyFilter;
class KEduVocWordClassModel;

namespace Editor
{

/**
* Represents the overview of a KEduVocExpression
* Shows the language word
*       the lesson
        the word type
        pronunciation
        example
        paraphrase
        comment
*/
class SummaryWordWidget : public QWidget, public Ui::SummaryWordWidget
{
    Q_OBJECT

public:
    SummaryWordWidget(VocabularyFilter *model, KEduVocDocument *doc, QWidget *parent = 0);

public slots:
    /**
    * Sets the selected word (KEduVocExpression) from the vocabularyView
    */
    void setTranslation(KEduVocExpression *entry, int translation);

    /**
    * Called when a KEduVocDocument change happened
    */
    void slotDocumentChanged(KEduVocDocument *doc);

    /**
    * Called when the selection changed in the vocabulary view
    */
    void slotSelectionChanged(const QItemSelection &, const QItemSelection &);

    void wordTypeSelected(const QString& wordTypeName);

private:
    /**
    * Clears the form and the comboboxes
    */
    void clear();

    /**
    * Fill the lesson combobox with the available lessons of the KEduVocDocument

    void populateLessonList(KEduVocExpression *entry);
    */
    /**
    * Fill the word types list with the available word types of the KEduVocDocument
    */
    void setCurrentWordType(KEduVocExpression *entry, int translation);

private:
    VocabularyFilter *m_model;
    KEduVocDocument *m_doc;
    QDataWidgetMapper *m_mapper;
    KEduVocWordClassModel *m_wordTypeModel;
    QTreeView *m_wordTypeView;
    KEduVocExpression *m_entry;
    int m_translationId;
};

class SummaryWordDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SummaryWordDelegate(QObject *parent = 0);

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
};
}

#endif
