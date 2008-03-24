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

#include "lessonmodel.h"

#include <KRandom>

/** @file
  * Implementation of LessonModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */

LessonModel::LessonModel(QObject * parent)
    :ContainerModel(KEduVocContainer::Lesson, parent)
{
}

void LessonModel::splitLesson(const QModelIndex& containerIndex, int entriesPerLesson, SplitLessonOrder order)
{
    if (!containerIndex.isValid()) {
        return;
    }

    if (!static_cast<KEduVocContainer*>(containerIndex.internalPointer())->containerType() == KEduVocContainer::Lesson) {
        return;
    }

    KEduVocLesson* parentLesson = static_cast<KEduVocLesson*>(containerIndex.internalPointer());

    int numNewLessons = parentLesson->entryCount() / entriesPerLesson;
    // modulo - fraction lesson if not 0 we need one more
    if (parentLesson->entryCount()%entriesPerLesson) {
        numNewLessons++;
    }

    while (parentLesson->entryCount() > 0) {
        beginInsertRows(containerIndex, parentLesson->entryCount(), parentLesson->entryCount() );
        KEduVocLesson* child = new KEduVocLesson(parentLesson->name()
                + QString(" %1").arg(parentLesson->childContainerCount() + 1), parentLesson);
        parentLesson->appendChildContainer(child);
        endInsertRows();

        while (parentLesson->entryCount() > 0 && child->entryCount() < entriesPerLesson) {
            // next entry to be assigned to one of the new lessons
            int nextEntry=0;
            if (order == Random) {
                nextEntry = KRandom::random() % parentLesson->entryCount();
                child->appendEntry(parentLesson->entry(nextEntry));
                parentLesson->removeEntry(parentLesson->entry(nextEntry));
            }
        }
    }
}


#include "lessonmodel.moc"


