/***************************************************************************

                             kvttableview

    -----------------------------------------------------------------------

    copyright     : (C) 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef KVTTABLEVIEW_H
#define KVTTABLEVIEW_H

#include <QTableView>
#include <QtGui/QPrinter>

#include <kvtsortfiltermodel.h>
#include <kvttabledelegate.h>

/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/
class KVTTableView : public QTableView
{
    Q_OBJECT
public:
    KVTTableView(QWidget *parent = 0);

    void setModel(KVTSortFilterModel * model);
    void print(QPrinter *pPrinter);
    void adjustContent();

public slots:
    void slotModelReset();

signals:
    void appendEntry();

private slots:
    void verticalHeaderResized(int, int, int);
    void horizontalHeaderResized(int, int, int);
    void slotCurrentColumnChanged(const QModelIndex & current, const QModelIndex & previous);
    /** Show the lesson column of the table.
     * @param show if @c true the lesson column is shown
     */
    void slotShowLessonColumn(bool show);

protected:
    /** resizes table when frame is resized */
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
    void keyPressEvent(QKeyEvent*);

private:
    void newPage(QPainter &, int, int, int);
    void endOfPage(QPainter &, int, int);

    KVTTableDelegate * m_delegate;
};

#endif
