/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "wordtypewidget.h"

#include <keduvocexpression.h>
#include <kdebug.h>


WordTypeWidget::WordTypeWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_doc = 0;

    // subdialog
//     connect(b_TypeDlg, SIGNAL(clicked()), SLOT(invokeTypeDlg()));

    connect(subWordTypeComboBox, SIGNAL(activated(int)), SLOT(slotDataChanged(int)));

    connect(wordTypeComboBox, SIGNAL(activated(int)), SLOT(slotDataChanged(int)));
    connect(wordTypeComboBox, SIGNAL(activated(const QString&)), SLOT(slotTypeBoxChanged(const QString&)));

    subDialog = 0;

    m_currentTranslation = -1;
}


void WordTypeWidget::setDocument(KEduVocDocument * doc)
{
    m_doc = doc;
    wordTypeComboBox->clear();
    subWordTypeComboBox->clear();
    // fill the boxes

    kDebug() << "WordTypeWidget::setDocument()" << doc->title();
    updateMainTypeBoxContents();
}

void WordTypeWidget::updateMainTypeBoxContents()
{
    wordTypeComboBox->clear();
    wordTypeComboBox->addItem( i18n("No word type set") );
    wordTypeComboBox->addItems( m_doc->wordTypes().typeNameList() );
}


void WordTypeWidget::updateSubTypeBoxContents(const QString & mainType)
{
    subWordTypeComboBox->clear();
    subWordTypeComboBox->addItems( m_doc->wordTypes().subTypeNameList(mainType) );
    if( subWordTypeComboBox->count() == 0 ) {
        if( wordTypeComboBox->currentIndex() > 0 ) {
            subWordTypeComboBox->addItem(i18n("No sub types defined"));
        }
    } else {
        subWordTypeComboBox->insertItem(0, i18n("No secondary word type set"));
    }
}


void WordTypeWidget::setEntries(const QList< int > & entries, int currentTranslation)
{
    m_currentTranslation = currentTranslation;
    m_entries = entries;

    KEduVocExpression *firstEntry = m_doc->entry(m_entries.value(0));

    updateMainTypeBoxContents();

    QString mainType =
        m_doc->entry(m_entries.value(0))->
            translation(m_currentTranslation).type();
    QString subType =
        m_doc->entry(m_entries.value(0))->
            translation(m_currentTranslation).subType();

    int typeIndex = wordTypeComboBox->findText( mainType );
    if(typeIndex > 0) {
        wordTypeComboBox->setCurrentIndex(typeIndex);
    } else {
        wordTypeComboBox->setCurrentIndex(0);
    }

    updateSubTypeBoxContents( mainType );
    subWordTypeComboBox->setCurrentIndex( subWordTypeComboBox->findText( subType ) );

    // fill enabled fields if equal for all edited entries, otherwise empty.
    foreach ( int entry, m_entries) {
        KEduVocExpression *currentEntry = m_doc->entry(entry);

        /// @todo as soon as we have a .subtype() function use that and check for type differences. Now cheating: only set, if type and subtype are equal.
        if ( firstEntry->translation(m_currentTranslation).type()
                != currentEntry->translation(m_currentTranslation).type()) {
            wordTypeComboBox->setCurrentIndex(-1);
            subWordTypeComboBox->setCurrentIndex(-1);
        }
    } // foreach
}


void WordTypeWidget::slotTypeBoxChanged(const QString &mainType)
{
    subWordTypeComboBox->setCurrentIndex(-1);
    // unchanged
    if ( wordTypeComboBox->currentIndex() == -1 ) {
        return;
    }

    QString wordType = wordTypeComboBox->currentText();
    if (wordTypeComboBox->currentIndex() == 0) {
        wordType = QString();
    }

    foreach(int entry, m_entries) {
        KEduVocExpression *expr = m_doc->entry(entry);
        if (m_currentTranslation >= 0) {
            // set the type
            expr->translation(m_currentTranslation).setType( wordType );
            expr->translation(m_currentTranslation).setSubType( subWordTypeComboBox->currentText() );
            // also set the same type for the other translations
            foreach (int j, expr->translationIndices()) {
                if (expr->translation(j).type().isEmpty())
                    expr->translation(j).setType( wordType );
                    ///@todo reset subtype if new type != old type
            }
        }
    }

    emit wordTypeSelected( mainType );
}


void WordTypeWidget::invokeTypeDlg()
{
//     KDialog *subDialog = new KDialog(b_TypeDlg);
//     subDialog->setCaption(i18n("Edit User Defined Types"));
//     subDialog->setButtons(KDialog::Ok|KDialog::Cancel);
// 
//     subDialog->setDefaultButton(KDialog::Ok);
// 
//     connect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));
// 
//     WordTypeOptionPage *typeOptPage = new WordTypeOptionPage(m_doc, this);
//     subDialog->setMainWidget(typeOptPage);
// 
//     if (subDialog->exec() == KDialog::Accepted) {
//         updateMainTypeBoxContents();
//     }
}


void WordTypeWidget::slotSubDialogClosed()
{
    if (subDialog) {
        disconnect(subDialog, SIGNAL(finished()), this, SLOT(slotSubDialogClosed()));
        subDialog->deleteLater();
        subDialog = 0;
    }
}

/*
void WordTypeWidget::commitData()
{
    foreach(int entry, m_entries) {
        KEduVocExpression *expr = m_doc->entry(entry);

        if (m_currentTranslation >= 0) {
            if ( wordTypeComboBox->currentIndex() != -1 ) {
                QString type = wordTypeComboBox->currentText();
                // set the type
                expr->translation(m_currentTranslation).setType( wordTypeComboBox->currentText() );
                expr->translation(m_currentTranslation).setSubType( subWordTypeComboBox->currentText() );
                // also set the same type for the other translations
                foreach (int j, expr->translationIndices()) {
                    if (expr->translation(j).type().isEmpty())
                        expr->translation(j).setType( type );
                        ///@todo reset subtype if new type != old type
                }
            }
        }
    }
}*/


void WordTypeWidget::slotSubTypeBoxChanged(const QString & mainType)
{
}


#include "wordtypewidget.moc"
