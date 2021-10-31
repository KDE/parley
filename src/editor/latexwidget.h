/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef LATEXWIDGET_H
#define LATEXWIDGET_H

#include "ui_latexwidget.h"

#include <QItemDelegate>
#include <QItemSelection>
#include <QWidget>

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
    LatexWidget(VocabularyFilter *model, KEduVocDocument *doc, QWidget *parent = nullptr);
    ~LatexWidget() override;

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
    VocabularyFilter *m_model{nullptr};
    KEduVocDocument *m_doc{nullptr};
    QDataWidgetMapper *m_mapper{nullptr};
    KEduVocTranslation *m_translation{nullptr};
    QLabel m_hiddenLabel;
    Practice::LatexRenderer *m_renderer{nullptr};
};

class LatexDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LatexDelegate(QObject *parent = nullptr);

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void setMathModeCheckBox(QCheckBox *checkBox)
    {
        m_checkBox = checkBox;
    }

private:
    QCheckBox *m_checkBox{nullptr};
};
}

#endif // LATEXWIDGET_H
