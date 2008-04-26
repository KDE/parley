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

#include "lessonstatistics.h"

#include "statisticsmodel.h"
#include "keduvoclesson.h"
#include "prefs.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KInputDialog>
#include <KAction>
#include <KActionCollection>

#include <QHeaderView>

#include <QPainter>

#include <QItemDelegate>

class GradeDelegate: public QItemDelegate
{
public:
    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {

        // empty lesson
        if (!index.data(StatisticsModel::TotalCount).toInt()) {
            return;
        }

        drawBackground(painter, option, index);

        painter->drawText(option.rect, Qt::AlignCenter, QString("%1%").arg(index.data(StatisticsModel::TotalPercent).toInt()));

//         drawDisplay(painter, option, option.rect, index.data(Qt::DisplayRole).toString());
    }

protected:
    void drawBackground ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
        QLinearGradient linearGrad(QPointF(option.rect.x(), 0), QPointF(option.rect.x() + option.rect.width(), 0));
        linearGrad.setColorAt(0, Qt::green);
        linearGrad.setColorAt(index.data(StatisticsModel::TotalPercent).toInt()/100.0, Qt::red);
        // add a little frame of 1 pixel
        painter->fillRect(option.rect.adjusted(1, 1, -1, -1), linearGrad);
    }
};

LessonStatisticsView::LessonStatisticsView(QWidget * parent) :ContainerView(parent)
{
    header()->setVisible(true);
}

void LessonStatisticsView::setModel(ContainerModel *model)
{
    ContainerView::setModel(model);

//     header()->setResizeMode(0, QHeaderView::Stretch);
//     header()->setResizeMode(1, QHeaderView::ResizeToContents);
    for (int i = 2; i < model->columnCount(QModelIndex()); i++) {
        setItemDelegateForColumn(i, new GradeDelegate());
        setColumnWidth(i, 200);
    }
}

#include "lessonstatistics.moc"


