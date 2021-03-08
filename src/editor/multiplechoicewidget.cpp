/*
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "multiplechoicewidget.h"

#include <KEduVocTranslation>
#include <KEduVocExpression>
#include <QDebug>

#include <QStringListModel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

using namespace Editor;

MultipleChoiceWidget::MultipleChoiceWidget(QWidget *parent)
    : QWidget(parent)
    , m_translation(Q_NULLPTR)
    , m_choicesModel(new QStringListModel(this))
{
    setupUi(this);

    connect(addChoiceButton, &QPushButton::clicked, this, &MultipleChoiceWidget::slotAddChoiceButton);
    connect(removeChoiceButton, &QPushButton::clicked, this, &MultipleChoiceWidget::slotRemoveChoiceButton);

    multipleChoiceListView->setModel(m_choicesModel);

    connect(m_choicesModel, &QStringListModel::dataChanged, this, &MultipleChoiceWidget::slotDataChanged);

    multipleChoiceListView->setAcceptDrops(true);
    multipleChoiceListView->installEventFilter(this);

    setEnabled(false);
}


void MultipleChoiceWidget::slotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)
    if (m_translation) {
        m_translation->setMultipleChoice(m_choicesModel->stringList());
        removeChoiceButton->setEnabled(m_translation->getMultipleChoice().count() > 0);
    } else {
        removeChoiceButton->setEnabled(false);
    }
}


void MultipleChoiceWidget::setTranslation(KEduVocExpression *entry, int translation)
{
    if (entry) {
        m_translation = entry->translation(translation);
    } else {
        m_translation = 0;
    }

    if (m_translation) {
        setEnabled(true);
        m_choicesModel->setStringList(m_translation->getMultipleChoice());
        removeChoiceButton->setEnabled(m_translation->getMultipleChoice().count() > 0);
    } else {
        setEnabled(false);
    }
    removeChoiceButton->setEnabled(m_translation && m_translation->getMultipleChoice().count() > 0);
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
    if (m_translation) {
        m_translation->setMultipleChoice(m_choicesModel->stringList());
        removeChoiceButton->setEnabled(m_translation->getMultipleChoice().count() > 0);
    } else {
        removeChoiceButton->setEnabled(false);
    }
}


bool MultipleChoiceWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == multipleChoiceListView) {
        if (event->type() == QEvent::DragEnter) {
            QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>(event);
            //qDebug() << "DragEnter mime format: " << dragEnterEvent->format();
            if (( dragEnterEvent->mimeData() != NULL ) && dragEnterEvent->mimeData()->hasText()) {
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
            //qDebug() << "You dropped onto me: " << dropEvent->mimeData()->text();
            if (( dropEvent->mimeData() != NULL ) && dropEvent->mimeData()->hasText()) {

                QStringList choices = dropEvent->mimeData()->text().split('\n');
                foreach(const QString & choice, choices) {
                    m_choicesModel->insertRow(multipleChoiceListView->model()->rowCount());
                    m_choicesModel->setData(m_choicesModel->index(multipleChoiceListView->model()->rowCount() - 1), choice);
                }

                return true;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
