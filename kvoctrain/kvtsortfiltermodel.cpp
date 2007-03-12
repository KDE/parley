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
  m_searchFilter = QRegExp();
  m_lessonFilter = QRegExp();

  // only show things starting with a:
  //m_searchFilter = QRegExp("a.*");
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

void KVTSortFilterModel::setLessonRegExp(QRegExp filter)
{
  m_lessonFilter = filter;
}

void KVTSortFilterModel::setSearchRegExp(QRegExp filter)
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
 * Check if the lesson is selected
 */
bool KVTSortFilterModel::checkLesson(int sourceRow, const QModelIndex &sourceParent) const
{
  QModelIndex lessons = sourceModel()->index(sourceRow, 0, sourceParent);
  if(m_lessonFilter.exactMatch(sourceModel()->data(lessons, Qt::DisplayRole).toString() ) )
    return true;
  return false;
}

/**
 * Check the search terms
 */
bool KVTSortFilterModel::checkSearch(int sourceRow, const QModelIndex &sourceParent) const
{
  /// Check if the vocabs contain the expression:
  QModelIndex lang;
  for (int i=0 ; i<m_sourceModel->document()->identifierCount(); i++)
  {
    QModelIndex lang = sourceModel()->index(sourceRow, i+2, sourceParent);
    if( m_searchFilter.exactMatch(sourceModel()->data(lang, Qt::DisplayRole).toString()) )
      return true;
  }
  return false;
}


/**
 * Determines if the row is displayed
 * @param sourceRow 
 * @param sourceParent 
 * @return 
 */
bool KVTSortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
  if(!m_lessonFilter.isEmpty())
    if (!checkLesson(sourceRow, sourceParent))
      return false;

  if(!m_searchFilter.isEmpty())
    if (!checkSearch(sourceRow, sourceParent))
      return false;

  return true;
}

void KVTSortFilterModel::delLessonFilter()
{
  m_lessonFilter = QRegExp();
}
void KVTSortFilterModel::delSearchFilter()
{
  m_searchFilter = QRegExp();
}

#include "kvtsortfiltermodel.moc"
