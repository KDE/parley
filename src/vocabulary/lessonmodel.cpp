/***************************************************************************

    Copyright 2008-2009 Frederik Gladhorn <gladhorn@kde.org>

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

#include "parleydocument.h"
#include <KRandom>
#include <KLocalizedString>
#include <QFont>

/** @file
  * Implementation of LessonModel.
  * Functions to create the model from the lessons of the vocabulary document.
  */

using namespace Editor;

LessonModel::LessonModel(QObject * parent)
    :ContainerModel(KEduVocContainer::Lesson, parent)
{
}

KEduVocContainer * LessonModel::rootContainer() const
{
    if (!m_doc) {
        return 0;
    }
    return m_doc->lesson();
}

Qt::ItemFlags LessonModel::flags(const QModelIndex &index) const
{
    if (index.isValid() && index.parent() == QModelIndex()) {
        return (Qt::ItemIsEnabled 
              | Qt::ItemIsEditable
              | Qt::ItemIsSelectable
              | Qt::ItemIsUserCheckable );
    }

    return  ContainerModel::flags(index);
}

QVariant LessonModel::data(const QModelIndex & index, int role) const
{
    if (index.isValid() && !index.parent().isValid()) {
        if (index.column() == 0) {
            switch (role) {
                case Qt::DisplayRole:
                    return i18nc("display of the name of the vocabulary collection", "Collection: %1", ContainerModel::data(index, role).toString());
                case Qt::FontRole:
                    QFont f;
                    f.setBold(true);
                    return f;                
            }
        }
    }
    return ContainerModel::data(index, role);
}


bool LessonModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && !index.parent().isValid()) {
        if (index.column() == 0) {
            ParleyDocument::instance()->setTitle(value.toString());
        }
    }
    
    return ContainerModel::setData(index, value, role);
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
            }
        }
    }
}


#include "lessonmodel.moc"


