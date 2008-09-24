//
// C++ Interface: messagebox
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QGraphicsTextItem>

class KSvgRenderer;
class QString;
class QGraphicsView;
class QWidget;
class QRectF;
class ActiveArea;

/// @class MessageBox
/// Used to display static messages to the user; all input to @param text should be passed through i18n()
class MessageBox : public QGraphicsTextItem
{
        Q_OBJECT

    public:
        MessageBox(KSvgRenderer * renderer, ActiveArea * area,  const QString& elementId, const QString& text, QGraphicsItem * parent = 0);
    private:
        KSvgRenderer * m_renderer;
        QRectF m_backgroundRect;
        ActiveArea* m_area;
};

#endif

