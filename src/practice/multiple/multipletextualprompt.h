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



/**
* @class MultipleTextualInput
* @author David Capel <wot.narg@gmail.com>
* @brief Manages a set of input widgets.
* All QStringLists passed to a given instance of this class *must* be the same length and *must* be in the same order.
* For example, the first QString passed to slotSetText(QStringList) will be put in the input widget for the first
* elementId passed to the constructor.
*/
class MultipleTextualPrompt : public QObject
{
    Q_OBJECT

    public:
        /// Creates a QGraphicsTextItem for each elementId passed.
        MultipleTextualPrompt(KSvgRenderer * renderer, ActiveArea * area, QStringList& elementIds, QObject * parent = 0);
        ~MultipleTextualPrompt();
    public slots:
        /// Filled each QGraphicsTextItem with an item from the list of texts.
        /// These will be applied in the same order as the list passed othe constructor.
        void slotSetText(const QStringList& texts);
        /// Clears all QGraphicsTextItems.
        void slotClear();
    protected:
        KSvgRenderer * m_renderer;
        QStringList m_elementIds;
        QList<QGraphicsTextItem*> m_kids;
        QMap<QString, QGraphicsTextItem*> m_map;
        ActiveArea* m_area;
};

#endif
