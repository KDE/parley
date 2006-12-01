//
// C++ Implementation: kvttabledelegate
//
// Description: 
//
//
// Author:  (C) 2006 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QtGui>

#include "kdebug.h"

#include "kvttabledelegate.h"

KVTTableDelegate::KVTTableDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget * KVTTableDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QLineEdit *editor = new QLineEdit(parent);
  editor->setFrame(false);
  editor->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
  //editor->installEventFilter(const_cast<KWQTableDelegate*>(this));
  connect(editor, SIGNAL(returnPressed()), this, SLOT(commitAndCloseEditor()));
  return editor;
}

void KVTTableDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
  QString value = index.model()->data(index, Qt::DisplayRole).toString();

  if (value == "@empty@")
    value = "";

  QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  lineEdit->setText(value);
}

void KVTTableDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
  QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  QString value = lineEdit->text();

  model->setData(index, value);
}

void KVTTableDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
  editor->setGeometry(option.rect);
}

void KVTTableDelegate::commitAndCloseEditor()
{
  QWidget *editor = qobject_cast<QWidget *>(sender());
  kDebug() << "Committing and closing\n";
  emit commitData(editor);
  emit closeEditor(editor, QAbstractItemDelegate::NoHint);
}

void KVTTableDelegate::drawDisplay(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, const QString & text) const
{
  QPen pen = painter->pen();
  QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
  if (option.state & QStyle::State_Selected) {
      painter->fillRect(rect, option.palette.brush(cg, option.state & QStyle::State_HasFocus ?
        QPalette::Base : QPalette::Highlight));
      painter->setPen(option.palette.color(cg, option.state & QStyle::State_HasFocus ?
        QPalette::Text : QPalette::HighlightedText));
  } else {
      painter->setPen(option.palette.color(cg, QPalette::Text));
  }

  if (option.state & QStyle::State_Editing) {
      painter->save();
      painter->setPen(option.palette.color(cg, QPalette::Text));
      painter->drawRect(rect.adjusted(0, 0, -1, -1));
      painter->restore();
  }

  QFont font = painter->font();
  painter->setFont(option.font);
  QRect textRect = rect.adjusted(3, 0, -3, 0); // remove width padding
  QString str = text;
  if (str == "@empty@")
    str = "";

  if (painter->fontMetrics().width(text) > textRect.width() && !text.contains(QLatin1Char('\n')))
      str = elidedText(option.fontMetrics, textRect.width(), option.textElideMode, text);

  painter->drawText(textRect, option.displayAlignment, str);
  painter->setFont(font);
  painter->setPen(pen);
}

void KVTTableDelegate::drawFocus(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect) const
{
  if (option.state & QStyle::State_HasFocus) {
    painter->save();
    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    pen.setWidth(0);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect.adjusted(0,0,-1,-1));
    painter->drawRect(rect.adjusted(1,1,-2,-2));
    painter->restore();
  }
}

QSize KVTTableDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  return QSize(300, 25);
}

void KVTTableDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QItemDelegate::paint(painter, option, index);
}

#include "kvttabledelegate.moc"
