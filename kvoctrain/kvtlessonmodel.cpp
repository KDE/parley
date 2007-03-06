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

#include <kdebug.h>
#include <klocale.h>

#include <keduvocdocument.h>
#include "kvtlessonmodel.h"

/** @file
  * implementation of KVTLessonModel
  * basic functions to create the model from the stringlist from m_doc.
  */


/**
 * Set the new source kvtml file
 * @param doc the new file
 */
void KVTLessonModel::setDocument(KEduVocDocument * doc)
{
  m_doc = doc;
  /*
  m_lessonList.clear();
  foreach(QString lesson, m_doc->lessonDescriptions())
    m_lessonList.append(lesson);
  //m_lessonList.append(i18n("<no lesson>")); */
  reset();
}

/**
 * The number of Lessons +2 because we have those "All lessons" and "Not in lesson" entries.
 * @param parent will always be QModelIndex() as long as we only have a list here.
 * @return number lessons +2.
 */
int KVTLessonModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return m_doc->lessonCount();
}

// at this moment not used
/** @todo I found out, qlistview does NOT support a header. They recomend using treeview instead. Eventually I will thus change to treeview. */
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

/** flags of the items - the drag and drop flag goes here one day... 
 * @param index item
 * @return flags
 */
Qt::ItemFlags KVTLessonModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

/**
 * data of an entry.
 * @param index index of an entry
 * @param role Qt::DisplayRole = lesson name, Qt::CheckStateRole = checkbox state
 * @return data
 */
QVariant KVTLessonModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_doc->lessonCount())
    return QVariant();

  if (role == Qt::DisplayRole || role == Qt::EditRole)
    return m_doc->lessonDescriptions().at(index.row());

  /** checkboxes */
  if (role == Qt::CheckStateRole) {
    if (m_doc->lessonsInQuery().contains(index.row()))
      return Qt::Checked;
    else
      return Qt::Unchecked;
  } else
      return QVariant();
}


/**
 * Change the name of a lesson. This is called automatically when the user changes an entry in the list.
 * @param index which lesson
 * @param value new name
 * @param role
 * @return bool: worked
 */
bool KVTLessonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!index.isValid())
    return false;

  if (index.row() >= m_doc->lessonCount())
    return false;

  if (role == Qt::EditRole) {
    QStringList list = m_doc->lessonDescriptions();
    list.replace(index.row(), value.toString());

    //    kDebug() << list << endl;
    m_doc->setLessonDescriptions(list);

    // not only in m_doc, but here as well:
    //m_lessonList.replace(index.row() , value.toString());

    emit dataChanged(index, index);
    return true;
  }

  /** checkboxes */
  if (role == Qt::CheckStateRole) {
    QList<int> intLessons;

    foreach(int lesson, m_doc->lessonsInQuery())
      intLessons.append(lesson);
    if (intLessons.contains(index.row())){
      intLessons.removeAt(intLessons.indexOf(index.row()));
    } else {
        if (value.toInt() == Qt::Checked)
          intLessons.append(index.row());
    }

    m_doc->setLessonsInQuery(intLessons);
    m_doc->setModified();
    return true;
  }
  return false;
}


/**
 * NOT IMPLEMENTED - add a new lesson.
 * @param position
 * @param rows
 * @param parent
 * @return
 */
bool KVTLessonModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  Q_UNUSED(parent);
  beginInsertRows(QModelIndex(), position, position + rows - 1);

  for (int row = 0; row < rows; ++row) {
      //TODO how do I set up a new lesson?
      //m_doc->lessonDescriptions().insert(position, "");
  }

  endInsertRows();
  return true;
}


/**
 * NOT IMPLEMENTED - Delete a lesson
 * @param position
 * @param rows
 * @param parent
 * @return
 */
bool KVTLessonModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  Q_UNUSED(parent);
  beginRemoveRows(QModelIndex(), position, position+rows-1);

  for (int row = 0; row < rows; ++row) {
      //stringList.removeAt(position);
      //TODO delete lesson? Only if empty?
  }

  endRemoveRows();
  return true;
}


/**
 * propagete the change of current lesson
 * @param start
 * @param end
 */
void KVTLessonModel::slotLessonSelectionChanged(const QModelIndex &start, const QModelIndex &end){
  Q_UNUSED(end);
  /** for now we only have one lesson selectable at the time - start is enough */

  int index = start.row(); /** no use making all current */
  if (index >= 0){
    m_doc->setCurrentLesson(index);
    //kDebug() << "Current lesson set to: " << index << " " << m_lessonList.at(index+1) << endl;
  }
}


#include "kvtlessonmodel.moc"
