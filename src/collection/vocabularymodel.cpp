/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "vocabularymodel.h"

#include "parleydocument.h"
#include "prefs.h"
#include "languagesettings.h"

#include <KEduVocLesson>
#include <KEduVocWordtype>

#include "vocabularymimedata.h"

#include <KLocalizedString>
#include <QDebug>
#include <KMessageBox>
#include <QPixmap>
#include <QTextStream>


VocabularyModel::VocabularyModel(QObject *parent)
    : QAbstractTableModel(parent),
      m_container(0), m_document(0)
{
    m_recursive = Prefs::showSublessonentries() ? KEduVocContainer::Recursive
                  : KEduVocContainer::NotRecursive;

    qRegisterMetaType<KEduVocTranslation*>("KEduVocTranslationPointer");
}

VocabularyModel::~VocabularyModel()
{
}

void VocabularyModel::setDocument(KEduVocDocument * doc)
{
    beginResetModel();

    m_document = doc;
    m_container = 0;
    m_lesson = 0;

    if (m_document) {
        showContainer(m_document->lesson());
    } else {
        showContainer(0);
    }

    endResetModel();
}


void VocabularyModel::showContainer(KEduVocContainer * container)
{
    // use remove and insert rows. using reset resets all table headers too.
    if (rowCount(QModelIndex()) > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);
        m_container = 0;
        endRemoveRows();
    }
    if (container) {
        if (container->entryCount(m_recursive) > 0) {
            beginInsertRows(QModelIndex(), 0, container->entryCount(m_recursive) - 1);
            m_container = container;
            endInsertRows();
        } else {
            m_container = container;
        }
    }
}

void VocabularyModel::setLesson(KEduVocLesson * lessonContainer)
{
    m_lesson = lessonContainer;
}

KEduVocLesson * VocabularyModel::lesson()
{
    return m_lesson;
}

int VocabularyModel::rowCount(const QModelIndex &index) const
{
    // no lesson set - zarro rows
    if (!m_container) {
        return 0;
    }
    // only the root index has children because we have no hierarchical model.
    if (index == QModelIndex()) {
        return m_container->entryCount(m_recursive);
    }
    return 0;
}

int VocabularyModel::columnCount(const QModelIndex &) const
{
    if (!m_document) {
        return 0;
    }
    return m_document->identifierCount() * EntryColumnsMAX;
}

QVariant VocabularyModel::data(const QModelIndex & index, int role) const
{
    if (!m_document || !m_container) {
        return QVariant();
    }

    int translationId = translation(index.column());
    int entryColumn = columnType(index.column());

    switch (role) {
    case Qt::DisplayRole:
        switch (entryColumn) {
        case Translation:
            return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->text());
        case Pronunciation:
            return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->pronunciation());
        case WordClass:
            // if no word type is set, we get a null pointer
            if (m_container->entry(index.row(), m_recursive)->translation(translationId)->wordType()) {
                return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->wordType()->name());
            }
            return QVariant(QString());
        case Synonym: {
            QStringList displayElements;
            foreach(KEduVocTranslation * synonym,  m_container->entry(index.row(), m_recursive)->translation(translationId)->synonyms()) {
                displayElements.append(synonym->text());
            }
            return QVariant(displayElements.join(QStringLiteral("; ")));
        }
        case Antonym: {
            QStringList displayElements;
            foreach(KEduVocTranslation * antonym,  m_container->entry(index.row(), m_recursive)->translation(translationId)->antonyms()) {
                displayElements.append(antonym->text());
            }
            return QVariant(displayElements.join(QStringLiteral("; ")));
        }
        case Example: {
            QString example = m_container->entry(index.row(), m_recursive)->translation(translationId)->example();
            /*QString word = m_container->entry(index.row(), m_recursive)->translation(translationId)->text();
            int pos = 0;
            QString start = "<font color=\"#FF0000\"><b>";
            QString end = "</b></font>";
            while ((pos = example.indexOf(word, pos)) >= 0) {
                example.insert(pos, start);
                example.insert(pos+word.length()+start.length(), end);
                pos += word.length()+start.length()+end.length();
            }*/
            return QVariant(example);
        }
        case Comment:
            return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->comment());
        case Paraphrase:
            return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->paraphrase());
        default:
            return QVariant();
        }
        break;
    case Qt::FontRole:
        if (entryColumn == Translation) {
            QString locale = m_document->identifier(translationId).locale();
            LanguageSettings ls(locale);
            ls.load();
            return ls.editorFont();
        }
        return QVariant();
    case LocaleRole:
        return QVariant(m_document->identifier(translationId).locale());
    case AudioRole:
        return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->soundUrl());
    case ImageRole:
        return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->imageUrl());
    case EntryRole: {
        QVariant v;
        v.setValue(m_container->entry(index.row(), m_recursive));
        return v;
    }
    case Qt::ToolTipRole: {
        ///@todo more tooltips?
        switch (entryColumn) {
        case WordClass:
            return i18n("You can drag and drop words onto their word type.");
        case Synonym:
            return i18n("Enable the synonym view to edit synonyms.");
        case Antonym:
            return i18n("Enable the antonym view to edit antonyms.");
        }
    }
    }
    return QVariant();
}


