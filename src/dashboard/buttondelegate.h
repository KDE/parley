/*
    SPDX-FileCopyrightText: 2008 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>

#include <QPoint>

class Dashboard;
class QToolButton;

class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ButtonDelegate(QAbstractItemView *itemView, Dashboard *parent);

    ~ButtonDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QModelIndex hoveredIndex() const;

public Q_SLOTS:
    void slotEdit();
    void slotPractice();

private:
    int m_rightMargin;
    int m_buttonHeight;
    Dashboard *m_dashboard;
    QAbstractItemView *m_itemView;
    QToolButton *m_editButton;
};

#endif // BUTTONDELEGATE_H
