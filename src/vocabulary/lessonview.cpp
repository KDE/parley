/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lessonview.h"

#include <KAction>
#include <KActionCollection>
#include <KInputDialog>
#include <KLocalizedString>
#include <KMessageBox>

#include <keduvocexpression.h>
#include <keduvoclesson.h>

#include "editor/editor.h"
#include "lessonmodel.h"
#include "prefs.h"

using namespace Editor;

LessonView::LessonView(EditorWindow * parent) : ContainerView(parent)
{
    KAction *actionNewLesson = new KAction(this);
    parent->actionCollection()->addAction("new_lesson", actionNewLesson);
    actionNewLesson->setText(i18n("New Lesson"));
    actionNewLesson->setIcon(KIcon("lesson-add"));
    actionNewLesson->setWhatsThis(i18n("Add a new lesson to your document"));
    actionNewLesson->setToolTip(actionNewLesson->whatsThis());
    actionNewLesson->setStatusTip(actionNewLesson->whatsThis());

    KAction *actionRenameLesson = new KAction(this);
    parent->actionCollection()->addAction("rename_lesson", actionRenameLesson);
    actionRenameLesson->setText(i18n("Rename Lesson"));
    actionRenameLesson->setIcon(KIcon("edit-rename"));
//    actionRenameLesson->setWhatsThis(i18n("Rename the selected lesson"));
    actionRenameLesson->setToolTip(actionRenameLesson->whatsThis());
    actionRenameLesson->setStatusTip(actionRenameLesson->whatsThis());

    KAction *actionDeleteLesson = new KAction(this);
    parent->actionCollection()->addAction("delete_lesson", actionDeleteLesson);
    actionDeleteLesson->setText(i18n("Delete Lesson"));
    actionDeleteLesson->setIcon(KIcon("lesson-remove"));
    actionDeleteLesson->setWhatsThis(i18n("Delete the selected lesson."));
    actionDeleteLesson->setToolTip(actionDeleteLesson->whatsThis());
    actionDeleteLesson->setStatusTip(actionDeleteLesson->whatsThis());

    KAction *actionSplitLesson = new KAction(this);
    parent->actionCollection()->addAction("split_lesson", actionSplitLesson);
    actionSplitLesson->setText(i18n("Split Lesson into Smaller Lessons"));
    actionSplitLesson->setIcon(KIcon("edit-copy"));  /// @todo better icon
    actionSplitLesson->setWhatsThis(i18n("Make multiple smaller lessons out of one big lesson."));
    actionSplitLesson->setToolTip(actionSplitLesson->whatsThis());
    actionSplitLesson->setStatusTip(actionSplitLesson->whatsThis());

    KAction *actionRemoveGradesLesson = new KAction(this);
    parent->actionCollection()->addAction("remove_grades_lesson", actionRemoveGradesLesson);
    actionRemoveGradesLesson->setText(i18n("Remove Grades from this Lesson"));
    actionRemoveGradesLesson->setIcon(KIcon("edit-clear")); /// @todo better icon
    actionRemoveGradesLesson->setWhatsThis(i18n("Remove grades from this lesson."));
    actionRemoveGradesLesson->setToolTip(actionRemoveGradesLesson->whatsThis());
    actionRemoveGradesLesson->setStatusTip(actionRemoveGradesLesson->whatsThis());

    KAction *actionRemoveGradesLessonChildren = new KAction(this);
    parent->actionCollection()->addAction("remove_grades_lesson_children", actionRemoveGradesLessonChildren);
    actionRemoveGradesLessonChildren->setText(i18n("Remove Grades from this Lesson and all Sub-Lessons"));
    actionRemoveGradesLessonChildren->setIcon(KIcon("edit-clear"));
    actionRemoveGradesLessonChildren->setWhatsThis(i18n("Remove grades from this lesson and all sub-lessons."));
    actionRemoveGradesLessonChildren->setToolTip(actionRemoveGradesLessonChildren->whatsThis());
    actionRemoveGradesLessonChildren->setStatusTip(actionRemoveGradesLessonChildren->whatsThis());

    connect(actionNewLesson, SIGNAL(triggered()),
            SLOT(slotCreateNewLesson()));
    connect(actionRenameLesson, SIGNAL(triggered()),
            SLOT(slotRename()));
    connect(actionDeleteLesson, SIGNAL(triggered()),
            SLOT(slotDeleteLesson()));
    connect(actionSplitLesson, SIGNAL(triggered()),
            SLOT(slotSplitLesson()));
    connect(actionRemoveGradesLesson, SIGNAL(triggered()),
            SLOT(slotRemoveGradesLesson()));
    connect(actionRemoveGradesLessonChildren, SIGNAL(triggered()),
            SLOT(slotRemoveGradesLessonChildren()));

    // right cick menu for the lesson view:
    addAction(actionNewLesson);
    addAction(actionRenameLesson);
    addAction(actionDeleteLesson);
    QAction* separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    addAction(actionRemoveGradesLesson);
    addAction(actionRemoveGradesLessonChildren);
    separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);
    addAction(actionSplitLesson);
}

