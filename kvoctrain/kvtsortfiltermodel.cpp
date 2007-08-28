/***************************************************************************

                             kvtsortfiltermodel

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                    (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kvtsortfiltermodel.h"
#include "kvttablemodel.h"

#include <keduvocexpression.h>

KVTSortFilterModel::KVTSortFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_sourceModel = 0;
    setSortCaseSensitivity(Qt::CaseInsensitive);
    m_searchFilter = QRegExp();
    m_lessonFilter = QRegExp();
    m_restoreNativeOrder = false;
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

void KVTSortFilterModel::setLessonRegExp(const QRegExp& filter)
{
    m_lessonFilter = filter;
    invalidateFilter();
}

void KVTSortFilterModel::setSearchRegExp(const QRegExp& filter)
{
    m_searchFilter = filter;
    invalidateFilter();
}

/*
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
Also space as separator would be great.
So searching for "walk go" would find "to go" and "to walk" maybe. This is easy by doing regexp stuff and translating spaces into OR.
*/
/**
 * Check if the lesson is selected
 */
bool KVTSortFilterModel::checkLesson(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex lesson = sourceModel()->index(sourceRow, 0, sourceParent);
    if (m_lessonFilter.exactMatch( sourceModel()->data(lesson, Qt::DisplayRole).toString() ) )
        return true;
    return false;
}


void KVTSortFilterModel::slotSearch(const QString& s)
{
    // searching for one letter takes up much time and is probably not needed. so start at lenth 2.
    if (s.length() <= 1) {
        setSearchRegExp(QRegExp());
        return;
    }
    // this can probably be done a little cleaner
    // now "hello world" becomes "(hello|world)" so it basically works,
    // but there are bound to be exceptions
    QString searchterm = s.simplified();
    searchterm.replace(QString(" "), QString("|"));
    QRegExp searchRegExp = QRegExp('(' + searchterm + ')', Qt::CaseInsensitive);

    setSearchRegExp(searchRegExp);
}


/**
 * Check the search terms
 */
bool KVTSortFilterModel::checkSearch(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    /// Check if the entries contain the expression:
    for (int i=0; i < m_sourceModel->document()->identifierCount(); i++) {
        if ( m_sourceModel->document()->
            entry(sourceRow)->translation(i).
            text().contains(m_searchFilter) ) {
            return true;
        }
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
    if (!m_lessonFilter.isEmpty()) {
        if (!checkLesson(sourceRow, sourceParent)) {
            return false;
        }
    }

    if (!m_searchFilter.isEmpty()) {
        if (!checkSearch(sourceRow, sourceParent)) {
            return false;
        }
    }

    return true;
}

bool KVTSortFilterModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{
    if (m_restoreNativeOrder) {
        return sourceModel()->index(right.row(),  right.column(),  QModelIndex()).row() <
               sourceModel()->index(left.row(), left.column(), QModelIndex()).row();
    } else {
        return QSortFilterProxyModel::lessThan(left, right);
    }
}

void KVTSortFilterModel::restoreNativeOrder()
{
    m_restoreNativeOrder = true;
    sort(-1, Qt::AscendingOrder);
    invalidate();
    m_restoreNativeOrder = false;
}


#include "kvtsortfiltermodel.moc"
