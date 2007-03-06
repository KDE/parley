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
#include <klocale.h>

#include <keduvocdocument.h>

#include "kvtlessonmodel.h"
#include "kvtlessonview.h"

KVTLessonView::KVTLessonView(QWidget *parent) : QTreeView(parent)
{}

void KVTLessonView::setModel(KVTLessonModel *model)
{
  QTreeView::setModel(model);
  m_model = model;
  
  /** m_doc starts counting lessons at 1 */
  int currentLesson = model->m_doc->currentLesson() -1; 

  QModelIndex indexOfCurrent;
  QItemSelection mySelection;

  indexOfCurrent = model->index(currentLesson, 0, QModelIndex());

  mySelection.select(indexOfCurrent, indexOfCurrent);

  QItemSelectionModel *selectionModel = this->selectionModel();
  selectionModel->select(mySelection, QItemSelectionModel::Select);

  emit currentChanged(indexOfCurrent, indexOfCurrent);
}

void KVTLessonView::slotCreateNewLesson(){
  int i = 1;

  while ( m_model->m_doc->lessonIndex(i18n("New lesson") + QString(" %1").arg(i)) > 0 )
    i++;

  QStringList list = m_model->m_doc->lessonDescriptions();
  list.append(i18n("New lesson") + QString(" %1").arg(i));

  m_model->m_doc->setLessonDescriptions(list);
  m_model->m_doc->setModified();
  reset();

  int newLessonIndex = m_model->m_doc->lessonIndex(i18n("New lesson") + QString(" %1").arg(i));

  // select it and set the user to enter the real name!!!

  QModelIndex indexOfCurrent;
  QItemSelection mySelection;

  // -1 because of counting from 1 of m_doc
  indexOfCurrent = m_model->index(newLessonIndex -1, 0, QModelIndex());

  mySelection.select(indexOfCurrent, indexOfCurrent);

  QItemSelectionModel *selectionModel = this->selectionModel();
  selectionModel->select(mySelection, QItemSelectionModel::ClearAndSelect);

  emit currentChanged(indexOfCurrent, indexOfCurrent);
  
  
  edit ( indexOfCurrent );
}

#include "kvtlessonview.moc"
