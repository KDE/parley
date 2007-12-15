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
#include <keduvocwordtype.h>

#include "vocabularymimedata.h"

#include <keduvocexpression.h>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>
#include <QPixmap>

VocabularyModel::VocabularyModel(QObject *parent)
 : QAbstractTableModel(parent)
{
    m_document = 0;
    m_container = 0;

    qRegisterMetaType<KEduVocTranslation*>("KEduVocTranslationStar");
}


VocabularyModel::~VocabularyModel()
{
}

void VocabularyModel::setDocument(KEduVocDocument * doc)
{
    m_document = doc;
    m_container = 0;
    m_lesson = 0;
    m_wordType = 0;


    /// @todo does this make sense?
    if ( m_document ) {
        showContainer(m_document->lesson());
    }
    reset();
}


void VocabularyModel::showContainer(KEduVocContainer * container)
{
    // use remove and insert rows. using reset resets all table headers too.
    if (rowCount(QModelIndex()) > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex())-1);
        endRemoveRows();
    }
    m_container = container;
    if (container && container->entryCount() > 0) {
        beginInsertRows(QModelIndex(), 0, m_container->entryCount()-1);
        endInsertRows();
    }
}



void VocabularyModel::setLesson(KEduVocLesson * lessonContainer)
{
    m_lesson = lessonContainer;
}

void VocabularyModel::setWordType(KEduVocWordType * wordTypeContainer)
{
    m_wordType = wordTypeContainer;
}



int VocabularyModel::rowCount(const QModelIndex &) const
{
    if ( !m_container ) {
        return 0;
    }
    return m_container->entryCount();
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
    if ( !m_document || !m_container ) {
        return QVariant();
    }

    int translationId = translation(index.column());
    int entryColumn = columnType(index.column());

    switch (role) {
    case Qt::DisplayRole:
        switch (entryColumn) {
        case Translation:
            return QVariant(m_container->entry(index.row())->translation(translationId)->text());
        case Pronunciation:
            return QVariant(m_container->entry(index.row())->translation(translationId)->pronunciation());
        case WordType:
            // if no word type is set, we get a null pointer
            if(m_container->entry(index.row())->translation(translationId)->wordType()) {
                return QVariant(m_container->entry(index.row())->translation(translationId)->wordType()->name());
            }
            return QVariant();
        case Synonym:
            return QVariant(m_container->entry(index.row())->translation(translationId)->synonym());
        case Antonym:
            return QVariant(m_container->entry(index.row())->translation(translationId)->antonym());
        case Example: {
            QString example = m_container->entry(index.row())->translation(translationId)->example();
            /*QString word = m_container->entry(index.row())->translation(translationId)->text();
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
            return QVariant(m_container->entry(index.row())->translation(translationId)->comment());
        case Paraphrase:
            return QVariant(m_container->entry(index.row())->translation(translationId)->paraphrase());
//         case Audio:
//         case Image:
        default:
            return QVariant();
        }
        break;
//     case Qt::DecorationRole: {
//         switch (entryColumn) {
//         case Audio:
//             if ( !m_container->entry(index.row())->translation(translationId)->soundUrl().isEmpty() ) {
//                 return KIcon("media-playback-start");
//             }
//             return QVariant();
//         case Image:
//             if ( !m_container->entry(index.row())->translation(translationId)->imageUrl().isEmpty() ) {
//                 return QPixmap(m_container->entry(index.row())->translation(translationId)->imageUrl().toLocalFile()).scaled(QSize(30,30));
//             }
//             return QVariant();
//         default:
//             return QVariant();
//         }
//         break;
//     }
//     case Qt::SizeHintRole:
//         switch (entryColumn) {
//         case Audio:
//         case Image:
//             return QSize(25, 25);
//         }

    case EntryRole: {
        QVariant v;
        v.setValue(m_container->entry(index.row()));
        return v;
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
        m_container->entry(index.row())->translation(translationId)->setText(value.toString());
        break;
    case Pronunciation:
        m_container->entry(index.row())->translation(translationId)->setPronunciation(value.toString());
        break;
    case WordType:
//             m_container->entry(index.row())->translation(translationId)->type();
        break;
    case Synonym:
        m_container->entry(index.row())->translation(translationId)->setSynonym(value.toString());
        break;
    case Antonym:
        m_container->entry(index.row())->translation(translationId)->setAntonym(value.toString());
        break;
    case Example:
        m_container->entry(index.row())->translation(translationId)->setExample(value.toString());
        break;
    case Comment:
        m_container->entry(index.row())->translation(translationId)->setComment(value.toString());
        break;
    case Paraphrase:
        m_container->entry(index.row())->translation(translationId)->setParaphrase(value.toString());
        break;
//     case Audio:
//     case Image:
    default:
        return false;
    }

    return true;
}

Qt::ItemFlags VocabularyModel::flags(const QModelIndex & index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
    }

    switch (columnType(index.column())) {
    case Translation:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    case WordType:
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
                return i18n("Word Type");
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
//         case Qt::DecorationRole:
//             switch(entryColumn){
//             case Audio:
//                 return KIcon("speaker");
//             case Image:
//                 return KIcon("view-preview");
//             }

//         case Qt::SizeHintRole:
//             switch (entryColumn) {
//             case Audio:
//             case Image:
//                 return QSize(25, 25);
//             }
        } // switch role
    } // if horizontal
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

QModelIndex VocabularyModel::appendEntry()
{
kDebug() << "appendEntry";
    if(!m_lesson) {
        return QModelIndex();
    }

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    KEduVocExpression* entry = new KEduVocExpression;
    m_lesson->addEntry(entry);
    endInsertRows();
kDebug() << "appendEntry done";
    // the last row will be the new entry
    return index(rowCount(QModelIndex()) - 1, 0, QModelIndex());
}

QStringList VocabularyModel::mimeTypes() const
{
    return QStringList() << "text/plain";
}

QMimeData * VocabularyModel::mimeData(const QModelIndexList & indexes) const
{
    VocabularyMimeData *mimeData = new VocabularyMimeData();

    foreach (QModelIndex index, indexes) {
        mimeData->addTranslation(m_container->entry(index.row())->translation(translation(index.column())));
    }

    return mimeData;
}



#include "vocabularymodel.moc"
