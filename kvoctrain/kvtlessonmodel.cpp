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
  m_lessonList.clear();
  foreach(QString lesson, m_doc->lessonDescriptions())
    m_lessonList.append(lesson);
  m_lessonList.prepend(i18n("All Lessons"));
  m_lessonList.append(i18n("<no lesson>"));
  reset();
  /** Create the additional entries: */
}


/**
 * The number of Lessons +2 because we have those "All lessons" and "Not in lesson" entries.
 * @param parent will always be QModelIndex() as long as we only have a list here.
 * @return number lessons +2.
 */
int KVTLessonModel::rowCount(const QModelIndex &parent) const
{
  return m_lessonList.count();
}


/**
 * The lesson name of an entry.
 * @param index 
 * @param role 
 * @return 
 */
QVariant KVTLessonModel::data(const QModelIndex &index, int role) const
{

  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_lessonList.count())
    return QVariant();

  if (role == Qt::DisplayRole)
    return m_lessonList.at(index.row());
  else
    return QVariant();
}


// at this moment not used
QVariant KVTLessonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal)
    return QString("Column %1").arg(section);
  else
    return QString("Row %1").arg(section);
}


Qt::ItemFlags KVTLessonModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


/**
 * Change the name of a lesson. This is called automatically when the user changes an entry in the list.
 * @param index which lesson is renamed
 * @param value new name
 * @param role 
 * @return bool: worked
 */
bool KVTLessonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  /** The first is All, the last none */
  if(index.row() == 0 || index.row() == m_lessonList.count() -1)
    return false; // cannot change all/none

  if (index.isValid() && role == Qt::EditRole) {
    QStringList list = m_doc->lessonDescriptions();
    list.replace(index.row() -1 , value.toString()); // Very ugly! -1 because we added All as 0. But using the m_doc entries is more sensible.
    //kDebug() << list << endl;
    m_doc->setLessonDescriptions(list);

    // not only in m_doc, but here as well:
    m_lessonList.replace(index.row() , value.toString());

    emit dataChanged(index, index);
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
  beginInsertRows(QModelIndex(), position, position+rows-1);

  for (int row = 0; row < rows; ++row) {
      //TODO how do I set up a new lesson?
      //m_doc->lessonDescriptions().insert(position, "");
  }

  endInsertRows();
  return true;
}


/**
 * NOT IMPLEMENTED - Delete a lesson.
 * @param position 
 * @param rows 
 * @param parent 
 * @return 
 */
bool KVTLessonModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  beginRemoveRows(QModelIndex(), position, position+rows-1);

  for (int row = 0; row < rows; ++row) {
      //stringList.removeAt(position);
      //TODO delete lesson? Only if empty?
  }

  endRemoveRows();
  return true;
}

#include "kvtlessonmodel.moc"
