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

#include "leitnerview.h"

#include "parleymainwindow.h"
#include "leitnermodel.h"
#include "keduvocleitnerbox.h"
#include "prefs.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KInputDialog>
#include <KAction>
#include <KActionCollection>

LeitnerView::LeitnerView(ParleyMainWindow * parent) :QListView(parent)
{
/*
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
    actionRenameLesson->setWhatsThis(i18n("Rename the selected lesson"));
    actionRenameLesson->setToolTip(actionRenameLesson->whatsThis());
    actionRenameLesson->setStatusTip(actionRenameLesson->whatsThis());

    KAction *actionDeleteLesson = new KAction(this);
    parent->actionCollection()->addAction("delete_lesson", actionDeleteLesson);
    actionDeleteLesson->setText(i18n("Delete Lesson"));
    actionDeleteLesson->setIcon(KIcon("lesson-remove"));
    actionDeleteLesson->setWhatsThis(i18n("Delete the selected lesson."));
    actionDeleteLesson->setToolTip(actionDeleteLesson->whatsThis());
    actionDeleteLesson->setStatusTip(actionDeleteLesson->whatsThis());


    connect(actionNewLesson, SIGNAL(triggered()),
            SLOT(slotCreateNewLesson()));
    connect(actionRenameLesson, SIGNAL(triggered()),
            SLOT(slotRename()));
    connect(actionDeleteLesson, SIGNAL(triggered()),
            SLOT(slotDeleteLesson()));

    // right cick menu for the lesson view:
    addAction(actionNewLesson);
    addAction(actionRenameLesson);
    addAction(actionDeleteLesson);
*/
}

void LeitnerView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
    QListView::currentChanged(current, previous);

    if (current.isValid()) {
        KEduVocLeitnerBox *container = static_cast<KEduVocLeitnerBox*>(current.internalPointer());
        if (container) {
            emit selectedLeitnerBoxChanged(container);
            emit signalShowContainer(container);
        }
    }
}

void LeitnerView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    QListView::selectionChanged(selected, deselected);

    if(selected.count() == 0) {
        return;
    }

    KEduVocLeitnerBox *container = static_cast<KEduVocLeitnerBox*>(selected.indexes().value(0).internalPointer());
    if (container) {
        emit selectedLeitnerBoxChanged(container);
    }
}

void LeitnerView::slotCreateLeitnerBox()
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

void LeitnerView::slotDeleteLeitnerBox()
{
    QModelIndex selectedIndex = selectionModel()->currentIndex();

    if (selectedIndex.parent() == QModelIndex()) {
        KMessageBox::information(this, i18n("The root lesson cannot be deleted."));
        return;
    }

    KEduVocLesson* lesson = static_cast<KEduVocLesson*>(selectedIndex.internalPointer());

    int count = lesson->entryCount(KEduVocLesson::Recursive);

    if ( count == 0 ||
         KMessageBox::warningYesNo(this, i18np("There is %1 word left in this lesson. Do you want to delete them?", "There are %1 words left in this lesson. Do you want to delete them?", count)) == KMessageBox::Yes) {
        m_model->deleteContainer(selectedIndex);
    }
}

void LeitnerView::setModel(LeitnerModel * model)
{
    QListView::setModel(model);
    m_model = model;
    setRootIndex(model->index(0,0, model->index(0,0, QModelIndex())));
}

#include "leitnerview.moc"


