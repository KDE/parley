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
    GradeDelegate(QObject *parent = 0)
        : QItemDelegate(parent) {
    }

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        // empty lesson
        if (!index.data(StatisticsModel::TotalCount).toInt()) {
            return;
        }
        drawBackground(painter, option, index);
        painter->drawText(option.rect, Qt::AlignCenter, QString("%1%").arg(index.data(StatisticsModel::TotalPercent).toInt()));
    }

protected:
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
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
            // Display empty rectangle (white) for grade 0 and color for others.
            //
            // 255 being the max alpha, ie the darkest shade. As
            // grades become lower, the alpha value gets decremented
            // by 35 for every decrement in grade by 1.  Here 7 =>
            // No. of grades. 35 => arbitrary number for a good
            // difference in alpha values for consecutive grades, also
            // such that for grade 1, the alpha value isn't too low.
            QColor color = Prefs::gradeColor();
            color.setAlpha(255 - (7 - i) * 35);
            if (i == 0) {
                painter->setBrush(QBrush(QColor(255, 255, 255, 0)));
            } else {
                painter->setBrush(QBrush(color));
            }
            painter->drawPath(barElementIntersectedPath);
        }
    }
};


class LessonStatisticsHeader : public QHeaderView
{
public:
    LessonStatisticsHeader(Qt::Orientation orientation, QWidget *parent = 0) : QHeaderView(orientation, parent)
    {
    }

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
    {
        // Paints the legend in the header.
        painter->save();
        QHeaderView::paintSection(painter, rect, logicalIndex);
        painter->restore();

        const int legendWidth = 320;
        const int legendHeight = 30;
        const int legendOffsetY = 10;
        const int legendOffsetX = (rect.width() / 2) - (legendWidth / 2);
        const int gradeBarWidth = 40;
        const int alphaValueIncrement = 35;

        int iFirstUsedDataColumn;

        switch (Prefs::practiceDirection()) {
        case Prefs::EnumPracticeDirection::LearningToKnown:
            iFirstUsedDataColumn = Prefs::knownLanguage();
            break;
        case Prefs::EnumPracticeDirection::MixedDirectionsWordsOnly:
        case Prefs::EnumPracticeDirection::MixedDirectionsWithSound:
            iFirstUsedDataColumn = qMax( Prefs::knownLanguage(),  Prefs::learningLanguage() );
            break;
        case Prefs::EnumPracticeDirection::KnownToLearning:
        // Use KnownToLearning as default.
        default:
            iFirstUsedDataColumn = Prefs::learningLanguage();
            break;
        }
        iFirstUsedDataColumn += ContainerModel::FirstDataColumn;

        if (logicalIndex == iFirstUsedDataColumn) {
            QRect roundedRect(rect.x() + legendOffsetX, rect.y() + legendOffsetY, legendWidth, legendHeight);
            roundedRect.adjust(1, 1, -1, -1);
            QPainterPath roundedPath;
            roundedPath.addRoundedRect(roundedRect, 2.0, 2.0);

            for (int i = 7; i >= 0; i--) {
                QRectF barElement(rect.x() + legendOffsetX + (7 - i) * gradeBarWidth, rect.y() + legendOffsetY, gradeBarWidth, legendHeight);
                QPainterPath barElementPath;
                barElementPath.addRect(barElement);
                QPainterPath barElementIntersectedPath = roundedPath.intersected(barElementPath);
                QColor color = Prefs::gradeColor();
                color.setAlpha(255 - (7 - i) * alphaValueIncrement);
                if (i != 0) {
                    painter->setBrush(QBrush(color));
                } else {
                    painter->setBrush(QBrush(QColor(255, 255, 255, 200))); // White for the 'not learned' grade box in the legend.
                }
                painter->drawPath(barElementIntersectedPath);
            }

            painter->drawLine(QLine(rect.x() + legendOffsetX + 7 * gradeBarWidth, rect.y() + legendOffsetY + 1, rect.x() + legendOffsetX + 7 * gradeBarWidth, rect.y() + legendHeight + legendOffsetY - 1)); // Necessary hack to get the border on white.
            painter->setPen(Qt::black);
            QString leftString = i18nc("adjective, The word has been properly and fully learned by the user","Fully learned");
            QString rightString = i18nc("adjective, The word has not even been practiced once by the user","Not practiced");
            const int extraRoomInRectSize = 10;
            const int horizontalDistanceFromLegend = 10;
            QFontMetrics fontMetrics(painter->font());
            // Calculate the size and position of the rectangle that will contain the string on the left side of the legend
            QRect leftRect = fontMetrics.boundingRect(leftString);
            leftRect.setWidth(leftRect.width() + extraRoomInRectSize);
            leftRect.setHeight(leftRect.height() + extraRoomInRectSize);
            leftRect.moveBottomRight(QPoint(rect.x() + legendOffsetX - horizontalDistanceFromLegend, rect.y() + legendOffsetY + legendHeight));
            // Calculate the size and position of the rectangle that will contain the string on the right side of the legend
            QRect rightRect = fontMetrics.boundingRect(rightString);
            rightRect.setWidth(rightRect.width() + extraRoomInRectSize);
            rightRect.setHeight(rightRect.height() + extraRoomInRectSize);
            rightRect.moveBottomLeft(QPoint(rect.x() + legendOffsetX + legendWidth + horizontalDistanceFromLegend, rect.y() + legendOffsetY + legendHeight));
            painter->drawText(leftRect, Qt::AlignRight | Qt::AlignVCenter, leftString);
            painter->drawText(rightRect, Qt::AlignLeft | Qt::AlignVCenter, rightString);
        }
    }

