/***************************************************************************

    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "vocabularyfilter.h"

#include "vocabularymodel.h"


VocabularyFilter::VocabularyFilter(QObject *parent)
    : QSortFilterProxyModel(parent), m_model(0)
{
    // do not use capitalization for searches
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    // sort locale aware: at least puts umlauts and accents in the right position.
    // Not sure about languages that are more different.
    // Also depends on the current locale.
    setSortLocaleAware(true);

    // eventually accept only one language if so desired...
    setFilterKeyColumn(-1);
}

QModelIndex VocabularyFilter::appendEntry(KEduVocExpression *expression)
{
    if (!m_model) {
        return QModelIndex();
    }
    return mapFromSource(m_model->appendEntry(expression));
}

void VocabularyFilter::setSourceModel(VocabularyModel * model)
{
    QSortFilterProxyModel::setSourceModel(model);
    m_model = model;
}

VocabularyModel* VocabularyFilter::sourceVocabularyModel()
{
    return m_model;
}

void VocabularyFilter::setSearchString(const QString & expression)
{
    m_filterString = expression;
    invalidateFilter();
}

bool VocabularyFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_filterString.isEmpty()) {
        return true;
    }

    int columns = m_model->columnCount(QModelIndex());
    for (int i = 0; i < columns; i += VocabularyModel::EntryColumnsMAX) {
        QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
        if (sourceModel()->data(index).toString().contains(m_filterString, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

KEduVocLesson * VocabularyFilter::lesson()
{
    if (m_model) {
        return m_model->lesson();
    }
    return 0;
}
