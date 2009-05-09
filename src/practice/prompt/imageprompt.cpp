//
// C++ Implementation: imageprompt
//
// Description:
//
//
// Copyright 2008 David Capel <wot.narg@gmail.com>
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
#include "../activearea.h"

#include <KSvgRenderer>
#include <QString>
#include <QGraphicsView>
#include <KDebug>
#include <kio/netaccess.h>


ImagePrompt::ImagePrompt ( KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent ) :
        QLabel ( parent ),
        m_area(area),
        m_pic ( QPixmap() ),
        m_renderer ( renderer )
{
    QString tId = area->translateElementId(elementId);
    m_enabled = !tId.isEmpty();
    if (!m_enabled) setVisible(false);


     QRectF bounds = m_renderer->boundsOnElement(tId);
     bounds.translate(area->offset(tId));
     setGeometry(bounds.toRect());


    slotSetImage ( KUrl() );
}



void ImagePrompt::slotSetImage ( const KUrl& image )
{
    setVisible(m_area->active() && m_enabled);
    if (!m_area->active() || !m_enabled)
    {
        m_pic = QPixmap();
        return;
    }
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
