/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <laidig@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "latexwidget.h"

#include "practice/latexrenderer.h"
#include "vocabularyfilter.h"
#include "vocabularymodel.h"

#include <KLocalizedString>
#include <QDataWidgetMapper>

using namespace Editor;

LatexWidget::LatexWidget(VocabularyFilter *model, KEduVocDocument *doc, QWidget *parent)
    : QWidget(parent)
    , m_model{model}
    , m_doc{doc}
{
    setupUi(this);
    lineEdit->setPlaceholderText(i18n("Enter LaTeX code here."));
    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(model);
    LatexDelegate *delegate = new LatexDelegate(this);
    delegate->setMathModeCheckBox(mathModeCheckBox);
    m_mapper->setItemDelegate(delegate);
#if QT_VERSION < QT_VERSION_CHECK(6, 7, 0)
    connect(mathModeCheckBox, &QCheckBox::stateChanged, this, &LatexWidget::checkBoxToggled);
#else
    connect(mathModeCheckBox, &QCheckBox::checkStateChanged, this, &LatexWidget::checkBoxToggled);
#endif
    connect(lineEdit, &QLineEdit::editingFinished, this, &LatexWidget::updateLatex, Qt::QueuedConnection);
}

LatexWidget::~LatexWidget()
{
    delete m_mapper;
}

void LatexWidget::setTranslation(KEduVocExpression *entry, int translation)
{
    previewLabel->clear();
    previewLabel->setMinimumSize(QSize(1, 1));

    if (entry) {
        // we need to map the widgets relative to the translation (each translation has 9 columns)
        m_mapper->clearMapping();

        m_mapper->addMapping(lineEdit, VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::Translation);
        m_translation = entry->translation(translation);
        updateLatex();
    }
}

void LatexWidget::slotDocumentChanged(KEduVocDocument *doc)
{
    m_doc = doc;
}

void LatexWidget::slotSelectionChanged(const QItemSelection &itemSelected, const QItemSelection &itemDeselected)
{
    Q_UNUSED(itemDeselected)

    if (itemSelected.indexes().size() >= 1) {
        // the selected index belongs to VocabularyFilter, when we need it from the vocabulary model
        QModelIndex index = m_model->index(itemSelected.indexes().at(0).row(), itemSelected.indexes().at(0).column());
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
    if (!m_translation) {
        return;
    }
    if (Practice::LatexRenderer::isLatex(m_translation->text())) {
        if (!m_renderer) {
            m_renderer = new Practice::LatexRenderer(this);
            m_renderer->setResultLabel(previewLabel);
        }
        m_renderer->renderLatex(m_translation->text());
    }
}

LatexDelegate::LatexDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void LatexDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    if (editor) {
        QLineEdit *entry = static_cast<QLineEdit *>(editor);
        if (entry) {
            QString text = index.model()->data(index).toString();
            if (text.startsWith(QLatin1String("$$")) && text.endsWith(QLatin1String("$$"))) {
                entry->setText(text.mid(2, text.size() - 4));
                m_checkBox->setChecked(true);
            } else if (text.startsWith(QStringLiteral("§§")) && text.endsWith(QStringLiteral("§§"))) {
                entry->setText(text.mid(2, text.size() - 4));
                m_checkBox->setChecked(false);
            } else {
                entry->setText(QString());
                return;
            }
        }
    }
}

void LatexDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }

    if (editor) {
        QLineEdit *entry = static_cast<QLineEdit *>(editor);
        if (entry) {
            QString text = entry->text();
            if (m_checkBox->isChecked()) {
                model->setData(index, QString(QStringLiteral("$$") + text + QStringLiteral("$$")));
            } else {
                model->setData(index, QString(QStringLiteral("§§") + text + QStringLiteral("§§")));
            }
        }
    }
}

#include "moc_latexwidget.cpp"
