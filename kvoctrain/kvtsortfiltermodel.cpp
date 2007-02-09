//
// C++ Implementation: kvtsortfiltermodel
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "kvtsortfiltermodel.h"
#include "kvttablemodel.h"

KVTSortFilterModel::KVTSortFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
  m_sourceModel = 0;
  setSortCaseSensitivity(Qt::CaseInsensitive);
}

void KVTSortFilterModel::setSourceModel(KVTTableModel * sourceModel)
{
  m_sourceModel = sourceModel;
  QSortFilterProxyModel::setSourceModel(sourceModel);
}

KVTTableModel * KVTSortFilterModel::sourceModel() const
{
  return m_sourceModel;
}

#include "kvtsortfiltermodel.moc"
