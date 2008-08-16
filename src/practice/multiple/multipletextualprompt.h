//
// C++ Interface: multipletextualprompt
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

#ifndef MULTIPLETEXTUALPROMPT_H
#define MULTIPLETEXTUALPROMPT_H

#include <QMap>
#include <QList>
#include <QObject>
#include <QStringList>
class ActiveArea;
class QString;
class KSvgRenderer;
class QGraphicsTextItem;


class MultipleTextualPrompt : public QObject
{
    Q_OBJECT

    public:
        MultipleTextualPrompt(KSvgRenderer * renderer, ActiveArea * area, QStringList& elementIds, QObject * parent = 0);
        ~MultipleTextualPrompt();
    public slots:
        void slotSetText(const QStringList& texts);
        void slotClear();
    protected:
        KSvgRenderer * m_renderer;
        QStringList m_elementIds;
        QList<QGraphicsTextItem*> m_kids;
        QMap<QString, QGraphicsTextItem*> m_map;
        ActiveArea* m_area;
};

#endif
