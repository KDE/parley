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
#include "welcomescreen.h"

#include <kwidgetitemdelegate.h>
#include <KMessageBox>

#include <QStandardItemModel>
#include <QPainter>
#include <QAbstractItemDelegate>
#include <QApplication>
#include <QToolButton>

const int margin = 5;
const int iconSize = 22;

ButtonDelegate::ButtonDelegate(QAbstractItemView *itemView, WelcomeScreen *parent )
        : KWidgetItemDelegate(itemView, parent), m_rightMargin(0), m_buttonHeight(0), m_welcomeScreen(parent)
{
}

ButtonDelegate::~ButtonDelegate()
{
}

QList<QWidget*> ButtonDelegate::createItemWidgets() const
{
    QList<QWidget*> widgetList;
    QToolButton *editButton = new QToolButton();
    editButton->setIcon(KIcon("document-edit"));
    editButton->setToolTip(i18n("Open this vocabulary collection in the editor"));
    m_rightMargin = editButton->sizeHint().width() + 3*margin;
    m_buttonHeight = editButton->sizeHint().height();

    connect(editButton, SIGNAL(clicked()), this, SLOT(slotEdit()));

    widgetList << editButton;
    return widgetList;
}

void ButtonDelegate::updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem &option, const QPersistentModelIndex &index) const
{
    Q_UNUSED(index)
    QToolButton *editButton = static_cast<QToolButton*>(widgets[0]);
    QSize editButtonSizeHint = editButton->sizeHint();
    editButton->resize(editButtonSizeHint);
    editButton->setAutoRaise(true);

    editButton->move(option.rect.width() - editButtonSizeHint.width() - 2*margin, (option.rect.height() - editButtonSizeHint.height())/2);
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
    
    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    int iconMargin = (option.rect.height() - iconSize)/2;
    painter->drawPixmap(option.rect.x() + iconMargin, option.rect.y() + iconMargin, icon.pixmap(iconSize, iconSize));
    
    int rectX = option.rect.x() + iconMargin + iconSize + margin;
    QRect textRect(rectX, option.rect.y(), option.rect.width() - rectX - m_rightMargin, option.rect.height());
    
    QFontMetrics fm(option.font);
    QString elidedText = fm.elidedText(index.data().toString(), Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elidedText);
}

QSize ButtonDelegate::sizeHint(const QStyleOptionViewItem &option,
                const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);

    return QSize(qMax(iconSize, m_buttonHeight), 32);
}

void ButtonDelegate::slotEdit()
{
    const QModelIndex index = focusedIndex();
    KUrl url = index.data(Qt::UserRole).toUrl();
    m_welcomeScreen->slotOpenUrl(url);
}

void ButtonDelegate::slotPractice()
{
    const QModelIndex index = focusedIndex();
    KUrl url = index.data(Qt::UserRole).toUrl();
    m_welcomeScreen->slotPracticeUrl(url);
}

#include "buttondelegate.moc"
