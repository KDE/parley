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
#include "latexwidget.h"

#include "vocabulary/vocabularymodel.h"
#include "vocabulary/vocabularyfilter.h"
#include "practice/latexrenderer.h"

#include <QDataWidgetMapper>

using namespace Editor;

LatexWidget::LatexWidget(VocabularyFilter *model, KEduVocDocument *doc, QWidget *parent) :
    QWidget(parent), m_translation(0), m_renderer(0)
{
    m_doc = doc;
    m_model = model;

    setupUi(this);
    lineEdit->setClickMessage(i18n("Enter LaTeX code here."));
    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(model);
    LatexDelegate *delegate = new LatexDelegate(this);
    delegate->setMathModeCheckBox(mathModeCheckBox);
    m_mapper->setItemDelegate(delegate);
    connect(mathModeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkBoxToggled()));
    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(updateLatex()), Qt::QueuedConnection);
}

LatexWidget::~LatexWidget()
{
    delete m_mapper;
}

void LatexWidget::setTranslation(KEduVocExpression *entry, int translation)
{
    if (entry) {
        // we need to map the widgets relative to the translation (each translation has 9 columns)
        m_mapper->clearMapping();

        m_mapper->addMapping(lineEdit,
                            VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::Translation);
        m_translation = entry->translation(translation);
        previewLabel->clear();
        updateLatex();
    }
}

void LatexWidget::slotDocumentChanged(KEduVocDocument *doc)
{
    m_doc = doc;
}

void LatexWidget::slotSelectionChanged(const QItemSelection &itemSelected,
                                             const QItemSelection &itemDeselected)
{
    Q_UNUSED(itemDeselected)

    if (itemSelected.indexes().size() >= 1) {
        // the selected index belongs to VocabularyFilter, when we need it from the vocabulary model
        QModelIndex index = m_model->index(itemSelected.indexes().at(0).row(),
                                           itemSelected.indexes().at(0).column());
        m_mapper->setCurrentModelIndex(index);
    }
}

void LatexWidget::checkBoxToggled()
{
    // emulate editing of the line edit
    lineEdit->setFocus();
    mathModeCheckBox->setFocus();
}

void LatexWidget::updateLatex()
{
    if (Practice::LatexRenderer::isLatex(m_translation->text())) {
        if (!m_renderer) {
            m_renderer = new Practice::LatexRenderer(this);
            m_renderer->setResultLabel(previewLabel);
        }
        m_renderer->renderLatex(m_translation->text());
    }
}


LatexDelegate::LatexDelegate(QObject *parent) : QItemDelegate(parent), m_checkBox(0)
{
}

void LatexDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    if (editor) {
        KLineEdit *entry = static_cast <KLineEdit *> (editor);
        if (entry) {
            QString text = index.model()->data(index).toString();
            if (text.startsWith(QLatin1String("$$")) && text.endsWith(QLatin1String("$$"))) {
                entry->setText(text.mid(2, text.count()-4));
                m_checkBox->setChecked(true);
            } else if(text.startsWith(QString::fromUtf8("§§")) && text.endsWith(QString::fromUtf8("§§"))) {
                entry->setText(text.mid(2, text.count()-4));
                m_checkBox->setChecked(false);
            } else {
                entry->setText(QString());
                return;
            }
        }
    }
}

void LatexDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    if (!index.isValid()) {
        return;
    }

    if (editor) {
        KLineEdit *entry = static_cast <KLineEdit *> (editor);
        if (entry) {
            QString text = entry->text();
            if (m_checkBox->isChecked()) {
                model->setData(index, "$$"+text+"$$");
            } else {
                model->setData(index, QString::fromUtf8("§§")+text+QString::fromUtf8("§§"));
            }
        }
    }
}

#include "latexwidget.moc"
