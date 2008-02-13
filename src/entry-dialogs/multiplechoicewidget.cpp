/***************************************************************************

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "multiplechoicewidget.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <KDebug>

#include <QStringListModel>
#include <QDragEnterEvent>

MultipleChoiceWidget::MultipleChoiceWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(addChoiceButton, SIGNAL(clicked()), SLOT(slotAddChoiceButton()));
    connect(removeChoiceButton, SIGNAL(clicked()), SLOT(slotRemoveChoiceButton()));

    m_choicesModel = new QStringListModel(this);
    multipleChoiceListView->setModel(m_choicesModel);

    connect(m_choicesModel, SIGNAL(dataChanged ( const QModelIndex &, const QModelIndex & )), SLOT(slotDataChanged( const QModelIndex &, const QModelIndex & )));

    multipleChoiceListView->setAcceptDrops(true);
    multipleChoiceListView->installEventFilter(this);

    setEnabled(false);
}


void MultipleChoiceWidget::slotDataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
    m_translation->multipleChoice() = m_choicesModel->stringList();
    removeChoiceButton->setEnabled(m_translation && m_translation->multipleChoice().count() > 0);
}


void MultipleChoiceWidget::setTranslation(KEduVocExpression * entry, int translation)
{
    if (entry) {
        m_translation = entry->translation(translation);
    } else {
        m_translation = 0;
    }

    if (m_translation) {
        setEnabled(true);
        m_choicesModel->setStringList(m_translation->multipleChoice());
        removeChoiceButton->setEnabled(m_translation->multipleChoice().count() > 0);
    } else {
        setEnabled(false);
    }
    removeChoiceButton->setEnabled(m_translation && m_translation->multipleChoice().count() > 0);
}


void MultipleChoiceWidget::slotAddChoiceButton()
{
    m_choicesModel->insertRow(m_choicesModel->rowCount());
    QModelIndex index(m_choicesModel->index(m_choicesModel->rowCount() - 1));
    m_choicesModel->setData(index, "");
    multipleChoiceListView->scrollTo(index);
    multipleChoiceListView->setCurrentIndex(index);
    multipleChoiceListView->edit(index);
}


void MultipleChoiceWidget::slotRemoveChoiceButton()
{
    QModelIndex index = multipleChoiceListView->selectionModel()->currentIndex();
    if (index.isValid()) {
        m_choicesModel->removeRows(index.row(), 1, QModelIndex());
    } else {
        m_choicesModel->removeRows(m_choicesModel->rowCount(QModelIndex()) - 1, 1, QModelIndex());
    }
    m_translation->multipleChoice() = m_choicesModel->stringList();
    removeChoiceButton->setEnabled(m_translation && m_translation->multipleChoice().count() > 0);
}


bool MultipleChoiceWidget::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == multipleChoiceListView) {
        if (event->type() == QEvent::DragEnter) {
            QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>(event);
            kDebug() << "DragEnter mime format: " << dragEnterEvent->format();
            if (dragEnterEvent->mimeData()->hasText()) {
                event->accept();
            }
            return true;
        }

        if (event->type() == QEvent::DragMove) {
            event->accept();
            return true;
        }

        if (event->type() == QEvent::Drop) {
            QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
            kDebug() << "You dropped onto me: " << dropEvent->mimeData()->text();

            QStringList choices = dropEvent->mimeData()->text().split('\n');
            foreach(const QString &choice, choices) {
                m_choicesModel->insertRow(multipleChoiceListView->model()->rowCount());
                m_choicesModel->setData(m_choicesModel->index(multipleChoiceListView->model()->rowCount()-1), choice);
            }
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}


#include "multiplechoicewidget.moc"

