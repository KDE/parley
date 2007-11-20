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

#include <keduvoclesson.h>

#include <keduvocexpression.h>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>
#include <QPixmap>

VocabularyModel::VocabularyModel(QObject *parent)
 : QAbstractTableModel(parent)
{
    m_document = 0;
    m_lesson = 0;
}


VocabularyModel::~VocabularyModel()
{
}

void VocabularyModel::setDocument(KEduVocDocument * doc)
{
    m_document = doc;
    m_lesson = 0;

    if ( m_document ) {
        setLesson(m_document->lesson());
    } else {
        reset();
    }
}


void VocabularyModel::setLesson(KEduVocLesson * lesson)
{
    m_lesson = lesson;
    reset();
}


int VocabularyModel::rowCount(const QModelIndex &) const
{
    if ( !m_lesson ) {
        return 0;
    }
    return m_lesson->entryCount();
}

int VocabularyModel::columnCount(const QModelIndex &) const
{
    if ( !m_document ) {
        return 0;
    }
//     kDebug() << " col count " << m_document->identifierCount()*EntryColumnsMAX;
    return m_document->identifierCount()*EntryColumnsMAX;
}

QVariant VocabularyModel::data(const QModelIndex & index, int role) const
{
    if ( !m_document || !m_lesson ) {
        return QVariant();
    }

    int translationId = translation(index.column());
    int entryColumn = columnType(index.column());

    switch (role) {
    case Qt::DisplayRole:
        switch (entryColumn) {
        case Translation:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).text());
        case Pronunciation:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).pronunciation());
        case WordType:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).type());
        case SubWordType:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).subType());
        case Synonym:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).synonym());
        case Antonym:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).antonym());
        case Example:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).example());
        case Comment:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).comment());
        case Paraphrase:
            return QVariant(m_lesson->entry(index.row())->translation(translationId).paraphrase());
        case Audio:
        case Image:
        default:
            return QVariant();
        }
        break;
    case Qt::DecorationRole:
        switch (entryColumn) {
        case Audio:
            if ( !m_lesson->entry(index.row())->translation(translationId).soundUrl().isEmpty() ) {
                return KIcon("media-playback-start");
            }
            return QVariant();
        case Image:
            if ( !m_lesson->entry(index.row())->translation(translationId).imageUrl().isEmpty() ) {
                return QPixmap(m_lesson->entry(index.row())->translation(translationId).imageUrl().toLocalFile()).scaled(QSize(30,30));
            }
            return QVariant();
        default:
            return QVariant();
        }
        break;
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
        m_lesson->entry(index.row())->translation(translationId).setText(value.toString());
        break;
    case Pronunciation:
        m_lesson->entry(index.row())->translation(translationId).setPronunciation(value.toString());
        break;
    case WordType:
//             m_lesson->entry(index.row())->translation(translationId).type();
            break;
    case SubWordType:
//             m_lesson->entry(index.row())->translation(translationId).subType();
        break;
    case Synonym:
        m_lesson->entry(index.row())->translation(translationId).setSynonym(value.toString());
        break;
    case Antonym:
        m_lesson->entry(index.row())->translation(translationId).setAntonym(value.toString());
        break;
    case Example:
        m_lesson->entry(index.row())->translation(translationId).setExample(value.toString());
        break;
    case Comment:
        m_lesson->entry(index.row())->translation(translationId).setComment(value.toString());
        break;
    case Paraphrase:
        m_lesson->entry(index.row())->translation(translationId).setParaphrase(value.toString());
        break;
    case Audio:
    case Image:
    default:
        return false;
    }

    return true;
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
    if (!m_document) {
        return QVariant();
    }

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

int VocabularyModel::translation(int column)
{
    return column / EntryColumnsMAX;
}

int VocabularyModel::columnType(int column)
{
    return column % EntryColumnsMAX;
}

#include "vocabularymodel.moc"
