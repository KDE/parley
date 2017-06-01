/***************************************************************************
    Copyright 2008 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>

#include <QPoint>

class Dashboard;
class QToolButton;

class ButtonDelegate
    : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ButtonDelegate(QAbstractItemView *itemView, Dashboard *parent);

    virtual ~ButtonDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QModelIndex hoveredIndex() const;

public slots:
    void slotEdit();
    void slotPractice();

private:
    int m_rightMargin;
    int m_buttonHeight;
    Dashboard *m_dashboard;
    QAbstractItemView *m_itemView;
    QToolButton *m_editButton;
};

#endif //BUTTONDELEGATE_H
