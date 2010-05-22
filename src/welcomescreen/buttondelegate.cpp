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

#include <KMessageBox>

#include <QStandardItemModel>
#include <QPainter>
#include <QAbstractItemDelegate>
#include <QApplication>
#include <QToolButton>

const int margin = 5;
const int iconSize = 22;

ButtonDelegate::ButtonDelegate(QAbstractItemView *itemView, WelcomeScreen *parent)
        : QStyledItemDelegate(itemView), m_rightMargin(0), m_buttonHeight(0), m_welcomeScreen(parent), m_itemView(itemView)
{
    m_editButton = new QToolButton(itemView->viewport());
    m_editButton->setIcon(KIcon("document-edit"));
    m_editButton->setToolTip(i18n("Open this vocabulary collection in the editor"));
    m_editButton->resize(m_editButton->sizeHint());
    m_editButton->setAutoRaise(true);
    m_editButton->hide();
    m_rightMargin = m_editButton->sizeHint().width() + margin;
    m_buttonHeight = m_editButton->sizeHint().height();

    connect(m_editButton, SIGNAL(clicked()), this, SLOT(slotEdit()));
}

ButtonDelegate::~ButtonDelegate()
{
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, 0);
    if(option.state & QStyle::State_MouseOver) {
        m_editButton->show();
        m_editButton->move(QPoint(option.rect.right()-margin-m_editButton->sizeHint().width(),
                                  option.rect.top()+(option.rect.height()-m_editButton->height())/2+1));
    } else if (!hoveredIndex().isValid()) {
        m_editButton->hide();
    }

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
    const QModelIndex index = hoveredIndex();
    KUrl url = index.data(Qt::UserRole).toUrl();
    m_welcomeScreen->slotOpenUrl(url);
}

void ButtonDelegate::slotPractice()
{
    const QModelIndex index = hoveredIndex();
    KUrl url = index.data(Qt::UserRole).toUrl();
    m_welcomeScreen->slotPracticeUrl(url);
}

QModelIndex ButtonDelegate::hoveredIndex() const
{
    const QPoint pos = m_itemView->viewport()->mapFromGlobal(QCursor::pos());
    return m_itemView->indexAt(pos);
}

#include "buttondelegate.moc"
