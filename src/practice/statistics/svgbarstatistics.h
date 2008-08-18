//
// C++ Interface: svgbarstatistics
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
#ifndef SVGBARSTATISTICS_H
#define SVGBARSTATISTICS_H

#include <QGraphicsSvgItem>

class QSvgRenderer;
class QString;
class QRectF;
class Statistics;
class ActiveArea;

/**
* @class SvgBarStatistics
* @author David Capel <wot.narg@gmail.com>
* @brief This widget shows progress through a set of questions with a graphical progress bar.
*/
class SvgBarStatistics : public QGraphicsSvgItem
{
    Q_OBJECT

    public:
        /// The @param foregroundElementId must be located above the backgroundElementId; it will be resized to show progress.
        SvgBarStatistics(QSvgRenderer * renderer, ActiveArea * area, const QString& foregroundElementId, const QString& backgroundElementId, QGraphicsItem * parent = 0);
        ~SvgBarStatistics();

    public slots:
        /// Refreshes the GUI display.
        void slotUpdateDisplay(Statistics*);


    private:
        QRectF m_backgroundRect;
};


#endif
