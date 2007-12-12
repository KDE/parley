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

#include "imagechooserwidget.h"

#include <keduvocexpression.h>
#include <kdebug.h>


ImageChooserWidget::ImageChooserWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;
}


void ImageChooserWidget::setTranslation(KEduVocExpression* entry, int translation)
{
    m_currentTranslation = translation;
    m_entry = entry;

    if (m_entry) {
        setEnabled(true);
        if ( !m_entry->translation(m_currentTranslation)->imageUrl().isEmpty() ) {
            imageUrlRequester->setUrl( m_entry->translation(m_currentTranslation)->imageUrl() );
kDebug() << "Setting pixmap: " << m_entry->translation(m_currentTranslation)->imageUrl().url();

            QPixmap pixmap(m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile());

            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
        } else {
            imageUrlRequester->clear();
            imageLabel->setText(i18nc("@label image preview is empty", "No Image"));
        }

    } else {
        setEnabled(false);
//         imageUrlRequester->clear();
        imageLabel->setText(i18nc("@label image preview is empty", "No Preview"));
    }
}


#include "imagechooserwidget.moc"
