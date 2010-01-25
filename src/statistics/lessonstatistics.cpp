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

#include <QApplication>
#include <QHeaderView>

#include <QPainter>

#include <QItemDelegate>

using namespace Editor;

class GradeDelegate: public QItemDelegate
{
public:
    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

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

        int total = index.data(StatisticsModel::TotalCount).toInt();

        int sum = 0;
        for (int i = 7; i >= 1; i--) {
            int count = index.data(StatisticsModel::Grade0 + i).toInt();
            if (count) {
                sum += count;
                linearGrad.setColorAt(((double)sum)/total, Prefs::gradeColor(i));
            }
        }

        if (sum == 0) {
            return;
        }

        QRect rect(option.rect);
        rect.adjust(1, 1, -(rect.width()-rect.width()*sum/total)-1, -1);

        QPainterPath path;
        path.addRoundedRect( rect, 2.0, 2.0 );
        painter->setBrush(QBrush(linearGrad));
        painter->drawPath(path);
    }
};

LessonStatisticsView::LessonStatisticsView(QWidget * parent) :ContainerView(parent)
{
    header()->setVisible(true);

    // inherits context menu policy - so action will show up in right click menu
    KAction *removeGradesAction = new KAction(this);
    removeGradesAction->setText(i18n("Remove &Grades"));
    removeGradesAction->setIcon(KIcon("edit-clear"));
    removeGradesAction->setWhatsThis(i18n("Remove all grades from this lesson"));
    removeGradesAction->setToolTip(removeGradesAction->whatsThis());
    removeGradesAction->setStatusTip(removeGradesAction->whatsThis());

    connect(removeGradesAction, SIGNAL(triggered()), SLOT(removeGrades()));

    addAction(removeGradesAction);
}

void LessonStatisticsView::setModel(ContainerModel *model)
{
    ContainerView::setModel(model);

//     header()->setResizeMode(0, QHeaderView::Stretch);
    header()->setResizeMode(QHeaderView::ResizeToContents);
    for (int i = 2; i < model->columnCount(QModelIndex()); i++) {
        setItemDelegateForColumn(i, new GradeDelegate());
        setColumnWidth(i, 200);
    }
}

void LessonStatisticsView::removeGrades()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    KEduVocLesson* lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());
    lesson->resetGrades(-1, KEduVocContainer::NotRecursive);
}

#include "lessonstatistics.moc"


