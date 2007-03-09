//
// C++ Implementation: kvttabledelegate
//
// Description: 
//
//
// Author:  (C) 2006, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QComboBox>
#include <QLineEdit>
#include <QPainter>

#include "kdebug.h"
#include <kglobalsettings.h>
#include <klocale.h>
#include <kiconloader.h>

#include "kvttabledelegate.h"
#include "kvttablemodel.h"
#include "prefs.h"

#include <keduvocexpression.h>

KVTTableDelegate::KVTTableDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget * KVTTableDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  Q_UNUSED(option); /// as long as it's unused

  if (!index.isValid())
    return 0;

  switch (index.column()) {
    case 0:
    {
      QComboBox *lessonbox = new QComboBox(parent);
      lessonbox->setFrame(false);
      lessonbox->setFont(KGlobalSettings::generalFont());
      connect(lessonbox, SIGNAL(returnPressed()), this, SLOT(commitAndCloseEditor()));
      return lessonbox;
    }
    break;

    case 1:
    {
      QComboBox *statebox = new QComboBox(parent);
      statebox->setFrame(false);
      statebox->setFont(KGlobalSettings::generalFont());
      connect(statebox, SIGNAL(returnPressed()), this, SLOT(commitAndCloseEditor()));
      return statebox;
    }
    break;

    default:
    {
      QLineEdit *editor = new QLineEdit(parent);
      editor->setFrame(false);
      editor->setFont(index.model()->data(index, Qt::FontRole).value<QFont>());
      ///@todo activate the keyboard layout switching code
      /*if (m_doc) {
        QString id = (col == KV_COL_ORG) ? m_doc->originalIdentifier()
          : m_doc->identifier(col - KV_EXTRA_COLS);

        if (langs) {
          QString kbLayout(langs->keyboardLayout(langs->indexShortId(id)));
          if (!kbLayout.isEmpty()) {
              // TODO use generated interface instead
              QDBusInterface kxkb( "org.kde.kxkb", "/kxkb", "org.kde.KXKB" );
              if (kxkb.isValid())
                  kxkb.call( "setLayout", kbLayout );
          }
        }
      }*/
      connect(editor, SIGNAL(returnPressed()), this, SLOT(commitAndCloseEditor()));
      return editor;
    }
  }
}


void KVTTableDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
  if (!index.isValid())
    return;

  switch (index.column()) {
    case 0:
    {
      QComboBox *lessonbox = static_cast<QComboBox*>(editor);
      lessonbox->addItems(index.model()->data(index, KVTTableModel::LessonsRole).toStringList());
      lessonbox->setCurrentIndex(index.model()->data(index, KVTTableModel::LessonRole).toInt());
    }
    break;

    case 1:
    {
      QComboBox *statebox = static_cast<QComboBox*>(editor);
      statebox->addItem (i18nc("state of a row", "Active, Not in Query"));
      statebox->addItem (i18nc("state of a row", "In Query"));
      statebox->addItem (i18nc("state of a row", "Inactive"));
      statebox->setCurrentIndex(index.model()->data(index, KVTTableModel::StateRole).toInt());
    }
    break;

    default:
    {
      QString value = index.model()->data(index, Qt::DisplayRole).toString();

      if (value == "@empty@")
        value = "";

      QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
      lineEdit->setText(value);
    }
  }
}

void KVTTableDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
  if (!index.isValid())
    return;

  switch (index.column()) {
    case 0:
    {
      QComboBox *lessonbox = static_cast<QComboBox*>(editor);
      int value = lessonbox->currentIndex();
      model->setData(index, value);
    }
    break;

    case 1:
    {
      QComboBox *statebox = static_cast<QComboBox*>(editor);
      int value = statebox->currentIndex();
      model->setData(index, value);
    }
    break;

    default:
    {
      QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
      QString value = lineEdit->text();
      model->setData(index, value);
    }
  }
}

void KVTTableDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (index.column() == 1)
  {
    /// @todo a better way to calculate the width of this combobox?
    //QSize sz = editor->sizeHint();
    QRect r = option.rect;
    //sz.setHeight(option.rect.height());
    r.setWidth(150);
    editor->setGeometry(r);
  }
  else
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
      painter->fillRect(rect, option.palette.brush(cg, option.state & QStyle::State_HasFocus ? QPalette::Base : QPalette::Highlight));
      if (option.state & QStyle::State_HasFocus)
        /**/;
      else
        painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
  } else {
      //painter->setPen(option.palette.color(cg, QPalette::Text));
  }

  if (option.state & QStyle::State_Editing) {
      painter->save();
      painter->setPen(option.palette.color(cg, QPalette::Text));
      painter->drawRect(rect.adjusted(0, 0, -1, -1));
      painter->restore();
  }

  if (text == "@inactive@")
  {
    painter->drawPixmap((rect.width() / 2) + rect.left() - 8, (rect.height() / 2) + rect.top() - 8, 16, 16, SmallIcon("no"));
  }
  else if (text == "@inquery@")
  {
    painter->drawPixmap((rect.width() / 2) + rect.left() - 8, (rect.height() / 2) + rect.top() - 8, 16, 16, SmallIcon("ok"));
  }
  else
  {
    QFont font = painter->font();
    painter->setFont(option.font);
    QRect textRect = rect.adjusted(3, 0, -3, 0); // remove width padding
    QString str = text;
    if (str == "@empty@")
      str = ""; 
    if (painter->fontMetrics().width(text) > textRect.width() && !text.contains(QLatin1Char('\n')))
        str = elidedText(option.fontMetrics, textRect.width(), option.textElideMode, str);

    painter->drawText(textRect, option.displayAlignment, str);
    painter->setFont(font);
    painter->setPen(pen);  
  }
}

void KVTTableDelegate::drawFocus(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect) const
{
  if (option.state & QStyle::State_HasFocus) {
    painter->save();
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect.adjusted(0,0,-1,-1));
    painter->drawRect(rect.adjusted(1,1,-2,-2));
    painter->restore();
  }
}

QSize KVTTableDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  return QItemDelegate::sizeHint(option, index);
}

void KVTTableDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (Prefs::useGradeCol())
  {
    QColor color = KV_NORM_COLOR;

    if (index.column() > KV_COL_ORG)
    {
      color = Prefs::gradeCol(0);
      switch (index.model()->data(index, KVTTableModel::GradeRole).toInt())
      {
        case KV_NORM_GRADE: color = Prefs::gradeCol(0); break;
        case KV_LEV1_GRADE: color = Prefs::gradeCol(1); break;
        case KV_LEV2_GRADE: color = Prefs::gradeCol(2); break;
        case KV_LEV3_GRADE: color = Prefs::gradeCol(3); break;
        case KV_LEV4_GRADE: color = Prefs::gradeCol(4); break;
        case KV_LEV5_GRADE: color = Prefs::gradeCol(5); break;
        case KV_LEV6_GRADE: color = Prefs::gradeCol(6); break;
        case KV_LEV7_GRADE: color = Prefs::gradeCol(7); break;
        default           : color = Prefs::gradeCol(1);
      }
    }
    else if (index.column() == KV_COL_ORG )
    {
      color = Prefs::gradeCol(0);
      QList<QVariant> result = index.model()->data(index, KVTTableModel::GradeRole).toList();

      if (m_currentIndex.column() > 2)
      {
        switch (result[m_currentIndex.column() - 3].toInt())
        {
          case KV_NORM_GRADE: color = Prefs::gradeCol(0); break;
          case KV_LEV1_GRADE: color = Prefs::gradeCol(1); break;
          case KV_LEV2_GRADE: color = Prefs::gradeCol(2); break;
          case KV_LEV3_GRADE: color = Prefs::gradeCol(3); break;
          case KV_LEV4_GRADE: color = Prefs::gradeCol(4); break;
          case KV_LEV5_GRADE: color = Prefs::gradeCol(5); break;
          case KV_LEV6_GRADE: color = Prefs::gradeCol(6); break;
          case KV_LEV7_GRADE: color = Prefs::gradeCol(7); break;
          default           : color = Prefs::gradeCol(1);
        }
      }
    }

    painter->setPen(color);
  }
  QItemDelegate::paint(painter, option, index);
}

void KVTTableDelegate::setCurrentIndex(const QModelIndex & index)
{
  m_currentIndex = index;
}

#include "kvttabledelegate.moc"
