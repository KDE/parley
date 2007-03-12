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
#include <KDebug>

#include "kvtsortfiltermodel.h"
#include "kvttablemodel.h"

KVTSortFilterModel::KVTSortFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
  m_sourceModel = 0;
  setSortCaseSensitivity(Qt::CaseInsensitive);
  m_lessonFilter = 0;
  m_searchFilter = 0;
  
  
  m_lessonFilter = new QRegExp("Lektion eins");
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

void KVTSortFilterModel::setLessonRegExp(QRegExp *filter)
{
  m_lessonFilter = filter;
}

void KVTSortFilterModel::setSearchRegExp(QRegExp *filter)
{
  m_searchFilter = filter;
}

/**
At the moment I use this to filter the lessons I want:
m_sortFilterModel->setFilterFixedString( "Lesson name" );      // one lesson
m_sortFilterModel->setFilterRegExp( "(Lesson 1)|(Lesson 2)" ); // regexp with or

What about special chars in lesson names!?! ahhh

I'd like to have someting easier here:
setVisibleLessonsFilter(QStringList)
setVisibleLessonsFilterRexExp(QString)
This should of course only work on the Lesson Column.

Also when doing a search I want to filter out all that does not contain the search string.
This on the other hand needs to work on the vocab columns.
Also space as seperator would be great.
So searching for "walk go" would find "to go" and "to walk" maybe. This is easy by doing regexp stuff and translating spaces into OR.
*/



/**
 * 
 * @param sourceRow 
 * @param sourceParent 
 * @return 
 
 
 * @todo Do this for all languages, not only two!!
 */
bool KVTSortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
  /*
  if(m_lessonFilter && m_searchFilter)
  {
  }*/
  if(m_lessonFilter)
  {
    QModelIndex lessons = sourceModel()->index(sourceRow, 0, sourceParent);
    if(m_lessonFilter->exactMatch(sourceModel()->data(lessons, Qt::DisplayRole).toString() ) )
      return true;
    return false;
     
  }/*
  if(m_searchFilter)
  {
     QModelIndex language1 = sourceModel()->index(sourceRow, 2, sourceParent);
     QModelIndex language2 = sourceModel()->index(sourceRow, 3, sourceParent);
     return ( sourceModel()->data(language1).toString().contains(m_searchFilter)
             || (sourceModel()->data(language2).toString().contains(m_searchFilter) ) );
  }*/
  /// no filter set? then show all.
  return true;
/* original qt example:
     QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
     QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
     QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);

     return (sourceModel()->data(index0).toString().contains(filterRegExp())
             || sourceModel()->data(index1).toString().contains(filterRegExp()))
            && dateInRange(sourceModel()->data(index2).toDate());
 */
 }

#include "kvtsortfiltermodel.moc"
