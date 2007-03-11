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

#include <QTreeView>
#include <QMenu>
#include <QContextMenuEvent>
#include <KAction>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kicon.h>

#include <keduvocdocument.h>

#include "kvtlessonmodel.h"
#include "kvtlessonview.h"

KVTLessonView::KVTLessonView(QWidget *parent) : QTreeView(parent)
{
  m_lessonPopupMenu = new QMenu(this);
  KAction *actionNewLesson = new KAction(i18n("New lesson"), this);
  m_lessonPopupMenu->addAction(actionNewLesson);
  actionNewLesson->setIcon(KIcon("edit-add"));
  connect(actionNewLesson, SIGNAL(triggered()), this, SLOT(slotCreateNewLesson()));
  
  KAction *actionRenameLesson = new KAction(i18n("Rename lesson"), this);
  m_lessonPopupMenu->addAction(actionRenameLesson);
  actionRenameLesson->setIcon(KIcon("edit"));
  connect(actionRenameLesson, SIGNAL(triggered()), this, SLOT(slotRenameLesson()));
  
  KAction *actionDeleteLesson = new KAction(i18n("Delete lesson"), this);
  m_lessonPopupMenu->addAction(actionDeleteLesson);
  actionDeleteLesson->setIcon(KIcon("edit-delete"));
  connect(actionDeleteLesson, SIGNAL(triggered()), this, SLOT(slotDeleteLesson()));
}

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
  //QItemSelectionModel *selectionModel = this->selectionModel();
  selectionModel()->select(mySelection, QItemSelectionModel::ClearAndSelect);

  QList<int> intLessons;
  foreach(int lesson, m_model->document()->lessonsInQuery())
    intLessons.append(lesson);
  intLessons.append(newLessonIndex);
  m_model->document()->setLessonsInQuery(intLessons);

  emit currentChanged(indexOfCurrent, indexOfCurrent);
  edit ( indexOfCurrent ); // let the user type a new name for the lesson

  m_model->document()->setModified();
}


void KVTLessonView::slotRenameLesson(){
  QModelIndexList indexes = selectionModel()->selectedIndexes();
  // oops - this crashes if there is no selection - there should always be a current lesson!!!
  if (indexes.empty()) {
    kDebug() << "WARNING - NO SELECTION FOR ACTIVE LESSON! THIS SHOULD NOT HAPPEN!" << endl;
    return;
  }
  edit ( indexes.at(0) ); // let the user type a new name for the lesson
}
/*7
void KVTLessonView::slotRenameLesson(const QModelIndex & index ){
}
*/

void KVTLessonView::contextMenuEvent(QContextMenuEvent * ev) {
  m_lessonPopupMenu->exec(mapToGlobal(ev->pos()));
}

#include "kvtlessonview.moc"
