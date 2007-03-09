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
  //initializeSelection();
}

void KVTLessonView::slotModelReset()
{
  initializeSelection();
}

void KVTLessonView::initializeSelection()
{
  reset();

  /** m_doc starts counting lessons at 1 */
  int currentLesson = m_model->document()->currentLesson() ;

  // if current lesson is not set in the document default to the first one. Because we do -1 this is 1.
  if (currentLesson <= 0)
    currentLesson = 1;

  QModelIndex indexOfCurrent = m_model->index(currentLesson-1, 0, QModelIndex());
  this->selectionModel()->select(indexOfCurrent, QItemSelectionModel::Select);

  /// @todo also set focus if possible - feels a little odd otherwise (dotted frame around other element)
  /// @todo the tablemodel also needs to update !
  emit currentChanged(indexOfCurrent, indexOfCurrent);
}

void KVTLessonView::slotCreateNewLesson(){
/// @todo this should move - hardly part of the view to create a new lesson?!?!
  int i = 1;

  while ( m_model->document()->lessonIndex(i18n("New lesson") + QString(" %1").arg(i)) > 0 )
    i++;

  QStringList list = m_model->document()->lessonDescriptions();
  list.append(i18n("New lesson") + QString(" %1").arg(i));

  m_model->document()->setLessonDescriptions(list);
  m_model->document()->setModified();
  reset(); // maybe better just begin insert rows?

  int newLessonIndex = m_model->document()->lessonIndex(i18n("New lesson") + QString(" %1").arg(i));

  // select the new lesson
  QItemSelection mySelection;
  // -1 because of counting from 1 of m_doc
  QModelIndex indexOfCurrent = m_model->index(newLessonIndex -1, 0, QModelIndex());
  mySelection.select(indexOfCurrent, indexOfCurrent);
  QItemSelectionModel *selectionModel = this->selectionModel();
  selectionModel->select(mySelection, QItemSelectionModel::ClearAndSelect);

  emit currentChanged(indexOfCurrent, indexOfCurrent);
  edit ( indexOfCurrent ); // let the user type a new name for the lesson
}

#include "kvtlessonview.moc"
