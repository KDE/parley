/***************************************************************************

                      options to enable/disable columns for the languages

    -----------------------------------------------------------------------

    begin         : Tue Mar 29 2005

    copyright     :(C) 2008 Javier goday <jgoday@gmail.com>
    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "vocabularycolumnsdialog.h"

// parley imports
#include "vocabulary/vocabularymodel.h"
#include "documentsettings.h"

// Qt imports
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

// KDE imports
#include <keduvocdocument.h>
#include <KIcon>
#include <KLocale>
#include <KTitleWidget>

const int COLUMNS_LIMIT = 1; // columns for row

VocabularyColumnsDialog::VocabularyColumnsDialog(KEduVocDocument *doc, QWidget *parent)
    : KDialog(parent),
    m_models()
{
    m_box = new QGridLayout();
    m_doc = doc;
    m_settings = new DocumentSettings(m_doc->url().url());

    setCaption(i18n("Vocabulary columns"));
    setButtons(Ok|Cancel);

    QWidget *main_widget = new QWidget(this);
    main_widget->setLayout(m_box);

    setMainWidget(main_widget);

    KTitleWidget *titleWidget = new KTitleWidget(this);
    titleWidget->setText(i18n("Enable/Disable the columns for each language"));
    titleWidget->setPixmap(KIcon("view-file-columns").pixmap(22, 22), KTitleWidget::ImageRight);
    m_box->addWidget(titleWidget, 0, 0, 1, 2);

    createLanguagesLayout();
}

VocabularyColumnsDialog::~VocabularyColumnsDialog()
{
    delete m_settings;
}

void VocabularyColumnsDialog::accept()
{
    saveVisibleColumns();

    KDialog::accept();
}

void VocabularyColumnsDialog::reject()
{
    KDialog::reject();
}

void VocabularyColumnsDialog::createLanguagesLayout()
{
    int row = 1;
    int column = 0;
    for (int i = 0; i < m_doc->identifierCount(); i++ ) {
        m_models [i] = new QStandardItemModel(this);

        QGroupBox *box = new QGroupBox(m_doc->identifier(i).name(), 0);
        QVBoxLayout *box_layout = new QVBoxLayout(0);

        m_box->addWidget(box, row, column);

        if (column < COLUMNS_LIMIT) {
            column ++;
        }
        else {
            row ++;
            column = 0;
        }

        box->setLayout(box_layout);

        addLanguage(i, box_layout);
    }
}

void VocabularyColumnsDialog::addLanguage(int i, QVBoxLayout *parent)
{
    for(int c=1; c<VocabularyModel::EntryColumnsMAX; c++) {
        QStandardItem *item_data = new QStandardItem(VocabularyModel::columnTitle(m_doc, i, c));
        item_data->setData(QVariant(c-1), VocabularyColumnsDialog::ColumnRole);
        item_data->setData(QVariant(i), VocabularyColumnsDialog::LanguageRole);
        item_data->setCheckable(true);
        item_data->setEditable(false);

        // check if the column is active
        if (m_settings->visibleColumns().size() >= (i*8 + c)) {
            item_data->setCheckState(m_settings->visibleColumns().at(i*8 + c) == 1 ? Qt::Checked : Qt::Unchecked);
        }

        m_models [i]->insertRow(c-1, item_data);
    }

    QTreeView *treeView = new QTreeView(this);
    treeView->header()->hide();
    treeView->setModel(m_models [i]);

    parent->addWidget(treeView);
}

void VocabularyColumnsDialog::saveVisibleColumns()
{
    QList <int> columns;

    for (int i = 0; i < m_doc->identifierCount(); i++ ) { // each model for every language
        columns << 1;
        for (int row = 0; row < m_models [i]->rowCount(); row++) { // the columns for one language
            int enabled = (m_models [i]->item(row)->checkState() == Qt::Checked) ? 1 : 0;
            columns << enabled;
        }
    }

    // i think that this is not the place for this
    m_settings->setVisibleColumns(columns);
    m_settings->writeConfig();
}
