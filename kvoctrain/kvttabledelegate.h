//
// C++ Interface: kvttabledelegate
//
// Description:
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTTABLEDELEGATE_H
#define KVTTABLEDELEGATE_H

#include <QItemDelegate>

/**
  @author Peter Hedlund <peter.hedlund@kdemail.net>
*/
class KVTTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    KVTTableDelegate(QObject *parent = 0);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setCurrentIndex(const QModelIndex &index);

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
protected:
    void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QString &text) const;
    void drawFocus(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect) const;

private slots:
    void commitAndCloseEditor();

private:
    QModelIndex m_currentIndex;
};

#endif
