/***************************************************************************

                    types options dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
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

#include "TypeOptPage.h"

#include "keduvocwordtype.h"

#include <keduvocdocument.h>
#include <KMessageBox>


WordTypeOptionPage::WordTypeOptionPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    titleGroupBox->setTitle(i18n("Type Descriptions"));

    m_doc = doc;

    m_wordTypes = m_doc->wordTypes();

    m_wordTypeModel = new QStandardItemModel();
    m_wordTypeModel->setHorizontalHeaderLabels( QStringList() << i18n("Word type") );

    foreach ( QString typeString, m_wordTypes->typeNameList() ) {
        QStandardItem *item = new QStandardItem(typeString);
        m_wordTypeModel->appendRow(item);
        foreach ( QString subTypeString, m_wordTypes->subTypeNameList( typeString ) ) {
            QStandardItem *subItem = new QStandardItem(subTypeString);
            item->appendRow(subItem);
        }
    }

    wordTypeTreeView->setModel(m_wordTypeModel);

    connect(wordTypeTreeView, SIGNAL(activated( QModelIndex * )), this, SLOT(itemSelected( QModelIndex * )));
    connect(m_wordTypeModel, SIGNAL(itemChanged( QStandardItem * )), this, SLOT(itemChanged( QStandardItem * )));

    connect(newWordTypeButton, SIGNAL(clicked()), this, SLOT(slotNewWordType()));
    connect(newSubWordTypeButton, SIGNAL(clicked()), this, SLOT(slotNewSubWordType()));
    connect(renameButton, SIGNAL(clicked()), this, SLOT(slotRename()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDelete()));
}

void WordTypeOptionPage::itemChanged(QStandardItem * item)
{
    kDebug() << "itemChanged() " << item->text();
    QString oldType; // debug

    if ( item->index().parent() == QModelIndex() ) {
        // get the old name, rename
        m_wordTypes->renameType( m_wordTypes->typeNameList().value(item->index().row()), item->text() );
        oldType = m_wordTypes->typeNameList().value(item->index().row());

    } else {
        QString mainType = item->parent()->text();
kDebug() << "Parent is: " << mainType << " with children: " << m_wordTypes->subTypeNameList( mainType ).count();
        oldType = m_wordTypes->subTypeNameList( mainType )[item->index().row()];

        QString subType = m_wordTypes->subTypeNameList(mainType)[item->index().row()];
        m_wordTypes->renameSubType( mainType, subType, item->text() );

        oldType = subType;
    }

    kDebug() << "WordTypeOptionPage::itemChanged(): " << oldType << " is now: " << item->text();
}


void WordTypeOptionPage::slotNewWordType()
{
    QString newTypeName = i18n("New type");
    m_wordTypes->addType(newTypeName);
    QStandardItem *item = new QStandardItem(newTypeName);
    m_wordTypeModel->appendRow(item);
    wordTypeTreeView->setCurrentIndex(item->index());
    wordTypeTreeView->edit(item->index());
}

void WordTypeOptionPage::slotNewSubWordType()
{
    QString newTypeName = i18n("New subtype");

    // get current selection
    QStandardItem * currentItem =
        m_wordTypeModel->itemFromIndex(wordTypeTreeView->currentIndex());

    // if the current item is not a top level one, use its parent.
    if ( currentItem->index().parent().isValid() ) {
        currentItem = currentItem->parent();
    }
    QString parentName = currentItem->text();
    m_wordTypes->addSubType( parentName, newTypeName );
    QStandardItem *item = new QStandardItem(newTypeName);
    currentItem->appendRow(item);
    wordTypeTreeView->setCurrentIndex(item->index());
    wordTypeTreeView->edit(item->index());

    // add to doc:
}

void WordTypeOptionPage::slotRename()
{
    // get current selection
    QModelIndex currentIndex = wordTypeTreeView->currentIndex();
    wordTypeTreeView->edit(currentIndex);
}

void WordTypeOptionPage::slotDelete()
{
    kDebug() << "slotDelete";
    QStandardItem * item =
        m_wordTypeModel->itemFromIndex(wordTypeTreeView->currentIndex());

    if ( item->index().parent() == QModelIndex() ) {
        // get the old name, rename
        if(!m_wordTypes->removeType( item->text() )){
            KMessageBox::information(this, i18n("The type you selected could not be deleted since it is a special type used for practicing."), i18n("Delete type"));
            return; // special types cannot be deleted.
        }
    } else {
        if(!m_wordTypes->removeSubType( item->parent()->text(), item->text() )){
            KMessageBox::information(this, i18n("The type you selected could not be deleted since it is a special type used for practicing."), i18n("Delete subtype"));
            return; // special types cannot be deleted.
        }
    }

    m_wordTypeModel->removeRow(item->index().row(), item->index().parent());
    m_wordTypes->printDebugWordTypes();
}

void WordTypeOptionPage::slotActivated(QModelIndex * modelIndex)
{
    kDebug() << "activated";
}

#include "TypeOptPage.moc"
