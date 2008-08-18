//
// C++ Interface: imageprompt
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


#ifndef IMAGEPROMPT_H
#define IMAGEPROMPT_H

#include <QLabel>

class KSvgRenderer;
class QString;
class QGraphicsView;
class QWidget;
class KUrl;
class ActiveArea;

/**
* @class ImagePrompt
* @author David Capel <wot.narg@gmail.com>
* @brief This widget provides the a way to show images.
*/
class ImagePrompt : public QLabel
{
    Q_OBJECT


    public:
        ImagePrompt(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent = 0);
    public slots:
        /// Shows the given image. Pass an empty KUrl to clear this.
        void slotSetImage(const KUrl& image);

    private:
        ActiveArea * m_area;
        class QPixmap m_pic;
        KSvgRenderer * m_renderer;
        class QRectF m_backgroundRect;
        bool m_enabled;
};


#endif
