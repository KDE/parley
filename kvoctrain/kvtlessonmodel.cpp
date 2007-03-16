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
  reset();
}

/**
 * The number of Lessons.
 * @param parent will always be QModelIndex() as long as we only have a list here
 * @return number of lessons
 */
int KVTLessonModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return m_doc->lessonCount();
}

/**
  * Header of the treeview
  */
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
 * data of an entry
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
    if (m_doc->lessonsInQuery().contains(index.row()+1))
      return Qt::Checked;
    else
      return Qt::Unchecked;
  } else
      return QVariant();
}


/**
 * Change the name or checkbox of a lesson.
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

  /** rename a lesson */
  if (role == Qt::EditRole) {
    QStringList list = m_doc->lessonDescriptions();
    list.replace(index.row(), value.toString());

    m_doc->setLessonDescriptions(list);
    emit dataChanged(index, index);
    return true;
  }

  /** checkboxes */
  if (role == Qt::CheckStateRole) {
    QList<int> intLessons;
    foreach(int lesson, m_doc->lessonsInQuery())
      intLessons.append(lesson);
      
    kDebug() << "hi - checkboxes: " << intLessons << " -- curind: " << index.row()+1 << endl;

    if (intLessons.contains(index.row()+1)){
      intLessons.removeAt(intLessons.indexOf(index.row()+1));
    } else {
        if (value.toInt() == Qt::Checked)
          intLessons.append(index.row()+1);
    }

    m_doc->setLessonsInQuery(intLessons);
    m_doc->setModified();
    return true;
  }
  return false;
}


/**
 * Eventually adding new lessons could go here. For now it's in kvtlessonview.
 * @param position
 * @param rows
 * @param parent
 * @return
 */
 /* creating lessons is in the view...
bool KVTLessonModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  Q_UNUSED(parent);
  beginInsertRows(QModelIndex(), position, position + rows - 1);

  for (int row = 0; row < rows; ++row) {
      //m_doc->lessonDescriptions().insert(position, "");
  }

  endInsertRows();
  return true;
}
*/

/**
 * 
 * @param position
 * @param rows
 * @param parent
 * @return
 */
 /* view? maybe should be reworked
bool KVTLessonModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  Q_UNUSED(parent);
  beginRemoveRows(QModelIndex(), position, position+rows-1);

  for (int row = 0; row < rows; ++row) {
      //stringList.removeAt(position);
  }

  endRemoveRows();
  return true;
}
*/

/**
 * propagete the change of current lesson
 * @param start
 * @param end
 */
/*
void KVTLessonModel::slotLessonSelectionChanged(const QModelIndex &start, const QModelIndex &end){
  Q_UNUSED(end);
  *//** we only have one lesson selectable at the time - start is enough */
/*  int index = start.row(); 
  if (index >= 0){
    m_doc->setCurrentLesson(index +1);
    //kDebug() << "Current lesson set to: " << index << " " << m_lessonList.at(index+1) << endl;
  }
}
*/

#include "kvtlessonmodel.moc"
