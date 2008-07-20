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

#include "buttondelegate.h"

#include <kwidgetitemdelegate.h>
#include <KPushButton>

#include <QStandardItemModel>
#include <QPainter>
#include <QAbstractItemDelegate>
#include <QApplication>

const int margin = 5;

ButtonDelegate::ButtonDelegate(QAbstractItemView *itemView, QObject *parent )
        : KWidgetItemDelegate(itemView, parent)
{
}

ButtonDelegate::~ButtonDelegate()
{
}
    
QList<QWidget*> ButtonDelegate::createItemWidgets() const
{
    QList<QWidget*> widgetList;
    KPushButton* editButton = new KPushButton();
    editButton->setText("Edit");
    KPushButton* practiceButton = new KPushButton();
    practiceButton->setText("Practice");
    widgetList << editButton << practiceButton;
    return widgetList;
}

void ButtonDelegate::updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    KPushButton *editButton = static_cast<KPushButton*>(widgets[0]);
    QSize editButtonSizeHint = editButton->sizeHint();
    editButton->resize(editButtonSizeHint);
    
    KPushButton *practiceButton = static_cast<KPushButton*>(widgets[1]);
    QSize practiceButtonSizeHint = practiceButton->sizeHint();
    practiceButton->resize(practiceButtonSizeHint);
    
    editButton->move(option.rect.width() - editButtonSizeHint.width() - practiceButtonSizeHint.width() - 2*margin, (option.rect.height() - editButtonSizeHint.height())/2);
    practiceButton->move(option.rect.width() - practiceButtonSizeHint.width() - margin, (option.rect.height() - practiceButtonSizeHint.height())/2);

}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
    
    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    int iconMargin = (option.rect.height() - 32)/2;
    painter->drawPixmap(option.rect.x() + iconMargin, option.rect.y() + iconMargin, icon.pixmap(32, 32));

    KWidgetItemDelegate::paintWidgets(painter, option, index);
}

QSize ButtonDelegate::sizeHint(const QStyleOptionViewItem &option,
                const QModelIndex &index) const
{
    Q_UNUSED(index);

    return QSize(32, 32);
}

#include "buttondelegate.moc"
