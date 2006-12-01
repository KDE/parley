//
// C++ Implementation: kvttablemodel
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QSize>

#include <klocale.h>
#include <kdebug.h>

#include "kvttablemodel.h"
#include "prefs.h"


KVTTableModel::KVTTableModel(QObject *parent) : QAbstractTableModel(parent)
{
  m_doc = 0;
}

/*!
    \fn KVTTableModel::setDocument(KEduVocDocument * doc)
 */
void KVTTableModel::setDocument(KEduVocDocument * doc)
{
  m_doc = doc;
  //reset();
}


/*!
    \fn KVTTableModel::rowCount(const QModelIndex &parent) const
 */
int KVTTableModel::rowCount(const QModelIndex &parent) const
{
  return m_doc->numEntries();
}


/*!
    \fn KVTTableModel::columnCount(const QModelIndex &parent) const
 */
int KVTTableModel::columnCount(const QModelIndex &parent) const
{
  return 2 + m_doc->numIdentifiers();
}


/*!
    \fn KVTTableModel::data(const QModelIndex &index, int role) const
 */
QVariant KVTTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  else if (role == LessonsRole)
    return QVariant(m_doc->lessonDescriptions());
  else if (role == LessonRole)
    return QVariant(m_doc->entry(index.row())->lesson());
  else if (role == StateRole) {
    if (!m_doc->entry(index.row())->isActive())
      return 2;
    else if (m_doc->entry(index.row())->isInQuery())
      return 1;
    else
      return 0;
  }
  else if (role == Qt::FontRole)
    return QVariant(Prefs::tableFont());
  else if (role != Qt::DisplayRole)
    return QVariant();

  QVariant result;
  if (index.column() == 0)
    result = m_doc->lessonDescription(m_doc->entry(index.row())->lesson());
  else if (index.column() == 1)
  {
      if (!m_doc->entry(index.row())->isActive())
        return QVariant("X");
      if (m_doc->entry(index.row())->isInQuery())
        return QVariant("Q");
      else
        result = "";
  }
  else if (index.column() == 2)
    result = m_doc->entry(index.row())->original();
  else
    result = m_doc->entry(index.row())->translation(index.column() - 2);

  if (result.toString().isEmpty())
    result = "@empty@";
  //kDebug() << result.toString() << endl;
  return result;
}


/*!
    \fn KVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const
 */
QVariant KVTTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal)
  {
    if (role == Qt::DisplayRole) {
      if (section == 0)
        return i18n("Lesson");
      else if (section == 1)
        return "";
      else if (section == 2)
        return m_doc->originalIdentifier();
      else
        return m_doc->identifier(section - 2);
    }
    if (role == Qt::SizeHintRole)
      return QSize(m_doc->sizeHint(section), 25);
    return QVariant();
  }
  else
    return QAbstractTableModel::headerData(section, orientation, role);
}


/*!
    \fn KVTTableModel::flags(const QModelIndex &index) const
 */
Qt::ItemFlags KVTTableModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


/*!
    \fn KVTTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
 */
bool KVTTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    if (index.column() == 0)
      m_doc->entry(index.row())->setLesson(value.toInt());
    else if (index.column() == 1)
    {
      bool inq = false;
      bool act = true;
      if (value.toInt() == 0) {
        inq = false;
        act = true;
      }
      else if (value.toInt() == 1) {
        inq = true;
        act = true;
      }
      else if (value.toInt() == 2) {
        inq = false;
        act = false;
      }
      m_doc->entry(index.row())->setInQuery(inq);
      m_doc->entry(index.row())->setActive(act);
    }
    else if (index.column() == 2)
      m_doc->entry(index.row())->setOriginal(value.toString());
    else
      m_doc->entry(index.row())->setTranslation(index.column() - 2, value.toString());

    emit dataChanged(index, index);
    m_doc->setModified(true);
    return true;
  }
  return false;
}


/*!
    \fn KVTTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
 */
bool KVTTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::EditRole) {
      /// @todo handle?
      /*if (section == 0)
        /// @todo handle
      else if (section == 1)
        /// @todo handle
      else*/ if (section == 2)
        m_doc->setOriginalIdentifier(value.toString());
      else
        m_doc->setIdentifier(section - 2, value.toString());
    }
    if (role == Qt::SizeHintRole)
      m_doc->setSizeHint(section, qvariant_cast<QSize>(value).width());

    emit headerDataChanged(orientation, section, section);
    m_doc->setModified(true);
    return true;
  }
  return false;
}

#include "kvttablemodel.moc"
