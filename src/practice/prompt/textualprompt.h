//
// C++ Interface: textualprompt
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

#ifndef TEXTUALPROMPT_H
#define TEXTUALPROMPT_H

#include <QGraphicsTextItem>

class KSvgRenderer;
class QString;
class QGraphicsView;
class QWidget;
class QRectF;

class TextualPrompt : public QGraphicsTextItem
{
        Q_OBJECT

    public:
        TextualPrompt(KSvgRenderer * renderer, const QString& elementId);

    public slots:
        void slotSetText(const QString& text);
    private:
        KSvgRenderer * m_renderer;
        QRectF m_backgroundRect;
};

#endif
