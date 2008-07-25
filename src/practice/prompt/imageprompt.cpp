//
// C++ Implementation: imageprompt
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "imageprompt.h"

#include <KSvgRenderer>
#include <QString>
#include <QGraphicsView>
#include <KDebug>
#include <kio/netaccess.h>


ImagePrompt::ImagePrompt ( KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent ) :
        QLabel ( parent ),
        m_pic ( QPixmap() ),
        m_renderer ( renderer )
{

    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

    QRect bounds = m_renderer->boundsOnElement ( elementId ).toRect();
    setGeometry ( view->mapToScene ( bounds ).boundingRect().toRect() );

    slotSetImage ( KUrl() );
}



void ImagePrompt::slotSetImage ( const KUrl& image )
{
    kDebug() << image;
    if ( !image.isEmpty() )
    {
        QString tmpFile;
        if ( KIO::NetAccess::download ( image, tmpFile, this ) )
        {
            m_pic.load ( tmpFile );
            KIO::NetAccess::removeTempFile ( tmpFile );
            setVisible ( true );
        }
        else
        {
            //KMessageBox::error(this, KIO::NetAccess::lastErrorString());
            kDebug() << KIO::NetAccess::lastErrorString();
        }
    }
    else
    {
        m_pic = QPixmap();
        setVisible ( false );
    }
    setPixmap ( m_pic );
}
