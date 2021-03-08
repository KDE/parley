/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2008 Javier Goday <jgoday@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "summarywordwidget.h"

#include "languagesettings.h"

#include "lessonmodel.h"
#include "vocabularymodel.h"
#include "vocabularyfilter.h"
#include "wordclassmodel.h"

// Qt headers
#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QHeaderView>
#include <QItemSelection>
#include <QLabel>
#include <QTreeView>

// KEduVocDocument
#include <KEduVocContainer>
#include <KEduVocDocument>
#include <KEduVocExpression>
#include <KEduVocWordtype>

using namespace Editor;


SummaryWordWidget::SummaryWordWidget(VocabularyFilter *model, const std::shared_ptr<KEduVocDocument> &doc, QWidget *parent)
    : QWidget(parent)
    , m_doc(doc)
    , m_wordTypeModel(0)
    , m_wordTypeView(0)
    , m_entry(0)
    , m_translationId(0)
{
    Q_ASSERT(model);
    Q_ASSERT(m_doc);
    m_model = model;

    setupUi(this);
    slotDocumentChanged(m_doc);

    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(model);
    m_mapper->setItemDelegate(new SummaryWordDelegate(this));

    connect(wordTypeComboBox, QOverload<const QString &>::of(&KComboBox::currentTextChanged),
            this, &SummaryWordWidget::wordTypeSelected);
}

void SummaryWordWidget::setTranslation(KEduVocExpression *entry, int translation)
{
    if (entry) {
        // we need to map the widgets relative to the translation (each translation has 9 columns)
        m_mapper->clearMapping();

        m_mapper->addMapping(wordEntry,
                             VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::Translation);
        //m_mapper->addMapping(wordTypeComboBox,
        //                    VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::WordType);
        m_mapper->addMapping(pronunciationEntry,
                             VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::Pronunciation);
        m_mapper->addMapping(exampleEntry,
                             VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::Example);
        m_mapper->addMapping(paraphraseEntry,
                             VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::Paraphrase);
        m_mapper->addMapping(commentEntry,
                             VocabularyModel::EntryColumnsMAX * translation + VocabularyModel::Comment);

        languageLabel->setText("<b>" + m_doc->identifier(translation).name() + "</b>");
        lessonLabel->setText(entry->lesson()->name());

        setCurrentWordType(entry, translation);
    } else {
        clear();
    }

    m_entry = entry;
    m_translationId = translation;
}

void SummaryWordWidget::slotDocumentChanged(const std::shared_ptr<KEduVocDocument> &doc)
{
    m_doc = doc;
    if (!m_doc) {
        qDebug() << "Set invalid document";
        delete m_wordTypeModel;
        m_wordTypeModel = 0;
    } else {
        delete m_wordTypeView;
        if (!m_wordTypeModel) {
            qDebug() << "Create word type model for summary view";
            m_wordTypeModel = new WordClassModel(this);
        }
        m_wordTypeModel->setDocument(m_doc);
        m_wordTypeView = new QTreeView(this);
        m_wordTypeView->setModel(m_wordTypeModel);
        wordTypeComboBox->setModel(m_wordTypeModel);
        wordTypeComboBox->setView(m_wordTypeView);

        m_wordTypeView->setColumnHidden(1, true);
        m_wordTypeView->header()->setVisible(false);
        m_wordTypeView->setRootIsDecorated(true);
        m_wordTypeView->expandAll();
    }
}


void SummaryWordWidget::slotSelectionChanged(const QItemSelection &itemSelected,
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

/*
void SummaryWordWidget::populateLessonList(KEduVocExpression *entry)
{
    lessonComboBox->clear();

    LessonModel *basicLessonModel = new LessonModel(this);
    lessonComboBox->setModel(basicLessonModel);
    QTreeView *view = new QTreeView(this);

    view->setModel(basicLessonModel);
    lessonComboBox->setView(view);

    basicLessonModel->setDocument(m_doc);

    view->header()->setVisible(false);
    view->setRootIsDecorated(true);
    view->expandAll();

    view->setCurrentIndex(basicLessonModel->index(entry->lesson()));
}
*/

void SummaryWordWidget::setCurrentWordType(KEduVocExpression *entry, int translation)
{
    if (entry && entry->translation(translation)->wordType()) {
        qDebug() << "Set current word type: " << entry->translation(translation)->wordType()->name();
        // select the right word type
        m_wordTypeView->setCurrentIndex(m_wordTypeModel->index(entry->translation(translation)->wordType()));
    } else {
        wordTypeComboBox->setCurrentIndex(-1);
    }
}

void SummaryWordWidget::clear()
{
    qDebug() << "Clear summary widget";

    languageLabel->setText(QString());
    wordEntry->setText(QString());

    // lessonComboBox->clear();
    lessonLabel->setText(QString());

    pronunciationEntry->setText(QString());
    exampleEntry->setText(QString());
    paraphraseEntry->setText(QString());
    commentEntry->setText(QString());
}

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
        case VocabularyModel::WordClass:
            break;

        case VocabularyModel::Comment:
        case VocabularyModel::Pronunciation:
        case VocabularyModel::Translation:
        case VocabularyModel::Example:
        case VocabularyModel::Paraphrase:

            QLineEdit *entry = static_cast <QLineEdit *>(editor);
            if (entry) {
                entry->setText(index.model()->data(index).toString());
            }
            break;
        }
    }
}

void SummaryWordWidget::wordTypeSelected(const QString& wordTypeName)
{
    if (!m_doc || !m_entry) {
        return;
    }

    KEduVocContainer* container = m_doc->wordTypeContainer()->childContainer(wordTypeName);
    if (container) {
        KEduVocWordType *wordType = static_cast<KEduVocWordType*>(container);
        if (wordType) {
            m_entry->translation(m_translationId)->setWordType(wordType);
        }
    }
}
