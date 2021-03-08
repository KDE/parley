/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "imagechooserwidget.h"

#include <KEduVocExpression>
#include <QDebug>

#include <QDragEnterEvent>
#include <QMimeData>

using namespace Editor;

ImageChooserWidget::ImageChooserWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    m_currentTranslation = -1;

    connect(imageUrlRequester, &KUrlRequester::textChanged, this, &ImageChooserWidget::slotImageChanged);

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
        imageUrlRequester->setUrl(m_entry->translation(m_currentTranslation)->imageUrl());
//         slotImageChanged( m_entry->translation(m_currentTranslation)->imageUrl().toLocalFile() );
    } else {
        setEnabled(false);
        imageUrlRequester->clear();
//         imageLabel->setText(i18nc("@label image preview is empty", "No Preview"));
        imageWidget->setPixmap(QPixmap());
    }
}

void ImageChooserWidget::slotImageChanged(const QString & urlStr)
{
    QUrl url = QUrl::fromUserInput(urlStr);
    if (!url.isEmpty() && url.isLocalFile()) {
        QPixmap pixmap(url.toLocalFile());
        imageWidget->setPixmap(pixmap);
    } else {
//         imageLabel->setText(i18nc("@label image preview is empty", "No Image"));
        imageWidget->setPixmap(QPixmap());
    }

    if (m_entry) {
        m_entry->translation(m_currentTranslation)->setImageUrl(url);
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
//              qDebug() << "Ate key press " << keyEvent->key();
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
            if (( dropEvent!= NULL ) && ( dropEvent->mimeData()!= NULL ) ) {
                const QMimeData * mimeData( dropEvent->mimeData() );
                if ( mimeData->hasUrls() && mimeData->hasUrls() && ( mimeData->urls().size() == 1 )) {
                    switch( event->type() ) {
                    case QEvent::DragEnter:
                        event->accept();
                        return true;
                    case QEvent::Drop:
                        imageUrlRequester->setUrl(mimeData->urls().first());
                        return true;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    return QObject::eventFilter(obj, event);
}
