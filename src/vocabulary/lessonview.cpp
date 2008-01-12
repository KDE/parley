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

#include "lessonview.h"

#include "containermodel.h"
#include "keduvoclesson.h"
#include "prefs.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KInputDialog>

LessonView::LessonView(QWidget * parent) :ContainerView(parent)
{
//     setContextMenuPolicy(Qt::CustomContextMenu);
}


void LessonView::slotCreateNewLesson()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (!selectedIndex.isValid()) {
        selectedIndex = m_model->index(0, 0, QModelIndex());
    }

    QModelIndex modelIndex = m_model->appendLesson(selectedIndex);

    scrollTo(modelIndex);
    selectionModel()->setCurrentIndex(modelIndex, QItemSelectionModel::ClearAndSelect);
    edit(modelIndex);    // let the user type a new name for the lesson
}

void LessonView::slotRenameLesson()
{
    edit(selectionModel()->currentIndex());
}

void LessonView::slotDeleteLesson()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (selectedIndex.parent() == QModelIndex()) {
        KMessageBox::information(this, i18n("The root lesson cannot be deleted."));
        return;
    }

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
    m_model->splitLesson(selectionModel()->currentIndex(), numEntries, ContainerModel::Random);
    setExpanded(selectionModel()->currentIndex(), true);
}



#include "lessonview.moc"


