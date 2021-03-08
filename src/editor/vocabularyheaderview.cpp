/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "vocabularyheaderview.h"

using namespace Editor;

VocabularyHeaderView::VocabularyHeaderView(Qt::Orientation orientation, QWidget * parent)
    : QHeaderView(orientation, parent)
{
    setSectionsClickable(true);
    connect(this, &VocabularyHeaderView::sectionClicked, this, &VocabularyHeaderView::updateSorting);
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
    m_sortSection = -1;
    model()->sort(-1);
}
