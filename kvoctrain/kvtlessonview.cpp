//
// C++ Implementation: kvtlessonview
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <kdebug.h>

#include <keduvocdocument.h>

#include "kvtlessonmodel.h"
#include "kvtlessonview.h"

KVTLessonView::KVTLessonView(QWidget *parent) : QTreeView(parent)
{
  
}


void KVTLessonView::setModel(KVTLessonModel *model)
{
  QTreeView::setModel(model);
  
  /** TODO this has to go into the VIEW! */
  int currentLesson = model->m_doc->currentLesson();
  kDebug() << "current: " << currentLesson << endl;

  QModelIndex indexOfCurrent;
  QItemSelection mySelection;

  indexOfCurrent = model->index(currentLesson, 0, QModelIndex());

  mySelection.select(indexOfCurrent, indexOfCurrent);

  QItemSelectionModel *selectionModel = this->selectionModel();
  selectionModel->select(mySelection, QItemSelectionModel::Select);

  //slotCurrentChanged(this->currentIndex(), this->currentIndex());
}

#include "kvtlessonview.moc"