bool VocabularyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ((!index.isValid()) || (role != Qt::EditRole)) {
        return false;
    }

    int translationId = translation(index.column());
    int column = columnType(index.column());

    switch (column) {
    case Translation:
        m_container->entry(index.row(), m_recursive)->translation(translationId)->setText(value.toString());
        break;
    case Pronunciation:
        m_container->entry(index.row(), m_recursive)->translation(translationId)->setPronunciation(value.toString());
        break;
    case WordClass:
        break;
    case Example:
        m_container->entry(index.row(), m_recursive)->translation(translationId)->setExample(value.toString());
        break;
    case Comment:
        m_container->entry(index.row(), m_recursive)->translation(translationId)->setComment(value.toString());
        break;
    case Paraphrase:
        m_container->entry(index.row(), m_recursive)->translation(translationId)->setParaphrase(value.toString());
        break;
    default:
        return false;
    }

    emit(dataChanged(index, index));
    ///@todo trust dirty bit
    m_document->setModified();
    return true;
}

Qt::ItemFlags VocabularyModel::flags(const QModelIndex & index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsDropEnabled;
    }

    switch (columnType(index.column())) {
    case Translation:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    case WordClass:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    case Synonym:
    case Antonym:
        return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
    default:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDropEnabled;
    }
}

QVariant VocabularyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_document) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        int translationId = section / EntryColumnsMAX;
        int entryColumn = section % EntryColumnsMAX;

        switch (role) {
        case Qt::DisplayRole:
            return VocabularyModel::columnTitle(m_document, translationId, entryColumn, /*addLocaleSuffix*/ true);
            break;
        } // switch role
    } // if horizontal
    return QVariant();
}

QString VocabularyModel::columnTitle(KEduVocDocument *document, int translation, int column, bool addLocaleSuffix)
{
    const QString localeSuffix = document->identifier(translation).locale();
    switch (column) {
    case Translation:
        if (document->identifierCount() - 1 < translation) {
            return QString();
        }
        return document->identifier(translation).name(); //returns "English", "German", etc
    case Pronunciation:
        return addLocaleSuffix ? i18n("Pronunciation (%1)", localeSuffix) : i18n("Pronunciation");
    case WordClass:
        return addLocaleSuffix ? i18n("Word Type (%1)", localeSuffix) : i18n("Word Type");
    case Synonym:
        return addLocaleSuffix ? i18n("Synonym (%1)", localeSuffix) : i18n("Synonym");
    case Antonym:
        return addLocaleSuffix ? i18n("Antonym (%1)", localeSuffix) : i18n("Antonym");
    case Example:
        return addLocaleSuffix ? i18n("Example (%1)", localeSuffix) : i18n("Example");
    case Comment:
        return addLocaleSuffix ? i18n("Comment (%1)", localeSuffix) : i18n("Comment");
    case Paraphrase:
        return addLocaleSuffix ? i18n("Paraphrase (%1)", localeSuffix) : i18n("Paraphrase");
    }

    return QString();
}

int VocabularyModel::translation(int column)
{
    return column / EntryColumnsMAX;
}

int VocabularyModel::columnType(int column)
{
    return column % EntryColumnsMAX;
}

QModelIndex VocabularyModel::appendEntry(KEduVocExpression *expression)
{
    if (m_document->identifierCount() == 0) {
        KMessageBox::information(0, i18n("Please use Edit -> Languages to set up your document."), i18n("No Languages Defined"));
        return QModelIndex();
    }

    if (!m_lesson || !m_lesson->parent()) {
        KMessageBox::information(0, i18n("Select a unit before adding vocabulary."), i18n("No Unit Selected"));
        delete expression;
        return QModelIndex();
    }

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    if (!expression) {
        expression = new KEduVocExpression;
    }
    m_lesson->appendEntry(expression);
    endInsertRows();

    return index(rowCount(QModelIndex()) - 1, 0, QModelIndex());
}

bool VocabularyModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if (count < 1 || row < 0 || row + count > m_container->entryCount(m_recursive)) {
        return false;
    }

    int bottomRow = row + count - 1;
    beginRemoveRows(QModelIndex(), row, bottomRow);

    for (int i = bottomRow; i >= row; i--) {
        delete m_container->entry(i, m_recursive);
    }

    endRemoveRows();
    return true;
}

QStringList VocabularyModel::mimeTypes() const
{
    return QStringList() << QStringLiteral("text/plain");
}

QMimeData * VocabularyModel::mimeData(const QModelIndexList & indexes) const
{
    VocabularyMimeData *mimeData = new VocabularyMimeData();
    QModelIndexList sortedIndexes = indexes;
    std::sort(sortedIndexes.begin(), sortedIndexes.end());

    qDebug() << "mimeData for " << indexes.count() << "indexes";

    QList<KEduVocTranslation*> translations;
    foreach(const QModelIndex & index, sortedIndexes) {
        // only add if it's a translation. other cells like word type are being ignored for now.
        if (columnType(index.column()) == Translation) {
            translations.append(m_container->entry(index.row(), m_recursive)->translation(translation(index.column())));
        }
    }

    mimeData->setTranslations(translations);
    return mimeData;
}

void VocabularyModel::showEntriesOfSubcontainers(bool show)
{
    beginResetModel();
    Prefs::setShowSublessonentries(show);
    if (show) {
        m_recursive = KEduVocContainer::Recursive;
    } else {
        m_recursive = KEduVocContainer::NotRecursive;
    }
    endResetModel();
}

void VocabularyModel::resetLanguages()
{
    // play it save - this happens seldom enough to warrant a reload
    setDocument(m_document);
}

void VocabularyModel::automaticTranslation(bool enabled)
{
    qDebug() << "auto trans enabled: " << enabled;
    Prefs::setAutomaticTranslation(true);
}
