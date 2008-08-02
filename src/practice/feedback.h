//
// C++ Interface: feedback
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

#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "answervalidator.h"

#include <QGraphicsTextItem>

class QString;
class KSvgRenderer;
class QRectF;
class ActiveArea;

class Feedback : public QGraphicsTextItem
{
        Q_OBJECT

    public:
        Feedback(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId);

    public slots:
        void slotSetText(const QString& htmltext);
        void slotClear();
    private:
        KSvgRenderer * m_renderer;
        QRectF m_backgroundRect;
};

#endif
