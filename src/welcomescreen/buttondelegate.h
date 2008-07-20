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

#include <kwidgetitemdelegate.h>

class ButtonDelegate
    : public KWidgetItemDelegate
{
Q_OBJECT
public:
    ButtonDelegate(QAbstractItemView *itemView, QObject *parent = 0);

    virtual ~ButtonDelegate();
    
    QList<QWidget*> createItemWidgets() const;
    void updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    mutable int m_rightMargin;
    mutable int m_buttonHeight;
};





#endif
