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

#ifndef LESSONSTATISTICSVIEW_H
#define LESSONSTATISTICSVIEW_H

#include "containerview.h"

/**
 * View for the lesson list.
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class LessonStatisticsView : public ContainerView
{
    Q_OBJECT

public:
    explicit LessonStatisticsView(QWidget *parent);
    ~LessonStatisticsView();
    void setModel(ContainerModel *model) Q_DECL_OVERRIDE;
    void saveExpandedStatus() const;
    void restoreExpandedStatus();

public Q_SLOTS:
    void adjustColumnWidths();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void removeGrades();
    void removeGradesChildren();
    void sectionResized(int index, int /*oldSize*/, int /*newSize*/);

private:
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE {
        Q_UNUSED(model)
    }
    void getCollapsedItems(QStringList &collapsedItems, const QModelIndex &item,
                           QString name) const;
    void setCollapsedItems(const QStringList &collapsedItems, const QModelIndex &item,
                           QString name);
};

#endif
