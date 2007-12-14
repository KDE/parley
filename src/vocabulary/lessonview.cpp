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
#include <keduvoclesson.h>
#include <keduvocwordtype.h>
#include <keduvocexpression.h>

LessonView::LessonView(QWidget *parent) : QTreeView(parent)
{
    header()->setResizeMode(QHeaderView::ResizeToContents);
    header()->setVisible(false);
    setAlternatingRowColors(true);

    // show the actions added by addAction() as right click menu.
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setSelectionMode(QAbstractItemView::SingleSelection);
    // setSelectionBehavior(QAbstractItemView::SelectRows);

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
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    QModelIndex modelIndex = m_model->appendLesson(selectedIndex);

    scrollTo(modelIndex);
    selectionModel()->select(modelIndex, QItemSelectionModel::ClearAndSelect);
    edit(modelIndex);    // let the user type a new name for the lesson
}

void LessonView::slotRenameLesson()
{
    edit(selectionModel()->currentIndex());
}

void LessonView::slotDeleteLesson()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    KEduVocLesson* lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());

    int count = lesson->entriesRecursive().count();

    if ( count == 0 ||
         KMessageBox::warningYesNo(this, i18np("There is %1 word left in this lesson. Do you want to delete them?", "There are %1 words left in this lesson. Do you want to delete them?", count)) == KMessageBox::Yes) {
        m_model->deleteLesson(selectedIndex);
    }
}


void LessonView::slotSplitLesson()
{
    if (!selectionModel()->currentIndex().isValid()) {
        return;
    }

    /** @todo A nicer dialog would be great.
     * Maybe with radio buttons to ask, if the entries should be in random order or as they come. */
    bool ok = false;
    int numEntries = KInputDialog::getInteger(i18n("Entries per Lesson"), i18n("The lesson will be split into smaller lessons. How many entries in each lesson do you want?"), Prefs::entriesPerLesson(), 1, 1000, 1, &ok, this);
    if (!ok) {
        return;
    }
    Prefs::setEntriesPerLesson(numEntries);
    m_model->splitLesson(selectionModel()->currentIndex(), numEntries, LessonModel::Random);
    setExpanded(selectionModel()->currentIndex(), true);
}

void LessonView::setTranslation(KEduVocExpression * entry, int translation)
{
    if (entry == 0) {
        return;
    }

    QModelIndex selectedIndex = selectionModel()->currentIndex();
    KEduVocContainer* container;

    // if it's the right container selected anyway, we don't worry
    if (selectedIndex.isValid()) {
        container = static_cast<KEduVocContainer*>(selectedIndex.internalPointer());
        if (container->entries().contains(entry)) {
            return;
        }
    }

    // attempt to find the container to select
    QModelIndex modelIndex;

    // who am I
    if(m_model->containerType() == KEduVocContainer::LessonContainer) {
            modelIndex = m_model->index(entry->lessons().value(0));
    }

    // if it's not one of the others, it has to be some sort of word type container.
    if(m_model->containerType() != KEduVocContainer::Container
        && m_model->containerType() != KEduVocContainer::LessonContainer
        && m_model->containerType() != KEduVocContainer::LeitnerContainer ) {

        modelIndex = m_model->index(entry->translation(translation)->wordType());
    }
// leitner as well?

    if(!modelIndex.isValid()) {
        scrollTo(QModelIndex());
        selectionModel()->clearSelection();
        return;
    }

    scrollTo(modelIndex);
    selectionModel()->select(modelIndex, QItemSelectionModel::ClearAndSelect);

}

void LessonView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    kDebug()<< "current changed";

    Q_UNUSED(previous);

    KEduVocContainer *lesson = 0;
    lesson = static_cast<KEduVocContainer*>(current.internalPointer());
    emit signalSelectedContainerChanged(lesson);

    QTreeView::currentChanged(current, previous);
}


#include "lessonview.moc"


