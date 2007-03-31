//
// C++ Implementation: kvtlessonmodel
//
// Description:
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QItemSelection>
#include <QMimeData>

#include <kdebug.h>
#include <klocale.h>

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include <krandom.h>

#include "kvtlessonmodel.h"

/** @file
  * Implementation of KVTLessonModel.
  * Functions to create the model from the stringlist of the vocabulary document.
  */



/** @todo the old clean up code is homeless right now: */
// void LessOptPage::slotCleanup()
// {
//   QVector<bool> used_lesson;
//   for (int i = 0; i < (int) optionsList->count(); i++)
//     used_lesson.append(false);
//
//   for (int i = 0; i < (int) doc->entryCount(); i++) {
//     int idx = doc->entry(i)->lesson();
//     if ((int) used_lesson.count() < idx)
//       used_lesson.resize(idx);
//     if (idx != 0)
//       used_lesson[idx - 1] = true ;
//   }
//
//   for (int i = used_lesson.count() - 1; i >= 0; i--)
//     if (!used_lesson[i]) {
//       for (int u = 0; u < (int) lessonIndex.count() ; u++) {
//         if (lessonIndex[u] == i || lessonIndex[u] < 0) {
//           m_currentLesson = i;
//           slotDeleteLesson();
//           break;
//         }
//       }
//     }
//
//   m_currentLesson = 0;
//   optionsList->setCurrentItem(optionsList->item(m_currentLesson));
// }




// void LessOptPage::cleanUnused(KEduVocDocument *doc, const QList<int> &lessonIndex, int old_lessons, QList<int> &lessons_in_query)
// {
//   QList<int> translate_index;
//
//   /////////////////////////////////////////////////////
//   // translate_index contains new index number for each
//   // old index
//   for (int i = 0; i <= qMax (old_lessons, (int) lessonIndex.count()); i++)
//     translate_index.append(0);
//
//   // now adjust lesson descriptions to new index
//   for (int i = 0; i < (int) lessonIndex.size(); i++) {
//     if (lessonIndex[i] >= 0)
//       translate_index[lessonIndex[i] /*+ 1*/] = i + 1;
//   }
//
//   // now adjust for query selected lessons to new index
//   for (int i = (int) lessons_in_query.count() - 1; i >= 0; i--) {
//     if (translate_index[lessons_in_query[i]] > 0)
//       lessons_in_query[i] = translate_index[lessons_in_query[i]];
//     else
//       lessons_in_query.erase(lessons_in_query.begin() + i);
//   }
//
//   // only keep remaining lesson member indices
//
//   // set lesson index to 0 when not needed any more
//   // and translate to new index
//   for (int i = 0; i < doc->entryCount(); i++) {
//     if (doc->entry(i)->lesson() != 0)
//       doc->entry(i)->setLesson(translate_index[doc->entry(i)->lesson()]);
//   }
// }

void KVTLessonModel::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
    reset();
}

int KVTLessonModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_doc->lessonCount();
}

QVariant KVTLessonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal && section == 0)
        return QString(i18n("Lesson"));

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

Qt::ItemFlags KVTLessonModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled); // | Qt::ItemIsDropEnabled);
    }
    return  Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
}

Qt::DropActions KVTLessonModel::supportedDropActions() const
{
    //return Qt::CopyAction | Qt::MoveAction;
    return Qt::MoveAction;
}

QVariant KVTLessonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_doc->lessonCount())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_doc->lessonDescription(index.row()+1);

    /** checkboxes */
    if (role == Qt::CheckStateRole) {
        if (m_doc->lessonInQuery(index.row()+1))
            return Qt::Checked;
        else
            return Qt::Unchecked;
    } else
        return QVariant();
}

bool KVTLessonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (index.row() >= m_doc->lessonCount())
        return false;

    /** rename a lesson */
    if (role == Qt::EditRole) {
        m_doc->renameLesson(index.row()+1, value.toString());
        emit dataChanged(index, index);
        return true;
    }

    /** checkboxes */
    if (role == Qt::CheckStateRole) {
        if (!m_doc->lessonInQuery(index.row()+1))
            m_doc->addLessonToQuery(index.row()+1);
        else
            m_doc->removeLessonFromQuery(index.row()+1);
        m_doc->setModified();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}


void KVTLessonModel::setAllLessonsInQuery()
{
    QList<int> intLessons;
    for (int lesson =1; lesson <= m_doc->lessonCount(); lesson++) {
        intLessons.append(lesson);
    }
    m_doc->setLessonsInQuery(intLessons);
    // this is not too great - say all data has changed
    emit dataChanged(index(0, 0, QModelIndex()), index(rowCount(), 0, QModelIndex()));
    //emit signalLessonsInQueryChanged(intLessons);
}


void KVTLessonModel::setNoLessonsInQuery()
{
    QList<int> intLessons;
    m_doc->setLessonsInQuery(intLessons);
    // this is not too great - say all data has changed
    emit dataChanged(index(0, 0, QModelIndex()), index(rowCount(), 0, QModelIndex()));
    //emit signalLessonsInQueryChanged(intLessons);
}

int KVTLessonModel::addLesson(const QString &lessonName)
{
    beginInsertRows(QModelIndex(), m_doc->lessonCount(), m_doc->lessonCount());
    int newLessonIndex;
    if (lessonName.isNull()) {
        int i = 1;
        while (m_doc->lessonIndex(i18n("New lesson") + QString(" %1").arg(i)) > 0)
            i++;
        newLessonIndex = m_doc->appendLesson(QString(i18n("New lesson") + QString(" %1").arg(i)));
    } else {
        newLessonIndex = m_doc->appendLesson(lessonName);
    }
    /// Now add the new lesson to the query. Not necessary, but nice.
    m_doc->addLessonToQuery(newLessonIndex);

    endInsertRows();
    return newLessonIndex;
}

bool KVTLessonModel::deleteLesson(int lessonIndex, KEduVocDocument::LessonDeletion mode)
{
    bool couldDelete = m_doc->deleteLesson(lessonIndex, mode);
    if (couldDelete) {
        beginRemoveRows(QModelIndex(), lessonIndex-1, lessonIndex-1);
        endRemoveRows();
    }
    return couldDelete;
}

bool KVTLessonModel::removeRows(int row, int count, const QModelIndex &parent)
{
    /// @todo either really use this or remove it
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();

    // to support drag and drop
    kDebug() << "removeRows(int row, int count, const QModelIndex &parent)" << row << ", " << count << endl;
    return true;
}


void KVTLessonModel::splitLesson(int lessonIndex, int entriesPerLesson, SplitLessonOrder order)
{
    // list of entries in the lesson
    QList<KEduVocExpression*> entryList;
    for (int i = 0; i < m_doc->entryCount(); i++) {
        KEduVocExpression *expr = m_doc->entry(i);
        if (expr->lesson() == lessonIndex)
            entryList.append(expr);
    }

    QString originalLessonName = m_doc->lessonDescription(lessonIndex);
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

    if (!deleteLesson(lessonIndex -1, KEduVocDocument::DeleteEmptyLesson))
        kDebug() << "Warning - could not delete old lesson!" << endl;

    m_doc->setModified(true);

    ///@todo select a sensible lesson - like the first new one.
}



bool KVTLessonModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    while (!stream.atEnd()) {
        QString text;
        stream >> text;
        newItems << text;
        ++rows;
    }

    kDebug() << "dropMimeData() " << newItems << " row: " << row << " Qt::DropAction: " << action << endl;
    return false;
}

#include "kvtlessonmodel.moc"
