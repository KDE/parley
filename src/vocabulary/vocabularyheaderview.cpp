/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "vocabularyheaderview.h"

using namespace Editor;

VocabularyHeaderView::VocabularyHeaderView(Qt::Orientation orientation, QWidget * parent)
 : QHeaderView(orientation, parent) 
{
    setClickable(true);
    connect(this, SIGNAL(sectionClicked(int)), this, SLOT(updateSorting(int)));
    m_sortSection = -1;
}

void VocabularyHeaderView::updateSorting(int index)
{
    if (m_sortSection != index) {
        setSortIndicatorShown(true);
        setSortIndicator(index, Qt::AscendingOrder);
        m_sortSection = index;
        return;
    }

    if (sortIndicatorOrder() == Qt::DescendingOrder) {
        setSortIndicatorShown(true);
        setSortIndicator(index, Qt::DescendingOrder);
        return;
    }

    if (!isSortIndicatorShown()) {
        setSortIndicatorShown(true);
        setSortIndicator(index, Qt::AscendingOrder);
        return;
    }

    setSortIndicatorShown(false);
    model()->sort(-1);
}

#include "vocabularyheaderview.moc"
