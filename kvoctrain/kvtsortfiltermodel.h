//
// C++ Interface: kvtsortfiltermodel
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTSORTFILTERMODEL_H
#define KVTSORTFILTERMODEL_H

#include <QSortFilterProxyModel>


/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/

class KVTTableModel;

class KVTSortFilterModel : public QSortFilterProxyModel
{
Q_OBJECT
public:

  KVTSortFilterModel(QObject *parent = 0);

  void setSourceModel(KVTTableModel * sourceModel);
  KVTTableModel * sourceModel () const;

  /** Returns true if a row is to be displayed. Respects the lesson setting and the search expression */
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
  void setLessonRegExp(QRegExp *filter);
  void setSearchRegExp(QRegExp *filter);

private:
  KVTTableModel * m_sourceModel;
  QRegExp * m_lessonFilter;
  QRegExp * m_searchFilter;
};

#endif
