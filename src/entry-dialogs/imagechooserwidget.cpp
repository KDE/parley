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
#include <KMimeType>

#include <QDragEnterEvent>

ImageChooserWidget::ImageChooserWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;

    connect(imageUrlRequester, SIGNAL(textChanged(const QString&)), SLOT(slotImageChanged(const QString&)));

    imageLabel->setAcceptDrops(true);
    imageLabel->installEventFilter(this);
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



    if ( !url.isEmpty() ) {
        QPixmap pixmap(url);
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
    } else {
        imageLabel->setText(i18nc("@label image preview is empty", "No Image"));
    }

    if (m_entry) {
        m_entry->translation(m_currentTranslation)->setImageUrl( KUrl(url) );
        foreach (int j, m_entry->translationIndices()) {
            if ( m_entry->translation(j)->imageUrl().isEmpty() ) {
                m_entry->translation(j)->setImageUrl( imageUrlRequester->url() );
            }
        }
    }
}

bool ImageChooserWidget::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == imageLabel) {
//         if (event->type() == QEvent::KeyPress) {
//              QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//              kDebug() << "Ate key press " << keyEvent->key();
//              return true;
//         }
        if (event->type() == QEvent::Resize) {
            QPixmap pixmap(imageUrlRequester->url().toLocalFile());
            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
            return false;
        }


        if (event->type() == QEvent::DragEnter) {
            if (!m_entry) {
                return true;
            }
            QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent *>(event);
            kDebug() << "DragEnter mime format: " << dragEnterEvent->format();
            if (dragEnterEvent->provides("text/uri-list")) {
                kDebug() << KMimeType::findByUrl(dragEnterEvent->mimeData()->urls()[0])->name();

                if(KMimeType::findByUrl(dragEnterEvent->mimeData()->urls()[0])->name().startsWith("image")) {
                    kDebug() << "text/uri-list contains image";
                    event->accept();
                    return true;
                }
            }
            return false;
        }

        if (event->type() == QEvent::DragMove) {
            event->accept();
            return true;
        }

        if (event->type() == QEvent::Drop) {
            QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
            kDebug() << "You dropped onto me: " << dropEvent->mimeData()->formats() << dropEvent->mimeData()->urls();

            imageUrlRequester->setUrl(dropEvent->mimeData()->urls()[0]);

            return true;
        }

        return QObject::eventFilter(obj, event);
    }
}

#include "imagechooserwidget.moc"
