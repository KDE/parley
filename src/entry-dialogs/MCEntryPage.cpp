/***************************************************************************

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin         : Mon Oct 29 18:09:29 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "MCEntryPage.h"

#include <keduvoctranslation.h>
#include <keduvocexpression.h>
#include <KDebug>

#include <QStringListModel>
#include <QDragEnterEvent>

MCEntryPage::MCEntryPage(QWidget *parent) : QWidget(parent)
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


void MCEntryPage::slotDataChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
    m_translation->multipleChoice() = m_choicesModel->stringList();
}


// also starting with 0 might be nicer. crashes though.
void MCEntryPage::setTranslation(KEduVocExpression * entry, int translation)
{
    if (entry) {
        m_translation = entry->translation(translation);
    } else {
        m_translation = 0;
    }

    if (m_translation) {
        setEnabled(true);
        m_choicesModel->setStringList(m_translation->multipleChoice());
    } else {
        setEnabled(false);
    }
}


void MCEntryPage::slotAddChoiceButton()
{
    kDebug() << "add choice";
    if (m_choicesModel) {
        m_choicesModel->insertRow(multipleChoiceListView->model()->rowCount());

        multipleChoiceListView->edit(m_choicesModel->index(multipleChoiceListView->model()->rowCount()-1));
    }
}


bool MCEntryPage::eventFilter(QObject * obj, QEvent * event)
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
            foreach(QString choice, choices) {
                m_choicesModel->insertRow(multipleChoiceListView->model()->rowCount());
                m_choicesModel->setData(m_choicesModel->index(multipleChoiceListView->model()->rowCount()-1), choice);
            }
            return true;
        }
        return QObject::eventFilter(obj, event);
    }
}


#include "MCEntryPage.moc"

