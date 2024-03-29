/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2008 Javier Goday <jgoday@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SUMMARYWORDWIDGET_H
#define SUMMARYWORDWIDGET_H

#include "ui_summarywordwidget.h"

#include <memory>

#include <QItemDelegate>
#include <QWidget>

class QTreeView;
class QDataWidgetMapper;
class QItemSelection;
class QModelIndex;
class KEduVocDocument;
class KEduVocExpression;

class VocabularyFilter;
class WordClassModel;

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
    SummaryWordWidget(VocabularyFilter *model, const std::shared_ptr<KEduVocDocument> &, QWidget *parent = nullptr);

public Q_SLOTS:
    /**
     * Sets the selected word (KEduVocExpression) from the vocabularyView
     */
    void setTranslation(KEduVocExpression *entry, int translation);

    /**
     * Called when a KEduVocDocument change happened
     */
    void slotDocumentChanged(const std::shared_ptr<KEduVocDocument> &doc);

    /**
     * Called when the selection changed in the vocabulary view
     */
    void slotSelectionChanged(const QItemSelection &, const QItemSelection &);

    void wordTypeSelected(const QString &wordTypeName);

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
    VocabularyFilter *m_model{nullptr};
    std::shared_ptr<KEduVocDocument> m_doc;
    QDataWidgetMapper *m_mapper{nullptr};
    WordClassModel *m_wordTypeModel{nullptr};
    QTreeView *m_wordTypeView{nullptr};
    KEduVocExpression *m_entry{nullptr};
    int m_translationId{0};
};

class SummaryWordDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SummaryWordDelegate(QObject *parent = nullptr);

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
};
}

#endif
