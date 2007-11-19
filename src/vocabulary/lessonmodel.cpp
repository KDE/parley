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

#include "lessonmodel.h"

#include <QItemSelection>
#include <QMimeData>

#include <kdebug.h>
#include <klocale.h>

#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocexpression.h>
#include <krandom.h>

/** @file
  * Implementation of LessonModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */





LessonModel::LessonModel(QObject * parent) : QAbstractItemModel(parent)
{
    m_rootLesson = 0;
}



void LessonModel::setDocument(KEduVocDocument * doc)
{
kDebug() << "LessonModel::setDocument()" << doc;
    m_rootLesson = doc->lesson();
    if(m_rootLesson) {
        beginInsertColumns(QModelIndex(), 0, 1);
        endInsertColumns();

        beginInsertRows(QModelIndex(), 0, 1);
        endInsertRows();

        kDebug() << m_rootLesson->name();
        kDebug() << m_rootLesson->childLessonCount();
    }
}



QModelIndex LessonModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    KEduVocLesson *parentLesson;

    if (!parent.isValid())
        parentLesson = m_rootLesson;
    else
        parentLesson = static_cast<KEduVocLesson*>(parent.internalPointer());

    KEduVocLesson *childLesson = parentLesson->childLesson(row);
    if (childLesson)
        return createIndex(row, column, childLesson);
    else
        return QModelIndex();
}



QModelIndex LessonModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    KEduVocLesson *childItem = static_cast<KEduVocLesson*>(index.internalPointer());
    KEduVocLesson *parentItem = childItem->parent();

    if (parentItem == m_rootLesson)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


int LessonModel::rowCount(const QModelIndex &parent) const
{
    KEduVocLesson *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid())
        parentItem = m_rootLesson;
    else
        parentItem = static_cast<KEduVocLesson*>(parent.internalPointer());

    return parentItem->childLessonCount();
}





// int LessonModel::rowCount(const QModelIndex &parent) const
// {
//     Q_UNUSED(parent);
//     return m_doc->lessonCount();
// }

// QVariant LessonModel::headerData(int section, Qt::Orientation orientation, int role) const
// {
//     if (role != Qt::DisplayRole)
//         return QVariant();
// 
//     if (orientation == Qt::Horizontal && section == 0)
//         return QString(i18n("Lesson"));
// 
//     if (orientation == Qt::Horizontal)
//         return QString("Column %1").arg(section);
//     else
//         return QString("Row %1").arg(section);
// }
// 
// Qt::ItemFlags LessonModel::flags(const QModelIndex &index) const
// {
//     if (index.isValid()) {
//         return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
//         // | Qt::ItemIsDragEnabled); // | Qt::ItemIsDropEnabled);
//     }
//     return  Qt::ItemIsEnabled; // | Qt::ItemIsDropEnabled;
// }
// 
// Qt::DropActions LessonModel::supportedDropActions() const
// {
//     //return Qt::CopyAction | Qt::MoveAction;
//     return Qt::MoveAction;
// }
// 
// QVariant LessonModel::data(const QModelIndex &index, int role) const
// {
//     if (!index.isValid())
//         return QVariant();
// 
//     if (index.row() >= m_doc->lessonCount())
//         return QVariant();
// 
//     if (role == Qt::DisplayRole || role == Qt::EditRole)
//         return m_doc->lesson(index.row()).name();
// 
//     /** checkboxes */
//     if (role == Qt::CheckStateRole) {
//         if (m_doc->lesson(index.row()).inPractice())
//             return Qt::Checked;
//         else
//             return Qt::Unchecked;
//     } else
//         return QVariant();
// }
// 
// bool LessonModel::setData(const QModelIndex &index, const QVariant &value, int role)
// {
//     if (!index.isValid())
//         return false;
// 
//     if (index.row() >= m_doc->lessonCount())
//         return false;
// 
//     /** rename a lesson */
//     if (role == Qt::EditRole) {
//         m_doc->lesson(index.row()).setName(value.toString());
//         emit dataChanged(index, index);
//         return true;
//     }
// 
//     /** checkboxes */
//     if (role == Qt::CheckStateRole) {
//         if (!m_doc->lesson(index.row()).inPractice())
//             m_doc->lesson(index.row()).setInPractice(true);
//         else
//             m_doc->lesson(index.row()).setInPractice(false);
//         m_doc->setModified();
//         emit dataChanged(index, index);
//         return true;
//     }
//     return false;
// }
// 
// 
// void LessonModel::setAllLessonsInPractice()
// {
//     for (int lesson = 0; lesson < m_doc->lessonCount(); lesson++) {
//         m_doc->lesson(lesson).setInPractice(true);
//     }
// 
//     // this is not too great - say all data has changed
//     emit dataChanged(index(0, 0, QModelIndex()), index(rowCount(), 0, QModelIndex()));
//     //emit signalLessonsInPracticeChanged(intLessons);
// }