    QSize sizeHint() const
    {
        // Get the base implementation size.
        QSize baseSize = QHeaderView::sizeHint();
        // Override the height with a custom value.
        baseSize.setHeight(70);
        return baseSize;
    }
};


LessonStatisticsView::LessonStatisticsView(QWidget *parent)
    : ContainerView(parent)
{
    LessonStatisticsHeader *lessonStatisticsHeader = new LessonStatisticsHeader(Qt::Horizontal, this);
    setHeader(lessonStatisticsHeader);
    header()->setVisible(true);
    header()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignBottom);

    // inherits context menu policy - so action will show up in right click menu
    KAction *removeGradesAction = new KAction(this);
    removeGradesAction->setText(i18n("Remove confidence levels from this unit"));
    removeGradesAction->setIcon(KIcon("edit-clear"));
    removeGradesAction->setWhatsThis(i18n("Remove confidence levels from this unit"));
    removeGradesAction->setToolTip(removeGradesAction->whatsThis());
    removeGradesAction->setStatusTip(removeGradesAction->whatsThis());

    connect(removeGradesAction, SIGNAL(triggered()), SLOT(removeGrades()));
    addAction(removeGradesAction);

    KAction *removeGradesChildrenAction = new KAction(this);
    removeGradesChildrenAction->setText(i18n("Remove confidence levels from this unit and all sub-units"));
    removeGradesChildrenAction->setIcon(KIcon("edit-clear"));
    removeGradesChildrenAction->setWhatsThis(i18n("Remove confidence level from this unit and all sub-units"));
    removeGradesChildrenAction->setToolTip(removeGradesChildrenAction->whatsThis());
    removeGradesChildrenAction->setStatusTip(removeGradesChildrenAction->whatsThis());

    connect(removeGradesChildrenAction, SIGNAL(triggered()), SLOT(removeGradesChildren()));
    addAction(removeGradesChildrenAction);
}

void LessonStatisticsView::setModel(Editor::ContainerModel *model)
{
    ContainerView::setModel(model);
    GradeDelegate *delegate = new GradeDelegate(this);
    for (int i = ContainerModel::FirstDataColumn; i < model->columnCount(QModelIndex()); i++) {
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
    KEduVocLesson *lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());
    lesson->resetGrades(-1, KEduVocContainer::NotRecursive);
}

void LessonStatisticsView::removeGradesChildren()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    KEduVocLesson *lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());
    lesson->resetGrades(-1, KEduVocContainer::Recursive);
}

#include "lessonstatistics.moc"
