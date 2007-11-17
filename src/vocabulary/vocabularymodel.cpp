/***************************************************************************

    Copyright: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "vocabularymodel.h"

#include "parleydocument.h"

#include <keduvocexpression.h>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>
#include <QPixmap>

VocabularyModel::VocabularyModel(QObject *parent)
 : QAbstractTableModel(parent)
{
    m_document = 0;
}


VocabularyModel::~VocabularyModel()
{
}

void VocabularyModel::setDocument(KEduVocDocument * doc)
{
    beginRemoveColumns(QModelIndex(), 0, columnCount(QModelIndex()) );
    endRemoveColumns();

    m_document = doc;
    beginInsertColumns(QModelIndex(), 0,
        (m_document->identifierCount()*EntryColumnsMAX) -1);
    endInsertColumns();

    beginInsertRows(QModelIndex(), 0, m_document->entryCount() -1);
    endInsertRows();

}

int VocabularyModel::rowCount(const QModelIndex &) const
{
    if ( !m_document ) {
        return 0;
    }
    return m_document->entryCount();
}

int VocabularyModel::columnCount(const QModelIndex &) const
{
    if ( !m_document ) {
        return 0;
    }
    return m_document->identifierCount()*EntryColumnsMAX;
}

QVariant VocabularyModel::data(const QModelIndex & index, int role) const
{
    if ( !m_document ) {
        return QVariant();
    }

    int translationId = index.column() / EntryColumnsMAX;
    int entryColumn = index.column() % EntryColumnsMAX;

    switch (role) {
    case Qt::DisplayRole:
        switch (entryColumn) {
        case Translation:
            return QVariant(m_document->entry(index.row())->translation(translationId).text());
        case Pronunciation:
            return QVariant(m_document->entry(index.row())->translation(translationId).pronunciation());
        case WordType:
            return QVariant(m_document->entry(index.row())->translation(translationId).type());
        case SubWordType:
            return QVariant(m_document->entry(index.row())->translation(translationId).subType());
        case Synonym:
            return QVariant(m_document->entry(index.row())->translation(translationId).synonym());
        case Antonym:
            return QVariant(m_document->entry(index.row())->translation(translationId).antonym());
        case Example:
            return QVariant(m_document->entry(index.row())->translation(translationId).example());
        case Comment:
            return QVariant(m_document->entry(index.row())->translation(translationId).comment());
        case Paraphrase:
            return QVariant(m_document->entry(index.row())->translation(translationId).paraphrase());
        case Audio:
        case Image:
        default:
            return QVariant();
        }
        break;
    case Qt::DecorationRole:
        switch (entryColumn) {
        case Audio:
            if ( !m_document->entry(index.row())->translation(translationId).soundUrl().isEmpty() ) {
                return KIcon("media-playback-start");
            }
            return QVariant();
        case Image:
            if ( !m_document->entry(index.row())->translation(translationId).imageUrl().isEmpty() ) {
                return QPixmap(m_document->entry(index.row())->translation(translationId).imageUrl().toLocalFile()).scaled(QSize(30,30));
            }
            return QVariant();
        default:
            return QVariant();
        }
        break;
    }

    return QVariant();
}

Qt::ItemFlags VocabularyModel::flags(const QModelIndex & index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant VocabularyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Horizontal ) {
        int translationId = section / EntryColumnsMAX;
        int entryColumn = section % EntryColumnsMAX;

        switch (role) {
        case Qt::DisplayRole:

            switch (entryColumn){
            case Translation:
                return m_document->identifier(translationId).name();
            case Pronunciation:
                return i18n("Pronunciation");
            case WordType:
                return i18n("Word type");
            case SubWordType:
                return i18n("Sub word type");
            case Synonym:
                return i18n("Synonym");
            case Antonym:
                return i18n("Antonym");
            case Example:
                return i18n("Example");
            case Comment:
                return i18n("Comment");
            case Paraphrase:
                return i18n("Paraphrase");
            }
            break;
        case Qt::DecorationRole:
            switch(entryColumn){
            case Audio:
                return KIcon("speaker");
            case Image:
                return KIcon("view-preview");
            }
        }
    }
    return QVariant();
}

