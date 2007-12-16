/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
 : QSortFilterProxyModel(parent)
{
    m_model = 0;
}


VocabularyFilter::~VocabularyFilter()
{
}

QModelIndex VocabularyFilter::appendEntry()
{
    if (!m_model) {
        return QModelIndex();
    }
    return mapFromSource(m_model->appendEntry());
}

void VocabularyFilter::setSourceModel(VocabularyModel * model)
{
    QSortFilterProxyModel::setSourceModel(model);
    m_model=model;
}

bool VocabularyFilter::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
//     QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
//     QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
// 
//     return (sourceModel()->data(index0).toString().contains(filterRegExp())
//             || sourceModel()->data(index1).toString().contains(filterRegExp()))
//             && dateInRange(sourceModel()->data(index2).toDate());
    return (sourceModel()->data(index0).toString().contains(filterRegExp()));

//     return false;
}

void VocabularyFilter::setSearchString(const QString & expression)
{
    setFilterWildcard ( expression );
}


#include "vocabularyfilter.moc"

