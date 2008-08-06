//
// C++ Interface: multiplewidgetparent
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

#ifndef MULTIPLEWIDGETPARENT_H
#define MULTIPLEWIDGETPARENT_H

#include "mwplogic.h"
#include <QMap>
#include <QList>
class ActiveArea;
class QStringList;
class QString;
class KSvgRenderer;
class QGraphicsScene;
class QLineEdit;

class MultipleWidgetParent : public QObject
{
    Q_OBJECT

    public:
        MultipleWidgetParent(KSvgRenderer * renderer, ActiveArea * area, QGraphicsScene * scene, QStringList& elementIds, MWPLogic* logic, QObject * parent = 0);
        ~MultipleWidgetParent();
    public slots:
        void slotSetTexts(const QStringList& texts);
        void slotSetEntry(const PracticeEntry* entry);
        void slotClear();
        void slotEmitAnswer();
    signals:
        void signalAnswer(const QStringList&);
    private:
        KSvgRenderer * m_renderer;
        QStringList m_elementIds;
        QList<QLineEdit*> m_kids;
        QMap<QString, QLineEdit*> m_map;
        MWPLogic* m_logic;
};

#endif
