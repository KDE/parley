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


LessonDockWidget::LessonDockWidget(ParleyApp *parent)
 : QDockWidget(i18n("Lessons"), parent)
{
    m_parleyApp = parent;
    m_lessonModel = 0;

    // Widget to get a boxLayout
    QWidget *left = new QWidget(this);
    // box layout for the left side
    QVBoxLayout *boxLayout = new QVBoxLayout(left);
    boxLayout->setMargin(0);
    boxLayout->setSpacing(KDialog::spacingHint());

    // This contains the lessons for now
    m_lessonView = new LessonView(left);
    // To make the treeview appear like a listview

//     m_lessonView->setRootIsDecorated(false);

    // I need to initialize the lessons with the model as well...
    m_lessonView->setModel(m_lessonModel);
    m_lessonView->setToolTip(i18n("Right click to add, delete, or rename lessons. \n"
            "With the checkboxes you can select which lessons you want to practice. \n"
            "Only checked lessons [x] will be asked in the tests!"));

    // Here the user selects whether he wants all lessons in the table, or the current one or the ones in query
    m_lessonSelectionCombo = new KComboBox();
    m_lessonSelectionCombo->addItem(i18n("Edit current lesson"));
    m_lessonSelectionCombo->addItem(i18n("Edit lessons in test"));
    m_lessonSelectionCombo->addItem(i18n("Edit all lessons"));
    m_lessonSelectionCombo->setToolTip(i18n("Select which lessons should be displayed for editing to the right."));

    boxLayout->addWidget(m_lessonSelectionCombo);
    boxLayout->addWidget(m_lessonView);

//     connect(m_lessonView, SIGNAL(signalCurrentLessonChanged(int)), parent, SLOT(slotCurrentLessonChanged(int)));

    connect(parent->actionCollection()->action("new_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotCreateNewLesson()));
    connect(parent->actionCollection()->action("rename_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotRenameLesson()));
    connect(parent->actionCollection()->action("delete_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotDeleteLesson()));
    connect(parent->actionCollection()->action("check_all_lessons"), SIGNAL(triggered()), m_lessonView, SLOT(slotCheckAllLessons()));
    connect(parent->actionCollection()->action("check_no_lessons"), SIGNAL(triggered()), m_lessonView, SLOT(slotCheckNoLessons()));
    connect(parent->actionCollection()->action("split_lesson"), SIGNAL(triggered()), m_lessonView, SLOT(slotSplitLesson()));

    m_lessonView->addAction(parent->actionCollection()->action("new_lesson"));
    m_lessonView->addAction(parent->actionCollection()->action("rename_lesson"));  m_lessonView->addAction(parent->actionCollection()->action("delete_lesson"));
    /// @todo add a separator here
    m_lessonView->addAction(parent->actionCollection()->action("check_all_lessons")); m_lessonView->addAction(parent->actionCollection()->action("check_no_lessons"));
    /// @todo add a separator here
    m_lessonView->addAction(parent->actionCollection()->action("split_lesson"));

    setWidget(left);
}


LessonDockWidget::~LessonDockWidget()
{
}


void LessonDockWidget::setDocument(KEduVocDocument * doc)
{
    if ( m_lessonModel ) {
        m_lessonModel->deleteLater();
    }

    m_lessonModel = new LessonModel(this);
    m_lessonModel->setDocument(doc);
    
    m_lessonView->setModel(m_lessonModel);

//     m_lessonView->initializeSelection();
//     connect(m_lessonModel, SIGNAL(modelReset()), m_lessonView, SLOT(slotModelReset()));

//     connect(m_lessonModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), m_parleyApp->m_sortFilterModel, SLOT(slotLessonsInQueryChanged()));
// 
//     m_lessonSelectionCombo->setCurrentIndex(Prefs::lessonEditingSelection());
//     connect(m_lessonSelectionCombo, SIGNAL(currentIndexChanged(int)), m_parleyApp->m_sortFilterModel, SLOT(setLessonSelection(int)));
}





// int LessonDockWidget::addLesson()
// {
//     return m_lessonModel->addLesson();
// }

// void LessonDockWidget::selectLesson(int lessonIndex)
// {
//     m_lessonView->slotSelectLesson(lessonIndex);
// }


void LessonDockWidget::saveOptions()
{
    Prefs::setLessonEditingSelection(m_lessonSelectionCombo->currentIndex());
}

void LessonDockWidget::makeLessonVisibleInTable(int lessonIndex)
{
    ///@todo
    switch (m_lessonSelectionCombo->currentIndex()) {
        case Prefs::EnumLessonEditingSelection::CurrentLesson:
            m_lessonView->slotSelectLesson(lessonIndex);
            break;
        case Prefs::EnumLessonEditingSelection::LessonsInQuery:
            kDebug() << "IMPLEMENT ME - makeLessonVisibleInTable";
//             m_parleyApp->m_doc->setCurrentLesson(lessonIndex);
//             if ( !m_parleyApp->m_doc->lesson(lessonIndex).inPractice() ) {
//                 m_lessonSelectionCombo->setCurrentIndex(Prefs::EnumLessonEditingSelection::CurrentLesson);
//             }
            ///@todo m_sortFilterModel->clear();
            break;
        case Prefs::EnumLessonEditingSelection::AllLessons:
            break;
    }
}

LessonView * LessonDockWidget::lessonView()
{
    return m_lessonView;
}

#include "lessondockwidget.moc"

