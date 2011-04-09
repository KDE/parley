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



#include "simpleeditor.h"

#include "vocabulary/vocabularymodel.h"
#include <QDataWidgetMapper>

#include <KDebug>

#include "languagesettings.h"
#include "vocabulary/lessonmodel.h"
#include "vocabulary/vocabularymodel.h"
#include "vocabulary/vocabularyfilter.h"
#include "vocabulary/wordtypemodel.h"

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QHeaderView>
#include <QItemSelection>
#include <QLabel>
#include <QTreeView>

#include <keduvoccontainer.h>
#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <keduvocwordtype.h>
#include <KIcon>
#include <KLocale>


using namespace Editor;

SimpleEditor::SimpleEditor(VocabularyModel* model, QWidget* parent)
: QWidget(parent), m_vocabularyModel(model), m_wordTypeModel(0), m_wordTypeView(0)
{
    ui = new Ui::Editor;
    ui->setupUi(this);

    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(model);
    m_mapper->setItemDelegate(new SummaryWordDelegate(this));

    int firstColumn = 0;
    int secondColumn = VocabularyModel::EntryColumnsMAX;
    
    m_mapper->addMapping(ui->wordEntry, firstColumn);
    
    m_mapper->addMapping(ui->translationEntry, secondColumn);
    
    m_mapper->addMapping(ui->pronunciationEntry,
                         firstColumn + VocabularyModel::Pronunciation);
    m_mapper->addMapping(ui->exampleEntry,
                         firstColumn + VocabularyModel::Example);
    m_mapper->addMapping(ui->paraphraseEntry,
                         firstColumn + VocabularyModel::Paraphrase);
    m_mapper->addMapping(ui->commentEntry,
                         firstColumn + VocabularyModel::Comment);

    
    connect(ui->wordTypeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(wordTypeSelected(QString)));
    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(addNewEntry()));

    connect(ui->nextButton, SIGNAL(clicked()), m_mapper, SLOT(toNext()));
    connect(ui->previousButton, SIGNAL(clicked()), m_mapper, SLOT(toPrevious()));

    connect(m_vocabularyModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(modelReset()));

    connect(m_mapper, SIGNAL(currentIndexChanged(int)), SLOT(mapperIndexChanged(int)));

    ui->currentIndex->installEventFilter(this);
    connect(ui->currentIndex, SIGNAL(editingFinished()), SLOT(userSetIndex()));

    ui->previousButton->setIcon(KIcon("go-previous"));
    
    ui->nextButton->setIcon(KIcon("go-next"));
    ui->newButton->setIcon(KIcon("list-add"));

    connect(m_vocabularyModel, SIGNAL(modelReset()), SLOT(modelReset()));
}

bool SimpleEditor::eventFilter(QObject* object, QEvent* event)
{
    if (object == ui->currentIndex && event->type() == QEvent::FocusIn) {
        ui->currentIndex->clear();
    }
    return QObject::eventFilter(object, event);
}

void SimpleEditor::userSetIndex()
{
    int row = ui->currentIndex->text().toInt();
    if (row < 1) {
        row = 1;
    }
    m_mapper->setCurrentIndex(row - 1);
}

void SimpleEditor::setDocument(KEduVocDocument* doc)
{
    m_doc = doc;
    if (!m_doc) {
        kDebug() << "Set invalid document";
        delete m_wordTypeModel;
        m_wordTypeModel = 0;
    } else {
        delete m_wordTypeView;
        if (!m_wordTypeModel) {
            m_wordTypeModel = new WordTypeModel(this);
        }
        m_wordTypeModel->setDocument(m_doc);
        m_wordTypeView = new QTreeView(this);
        m_wordTypeView->setModel(m_wordTypeModel);
        ui->wordTypeComboBox->setModel(m_wordTypeModel);
        ui->wordTypeComboBox->setView(m_wordTypeView);

        m_wordTypeView->setColumnHidden(1, true);
        m_wordTypeView->header()->setVisible(false);
        m_wordTypeView->setRootIsDecorated(true);
        m_wordTypeView->expandAll();
    }
}

void SimpleEditor::modelReset()
{
    int firstColumn = 0;
    int secondColumn = VocabularyModel::EntryColumnsMAX;

    qDebug() << "model data changed";
    setCurrentIndex(m_vocabularyModel->index(0,0));
    ui->languageLabel->setText(m_vocabularyModel->headerData(firstColumn, Qt::Horizontal).toString());
    ui->secondLanguageLabel->setText(m_vocabularyModel->headerData(secondColumn, Qt::Horizontal).toString());
}

void SimpleEditor::mapperIndexChanged(int row)
{
    ui->currentIndex->setText(i18n("%1 of %2", row + 1, m_vocabularyModel->rowCount(QModelIndex())));
}

void SimpleEditor::setCurrentIndex(const QModelIndex& index)
{
    if (index.isValid()) {
        kDebug() << "Selected: " << m_vocabularyModel->data(index, 0);
        m_mapper->setCurrentIndex(index.row());

        ui->wordTypeComboBox->setEnabled(true);
//         KEduVocExpression* entry;
//         entry = m_vocabularyModel->data(index, VocabularyModel::EntryRole).value<KEduVocExpression*>();
//         setCurrentWordType(entry, translation);
//         ui->lessonLabel->setText(entry->lesson()->name());
    } else {
        clear();
        ui->wordTypeComboBox->setEnabled(false);
    }
}

void SimpleEditor::setCurrentWordType(KEduVocExpression *entry, int translation)
{
    if (entry && entry->translation(translation)->wordType()) {
        kDebug() << "Set current word type: " << entry->translation(translation)->wordType()->name();
        // select the right word type
        m_wordTypeView->setCurrentIndex(m_wordTypeModel->index(entry->translation(translation)->wordType()));
    } else {
        ui->wordTypeComboBox->setCurrentIndex(-1);
    }
}

void SimpleEditor::clear()
{
    ui->languageLabel->clear();
    ui->translationEntry->clear();
    ui->wordEntry->clear();
    ui->lessonLabel->clear();
    ui->pronunciationEntry->clear();
    ui->exampleEntry->clear();
    ui->paraphraseEntry->clear();
    ui->commentEntry->clear();
}

void SimpleEditor::addNewEntry()
{
    QModelIndex index = m_vocabularyModel->appendEntry();
    setCurrentIndex(index);
    ui->wordEntry->setFocus();
}

// void SummaryWordWidget::wordTypeSelected(const QString& wordTypeName)
// {
//     if (!m_doc || !m_entry) {
//         return;
//     }
//     
//     KEduVocContainer* container = m_doc->wordTypeContainer()->childContainer(wordTypeName);
//     if (container) {
//         KEduVocWordType *wordType = static_cast<KEduVocWordType*>(container);
//         if (wordType) {
//             m_entry->translation(m_translationId)->setWordType(wordType);
//         }
//     }
// }

SummaryWordDelegate::SummaryWordDelegate(QObject *parent) : QItemDelegate(parent)
{
}

void SummaryWordDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid()) {
        return;
    }
    
    if (editor) {
        switch (VocabularyModel::columnType(index.column())) {
            case VocabularyModel::WordType:
                break;
                
            case VocabularyModel::Comment:
            case VocabularyModel::Pronunciation:
            case VocabularyModel::Translation:
            case VocabularyModel::Example:
            case VocabularyModel::Paraphrase:
                
                KLineEdit *entry = static_cast <KLineEdit *> (editor);
                if (entry) {
                    entry->setText(index.model()->data(index).toString());
                }
                break;
        }
    }
}

#include "simpleeditor.moc"
