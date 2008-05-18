/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
#include "prefs.h"

#include <keduvoclesson.h>
#include <keduvocwordtype.h>

#include "vocabularymimedata.h"

#include <keduvocexpression.h>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>
#include <KMessageBox>
#include <QPixmap>

VocabularyModel::VocabularyModel(QObject *parent)
 : QAbstractTableModel(parent)
{
    m_document = 0;
    m_container = 0;
    m_recursive = Prefs::showSublessonentries() ? KEduVocContainer::Recursive : KEduVocContainer::NotRecursive;

    qRegisterMetaType<KEduVocTranslation*>("KEduVocTranslationPointer");
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

    // to get the headers right
    // (better get rid of the reset)
    reset();

    emit documentChanged(doc);

    if ( m_document ) {
        showContainer(m_document->lesson());
    } else {
        showContainer(0);
    }
}


void VocabularyModel::showContainer(KEduVocContainer * container)
{
    // use remove and insert rows. using reset resets all table headers too.
    if (rowCount(QModelIndex()) > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex())-1);
        m_container = 0;
        endRemoveRows();
    }
    if (container) {
        if (container->entryCount(m_recursive) > 0) {
            beginInsertRows(QModelIndex(), 0, container->entryCount(m_recursive)-1);
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

void VocabularyModel::setWordType(KEduVocWordType * wordTypeContainer)
{
    m_wordType = wordTypeContainer;
}


int VocabularyModel::rowCount(const QModelIndex &index) const
{
    // no lesson set - zarro rows
    if ( !m_container ) {
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
            return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->text());
        case Pronunciation:
            return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->pronunciation());
        case WordType:
            // if no word type is set, we get a null pointer
            if(m_container->entry(index.row(), m_recursive)->translation(translationId)->wordType()) {
                return QVariant(m_container->entry(index.row(), m_recursive)->translation(translationId)->wordType()->name());
            }
            return QVariant();
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
//         case Audio:
//         case Image:
        default:
            return QVariant();
        }
        break;
//     case Qt::DecorationRole: {
//         switch (entryColumn) {
//         case Audio:
//             if ( !m_container->entry(index.row(), m_recursive)->translation(translationId)->soundUrl().isEmpty() ) {
//                 return KIcon("media-playback-start");
//             }
//             return QVariant();
//         case Image:
//             if ( !m_container->entry(index.row(), m_recursive)->translation(translationId)->imageUrl().isEmpty() ) {
//                 return QPixmap(m_container->entry(index.row(), m_recursive)->translation(translationId)->imageUrl().toLocalFile()).scaled(QSize(30,30));
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
    case Qt::TextColorRole:
        if (Prefs::useGradeColors() && entryColumn == Translation) {
            int grade = m_container->entry(index.row(), m_recursive)->translation(translationId)->grade();
            return Prefs::gradeColor(grade);
        } else {
            return QVariant();
        }
    case LocaleRole:
        return QVariant(m_document->identifier(translationId).locale());
    case EntryRole: {
        QVariant v;
        v.setValue(m_container->entry(index.row(), m_recursive));
        return v;
        }
    case Qt::ToolTipRole: {
        ///@todo more tooltips?
        switch (entryColumn) {
        case WordType:
            return i18n("You can drag and drop words onto their word type.");
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
    case WordType:
//             m_container->entry(index.row(), m_recursive)->translation(translationId)->type();
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
//     case Audio:
//     case Image:
    default:
        return false;
    }

    emit(dataChanged(index, index));
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
    case WordType:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
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
                if (m_document->identifierCount() - 1 < translationId) {
                    return QVariant();
                }
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

QModelIndex VocabularyModel::appendEntry(KEduVocExpression *expression)
{
    if(!m_lesson || !m_container) {
        return QModelIndex();
    }

    if (!m_container->parent()) {
        KMessageBox::information(0, i18n("Please select a lesson before pasting."), i18n("No Lesson Selected"));
        delete expression;
        return QModelIndex();
    }

//     if (m_container->containerType() != KEduVocContainer::Lesson) {
//         KMessageBox::information(0, i18n("Please select a lesson before pasting."), i18n("No Lesson Selected"));
//     }

    if(m_document->identifierCount() == 0) {
        KMessageBox::information(0, i18n("Please use Edit -> Languages to set up your document."), i18n("No Languages Defined"));
        return QModelIndex();
    }

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    if (!expression) {
        expression = new KEduVocExpression;
    }
    m_lesson->appendEntry(expression);
    endInsertRows();

    ///FIXME: not correct - the last row of the old lesson:
    // the last row will be the new entry
    return index(rowCount(QModelIndex()) - 1, 0, QModelIndex());
}

bool VocabularyModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if (count < 1 || row < 0 || row + count > m_container->entryCount(m_recursive)) {
        return false;
    }

    int bottomRow = row + count - 1;

    for (int i = bottomRow; i >= row; i--) {
        delete m_container->entry(i, m_recursive);
    }

    endRemoveRows();
    return true;
}

QStringList VocabularyModel::mimeTypes() const
{
    return QStringList() << "text/plain";
}

QMimeData * VocabularyModel::mimeData(const QModelIndexList & indexes) const
{
    VocabularyMimeData *mimeData = new VocabularyMimeData();
    QModelIndexList sortedIndexes = indexes;
    qSort(sortedIndexes);

    kDebug() << "mimeData for " << indexes.count() << "indexes";

    QList<KEduVocTranslation*> translations;
    foreach (const QModelIndex &index, sortedIndexes) {
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
    Prefs::setShowSublessonentries(show);
    if (show) {
        m_recursive = KEduVocContainer::Recursive;
    } else {
        m_recursive = KEduVocContainer::NotRecursive;
    }
    reset();
}

void VocabularyModel::resetLanguages()
{
    reset();
}



#include "vocabularymodel.moc"


