/*
    SPDX-FileCopyrightText: 2008-2009 Frederik Gladhorn <gladhorn@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LESSONMODEL_H
#define LESSONMODEL_H

#include "containermodel.h"

/**
  * Model for the tree of lessons.
  */
class LessonModel : public ContainerModel
{
    Q_OBJECT

public:
    /** When splitting a lesson into smaller ones - how to sort the entries into lessons.*/
    enum SplitLessonOrder {
        Sorted,    /**< The order of the entries in the document */
        Random /**< Randomized */
    };

    explicit LessonModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    /**
     * Divide a lesson into smaller ones.
     * Tip: If you create a lesson that is >= the original one and use random order, you get your lesson reshuffled. Maybe that is sometimes useful. For now the lessons are appended at the end.
     * @param containerIndex lesson to split
     * @param entriesPerLesson number of entries in each new lesson
     * @param order one of SplitLessonOrder
        */
    void splitLesson(const QModelIndex& containerIndex, int entriesPerLesson, SplitLessonOrder order);

protected:
    KEduVocContainer * rootContainer() const Q_DECL_OVERRIDE;
};


#endif
