/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

// Own
#include "lessonstatisticsview.h"

// Qt
#include <QApplication>
#include <QHeaderView>
#include <QItemDelegate>
#include <QPainter>

// KDE
#include <KLocalizedString>
#include <KMessageBox>
#include <QInputDialog>
#include <QAction>
#include <KActionCollection>

// Parley
#include "statisticslegendwidget.h"
#include "statisticsmodel.h"
#include <KEduVocLesson>
#include "utils.h"
#include "documentsettings.h"


// GradeDelegate shows the graphic colored bar in the statistics,
// showing how far the student has come on the way to enlightenment.

class GradeDelegate: public QItemDelegate
{
public:
    GradeDelegate(QObject *parent = 0)
        : QItemDelegate(parent) {
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);

        // empty lesson? If so, paint nothing.
        if (!index.data(StatisticsModel::TotalCount).toInt()) {
            return;
        }

        // Draw the colored bar.
        KEduVocContainer *container = index.data(StatisticsModel::Container)
                                           .value<KEduVocContainer*>();
        QStringList activeConjugationTenses = index.data(StatisticsModel::ActiveConjugationTenses)
                                                   .toStringList();
        WordCount  wordCount;
        wordCount.fillFromContainerForPracticeMode(
            *container,
            index.column() - ContainerModel::FirstDataColumn,
            activeConjugationTenses
        );
        ConfidenceColors  colors(ConfidenceColors::ProgressiveColorScheme);

        paintColorBar(*painter, option.rect, wordCount, colors); // in utils

        // Draw the text telling the percentage on top of the bar.
        painter->drawText(option.rect, Qt::AlignCenter,
                    QStringLiteral("%1%").arg(index.data(StatisticsModel::TotalPercent).toInt()));
    }
};


// ----------------------------------------------------------------


LessonStatisticsView::LessonStatisticsView(QWidget *parent)
    : ContainerView(parent)
{
    header()->setVisible(true);
    header()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignBottom);
    header()->setSectionsMovable( true );

    // inherits context menu policy - so action will show up in right click menu
    QAction *removeGradesAction = new QAction(this);
    removeGradesAction->setText(i18n("Remove confidence levels from this unit"));
    removeGradesAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-clear")));
    removeGradesAction->setWhatsThis(i18n("Remove confidence levels from this unit"));
    removeGradesAction->setToolTip(removeGradesAction->whatsThis());
    removeGradesAction->setStatusTip(removeGradesAction->whatsThis());

    connect(removeGradesAction, &QAction::triggered, this, &LessonStatisticsView::removeGrades);
    addAction(removeGradesAction);

    QAction *removeGradesChildrenAction = new QAction(this);
    removeGradesChildrenAction->setText(i18n("Remove confidence levels from this unit and all sub-units"));
    removeGradesChildrenAction->setIcon(QIcon::fromTheme(QStringLiteral("edit-clear")));
    removeGradesChildrenAction->setWhatsThis(i18n("Remove confidence level from this unit and all sub-units"));
    removeGradesChildrenAction->setToolTip(removeGradesChildrenAction->whatsThis());
    removeGradesChildrenAction->setStatusTip(removeGradesChildrenAction->whatsThis());

    connect(removeGradesChildrenAction, &QAction::triggered, this, &LessonStatisticsView::removeGradesChildren);
    addAction(removeGradesChildrenAction);

    connect(header(), &QHeaderView::geometriesChanged,
	    this,     &LessonStatisticsView::adjustColumnWidths);
    connect(header(), &QHeaderView::sectionResized,
	    this,     &LessonStatisticsView::sectionResized);
}

LessonStatisticsView::~LessonStatisticsView()
{
    saveExpandedStatus();
}

void LessonStatisticsView::setModel(ContainerModel *model)
{
    ContainerView::setModel(model);

    GradeDelegate *delegate = new GradeDelegate(this);
    for (int i = ContainerModel::FirstDataColumn; i < model->columnCount(QModelIndex()); i++) {
        setItemDelegateForColumn(i, delegate);
    }

    adjustColumnWidths();
}

void LessonStatisticsView::resizeEvent(QResizeEvent *event)
{
    adjustColumnWidths();
    ContainerView::resizeEvent(event);
}

void LessonStatisticsView::sectionResized(int index,
					  int /*oldSize*/, int /*newSize*/)
{
    if (index < ContainerModel::FirstDataColumn) {
	adjustColumnWidths();
    }
}

void LessonStatisticsView::adjustColumnWidths()
{
    int firstWidth = columnWidth(0) + columnWidth(1);
    int totalWidth = viewport()->width() - firstWidth;
    int columnCount = model()->columnCount(QModelIndex());
    int visibleColumns = 0;
    for (int i = ContainerModel::FirstDataColumn; i < columnCount; ++i) {
        if (!isColumnHidden(i))
            visibleColumns++;
    }
    int columnWidth = visibleColumns > 0 ? totalWidth / visibleColumns : 150;
    for (int i = ContainerModel::FirstDataColumn; i < model()->columnCount(QModelIndex()); i++) {
        setColumnWidth(i, columnWidth);
    }
//    header()->resizeSections(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(QHeaderView::Interactive);
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

void LessonStatisticsView::saveExpandedStatus() const
{
    auto statisticsModel = qobject_cast<StatisticsModel *>(model());
    Q_ASSERT(statisticsModel != nullptr);

    QStringList collapsedItems;
    getCollapsedItems(collapsedItems, statisticsModel->index(0, 0, QModelIndex()), QString());

    const KEduVocDocument *doc = statisticsModel->document().get();
    if (doc != nullptr) {
        DocumentSettings documentSettings(doc->url().url());
        documentSettings.setCollapsedStatisticsViewItems(collapsedItems);
        documentSettings.save();
    }
}

void LessonStatisticsView::getCollapsedItems(QStringList &collapsedItems, const QModelIndex &item,
                                            QString name) const
{
    if (!item.isValid()) {
        return;
    }

    int rowCount = model()->rowCount(item);
    if (rowCount > 0) {
        // Item has children and therefore expandable
        name += item.data().toString();
        if (!isExpanded(item)) {
            collapsedItems << name;
        }
        for (int row = 0; row < rowCount; ++row) {
            getCollapsedItems(collapsedItems, model()->index(row, 0, item), name + '/');
        }
    }
}

void LessonStatisticsView::restoreExpandedStatus()
{
    auto statisticsModel = qobject_cast<StatisticsModel *>(model());
    Q_ASSERT(statisticsModel != nullptr);

    const KEduVocDocument *doc = statisticsModel->document().get();
    if (doc != nullptr) {
        DocumentSettings documentSettings(doc->url().url());
        documentSettings.load();
        QStringList collapsedItems = documentSettings.collapsedStatisticsViewItems();
        setCollapsedItems(collapsedItems, statisticsModel->index(0, 0, QModelIndex()), QString());
    }
}

void LessonStatisticsView::setCollapsedItems(const QStringList &collapsedItems,
                                             const QModelIndex &item, QString name)
{
    if (!item.isValid()) {
        return;
    }

    int rowCount = model()->rowCount(item);
    if (rowCount > 0) {
        // Item has children and therefore expandable
        name += item.data().toString();
        if (collapsedItems.contains(name)) {
            collapse(item);
        } else {
            expand(item);
        }
        for (int row = 0; row < rowCount; ++row) {
            setCollapsedItems(collapsedItems, model()->index(row, 0, item), name + '/');
        }
    }
}

