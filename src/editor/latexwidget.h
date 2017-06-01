/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef LATEXWIDGET_H
#define LATEXWIDGET_H

#include "ui_latexwidget.h"

#include <QItemDelegate>
#include <QtWidgets/QWidget>

class QDataWidgetMapper;
class KEduVocDocument;
class KEduVocExpression;
class KEduVocTranslation;

namespace Practice
{
class LatexRenderer;
}

class VocabularyFilter;

namespace Editor
{

class LatexWidget : public QWidget, public Ui::LatexWidget
{
    Q_OBJECT

public:
    LatexWidget(VocabularyFilter *model, KEduVocDocument *doc, QWidget *parent = 0);
    ~LatexWidget();

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

    void checkBoxToggled();

    void updateLatex();

private:
    VocabularyFilter *m_model;
    KEduVocDocument *m_doc;
    QDataWidgetMapper *m_mapper;
    KEduVocTranslation *m_translation;
    QLabel m_hiddenLabel;
    Practice::LatexRenderer *m_renderer;
};

class LatexDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    LatexDelegate(QObject *parent = 0);

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const Q_DECL_OVERRIDE;
    void setMathModeCheckBox(QCheckBox *checkBox) {
        m_checkBox = checkBox;
    }

private:
    QCheckBox *m_checkBox;
};
}

#endif //LATEXWIDGET_H
