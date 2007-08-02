/***************************************************************************

                             kvtlessonview

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@gmx.de>

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

#include "kvtlessonview.h"

#include <QTreeView>
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

#include "prefs.h"
#include "kvtlessonmodel.h"

KVTLessonView::KVTLessonView(QWidget *parent) : QTreeView(parent)
{
    // drag and drop
    setDragEnabled(true);
    //setMovement(QListView::Snap);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    // only allow internal moves - so far no interaction with the outside world.
    setDragDropMode(QAbstractItemView::InternalMove);
    // show the actions added by addAction() as right click menu.
    setContextMenuPolicy(Qt::ActionsContextMenu);
}

void KVTLessonView::setModel(KVTLessonModel *model)
{
    QTreeView::setModel(model);
    m_model = model;
}

void KVTLessonView::slotModelReset()
{
    initializeSelection();
}

void KVTLessonView::initializeSelection()
{
    slotSelectLesson(m_model->currentLesson());
}

void KVTLessonView::slotCheckAllLessons()
{
    m_model->setAllLessonsInQuery();
}

void KVTLessonView::slotCheckNoLessons()
{
    m_model->setNoLessonsInQuery();
}

void KVTLessonView::slotCreateNewLesson()
{
    kDebug() << "Create new lesson!";
    
    int newLessonIndex = m_model->addLesson();
    slotSelectLesson(newLessonIndex);

    QModelIndex modelIndex = m_model->index(newLessonIndex -1, 0, QModelIndex());
    edit(modelIndex);    // let the user type a new name for the lesson
}

void KVTLessonView::slotRenameLesson()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    edit(indexes.at(0));
}

void KVTLessonView::slotDeleteLesson()
{
    int currentIndex = indexOfCurrentLesson();
    // Delete right away, if the lesson is empty, otherwise ask
    if (m_model->deleteLesson(currentIndex, KEduVocDocument::DeleteEmptyLesson))
        return; // lesson was empty - done.
    int exit = KMessageBox::warningYesNo(this, i18n("There are vocabularies left in this lesson. Do you want to delete them? You will loose your entries! You have been warned!")); ///@todo maybe a better message here...
    if (exit == KMessageBox::Yes) {
        m_model->deleteLesson(currentIndex, KEduVocDocument::DeleteEntriesAndLesson);
    }
}

void KVTLessonView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    Q_UNUSED(deselected);
    slotSelectLesson(selected.indexes().at(0).row() +1);
}


void KVTLessonView::slotSelectLesson(int lesson)
{
    // if current lesson is not set in the document default to the first one. Because we do -1 this is 1.
    if (lesson <= 0 || lesson > m_model->rowCount())
        lesson = 1;

    // -1 because of counting from 1 of m_doc
    QModelIndex indexOfCurrent = m_model->index(lesson -1, 0, QModelIndex());
    setCurrentIndex(indexOfCurrent);
    emit signalCurrentLessonChanged(lesson);
}

void KVTLessonView::slotSplitLesson()
{
    /** @todo A nicer dialog would be great.
     * Maybe with radio buttons to ask, if the entries should be in random order or as they come. */
    bool ok = false;
    int numEntries = KInputDialog::getInteger(i18n("Entries per Lesson"), i18n("The lesson will be split into smaller lessons. The order will be randomized.\nHow many entries in each lesson do you want?"), Prefs::entriesPerLesson(), 1, 1000, 1, &ok, this);
    if (!ok)
        return;
    Prefs::setEntriesPerLesson(numEntries);
    m_model->splitLesson(indexOfCurrentLesson() +1, numEntries, KVTLessonModel::random);
}

void KVTLessonView::dropEvent(QDropEvent * event)
{
    Q_UNUSED(event);
    kDebug() << "dropEvent()";
}

int KVTLessonView::indexOfCurrentLesson()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    if (indexes.empty()) {
        kDebug() << "WARNING - NO SELECTION FOR ACTIVE LESSON! THIS SHOULD NOT HAPPEN!";
        return 0;
    }
    return indexes.at(0).row();
}

#include "kvtlessonview.moc"
