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

#include "lessonview.h"

#include "prefs.h"
#include "lessonmodel.h"

#include <QTreeView>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <KAction>
#include <KMessageBox>
#include <KInputDialog>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kicon.h>
#include <kconfig.h>
#include <keduvocdocument.h>


LessonView::LessonView(QWidget *parent) : QTreeView(parent)
{
    header()->setResizeMode(QHeaderView::ResizeToContents);
    header()->setVisible(false);
    setAlternatingRowColors(true);
    // drag and drop
//     setDragEnabled(true);
    //setMovement(QListView::Snap);
//     setAcceptDrops(true);
//     setDropIndicatorShown(true);
    // only allow internal moves - so far no interaction with the outside world.
//     setDragDropMode(QAbstractItemView::InternalMove);
    // show the actions added by addAction() as right click menu.
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setSelectionMode(QAbstractItemView::SingleSelection);
//     setSelectionBehavior(QAbstractItemView::SelectRows);
}

void LessonView::setModel(LessonModel *model)
{
    QTreeView::setModel(model);
    m_model = model;
}

void LessonView::slotCheckAllLessons()
{
//     m_model->setAllLessonsInPractice();
}

void LessonView::slotCheckNoLessons()
{
//     m_model->setNoLessonsInPractice();
}

void LessonView::slotCreateNewLesson()
{
/*    int newLessonIndex = m_model->addLesson();
    slotSelectLesson(newLessonIndex);

    QModelIndex modelIndex = m_model->index(newLessonIndex, 0, QModelIndex());
    edit(modelIndex);    // let the user type a new name for the lesson*/
}

void LessonView::slotRenameLesson()
{
//     QModelIndexList indexes = selectionModel()->selectedIndexes();
//     edit(indexes.at(0));
}

void LessonView::slotDeleteLesson()
{
//     int currentIndex = indexOfCurrentLesson();
//     // Delete right away, if the lesson is empty, otherwise ask
//     if (m_model->deleteLesson(currentIndex, KEduVocDocument::DeleteEmptyLesson))
//         return; // lesson was empty - done.
//     int exit = KMessageBox::warningYesNo(this, i18n("There are vocabularies left in this lesson. Do you want to delete them? You will loose your entries! You have been warned!")); ///@todo maybe a better message here...
//     if (exit == KMessageBox::Yes) {
//         m_model->deleteLesson(currentIndex, KEduVocDocument::DeleteEntriesAndLesson);
//     }
}

void LessonView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    Q_UNUSED(deselected);

    KEduVocContainer *lesson = 0;

    if(selected.count() > 0) {
        QModelIndex index = selected.indexes().value(0);
        lesson = static_cast<KEduVocContainer*>(index.internalPointer());
    }

    emit signalSelectedContainerChanged(lesson);
}


void LessonView::slotSplitLesson()
{
    /** @todo A nicer dialog would be great.
     * Maybe with radio buttons to ask, if the entries should be in random order or as they come. */
//     bool ok = false;
//     int numEntries = KInputDialog::getInteger(i18n("Entries per Lesson"), i18n("The lesson will be split into smaller lessons. The order will be randomized.\nHow many entries in each lesson do you want?"), Prefs::entriesPerLesson(), 1, 1000, 1, &ok, this);
//     if (!ok)
//         return;
//     Prefs::setEntriesPerLesson(numEntries);
//     m_model->splitLesson(indexOfCurrentLesson(), numEntries, KVTLessonModel::random);
}

void LessonView::dropEvent(QDropEvent * event)
{
    Q_UNUSED(event);
    kDebug() << "dropEvent()";
}


#include "lessonview.moc"


