/***************************************************************************

                             kvttableview

    -----------------------------------------------------------------------

    copyright     : (C) 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kvttableview.h"

#include <QHeaderView>
#include <QPainter>
#include <QResizeEvent>

#include <KComponentData>
#include <KGlobalSettings>
#include <KAction>
#include <KLocale>

#include "kvttablemodel.h"
#include "prefs.h"

#define HEADER_MINSIZE   25
#define KV_COLWIDTH_MARK 25

KVTTableView::KVTTableView(QWidget *parent) : QTableView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
    setTabKeyNavigation(false);
    setSortingEnabled(true);

    connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(horizontalHeaderResized(int, int, int)));
    m_delegate = new KVTTableDelegate(this);
    setItemDelegate(m_delegate);

    // Enable context menus
    setContextMenuPolicy(Qt::ActionsContextMenu);
    horizontalHeader()->setContextMenuPolicy(Qt::ActionsContextMenu);
}

/*!
    \fn KVTTableView::setModel(KVTTableModel * model)
 */
void KVTTableView::setModel(KVTSortFilterModel * model)
{
    QTableView::setModel(model);
//     setCurrentIndex(model->index(0, 0));
//     scrollTo(currentIndex());
    connect(verticalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(verticalHeaderResized(int, int, int)));
    connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(horizontalHeaderResized(int, int, int)));
    connect(selectionModel(), SIGNAL(currentColumnChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotCurrentColumnChanged(const QModelIndex&, const QModelIndex&)));
}

void KVTTableView::verticalHeaderResized(int , int , int)
{
    //kDebug() << "Row resized\n";
}

void KVTTableView::horizontalHeaderResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    //kDebug() << "Column resized\n";
    model()->setHeaderData(logicalIndex, Qt::Horizontal, QSize(newSize, 25), Qt::SizeHintRole);
}


void KVTTableView::slotModelReset()
{
    for (int i = 0; i < horizontalHeader()->count(); ++i)
        setColumnWidth(i, qvariant_cast<QSize>(model()->headerData(i, Qt::Horizontal, Qt::SizeHintRole)).width());
}

void KVTTableView::slotCurrentColumnChanged(const QModelIndex & current, const QModelIndex & previous)
{
    Q_UNUSED(previous);
    m_delegate->setCurrentIndex(current);
    reset();
}

void KVTTableView::slotShowLessonColumn(bool show)
{
    Prefs::setTableLessonColumnVisible(show);
    setColumnHidden(KV_COL_LESS, !show);
}

void KVTTableView::print(KPrinter * pPrinter)
{
    ///@todo revisit and check before release
    QPainter painter;
    painter.setBrush(Qt::NoBrush);
    int res = pPrinter->resolution();
    int marg = res;

    int pw;
    int c = 0;
    int currentWidth;
    int startCol;
    int pageNum = 1;

    int colCount = model()->columnCount(QModelIndex());
    int hh = horizontalHeader()->height();
    int tPos = marg + hh;

    QStyleOptionViewItem option;
    option.initFrom(this);

    painter.begin(pPrinter);
    QRect w = painter.window();
    QRect cr;

    pw = w.width() - (2 * marg);

    while (c < colCount) {
        startCol = c;
        currentWidth = marg;

        while ((currentWidth < pw) && (c < colCount)) {
            currentWidth = currentWidth + columnWidth(c);
            c++;
        }
        if (c < colCount)
            c--;

        newPage(painter, res, startCol, c);

        for (int rc = 0; rc < model()->rowCount(QModelIndex()); ++rc) {
            int rh = rowHeight(rc);

            painter.resetMatrix();
            painter.setFont(Prefs::tableFont());
            painter.translate(marg, tPos);

            for (int i = startCol; i <= c && i < colCount; ++i) {
                cr.setRect(0, 0, columnWidth(i), rh);
                option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
                option.rect = cr;
                painter.drawRect(cr);
                painter.save();

                itemDelegate()->paint(&painter, option, model()->index(rc, i));
                painter.restore();

                painter.translate(columnWidth(i), 0);
            }

            tPos = tPos + rowHeight(rc);

            if (tPos + rowHeight(rc + 1) > w.height() - marg) {
                endOfPage(painter, pageNum++, res);
                tPos = marg + hh;
                pPrinter->newPage();
                newPage(painter, res, startCol, c);
            }
        }
        endOfPage(painter, pageNum++, res);

        if (c < colCount) {
            pPrinter->newPage();
            tPos = marg + hh;
            c++;
        }
    }
    painter.end();
}

