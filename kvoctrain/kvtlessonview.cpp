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
#include <KMessageBox>
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

  m_lessonPopupMenu->addSeparator();

  KAction *actionCheckAllLessons = new KAction(i18n("Check all lessons"), this);
  m_lessonPopupMenu->addAction(actionCheckAllLessons);
  actionCheckAllLessons->setIcon(KIcon("edit-add"));  /// @todo better icon
  connect(actionCheckAllLessons, SIGNAL(triggered()), this, SLOT(slotCheckAllLessons()));

  KAction *actionCheckNoLessons = new KAction(i18n("Deselect all lessons"), this);
  m_lessonPopupMenu->addAction(actionCheckNoLessons);
  actionCheckNoLessons->setIcon(KIcon("edit-delete"));  /// @todo better icon
  connect(actionCheckNoLessons, SIGNAL(triggered()), this, SLOT(slotCheckNoLessons()));
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
  //reset(); // is this neccessary?

  /** m_doc starts counting lessons at 1 */
  int currentLesson = m_model->document()->currentLesson() ;
  slotSetCurrentLesson(currentLesson);
/*
  // if current lesson is not set in the document default to the first one. Because we do -1 this is 1.
  if (currentLesson <= 0)
    currentLesson = 1;

  QModelIndex indexOfCurrent = m_model->index(currentLesson-1, 0, QModelIndex());
  selectionModel()->select(indexOfCurrent, QItemSelectionModel::Select);

  /// @todo also set focus if possible - feels a little odd otherwise (dotted frame around other element)
  /// @todo the tablemodel also needs to update !
  emit currentChanged(indexOfCurrent, indexOfCurrent);
  */
}

void KVTLessonView::slotCheckAllLessons (){
  QList<int> intLessons;
  for(int lesson =1; lesson <= m_model->document()->lessonCount(); lesson++){
    intLessons.append(lesson);
  }
  m_model->document()->setLessonsInQuery(intLessons);
  reset();
}

void KVTLessonView::slotCheckNoLessons (){
  QList<int> intLessons;
  m_model->document()->setLessonsInQuery(intLessons);
  reset();
}

void KVTLessonView::slotCreateNewLesson(){
// this should move - hardly part of the view to create a new lesson?!?!
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
  slotSetCurrentLesson(newLessonIndex -1);
/*  QItemSelection mySelection;
  // -1 because of counting from 1 of m_doc
  QModelIndex indexOfCurrent = m_model->index(newLessonIndex -1, 0, QModelIndex());
  mySelection.select(indexOfCurrent, indexOfCurrent);
  selectionModel()->select(mySelection, QItemSelectionModel::ClearAndSelect);
*/
  QModelIndex indexOfCurrent = m_model->index(newLessonIndex -1, 0, QModelIndex());

  QList<int> intLessons;
  foreach(int lesson, m_model->document()->lessonsInQuery())
    intLessons.append(lesson);
  intLessons.append(newLessonIndex);
  m_model->document()->setLessonsInQuery(intLessons);

  //emit currentChanged(indexOfCurrent, indexOfCurrent);
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
  // can I update the main table here?
  //emit SOME SIGNAL HERE(indexes.at(0), indexes.at(0));
}

void KVTLessonView::slotDeleteLesson(){
  //(note to self) see also LessOptPage.cpp, same name - original implementation
  
  QModelIndexList indexes = selectionModel()->selectedIndexes();
  if (indexes.empty()) {
    kDebug() << "WARNING - NO SELECTION FOR ACTIVE LESSON! THIS SHOULD NOT HAPPEN!" << endl;
    return;
  }
  // Delete right away, if the lesson is empty, otherwise ask
  
  kDebug() << " Del: " << indexes.at(0).row() << " of " << m_model->document()->lessonDescriptions() << endl;
  
  if( m_model->document()->deleteLesson ( indexes.at(0).row(), KEduVocDocument::DeleteEmptyLesson ))
    return; // lesson was empty - done.
  int exit = KMessageBox::warningYesNo(this, i18n("There are vocabularies left in this lesson. Do you want to delete them? You will loose your entries! You have been warned!"));
  if(exit == KMessageBox::Yes){
     m_model->document()->deleteLesson ( indexes.at(0).row(), KEduVocDocument::DeleteEntriesAndLesson );
  }
    /// do I have to make a new selection?
}

void KVTLessonView::slotSetCurrentLesson(int currentIndex)
{
  // if current lesson is not set in the document default to the first one. Because we do -1 this is 1.
  if (currentIndex <= 0)
    currentIndex = 1;

  // select the lesson
  QItemSelection mySelection;
  // -1 because of counting from 1 of m_doc
  QModelIndex indexOfCurrent = m_model->index(currentIndex -1, 0, QModelIndex());
  mySelection.select(indexOfCurrent, indexOfCurrent);
  selectionModel()->select(mySelection, QItemSelectionModel::ClearAndSelect);

  //emit selectionChanged(indexOfCurrent, indexOfCurrent);
  emit signalCurrentLessonChanged(currentIndex);
  //kDebug() << " emit signalCurrentLessonChanged: " << currentIndex << endl;
}

void KVTLessonView::selectionChanged( const QItemSelection & selected, const QItemSelection & deselected )
{
  Q_UNUSED(deselected);
  /** we only have one lesson selectable at the time - start is enough */
  int index = selected.indexes().at(0).row() +1; // start counting from 1
  if (index > 0){
    m_model->document()->setCurrentLesson(index);
    m_model->document()->setModified(true);
  }
  slotSetCurrentLesson(index);
  reset();

  /// @todo emit to update combobox (query selected)
  emit lessonsInQueryChanged();
}

void KVTLessonView::contextMenuEvent(QContextMenuEvent * ev) {
  m_lessonPopupMenu->exec(ev->globalPos());
}

#include "kvtlessonview.moc"
