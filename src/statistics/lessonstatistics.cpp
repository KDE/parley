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
    GradeDelegate(QObject* parent = 0)
        :QItemDelegate(parent)
    {}

    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        // empty lesson
        if (!index.data(StatisticsModel::TotalCount).toInt()) {
            return;
        }
        drawBackground(painter, option, index);
        painter->drawText(option.rect, Qt::AlignCenter, QString("%1%").arg(index.data(StatisticsModel::TotalPercent).toInt()));
    }

protected:
    void drawBackground ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
        // Create the outer rounded rectangle.
        QRect roundedRect(option.rect);
        roundedRect.adjust(1, 1, -1, -1);
        QPainterPath roundedPath;
        roundedPath.addRoundedRect(roundedRect, 2.0, 2.0);

        qreal total = index.data(StatisticsModel::TotalCount).toInt();
        int xPosition = 0;
        // Create the inner, grades' rectangles.
        for (int i = 7; i >= 0; i--) {
            int count = index.data(StatisticsModel::Grade0 + i).toInt();
            int barElementWidth = (double)(count / total) * option.rect.width();
            QRectF barElement(option.rect.x() + xPosition, option.rect.y(), barElementWidth, option.rect.height());
            QPainterPath barElementPath;
            barElementPath.addRect(barElement);
            xPosition += barElementWidth;
            // Intersect the QPainterPath of inner rectangle with outer,
            // so that the inner rectangle takes the shape of the outer rounded rectangle.
            QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);
            // Display empty rectangle (white) for Grade 0 and color for others.
            QColor color = Prefs::gradeColor();
            // 255 being the max alpha, ie the darkest shade. As grades become lower,
            // the alpha value gets decremented by 35 for every decrement in grade by 1.
            // Here 7 => No. of grades. 35 => arbitrary number for a good difference in alpha values for
            // consecutive grades, also such that for grade 1, the alpha value isn't too low.
            color.setAlpha(255 - (7 - i) * 35);
            if (i != 0) {
                painter->setBrush(QBrush(color));
            } else {
                painter->setBrush(QBrush(QColor(255, 255, 255, 0)));
            }
            painter->drawPath(barElementIntersectedPath);
        }
    }
};



LessonStatisticsView::LessonStatisticsView(QWidget * parent)
    :ContainerView(parent)
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

void LessonStatisticsView::setModel(Editor::ContainerModel *model)
{
    ContainerView::setModel(model);
    GradeDelegate* delegate = new GradeDelegate(this);
    for (int i = 2; i < model->columnCount(QModelIndex()); i++) {
        setItemDelegateForColumn(i, delegate);
        setColumnWidth(i, 150);
    }
//    header()->resizeSections(QHeaderView::ResizeToContents);
    header()->setResizeMode(QHeaderView::Interactive);
    header()->setStretchLastSection(true);
}

void LessonStatisticsView::removeGrades()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    KEduVocLesson* lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());
    lesson->resetGrades(-1, KEduVocContainer::NotRecursive);
}

#include "lessonstatistics.moc"


