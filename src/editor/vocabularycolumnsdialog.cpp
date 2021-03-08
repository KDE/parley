/*
    options to enable/disable columns for the languages
    SPDX-FileCopyrightText: 2008 Javier goday <jgoday@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "vocabularycolumnsdialog.h"

// parley imports
#include "documentsettings.h"
#include "vocabularymodel.h"

// KDE imports
#include <KEduVocDocument>
#include <KLocalizedString>
#include <KTitleWidget>

// Qt imports
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QDialogButtonBox>

const int COLUMNS_LIMIT = 1; // columns for row

using namespace Editor;

VocabularyColumnsDialog::VocabularyColumnsDialog(KEduVocDocument *doc, QWidget *parent)
    : QDialog(parent),
      m_models()
{
    QDialogButtonBox * button_dialog = new QDialogButtonBox;
    button_dialog->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel );

    m_box = new QGridLayout();
    m_doc = doc;
    m_settings = new DocumentSettings(m_doc->url().url());

    setWindowTitle(i18n("Vocabulary Columns"));

    QWidget *main_widget = new QWidget(this);
    main_widget->setLayout(m_box);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( main_widget );
    layout->addWidget( button_dialog );

    setLayout( layout );

    connect(button_dialog, &QDialogButtonBox::accepted, this, &VocabularyColumnsDialog::accept);
    connect(button_dialog, &QDialogButtonBox::rejected, this, &VocabularyColumnsDialog::reject);

    KTitleWidget *titleWidget = new KTitleWidget(this);
    titleWidget->setText(i18n("Enable/Disable the columns for each language"));
    titleWidget->setPixmap(QIcon::fromTheme(QStringLiteral("view-file-columns")).pixmap(22, 22), KTitleWidget::ImageRight);
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
    QDialog::accept();
}

void VocabularyColumnsDialog::createLanguagesLayout()
{
    int row = 1;
    int column = 0;
    for (int i = 0; i < m_doc->identifierCount(); i++) {
        m_models [i] = new QStandardItemModel(this);

        QGroupBox *box = new QGroupBox(m_doc->identifier(i).name(), 0);
        QVBoxLayout *box_layout = new QVBoxLayout(0);

        m_box->addWidget(box, row, column);

        if (column < COLUMNS_LIMIT) {
            column ++;
        } else {
            row ++;
            column = 0;
        }

        box->setLayout(box_layout);

        addLanguage(i, box_layout);
    }
}

void VocabularyColumnsDialog::addLanguage(int i, QVBoxLayout *parent)
{
    for (int c = 1; c < VocabularyModel::EntryColumnsMAX; c++) {
        QStandardItem *item_data = new QStandardItem(VocabularyModel::columnTitle(m_doc, i, c, /*addLocaleSuffix*/ false));
        item_data->setData(QVariant(c - 1), VocabularyColumnsDialog::ColumnRole);
        item_data->setData(QVariant(i), VocabularyColumnsDialog::LanguageRole);
        item_data->setCheckable(true);
        item_data->setEditable(false);

        // check if the column is active
        if (m_settings->visibleColumns().size() >= (i * 8 + c)) {
            item_data->setCheckState(m_settings->visibleColumns().at(i * 8 + c) == 1 ? Qt::Checked : Qt::Unchecked);
        }

        m_models [i]->insertRow(c - 1, item_data);
    }

    QTreeView *treeView = new QTreeView(this);
    treeView->header()->hide();
    treeView->setModel(m_models [i]);

    parent->addWidget(treeView);
}

void VocabularyColumnsDialog::saveVisibleColumns()
{
    QList <int> columns;

    for (int i = 0; i < m_doc->identifierCount(); i++) {  // each model for every language
        columns << 1;
        for (int row = 0; row < m_models [i]->rowCount(); row++) { // the columns for one language
            int enabled = (m_models [i]->item(row)->checkState() == Qt::Checked) ? 1 : 0;
            columns << enabled;
        }
    }

    // i think that this is not the place for this
    m_settings->setVisibleColumns(columns);
    m_settings->save();
}
