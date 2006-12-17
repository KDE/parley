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
#include <kglobal.h>
#include <kiconloader.h>

#include "kvttablemodel.h"
#include "prefs.h"


KVTTableModel::KVTTableModel(QObject *parent) : QAbstractTableModel(parent)
{
  m_doc = 0;
  //m_pixInQuery = SmallIcon("ok"); //QPixmap(KGlobal::iconLoader()->loadIcon("ok", K3Icon::Small));
  //m_pixInactive = SmallIcon("no"); //QPixmap(KGlobal::iconLoader()->loadIcon("no", K3Icon::Small));
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

  switch (role)
  {
    case KVTTableModel::LessonsRole: {
      return QVariant(m_doc->lessonDescriptions());
      break;
    }

    case KVTTableModel::LessonRole: {
      return QVariant(m_doc->entry(index.row())->lesson());
      break;
    }

    case KVTTableModel::StateRole: {
      if (!m_doc->entry(index.row())->isActive())
        return 2;
      else if (m_doc->entry(index.row())->isInQuery())
        return 1;
      else
        return 0;
      break;
    }

    case KVTTableModel::GradeRole: {
      if (index.column() > KV_EXTRA_COLS)
      {
        if (m_doc->entry(index.row())->queryCount(index.column() - KV_EXTRA_COLS, false) != 0)
          return QVariant(m_doc->entry(index.row())->grade(index.column() - KV_EXTRA_COLS, false));
        else
          return QVariant(KV_NORM_GRADE);
      }
      else if (index.column() == 2)
      {
        QList<QVariant> result;
        for (int i = 1; i <= m_doc->numIdentifiers(); ++i)
        {
          if (m_doc->entry(index.row())->queryCount(i, true) != 0)
            result.append(QVariant(m_doc->entry(index.row())->grade(i /*+ KV_EXTRA_COLS*/, true)));
          else
            result.append(QVariant(KV_NORM_GRADE));
        }
        return QVariant(result);
      }
      break;
    }

    case Qt::FontRole: {
      return QVariant(Prefs::tableFont());
      break;
    }

    case Qt::DisplayRole: {
      QVariant result;
      if (index.column() == 0)
        result = m_doc->lessonDescription(m_doc->entry(index.row())->lesson());
      else if (index.column() == 1)
      {
        kDebug() << m_doc->entry(index.row())->isInQuery() << endl;
        if (m_doc->entry(index.row())->isActive())
        {
          if (m_doc->entry(index.row())->isInQuery())
            return "@inquery@";
          else
            return QVariant();
        }
        else
          return "@inactive@";
      }
      else if (index.column() == 2)
        result = m_doc->entry(index.row())->original();
      else
        result = m_doc->entry(index.row())->translation(index.column() - 2);

      if (result.toString().isEmpty())
        result = "@empty@";
      //kDebug() << result.toString() << endl;
      return result;
      break;
    }

     /*case Qt::DecorationRole: {
      if (index.column() == 1)
      {
       if (!m_doc->entry(index.row())->isActive())
          return m_pixInactive;
        if (m_doc->entry(index.row())->isInQuery())
          return m_pixInQuery;
      }
      return QVariant();
      break;
    }*/

    default:
      break;
  }
  return QVariant();
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
    {
      switch (section)
      {
        case 0: return QSize(m_doc->sizeHint(-1), 25);
        case 1: return QSize(25, 25);
        default: return QSize(m_doc->sizeHint(section - KV_EXTRA_COLS), 25);
      }
    }
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
    {
      switch (section)
      {
        case 0: m_doc->setSizeHint(-1, qvariant_cast<QSize>(value).width());;
        case 1: //
        default: m_doc->setSizeHint(section - KV_EXTRA_COLS, qvariant_cast<QSize>(value).width());
      }
    }

    emit headerDataChanged(orientation, section, section);
    m_doc->setModified(true);
    return true;
  }
  return false;
}

#include "kvttablemodel.moc"