void LessonView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QTreeView::currentChanged(current, previous);

    if (current.isValid()) {
        KEduVocLesson *container = static_cast<KEduVocLesson*>(current.internalPointer());
        if (container) {
            emit selectedLessonChanged(container);
            emit signalShowContainer(container);
        }
    }
}

void LessonView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    QTreeView::selectionChanged(selected, deselected);

    if (selected.count() == 0) {
        return;
    }

    KEduVocLesson *container = static_cast<KEduVocLesson*>(selected.indexes().value(0).internalPointer());
    if (container) {
        emit selectedLessonChanged(container);
    }
}

void LessonView::setTranslation(KEduVocExpression * entry, int translation)
{
    Q_UNUSED(translation)

    if (entry == 0) {
        selectionModel()->clearSelection();
        return;
    }

    QModelIndex index(m_model->index(entry->lesson()));
    selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    scrollTo(index);
}

void LessonView::slotCreateNewLesson()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (!selectedIndex.isValid()) {
        selectedIndex = m_model->index(0, 0, QModelIndex());
    }

    QModelIndex modelIndex = m_model->appendContainer(selectedIndex);

    scrollTo(modelIndex);
    selectionModel()->setCurrentIndex(modelIndex, QItemSelectionModel::ClearAndSelect);
    edit(modelIndex);    // let the user type a new name for the lesson
}

void LessonView::slotDeleteLesson()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (selectedIndex.parent() == QModelIndex()) {
        KMessageBox::information(this, i18n("The root lesson cannot be deleted."));
        return;
    }

    KEduVocLesson* lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());

    int count = lesson->entryCount(KEduVocLesson::Recursive);

    if (count == 0 ||
            KMessageBox::warningYesNo(this, i18np("There is %1 word left in this lesson. Do you want to delete it?", "There are %1 words left in this lesson. Do you want to delete them?", count)) == KMessageBox::Yes) {
        m_model->deleteContainer(selectedIndex);
    }
}

void LessonView::slotSplitLesson()
{
    if (!selectionModel()->currentIndex().isValid()) {
        return;
    }

    /** @todo A nicer dialog would be great.
     * Maybe with radio buttons to ask, if the entries should be in random order or as they come. */
    bool ok = false;
    int numEntries = KInputDialog::getInteger(i18n("Entries per Lesson"), i18n("The lesson will be split into smaller lessons. How many entries in each lesson do you want?"), Prefs::entriesPerLesson(), 1, 1000, 1, &ok, this);
    if (!ok) {
        return;
    }
    Prefs::setEntriesPerLesson(numEntries);
    m_model->splitLesson(selectionModel()->currentIndex(), numEntries, LessonModel::Random);
    setExpanded(selectionModel()->currentIndex(), true);
}

void LessonView::slotRemoveGradesLesson()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    KEduVocLesson* lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());
    lesson->resetGrades(-1, KEduVocContainer::NotRecursive);
    emit signalShowContainer(lesson);
}

void LessonView::slotRemoveGradesLessonChildren()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();
    KEduVocLesson* lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());
    lesson->resetGrades(-1, KEduVocContainer::Recursive);
    emit signalShowContainer(lesson);
}

void LessonView::setModel(LessonModel * model)
{
    m_model = model;
    ContainerView::setModel(model);
    connect(model, SIGNAL(columnsInserted(const QModelIndex &, int, int)), this, SLOT(columnsInserted()));
    for (int i = 2; i < model->columnCount(QModelIndex()); i++) {
        setColumnHidden(i, true);
    }
}

void LessonView::columnsInserted()
{
    for (int i = 2; i < m_model->columnCount(QModelIndex()); i++) {
        setColumnHidden(i, true);
    }
}



#include "lessonview.moc"


