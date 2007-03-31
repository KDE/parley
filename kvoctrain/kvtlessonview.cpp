//
// C++ Implementation: kvtlessonview
//
// Description:
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

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
#include "kvtlessonview.h"

KVTLessonView::KVTLessonView(QWidget *parent) : QTreeView(parent)
{
    // drag and drop
    setDragEnabled(true);
    //setMovement(QListView::Snap);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    // only allow internal moves - so far no interaction with the outside world.
    setDragDropMode(QAbstractItemView::InternalMove);

    // right click menu
    m_lessonMenu = new QMenu(this);
    KAction *actionNewLesson = new KAction(i18n("New lesson"), this);
    m_lessonMenu->addAction(actionNewLesson);
    actionNewLesson->setIcon(KIcon("edit-add"));
    connect(actionNewLesson, SIGNAL(triggered()), this, SLOT(slotCreateNewLesson()));

    KAction *actionRenameLesson = new KAction(i18n("Rename lesson"), this);
    m_lessonMenu->addAction(actionRenameLesson);
    actionRenameLesson->setIcon(KIcon("edit"));
    connect(actionRenameLesson, SIGNAL(triggered()), this, SLOT(slotRenameLesson()));

    KAction *actionDeleteLesson = new KAction(i18n("Delete lesson"), this);
    m_lessonMenu->addAction(actionDeleteLesson);
    actionDeleteLesson->setIcon(KIcon("edit-delete"));
    connect(actionDeleteLesson, SIGNAL(triggered()), this, SLOT(slotDeleteLesson()));

    m_lessonMenu->addSeparator();

    KAction *actionCheckAllLessons = new KAction(i18n("Check all lessons"), this);
    m_lessonMenu->addAction(actionCheckAllLessons);
    actionCheckAllLessons->setIcon(KIcon("edit-add"));  /// @todo better icon
    connect(actionCheckAllLessons, SIGNAL(triggered()), this, SLOT(slotCheckAllLessons()));

    KAction *actionCheckNoLessons = new KAction(i18n("Deselect all lessons"), this);
    m_lessonMenu->addAction(actionCheckNoLessons);
    actionCheckNoLessons->setIcon(KIcon("edit-delete"));  /// @todo better icon
    connect(actionCheckNoLessons, SIGNAL(triggered()), this, SLOT(slotCheckNoLessons()));

    m_lessonMenu->addSeparator();

    KAction *actionSplitLesson = new KAction(i18n("Split lesson into smaller lessons"), this);
    m_lessonMenu->addAction(actionSplitLesson);
    actionSplitLesson->setIcon(KIcon("edit-copy"));  /// @todo better icon
    connect(actionSplitLesson, SIGNAL(triggered()), this, SLOT(slotSplitLesson()));
}


QMenu *KVTLessonView::lessonMenu()
{
    return m_lessonMenu;
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


void KVTLessonView::contextMenuEvent(QContextMenuEvent * event)
{
    m_lessonMenu->exec(event->globalPos());
}


void KVTLessonView::dropEvent(QDropEvent * event)
{
    kDebug() << "dropEvent()" << endl;
}

int KVTLessonView::indexOfCurrentLesson()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    if (indexes.empty()) {
        kDebug() << "WARNING - NO SELECTION FOR ACTIVE LESSON! THIS SHOULD NOT HAPPEN!" << endl;
        return 0;
    }
    return indexes.at(0).row();
}

#include "kvtlessonview.moc"
