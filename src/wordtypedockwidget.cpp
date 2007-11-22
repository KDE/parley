/***************************************************************************

    Copyright: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "wordtypedockwidget.h"
#include "lessondockwidget.h"

#include "parley.h"
#include "vocabulary/lessonmodel.h"
#include "vocabulary/lessonview.h"

//#include "kvtsortfiltermodel.h"

#include "prefs.h"

#include <keduvoclesson.h>

#include <KLocalizedString>
#include <KDialog>
#include <KComboBox>
#include <KXmlGuiWindow>
#include <KActionCollection>
#include <QAction>
#include <QVBoxLayout>


WordTypeDockWidget::WordTypeDockWidget(ParleyApp *parent)
  : QDockWidget(i18n("Word Types"), parent)
{
  m_parleyApp = parent;
  m_wordTypeModel = 0;

    // Widget to get a boxLayout
  QWidget *left = new QWidget(this);
    // box layout for the left side
  QVBoxLayout *boxLayout = new QVBoxLayout(left);
  boxLayout->setMargin(0);
  boxLayout->setSpacing(KDialog::spacingHint());

    // This contains the lessons for now
  m_wordTypeView = new LessonView(left);
    // To make the treeview appear like a listview

//     m_wordTypeView->setRootIsDecorated(false);

    // I need to initialize the lessons with the model as well...
  m_wordTypeView->setModel(m_wordTypeModel);
  m_wordTypeView->setToolTip(i18n("Right click to add, delete, or rename lessons. \n"
      "With the checkboxes you can select which lessons you want to practice. \n"
      "Only checked lessons [x] will be asked in the tests!"));

//     Here the user selects whether he wants all lessons in the table, or the current one or the ones in query
  m_wordTypeSelectionCombo = new KComboBox();
  m_wordTypeSelectionCombo->addItem(i18n("Edit current word types"));
  m_wordTypeSelectionCombo->addItem(i18n("Edit word types in test"));
  m_wordTypeSelectionCombo->addItem(i18n("Edit all word types"));
  m_wordTypeSelectionCombo->setToolTip(i18n("Select which lessons should be displayed for editing to the right."));

  boxLayout->addWidget(m_wordTypeSelectionCombo);
  boxLayout->addWidget(m_wordTypeView);

//     connect(m_wordTypeView, SIGNAL(signalCurrentLessonChanged(int)), parent, SLOT(slotCurrentLessonChanged(int)));

   connect(parent->actionCollection()->action("new_lesson"), SIGNAL(triggered()), m_wordTypeView, SLOT(slotCreateNewLesson()));
   connect(parent->actionCollection()->action("rename_lesson"), SIGNAL(triggered()), m_wordTypeView, SLOT(slotRenameLesson()));
   connect(parent->actionCollection()->action("delete_lesson"), SIGNAL(triggered()), m_wordTypeView, SLOT(slotDeleteLesson()));
   connect(parent->actionCollection()->action("check_all_lessons"), SIGNAL(triggered()), m_wordTypeView, SLOT(slotCheckAllLessons()));
   connect(parent->actionCollection()->action("check_no_lessons"), SIGNAL(triggered()), m_wordTypeView, SLOT(slotCheckNoLessons()));
   connect(parent->actionCollection()->action("split_lesson"), SIGNAL(triggered()), m_wordTypeView, SLOT(slotSplitLesson()));

  m_wordTypeView->addAction(parent->actionCollection()->action("new_lesson"));
  m_wordTypeView->addAction(parent->actionCollection()->action("rename_lesson")); 
  m_wordTypeView->addAction(parent->actionCollection()->action("delete_lesson"));

  QAction* separator = new QAction(this);
  separator->setSeparator(true);
  m_wordTypeView->addAction(separator);
  m_wordTypeView->addAction(parent->actionCollection()->action("check_all_lessons")); m_wordTypeView->addAction(parent->actionCollection()->action("check_no_lessons"));
  separator = new QAction(this);
  separator->setSeparator(true);
  m_wordTypeView->addAction(separator);
  m_wordTypeView->addAction(parent->actionCollection()->action("split_lesson"));

  parent->actionCollection()->addAction("show_lesson_dock", toggleViewAction());

  setWidget(left);
}


WordTypeDockWidget::~WordTypeDockWidget()
{
}


void WordTypeDockWidget::setDocument(KEduVocDocument * doc)
{
  if ( m_wordTypeModel ) {
    m_wordTypeModel->deleteLater();
  }

  m_wordTypeModel = new LessonModel(this);
  m_wordTypeModel->setWordTypeDocument(doc);
    
  m_wordTypeView->setModel(m_wordTypeModel);

//     m_wordTypeView->initializeSelection();
//     connect(m_lessonModel, SIGNAL(modelReset()), m_wordTypeView, SLOT(slotModelReset()));

//     connect(m_lessonModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), m_parleyApp->m_sortFilterModel, SLOT(slotLessonsInQueryChanged()));
  // 
//     m_wordTypeSelectionCombo->setCurrentIndex(Prefs::lessonEditingSelection());
//     connect(m_wordTypeSelectionCombo, SIGNAL(currentIndexChanged(int)), m_parleyApp->m_sortFilterModel, SLOT(setLessonSelection(int)));
}





// int WordTypeDockWidget::addLesson()
// {
//     return m_lessonModel->addLesson();
// }

// void WordTypeDockWidget::selectLesson(int lessonIndex)
// {
//     m_wordTypeView->slotSelectLesson(lessonIndex);
// }


void WordTypeDockWidget::saveOptions()
{
  Prefs::setLessonEditingSelection(m_wordTypeSelectionCombo->currentIndex());
}

// void WordTypeDockWidget::makeLessonVisibleInTable(int lessonIndex)
// {
//     ///@todo
//     switch (m_wordTypeSelectionCombo->currentIndex()) {
//         case Prefs::EnumLessonEditingSelection::CurrentLesson:
//             m_wordTypeView->slotSelectLesson(lessonIndex);
//             break;
//         case Prefs::EnumLessonEditingSelection::LessonsInQuery:
//             kDebug() << "IMPLEMENT ME - makeLessonVisibleInTable";
// //             m_parleyApp->m_doc->setCurrentLesson(lessonIndex);
// //             if ( !m_parleyApp->m_doc->lesson(lessonIndex).inPractice() ) {
// //                 m_wordTypeSelectionCombo->setCurrentIndex(Prefs::EnumLessonEditingSelection::CurrentLesson);
// //             }
//             ///@todo m_sortFilterModel->clear();
//             break;
//         case Prefs::EnumLessonEditingSelection::AllLessons:
//             break;
//     }
// }

LessonView * WordTypeDockWidget::wordTypeView()
{
  return m_wordTypeView;
}

#include "wordtypedockwidget.moc"

