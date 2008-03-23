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

#include "synonymwidget.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <KDebug>

#include <QStringListModel>
#include <QDragEnterEvent>

SynonymWidget::SynonymWidget(SynonymWidgetType type, QWidget *parent) : QWidget(parent)
{
    m_type = type;
    m_currentTranslation = 0;
    m_newTranslation = 0;
    setupUi(this);

    connect(translationLineEdit, SIGNAL(), SLOT());
    connect(setTranslationButton, SIGNAL(clicked()), SLOT(makeTranslationCurrent()));
    connect(listView, SIGNAL(), SLOT());
    connect(addToListButton, SIGNAL(clicked()), SLOT(addToList()));
    connect(removeFromListButton, SIGNAL(clicked()), SLOT(removeFromList()));

    m_listModel = new QStringListModel(this);     listView->setModel(m_listModel);

//     connect(m_listModel, SIGNAL(dataChanged ( const QModelIndex &, const QModelIndex & )), SLOT(slotDataChanged( const QModelIndex &, const QModelIndex & )));

    listView->setAcceptDrops(true);
    listView->installEventFilter(this);

    setEnabled(false);
}


void SynonymWidget::slotDataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
//     m_translation->multipleChoice() = m_choicesModel->stringList();
//     removeChoiceButton->setEnabled(m_translation && m_translation->multipleChoice().count() > 0);
}


void SynonymWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    m_newTranslation = entry->translation(translation);

    if (m_newTranslation) {
        setTranslationButton->setEnabled(true);
    }

    if (m_newTranslation || m_currentTranslation) {
        setEnabled(true);
    }
    addToListButton->setEnabled(m_newTranslation && m_currentTranslation);

    setTranslationButton->setText(entry->translation(translation)->text());

    if (m_currentTranslation) {
        addToListButton->setText(i18n("Add \"%1\" as Synonym", entry->translation(translation)->text()));
    }

//     if (entry) {
//         m_translation = entry->translation(translation);
//     } else {
//         m_translation = 0;
//     }
// 
//     if (m_translation) {
//         setEnabled(true);
//         m_choicesModel->setStringList(m_translation->multipleChoice());
//         removeChoiceButton->setEnabled(m_translation->multipleChoice().count() > 0);
//     } else {
//         setEnabled(false);
//     }
//     removeChoiceButton->setEnabled(m_translation && m_translation->multipleChoice().count() > 0);
}


void SynonymWidget::slotAddChoiceButton()
{
//     m_choicesModel->insertRow(m_choicesModel->rowCount());
//     QModelIndex index(m_choicesModel->index(m_choicesModel->rowCount() - 1));
//     m_choicesModel->setData(index, "");
//     multipleChoiceListView->scrollTo(index);
//     multipleChoiceListView->setCurrentIndex(index);
//     multipleChoiceListView->edit(index);
}


void SynonymWidget::slotRemoveChoiceButton()
{
//     QModelIndex index = multipleChoiceListView->selectionModel()->currentIndex();
//     if (index.isValid()) {
//         m_choicesModel->removeRows(index.row(), 1, QModelIndex());
//     } else {
//         m_choicesModel->removeRows(m_choicesModel->rowCount(QModelIndex()) - 1, 1, QModelIndex());
//     }
//     m_translation->multipleChoice() = m_choicesModel->stringList();
//     removeChoiceButton->setEnabled(m_translation && m_translation->multipleChoice().count() > 0);
}


bool SynonymWidget::eventFilter(QObject * obj, QEvent * event)
{
//     if (obj == multipleChoiceListView) {
//         if (event->type() == QEvent::DragEnter) {
//             QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>(event);
//             kDebug() << "DragEnter mime format: " << dragEnterEvent->format();
//             if (dragEnterEvent->mimeData()->hasText()) {
//                 event->accept();
//             }
//             return true;
//         }
// 
//         if (event->type() == QEvent::DragMove) {
//             event->accept();
//             return true;
//         }
// 
//         if (event->type() == QEvent::Drop) {
//             QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
//             kDebug() << "You dropped onto me: " << dropEvent->mimeData()->text();
// 
//             QStringList choices = dropEvent->mimeData()->text().split('\n');
//             foreach(const QString &choice, choices) {
//                 m_choicesModel->insertRow(multipleChoiceListView->model()->rowCount());
//                 m_choicesModel->setData(m_choicesModel->index(multipleChoiceListView->model()->rowCount()-1), choice);
//             }
//             return true;
//         }
//     }
    return QObject::eventFilter(obj, event);
}

void SynonymWidget::makeTranslationCurrent()
{
    m_currentTranslation = m_newTranslation;
    setTranslationButton->setEnabled(false);
    translationLineEdit->setText(m_currentTranslation->text());

    // load list of old synonyms
    m_listModel->removeRows(0, m_listModel->rowCount()-1);
    QList< KEduVocTranslation* > list;
    switch(m_type) {
        case Synonym:
            list = m_currentTranslation->synonyms();
            break;
        case Antonym:
            list = m_currentTranslation->antonyms();
            break;
        case FalseFriend:
            list = m_currentTranslation->falseFriends();
            break;
    }
    foreach (KEduVocTranslation* translation, list) {
        int row = m_listModel->rowCount();
        m_listModel->insertRow(row);
        m_listModel->setData(m_listModel->index(row), translation->text());
    }
}

void SynonymWidget::addToList()
{
    m_currentTranslation->addSynonym(m_newTranslation);

    int row = m_listModel->rowCount();
    m_listModel->insertRow(row);
    m_listModel->setData(m_listModel->index(row), m_newTranslation->text());
}

void SynonymWidget::removeFromList()
{
    
}


#include "synonymwidget.moc"

