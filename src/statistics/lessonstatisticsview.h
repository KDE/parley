/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    ~LessonStatisticsView() override;
    void setModel(ContainerModel *model) override;
    void saveExpandedStatus() const;
    void restoreExpandedStatus();

public Q_SLOTS:
    void adjustColumnWidths();

protected:
    void resizeEvent(QResizeEvent *event) override;

private Q_SLOTS:
    void removeGrades();
    void removeGradesChildren();
    void sectionResized(int index, int /*oldSize*/, int /*newSize*/);

private:
    void setModel(QAbstractItemModel *model) override
    {
        Q_UNUSED(model)
    }
    void getCollapsedItems(QStringList &collapsedItems, const QModelIndex &item, QString name) const;
    void setCollapsedItems(const QStringList &collapsedItems, const QModelIndex &item, QString name);
};

#endif