void KVTTableView::newPage(QPainter & painter, int res, int startCol, int endCol)
{
    int marg = res;
    int hh = horizontalHeader()->height();
    int cw;
    QRect cr;
    QRect w = painter.window();
    painter.resetMatrix();
    painter.setFont(KGlobalSettings::generalFont());
    /// @todo improve i18n("KVocTrain - %1", m_doc->title())
    painter.drawText(marg, marg - 20, KGlobal::caption());
    painter.translate(marg, marg);
    painter.drawLine(0 , 0, 0, hh);
    for (int i = startCol; i <= endCol && i < model()->columnCount(QModelIndex()); ++i) {
        cw = columnWidth(i);
        painter.drawLine(0, 0, cw, 0);
        painter.drawLine(cw, 0, cw, hh);
        cr.setRect(3, 0, columnWidth(i)- 6, hh);
        painter.drawText(cr, Qt::AlignLeft | Qt::AlignVCenter, model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        painter.translate(cw, 0);
    }
}

void KVTTableView::endOfPage(QPainter & painter, int pageNum, int res)
{
    painter.resetMatrix();
    painter.setFont(KGlobalSettings::generalFont());
    QRect w = painter.window();
    QRect r = painter.boundingRect(0, 0, 0, 0, Qt::AlignLeft, QString::number(pageNum));
    painter.drawText((w.width()/2) - (r.width()/2), w.height() - res + 20, QString::number(pageNum));
}

void KVTTableView::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);

    if (event == 0)
        return;

    QHeaderView * header = horizontalHeader();
    int colCount =  model()->columnCount(QModelIndex());
    int oldWidth = 0;

    for (int i = 0; i < colCount; ++i)
        oldWidth += header->sectionSize(i);

    int newWidth = viewport()->width();
    int remainder = newWidth;


    switch (Prefs::headerResizeMode()) {
    case Prefs::EnumHeaderResizeMode::Automatic: {
            // lesson is only half as wide as a original/translation
            // exclude fixed size of "mark"-column
            int x = (remainder - KV_COLWIDTH_MARK) / ((colCount - 1) * 2 - 1);
            header->resizeSection(KV_COL_LESS, x);
            remainder -= x;
            header->resizeSection(KV_COL_MARK, KV_COLWIDTH_MARK);
            remainder -= KV_COLWIDTH_MARK;
            for (int i = KV_COL_TRANS; i < colCount - 1; i++) {
                remainder -= 2 * x;
                header->resizeSection(i, 2 * x);
            }
            header->resizeSection(colCount - 1, remainder);
        }
        break;

    case Prefs::EnumHeaderResizeMode::Percent: {
            float grow = float(newWidth) / float(oldWidth);
            header->resizeSection(KV_COL_MARK, KV_COLWIDTH_MARK);

            int remainder = newWidth - KV_COLWIDTH_MARK;
            int x = qMax(HEADER_MINSIZE, (int)((header->sectionSize(KV_COL_LESS) * grow) + 0.5));
            header->resizeSection(KV_COL_LESS, x);
            remainder -= x;

            for (int i = KV_COL_TRANS; i < colCount - 1; i++) {
                x = qMax(HEADER_MINSIZE, (int)((header->sectionSize(i) * grow) + 0.5));
                remainder -= x;
                header->resizeSection(i, x);
            }
            header->resizeSection(colCount - 1, qMax(HEADER_MINSIZE, remainder));
        }
        break;

    case Prefs::EnumHeaderResizeMode::Fixed:
        // nix
        break;
    }
}

void KVTTableView::showEvent(QShowEvent * event)
{
    QWidget::showEvent(event);
    QResizeEvent rsEvent(size(), size());
    resizeEvent(&rsEvent);
}

void KVTTableView::adjustContent()
{
    QResizeEvent rsEvent(size(), size());
    resizeEvent(&rsEvent);
}

void KVTTableView::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        e->accept();
        // Should we be in the last row, will we autoappend a new entry?
        if ( currentIndex().row() == model()->rowCount() - 1 ) {
            if ( currentIndex().column() == model()->columnCount(QModelIndex()) - 1 ) {
                if( Prefs::smartAppend() ) {
                    emit(appendEntry());
                }
            }
        }
        // move to the next language entry. for the last language this means first of next row.
        if ( currentIndex().column() == model()->columnCount(QModelIndex()) - 1 ) {
            setCurrentIndex(model()->index( currentIndex().row() + 1, KV_EXTRA_COLS ) );
        } else {
            setCurrentIndex(QTableView::moveCursor(QAbstractItemView::MoveNext, 0));
        }
        return;
    }
    QTableView::keyPressEvent(e);
}

#include "kvttableview.moc"
