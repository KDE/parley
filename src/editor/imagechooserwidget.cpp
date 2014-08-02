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

using namespace Editor;

ImageChooserWidget::ImageChooserWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;

    connect(imageUrlRequester, SIGNAL(textChanged(const QString&)), SLOT(slotImageChanged(const QString&)));

    imageWidget->setAcceptDrops(true);
    imageWidget->installEventFilter(this);
    setEnabled(false);
}

void ImageChooserWidget::setTranslation(KEduVocExpression* entry, int translation)
{
    m_currentTranslation = translation;
    m_entry = entry;

    if (m_entry) {
        setEnabled(true);
        imageUrlRequester->setUrl(m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile());
//         slotImageChanged( m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile() );
    } else {
        setEnabled(false);
        imageUrlRequester->clear();
//         imageLabel->setText(i18nc("@label image preview is empty", "No Preview"));
        imageWidget->setPixmap(QPixmap());
    }
}

void ImageChooserWidget::slotImageChanged(const QString & url)
{
    if (!url.isEmpty()) {
        QPixmap pixmap(url);
        imageWidget->setPixmap(pixmap);
    } else {
//         imageLabel->setText(i18nc("@label image preview is empty", "No Image"));
        imageWidget->setPixmap(QPixmap());
    }

    if (m_entry) {
        m_entry->translation(m_currentTranslation)->setImageUrl(KUrl(url));
        foreach(int j, m_entry->translationIndices()) {
            if (m_entry->translation(j)->imageUrl().isEmpty()) {
                m_entry->translation(j)->setImageUrl(imageUrlRequester->url());
            }
        }
    }
}

bool ImageChooserWidget::eventFilter(QObject * obj, QEvent * event)
{
    if (obj == imageWidget) {
//         if (event->type() == QEvent::KeyPress) {
//              QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//              kDebug() << "Ate key press " << keyEvent->key();
//              return true;
//         }
//         if (event->type() == QEvent::Resize) {
//             QPixmap pixmap(imageUrlRequester->url().toLocalFile());
//             imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
//             return false;
//         }


        if (event->type() == QEvent::DragMove) {
            event->accept();
            return true;
        }

        //Events with drop data
        if (event->type() == QEvent::DragEnter || event->type() == QEvent::Drop) {
            if (!m_entry) {
                return true;
            }
            QDropEvent *dropEvent = dynamic_cast<QDropEvent *>(event);
            if (( dropEvent!= NULL ) && dropEvent->provides("text/uri-list")) {
                const QMimeData * mimeData(  dropEvent->mimeData() );
                if ( mimeData && mimeData->hasUrls() ) {
                    const QList<QUrl > qurls(mimeData->urls() ) ;
                    if ( !qurls.empty() ) {
                        const QUrl & qurl (qurls.first() );

                        switch( event->type() ) {
                        case QEvent::DragEnter: {
                            const QString name(KMimeType::findByUrl(qurl)->name());
                            if (name.startsWith("image")) {
                                event->accept();
                                return true;
                            }
                            break;
                        }
                        case QEvent::Drop:
                            imageUrlRequester->setUrl(qurl);
                            return true;
                            break;
                        default:
                            break;
                        }

                    }
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

#include "imagechooserwidget.moc"
