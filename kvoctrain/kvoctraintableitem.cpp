/***************************************************************************

                      table item for the main view

    -----------------------------------------------------------------------

    begin                : Mon Dec 27 17:05:53 PST 2004

    copyright            : (C) 2004-2005 Peter Hedlund

    email                : peter.hedlund@kdemail.net

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

#include <QLineEdit>
#include <QComboBox>

#include <klocale.h>
#include <kglobalsettings.h>

#include "kvoctraintableitem.h"


KVocTrainTableItem::KVocTrainTableItem( Q3Table *t, EditType et, kvoctrainDoc *doc)
  : Q3TableItem( t, et, QString::null )
{
  m_doc = doc;
  // we do not want that this item can be replaced
  setReplaceable(false);
}


void KVocTrainTableItem::setPosition(int curr_row, int curr_col)
{
  setRow(curr_row);
  setCol(curr_col);
}


void KVocTrainTableItem::setDoc(kvoctrainDoc *doc)
{
  m_doc = doc;
}


QWidget *KVocTrainTableItem::createEditor() const
{
  if (m_doc != 0 && m_doc->numEntries() != 0 && row() >= 0 && col() >= 0) {
    switch (col()) {
      case KV_COL_LESS: {
        QComboBox *lessonbox = new QComboBox(table()->viewport() );
        lessonbox->setFont(KGlobalSettings::generalFont());
        lessonbox->insertItem (m_doc->getLessonDescr(0));
        for (unsigned i = 1; i <= (unsigned) m_doc->numLessons(); ++i)
          lessonbox->insertItem (m_doc->getLessonDescr(i));
        lessonbox->setCurrentItem(m_doc->getEntry(row())->getLesson());
        return lessonbox;
      }
      break;

      case KV_COL_MARK: {
        QComboBox *statebox = new QComboBox(table()->viewport() );
        statebox->setFont(KGlobalSettings::generalFont());
        statebox->insertItem (i18n("state of a row", "Active, Not in Query"));
        statebox->insertItem (i18n("state of a row", "In Query"));
        statebox->insertItem (i18n("state of a row", "Inactive"));
        QSize sz = statebox->sizeHint();
        sz.setHeight(table()->rowHeight(row()));
        statebox->setMinimumSize(sz);
        if (!m_doc->getEntry(row())->isActive() )
          statebox->setCurrentItem(2);
        else if (m_doc->getEntry(row())->isInQuery() )
          statebox->setCurrentItem(1);
        else
          statebox->setCurrentItem(0);
        return statebox;
      }
      break;

      default: {
        QLineEdit *edit = new QLineEdit(table()->viewport() );
        edit->setFrame(false);
        if (col() == KV_COL_ORG)
          edit->setText(m_doc->getEntry(row())->getOriginal());
        else
          edit->setText(m_doc->getEntry(row())->getTranslation(col()-KV_COL_ORG));
        return edit;
      }
    }
  }
  return 0;
}


void KVocTrainTableItem::setContentFromEditor( QWidget *w )
{
  if (m_doc != 0) {
    if ( w->inherits( "QComboBox" ) ) {
      if (col() == KV_COL_MARK) {
        QComboBox *statebox = (QComboBox*) w;
        kvoctrainExpr *expr = m_doc->getEntry(row());
        bool inq = false;
        bool act = true;
        if (statebox->currentItem() == 0) {
          inq = false;
          act = true;
        }
        else if (statebox->currentItem() == 1) {
          inq = true;
          act = true;
        }
        else if (statebox->currentItem() == 2) {
          inq = false;
          act = false;
        }
        if (inq != expr->isInQuery() ||
            act != expr->isActive() )
          m_doc->setModified();
        expr->setInQuery(inq);
        expr->setActive(act);
      }
      else if (col() == KV_COL_LESS) {
        QComboBox *lessonbox = (QComboBox*) w;
        if (m_doc->getEntry(row())->getLesson() != lessonbox->currentItem())
          m_doc->setModified();
        m_doc->getEntry(row())->setLesson(lessonbox->currentItem());
      }
    }
    else {
      QLineEdit *edit = (QLineEdit*) w;
      if (col() == KV_COL_ORG) {
        if (m_doc->getEntry(row())->getOriginal() != edit->text())
          m_doc->setModified();
        m_doc->getEntry(row())->setOriginal(edit->text());
      }
      else {
        if (m_doc->getEntry(row())->getTranslation(col()-KV_COL_ORG) != edit->text())
          m_doc->setModified();
        m_doc->getEntry(row())->setTranslation(col()-KV_COL_ORG, (edit->text()));
      }
    }
  }
}
