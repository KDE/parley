//
// C++ Interface: multipletextualinput
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

#ifndef MULTIPLETEXTUALINPUT_H
#define MULTIPLETEXTUALINPUT_H

#include <QMap>
#include <QList>
#include <QStringList>
#include <QObject>
class ActiveArea;
class QString;
class KSvgRenderer;
class QLineEdit;

class MultipleTextualInput : public QObject
{
    Q_OBJECT

    public:
        MultipleTextualInput(KSvgRenderer * renderer, ActiveArea * area, QStringList& elementIds, QObject * parent = 0);
        ~MultipleTextualInput();
    public slots:
        void slotSetChoices(const QStringList& texts);
        void slotClear();
        void slotEmitAnswer();
    signals:
        void signalAnswer(const QStringList&);
    protected:
        KSvgRenderer * m_renderer;
        QStringList m_elementIds;
        QList<QLineEdit*> m_kids;
        QMap<QString, QLineEdit*> m_map;
};

#endif
