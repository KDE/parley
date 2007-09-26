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
#include "prefs.h"

#include <keduvocexpression.h>
#include <keduvoclesson.h>

#include <KDebug>

KVTSortFilterModel::KVTSortFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    // do not use capitalization for searches
    setSortCaseSensitivity(Qt::CaseInsensitive);
    // sort locale aware: at least puts umlauts and accents in the right position. Not sure about languages that are more different. Also depends on the current locale.
    setSortLocaleAware ( true );

    m_sourceModel = 0;
    m_searchFilter = QRegExp();
    m_wordType = QRegExp();
    m_restoreNativeOrder = false;
    m_lessonSelection = Prefs::lessonEditingSelection();


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

void KVTSortFilterModel::setSearchRegExp(const QRegExp& filter)
{
    m_searchFilter = filter;
    invalidateFilter();
}

void KVTSortFilterModel::setWordType(const QRegExp& wordType)
{
    m_wordType = wordType;
    invalidateFilter();
}

/**
 * Check if the lesson is selected
 */
bool KVTSortFilterModel::checkLesson(int sourceRow) const
{
    switch (m_lessonSelection) {
    case Prefs::EnumLessonEditingSelection::CurrentLesson:
        return m_sourceModel->document()->entry(sourceRow)->lesson()
             == m_sourceModel->document()->currentLesson();
        break;
    case Prefs::EnumLessonEditingSelection::LessonsInQuery:
        return m_sourceModel->document()->lesson(
            m_sourceModel->document()->entry(sourceRow)->lesson()
            ).inQuery();
        break;
    case Prefs::EnumLessonEditingSelection::AllLessons:
        return true;
        break;
    }
    return false;
}


void KVTSortFilterModel::slotSearch(const QString& s)
{
    // searching for one letter takes up much time and is probably not needed. so start at lenth 2.
    if (s.length() <= 1) {
        m_wordType = QRegExp();
        setSearchRegExp(QRegExp());
        return;
    }
    // this can probably be done a little cleaner
    // now "hello world" becomes "(hello|world)" so it basically works,
    // but there are bound to be exceptions
    QStringList searchterms = s.split(' ', QString::SkipEmptyParts);

    QStringList types = searchterms.filter("type:", Qt::CaseInsensitive);
    // get rid of type searches
    foreach (QString type, types) {
        searchterms.removeAt(searchterms.indexOf(type));
    }

    types.replaceInStrings("type:", "", Qt::CaseInsensitive);
kDebug() << "Search types: " << '(' + types.join("|") + ')';
    m_wordType = QRegExp('(' + types.join("|") + ')', Qt::CaseInsensitive);

    QRegExp searchRegExp = QRegExp('(' + searchterms.join("|") + ')', Qt::CaseInsensitive);

    setSearchRegExp(searchRegExp);
}


/**
 * Check the search terms
 */
bool KVTSortFilterModel::checkSearch(int sourceRow) const
{
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


bool KVTSortFilterModel::checkWordType(int sourceRow) const
{
    if (m_wordType.isEmpty()) {
        return true;
    }

    for (int i=0; i < m_sourceModel->document()->identifierCount(); i++) {
        if ( m_sourceModel->document()->
            entry(sourceRow)->translation(i).
            type().contains(m_wordType) ) {
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
    Q_UNUSED(sourceParent);

    if (!checkLesson(sourceRow)) {
        return false;
    }

    if (!m_searchFilter.isEmpty()) {
        if (!checkSearch(sourceRow)) {
            return false;
        }
    }

    if (!m_wordType.isEmpty()) {
        if (!checkWordType(sourceRow)) {
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

void KVTSortFilterModel::setLessonSelection(int lessonSelection)
{
    m_lessonSelection = lessonSelection;
    invalidate();
}

void KVTSortFilterModel::slotCurrentLessonChanged(int )
{
    if ( m_lessonSelection == Prefs::EnumLessonEditingSelection::CurrentLesson ) {
        invalidate();
    }
}

void KVTSortFilterModel::slotLessonsInQueryChanged()
{
    if ( m_lessonSelection == Prefs::EnumLessonEditingSelection::LessonsInQuery ) {
        invalidate();
    }
}


#include "kvtsortfiltermodel.moc"
