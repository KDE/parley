/***************************************************************************

                    types options dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

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

#include "TypeOptPage.h"

#include <keduvocdocument.h>
#include <KMessageBox>
#include <kdebug.h>


WordTypeOptionPage::WordTypeOptionPage(KEduVocDocument *doc, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    titleGroupBox->setTitle(i18n("Type Descriptions"));

    m_doc = doc;

    // ref
    m_originalWordTypes = m_doc->wordTypes();
    // copy
    m_newWordTypes = m_originalWordTypes;

    m_wordTypeModel = new QStandardItemModel();
    m_wordTypeModel->setHorizontalHeaderLabels( QStringList() << i18n("Word type") );

    foreach ( QString typeString, m_newWordTypes.typeNameList() ) {
        QStandardItem *item = new QStandardItem(typeString);
        m_wordTypeModel->appendRow(item);
        foreach ( QString subTypeString, m_newWordTypes.subTypeNameList( typeString ) ) {
            QStandardItem *subItem = new QStandardItem(subTypeString);
            item->appendRow(subItem);
        }
    }

    wordTypeTreeView->setModel(m_wordTypeModel);

    connect(wordTypeTreeView->selectionModel(), SIGNAL(currentChanged( const QModelIndex &, const QModelIndex & )), SLOT(currentChanged(const QModelIndex&)));

    connect(m_wordTypeModel, SIGNAL(itemChanged( QStandardItem * )), SLOT(itemChanged( QStandardItem * )));

    connect(newWordTypeButton, SIGNAL(clicked()), SLOT(slotNewWordType()));
    connect(newSubWordTypeButton, SIGNAL(clicked()), SLOT(slotNewSubWordType()));
    connect(renameButton, SIGNAL(clicked()), SLOT(slotRename()));
    connect(deleteButton, SIGNAL(clicked()), SLOT(slotDelete()));

    setupSpecialComboBox(QString(), QString());

    connect(specialTypeComboBox, SIGNAL(activated(int)), SLOT(specialTypeChanged(int)));
}

void WordTypeOptionPage::itemChanged(QStandardItem * item)
{
    if ( item->index().parent() == QModelIndex() ) {
        // Top level word type
        QString oldType = m_newWordTypes.typeNameList().value(item->index().row());
        if ( !item->text().isEmpty() ) {
            m_newWordTypes.renameType( m_newWordTypes.typeNameList().value(item->index().row()), item->text() );
            setupSpecialComboBox(item->text(), QString());
        } else {
            item->setText(oldType);
        }
    } else {
        // Subtype
        QString mainType = item->parent()->text();
        QString subType = m_newWordTypes.subTypeNameList(mainType)[item->index().row()];
        if ( !item->text().isEmpty() ) {
            m_newWordTypes.renameSubType( mainType, subType, item->text() );
            setupSpecialComboBox(mainType, item->text());
        } else {
            item->setText(subType);
        }
    }
}


void WordTypeOptionPage::slotNewWordType()
{
    QString newTypeName = i18n("New type");
    m_newWordTypes.addType(newTypeName);
    QStandardItem *item = new QStandardItem(newTypeName);
    m_wordTypeModel->appendRow(item);
    wordTypeTreeView->setCurrentIndex(item->index());
    wordTypeTreeView->edit(item->index());

    setupSpecialComboBox(item->text(), QString());
}

void WordTypeOptionPage::slotNewSubWordType()
{
    QString newTypeName = i18n("New subtype");

    // get current selection
    QStandardItem * currentItem =
        m_wordTypeModel->itemFromIndex(wordTypeTreeView->currentIndex());

    if ( !currentItem )  {
        return;
    }

    // if the current item is not a top level one, use its parent.
    if ( currentItem->index().parent().isValid() ) {
        currentItem = currentItem->parent();
    }
    QString parentName = currentItem->text();
    m_newWordTypes.addSubType( parentName, newTypeName );
    QStandardItem *item = new QStandardItem(newTypeName);
    currentItem->appendRow(item);
    wordTypeTreeView->setCurrentIndex(item->index());
    wordTypeTreeView->edit(item->index());

    setupSpecialComboBox(currentItem->parent()->text(), currentItem->text());
}

void WordTypeOptionPage::slotRename()
{
    // get current selection
    QModelIndex currentIndex = wordTypeTreeView->currentIndex();
    wordTypeTreeView->edit(currentIndex);
}

void WordTypeOptionPage::slotDelete()
{
    QStandardItem * item =
        m_wordTypeModel->itemFromIndex(wordTypeTreeView->currentIndex());

    if ( item->index().parent() == QModelIndex() ) {
        m_newWordTypes.removeType( item->text() );
    } else {
        m_newWordTypes.removeSubType( item->parent()->text(), item->text() );
    }
    m_wordTypeModel->removeRow(item->index().row(), item->index().parent());
}

void WordTypeOptionPage::accept()
{
    m_doc->wordTypes() = m_newWordTypes;
}


void WordTypeOptionPage::setupSpecialComboBox(const QString& wordType, const QString& subType)
{

kDebug() << "Setup special combobox: " << wordType << subType;

    if ( wordType.isEmpty() ) {
        specialTypeLabel->setText( i18n("Select a type") );
        specialTypeComboBox->setEnabled( false );
        return;
    }

    if ( subType.isEmpty() ) {
        // set up the special combo box
        specialTypeComboBox->setEnabled( true );
        specialTypeLabel->setText( i18n("Special meaning of %1:", wordType ) );
        specialTypeComboBox->clear();
        specialTypeComboBox->addItem( i18nc("@item:comobox selection for word types with special meaning in the practice (like noun for article and verb for conjugation) - no special word type","None") );
        specialTypeComboBox->addItem( m_newWordTypes.specialTypeNoun() );
        specialTypeComboBox->addItem( m_newWordTypes.specialTypeVerb() );
        specialTypeComboBox->addItem( m_newWordTypes.specialTypeAdjective() );
        specialTypeComboBox->addItem( m_newWordTypes.specialTypeAdverb() );
        if ( m_newWordTypes.specialType(wordType).isEmpty() ) {
            specialTypeComboBox->setCurrentIndex(0);
        } else {
            specialTypeComboBox->setCurrentIndex(
                specialTypeComboBox->findText(m_newWordTypes.specialType(wordType)));
        }
    } else {
// subtypes - only noun has meaningful subtypes right now
        if ( !m_newWordTypes.specialType(wordType).isEmpty() ) {
            if ( m_newWordTypes.specialType(wordType) == m_newWordTypes.specialTypeNoun() ) {
                specialTypeComboBox->clear();
                specialTypeComboBox->setEnabled( true );
                specialTypeLabel->setText( i18n("Special meaning of %1:", subType ) );
                specialTypeComboBox->addItem( i18nc("@item:comobox selection for word types with special meaning in the practice (like noun for article and verb for conjugation) - no special word type","None") );
                specialTypeComboBox->addItem( m_newWordTypes.specialTypeNounMale() );
                specialTypeComboBox->addItem( m_newWordTypes.specialTypeNounFemale() );
                specialTypeComboBox->addItem( m_newWordTypes.specialTypeNounNeutral() );
    kDebug() << "find text" << m_newWordTypes.specialSubType(wordType, subType);
                specialTypeComboBox->setCurrentIndex(
                    specialTypeComboBox->findText(m_newWordTypes.specialSubType(wordType, subType)));
            } else {
                specialTypeLabel->setText( i18n("%1 does not have special subtypes.", wordType) );
                specialTypeComboBox->setEnabled( false );
            }
        } else {
            // parent is not special? then sub cannot be either
            specialTypeLabel->setText( i18n("%1 is not a special type.", wordType) );
            specialTypeComboBox->setEnabled( false );
        }
    }
}

void WordTypeOptionPage::currentChanged(const QModelIndex& index)
{
    if ( !index.isValid() ) {
        setupSpecialComboBox(QString(), QString());
        return;
    }

    QStandardItem* item = m_wordTypeModel->itemFromIndex(index);
    if ( index.parent() == QModelIndex() ) {
        setupSpecialComboBox( item->text(), QString() );
    } else {
        setupSpecialComboBox( item->parent()->text(), item->text() );
    }
}

void WordTypeOptionPage::specialTypeChanged(int specialTypeIndex)
{
    QString newSpecialType;
    if ( specialTypeIndex == 0 ) {
        newSpecialType = QString();
    } else {
        newSpecialType = specialTypeComboBox->itemText(specialTypeIndex);
    }

    QStandardItem * item =
        m_wordTypeModel->itemFromIndex(wordTypeTreeView->currentIndex());

    if ( !item ) {
        kDebug() << "No item selected.";
        return;
    }

    if ( item->index().parent() == QModelIndex() ) {
kDebug() << "Change special type: " << item->text() << newSpecialType;

        m_newWordTypes.setSpecialType( item->text(), newSpecialType );
    } else {
    kDebug() << "Change special subtype: " ;
kDebug() << "Change special subtype: " << item->parent()->text() << item->text() << newSpecialType;
        m_newWordTypes.setSpecialSubType( item->parent()->text(), item->text(), newSpecialType );
    }
}

#include "TypeOptPage.moc"
