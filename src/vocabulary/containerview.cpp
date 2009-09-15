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

#include "prefs.h"
#include "containermodel.h"

#include <KAction>
#include <KMessageBox>
#include <KInputDialog>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kicon.h>
#include <kconfig.h>
#include <keduvocdocument.h>
#include <keduvoclesson.h>
#include <keduvocwordtype.h>
#include <keduvocexpression.h>
#include <QTreeView>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>

using namespace Editor;

ContainerView::ContainerView(QWidget *parent) : QTreeView(parent)
{
    header()->setStretchLastSection(false);
    header()->setVisible(false);

    setAlternatingRowColors(true);

    // show the actions added by addAction() as right click menu.
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setSelectionMode(QAbstractItemView::SingleSelection);
    // setSelectionBehavior(QAbstractItemView::SelectRows);

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

void ContainerView::setTranslation(KEduVocExpression * entry, int translation)
{
    ///@todo put into the view subclasses!
    if (entry == 0) {
        return;
    }

    // who am I
    if(m_model->containerType() == KEduVocContainer::Lesson) {
        QModelIndex current = m_model->index(entry->lesson());
        selectionModel()->select(current, QItemSelectionModel::ClearAndSelect);
        scrollTo(current);
        return;
    }

    if(m_model->containerType() == KEduVocContainer::WordType) {    
    // attempt to find the container to select
        QModelIndex modelIndex;
        modelIndex = m_model->index(entry->translation(translation)->wordType());
        scrollTo(modelIndex);
        selectionModel()->select(modelIndex, QItemSelectionModel::ClearAndSelect);
        return;
    }
// leitner as well?

    selectionModel()->clearSelection();
    return;
}

void ContainerView::slotRename()
{
    edit(selectionModel()->currentIndex());
}

#include "containerview.moc"


