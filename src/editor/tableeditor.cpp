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


#include "tableeditor.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QHeaderView>
#include <KLineEdit>
#include <KLocalizedString>

#include "vocabulary/vocabularyview.h"
#include "vocabulary/vocabularyfilter.h"
#include "prefs.h"

using namespace Editor;

TableEditor::TableEditor(Editor::VocabularyModel* model, QWidget* parent)
: QWidget(parent), m_vocabularyModel(model)
{
    ui = new Ui::TableEditor();
    ui->setupUi(this);
    
    m_vocabularyFilter = new VocabularyFilter(this);
    m_vocabularyFilter->setSourceModel(m_vocabularyModel);
    ui->m_vocabularyView->setModel(m_vocabularyFilter);
    ui->m_vocabularyView->setFont(Prefs::tableFont());
    connect(ui->m_vocabularyView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
        this, SLOT(currentIndexChanged(QModelIndex)));

    QWidget* table = new QWidget;
    QVBoxLayout *topLayout = new QVBoxLayout(table);
    topLayout->setMargin(KDialog::marginHint());
    topLayout->setSpacing(KDialog::spacingHint());

    ui->m_searchLine->setFocusPolicy(Qt::ClickFocus);
    connect(ui->m_searchLine, SIGNAL(textChanged(const QString&)), m_vocabularyFilter, SLOT(setSearchString(const QString&)));
}

void TableEditor::setDocument(KEduVocDocument* doc)
{
    ui->m_vocabularyView->setDocument(doc);
}

void TableEditor::focusSearch()
{
    ui->m_searchLine->setFocus();
}

void TableEditor::saveState() {
    ui->m_vocabularyView->saveColumnVisibility();
}

void TableEditor::applyPrefs()
{
    ui->m_vocabularyView->setFont(Prefs::tableFont());
    ui->m_vocabularyView->reset();
}

void TableEditor::addActionToHeader(QAction* action)
{
    ui->m_vocabularyView->horizontalHeader()->addAction(action);
}

QModelIndexList TableEditor::selectedIndexes() const
{
    return ui->m_vocabularyView->selectedIndexes();
}

void TableEditor::currentIndexChanged(const QModelIndex& index)
{
    emit currentChanged(m_vocabularyFilter->mapToSource(index));
}

void TableEditor::slotShowVocabularyColumnsDialog()
{
    ui->m_vocabularyView->slotShowVocabularyColumnsDialog();
}

#include "tableeditor.moc"