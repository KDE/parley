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

#include "vocabularyfilter.moc"

