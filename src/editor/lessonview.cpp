/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lessonview.h"

#include <QAction>
#include <KActionCollection>
#include <QInputDialog>
#include <KLocalizedString>
#include <KMessageBox>

#include <KEduVocExpression>
#include <KEduVocLesson>

#include "editor/editor.h"
#include "prefs.h"

using namespace Editor;

LessonView::LessonView(EditorWindow * parent) : ContainerView(parent)
{
    QAction *actionNewLesson = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("new_lesson"), actionNewLesson);
    actionNewLesson->setText(i18n("New Unit"));
    actionNewLesson->setIcon(QIcon::fromTheme(QStringLiteral("lesson-add")));
    actionNewLesson->setWhatsThis(i18n("Add a new unit to your document"));
    actionNewLesson->setToolTip(actionNewLesson->whatsThis());
    actionNewLesson->setStatusTip(actionNewLesson->whatsThis());

    QAction *actionRenameLesson = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("rename_lesson"), actionRenameLesson);
    actionRenameLesson->setText(i18n("Rename Unit"));
    actionRenameLesson->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
//    actionRenameLesson->setWhatsThis(i18n("Rename the selected unit"));
    actionRenameLesson->setToolTip(actionRenameLesson->whatsThis());
    actionRenameLesson->setStatusTip(actionRenameLesson->whatsThis());

    QAction *actionDeleteLesson = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("delete_lesson"), actionDeleteLesson);
    actionDeleteLesson->setText(i18n("Delete Unit"));
    actionDeleteLesson->setIcon(QIcon::fromTheme(QStringLiteral("lesson-remove")));
    actionDeleteLesson->setWhatsThis(i18n("Delete the selected unit."));
    actionDeleteLesson->setToolTip(actionDeleteLesson->whatsThis());
    actionDeleteLesson->setStatusTip(actionDeleteLesson->whatsThis());

    QAction *actionSplitLesson = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("split_lesson"), actionSplitLesson);
    actionSplitLesson->setText(i18n("Split Unit into Smaller Units"));
    actionSplitLesson->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));  /// @todo better icon
    actionSplitLesson->setWhatsThis(i18n("Make multiple smaller units out of one big unit."));
    actionSplitLesson->setToolTip(actionSplitLesson->whatsThis());
    actionSplitLesson->setStatusTip(actionSplitLesson->whatsThis());

    QAction *actionRemoveGradesLesson = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("remove_grades_lesson"), actionRemoveGradesLesson);
    actionRemoveGradesLesson->setText(i18n("Remove Confidence Levels"));
    actionRemoveGradesLesson->setIcon(QIcon::fromTheme(QStringLiteral("edit-clear"))); /// @todo better icon
    actionRemoveGradesLesson->setWhatsThis(i18n("Remove confidence levels from this unit."));
    actionRemoveGradesLesson->setToolTip(actionRemoveGradesLesson->whatsThis());
    actionRemoveGradesLesson->setStatusTip(actionRemoveGradesLesson->whatsThis());

    QAction *actionRemoveGradesLessonChildren = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("remove_grades_lesson_children"), actionRemoveGradesLessonChildren);
    actionRemoveGradesLessonChildren->setText(i18n("Remove Confidence Levels From This Unit And All Subunits"));
    actionRemoveGradesLessonChildren->setIcon(QIcon::fromTheme(QStringLiteral("edit-clear")));
    actionRemoveGradesLessonChildren->setWhatsThis(i18n("Remove confidence levels from this unit and all subunits."));
    actionRemoveGradesLessonChildren->setToolTip(actionRemoveGradesLessonChildren->whatsThis());
    actionRemoveGradesLessonChildren->setStatusTip(actionRemoveGradesLessonChildren->whatsThis());

    QAction *actionExpandAll = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("expand_all_lesson_children"), actionExpandAll);
    actionExpandAll->setText(i18n("Expand Units and Subunits"));
    actionExpandAll->setIcon(QIcon::fromTheme(QStringLiteral("go-down-search")));
    actionExpandAll->setWhatsThis(i18n("Expand all units and subunits."));
    actionExpandAll->setToolTip(actionExpandAll->whatsThis());
    actionExpandAll->setStatusTip(actionExpandAll->whatsThis());

    QAction *actionCollapseAll = new QAction(this);
    parent->actionCollection()->addAction(QStringLiteral("collapse_all_lesson_children"), actionCollapseAll);
    actionCollapseAll->setText(i18n("Collapse All Units And Subunits"));
    actionCollapseAll->setIcon(QIcon::fromTheme(QStringLiteral("go-up-search")));
    actionCollapseAll->setWhatsThis(i18n("Collapse all units and subunits."));
    actionCollapseAll->setToolTip(actionCollapseAll->whatsThis());
    actionCollapseAll->setStatusTip(actionCollapseAll->whatsThis());

    connect(actionNewLesson, &QAction::triggered, this, &LessonView::slotCreateNewLesson);
    connect(actionRenameLesson, &QAction::triggered, this, &LessonView::slotRename);
    connect(actionDeleteLesson, &QAction::triggered, this, &LessonView::slotDeleteLesson);
    connect(actionSplitLesson, &QAction::triggered, this, &LessonView::slotSplitLesson);
    connect(actionRemoveGradesLesson, &QAction::triggered, this, &LessonView::slotRemoveGradesLesson);
    connect(actionRemoveGradesLessonChildren, &QAction::triggered, this, &LessonView::slotRemoveGradesLessonChildren);
    connect(actionExpandAll, &QAction::triggered, this, &LessonView::expandAllLesson);
    connect(actionCollapseAll, &QAction::triggered, this, &LessonView::collapseAllLesson);

    // right click menu for the lesson view:
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
    addAction(actionExpandAll);
    addAction(actionCollapseAll);
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
            KMessageBox::warningYesNo(this, i18np("There is %1 word left in this unit. Do you want to delete it?", "There are %1 words left in this unit. Do you want to delete them?", count)) == KMessageBox::Yes) {
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
    int numEntries = QInputDialog::getInt(this,  i18n("Entries per Unit"), i18n("The unit will be split into smaller unit. How many entries in each unit do you want?"), Prefs::entriesPerLesson(), 1, 1000, 1, &ok);
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
    connect(model, &QAbstractItemModel::columnsInserted, this, &LessonView::columnsInserted);
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

void LessonView::expandAllLesson()
{
    expandAll();
}

void LessonView::collapseAllLesson()
{
    collapseAll();
}