// void LessonModel::setNoLessonsInPractice()
// {
//     for (int lesson = 0; lesson < m_doc->lessonCount(); lesson++) {
//         m_doc->lesson(lesson).setInPractice(false);
//     }
// 
//     // this is not too great - say all data has changed
//     emit dataChanged(index(0, 0, QModelIndex()), index(rowCount(), 0, QModelIndex()));
//     //emit signalLessonsInPracticeChanged(intLessons);
// }
// 
// int LessonModel::addLesson(const QString &lessonName)
// {
//     beginInsertRows(QModelIndex(), m_doc->lessonCount(), m_doc->lessonCount());
//     int newLessonIndex;
//     if (lessonName.isNull()) {
//         // add the lesson
//         newLessonIndex = m_doc->appendLesson(QString(), true);
//         // then name it according to its index (humans like to count from 1)
//         m_doc->lesson(newLessonIndex).setName(i18n("Lesson %1", newLessonIndex + 1));
//     } else {
//         newLessonIndex = m_doc->appendLesson(lessonName);
//     }
// 
//     endInsertRows();
//     return newLessonIndex;
// }
// 
// bool LessonModel::deleteLesson(int lessonIndex, KEduVocDocument::LessonDeletion mode)
// {
//     bool couldDelete = m_doc->removeLesson(lessonIndex, mode);
//     if (couldDelete) {
//         beginRemoveRows(QModelIndex(), lessonIndex, lessonIndex);
//         endRemoveRows();
//     }
//     return couldDelete;
// }
// 
// bool LessonModel::removeRows(int row, int count, const QModelIndex &parent)
// {
//     Q_UNUSED(parent);
//     /// @todo either really use this or remove it
//     beginRemoveRows(QModelIndex(), row, row);
//     endRemoveRows();
// 
//     // to support drag and drop
//     kDebug() << "removeRows(int row, int count, const QModelIndex &parent)" << row << ", " << count;
//     return true;
// }

/*
void LessonModel::splitLesson(int lessonIndex, int entriesPerLesson, SplitLessonOrder order)
{
    // list of entries in the lesson
    QList<KEduVocExpression*> entryList;
    for (int i = 0; i < m_doc->entryCount(); i++) {
        KEduVocExpression *expr = m_doc->entry(i);
        if (expr->lesson() == lessonIndex)
            entryList.append(expr);
    }

    QString originalLessonName = m_doc->lesson(lessonIndex).name();
    int numNewLessons = entryList.count()/entriesPerLesson;
    if (entryList.count()%entriesPerLesson) // modulo - fraction lesson if not 0 we need one more
        numNewLessons++;

    // create the empty lessons
    int first = addLesson(originalLessonName + QString(" %1").arg(1));
    int last;
    for (int i=1; i<numNewLessons; i++) {
        last = addLesson(originalLessonName + QString(" %1").arg(i+1));
    }

    int lessonToFill=first; /// lesson which receives the entries until full
    int entries = 0; /// number entries in the lesson that is being filled
    int nextEntry=0; /// next entry to be assigned to one of the new lessons
    while (!entryList.empty()) {
        if (entries == entriesPerLesson) {
            lessonToFill++;
            entries=0;
        }

        if (order == random)
            nextEntry = KRandom::random() % entryList.count(); /// @todo random from 0 to entryList.count() -1;
        entryList.at(nextEntry)->setLesson(lessonToFill);
        entryList.removeAt(nextEntry);
        entries++;
    }

    if (!deleteLesson(lessonIndex, KEduVocDocument::DeleteEmptyLesson))
        kDebug() << "Warning - could not delete old lesson!";

    m_doc->setModified(true);

    ///@todo check the entire function
}*/



// bool LessonModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
// {
//     Q_UNUSED(parent);
//     Q_UNUSED(column);
//     QByteArray encodedData = data->data("application/vnd.text.list");
//     QDataStream stream(&encodedData, QIODevice::ReadOnly);
//     QStringList newItems;
//     int rows = 0;
// 
//     while (!stream.atEnd()) {
//         QString text;
//         stream >> text;
//         newItems << text;
//         ++rows;
//     }
// 
//     kDebug() << "dropMimeData() " << newItems << " row: " << row << " Qt::DropAction: " << action;
//     return false;
// }

QVariant LessonModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) {
         return QVariant();
    }

    if (role != Qt::DisplayRole) {
         return QVariant();
    }

    KEduVocLesson *lesson = static_cast<KEduVocLesson*>(index.internalPointer());
    switch (index.column()){
    case 0:
        return lesson->name();
    case 1:
        return lesson->entryCount();
    }
}

Qt::ItemFlags LessonModel::flags(const QModelIndex & index) const
{
      if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant LessonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return "lesson";
        case 1:
            return "#";
        }
    }
    return QVariant();
}

int LessonModel::columnCount(const QModelIndex & parent) const
{
    if(!m_rootLesson) {
        return 0;
    }

    if (!parent.isValid()) {
        return 2;
    }
    return 2;
}


#include "lessonmodel.moc"
