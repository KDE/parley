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

#include "containerview.h"

#include <QHeaderView>

#include "containermodel.h"

using namespace Editor;

ContainerView::ContainerView(QWidget *parent) : QTreeView(parent)
{
    header()->setStretchLastSection(false);
    header()->setVisible(false);

    setAlternatingRowColors(true);

    // show the actions added by addAction() as right click menu.
    setContextMenuPolicy(Qt::ActionsContextMenu);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
}

void ContainerView::setModel(ContainerModel *model)
{
    QTreeView::setModel(model);
    m_model = model;

    header()->setResizeMode(0, QHeaderView::Stretch);
    header()->setResizeMode(1, QHeaderView::ResizeToContents);
}

void ContainerView::slotRename()
{
    edit(selectionModel()->currentIndex());
}

#include "containerview.moc"


