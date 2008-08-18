//
// C++ Interface: mixedlettersprompt
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


#ifndef MIXEDLETTERSPROMPT_H
#define MIXEDLETTERSPROMPT_H

#include <QWidget>
#include <QList>

class KSvgRenderer;
class QString;
class QGraphicsView;
class QWidget;
class QGraphicsScene;
class QGraphicsTextItem;
class ActiveArea;

/**
* @class MixedLettersPrompt
* @author David Capel <wot.narg@gmail.com>
* @brief This widget scrambles the input given to it and places the letters randomly throughout the area enclosed by the provided elementId. When connect to a @c TextualPrompt, it will bold correct letters as the user types them.
*/
class MixedLettersPrompt : public QWidget
{
    Q_OBJECT

    public:
        MixedLettersPrompt(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent = 0);
        QStringList scramble(const QString& input);
    public slots:
        /// Sets the text to be scrambled.
        void slotSetText(const QString& solution);
        /// Bolds all the letters of answer that match with the solution.
        void slotAnswerChanged(const QString& answer);
    private:
        /// Returns a random location that is within m_backgroundRect
        QPointF randomValidLocation();
        KSvgRenderer * m_renderer;
        QRectF m_backgroundRect;
        QGraphicsScene* m_scene;
        QList<QGraphicsTextItem*> m_letters;
        QString m_solution;
        ActiveArea * m_area;
        bool m_enabled;
};

#endif
