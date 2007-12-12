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

    connect(imageUrlRequester, SIGNAL(textChanged(const QString&)), SLOT(slotImageChanged(const QString&)));
}


void ImageChooserWidget::setTranslation(KEduVocExpression* entry, int translation)
{
    m_currentTranslation = translation;
    m_entry = entry;

    if (m_entry) {
        setEnabled(true);
kDebug() << "setting url" << m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile();
        imageUrlRequester->setUrl(m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile());
//         slotImageChanged( m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile() );
    } else {
        setEnabled(false);
        imageUrlRequester->clear();
        imageLabel->setText(i18nc("@label image preview is empty", "No Preview"));
    }
}


void ImageChooserWidget::slotImageChanged(const QString & url)
{
    kDebug() << "Setting image " << url;

    m_entry->translation(m_currentTranslation)->setImageUrl( KUrl(url) );

    if ( !url.isEmpty() ) {
        QPixmap pixmap(url);
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
    } else {
        imageLabel->setText(i18nc("@label image preview is empty", "No Image"));
    }

    foreach (int j, m_entry->translationIndices()) {
        if ( m_entry->translation(j)->imageUrl().isEmpty() ) {
            m_entry->translation(j)->setImageUrl( imageUrlRequester->url() );
        }
    }
}

#include "imagechooserwidget.moc"
