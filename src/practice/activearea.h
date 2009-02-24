//
// C++ Interface: activearea
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


#ifndef ACTIVEAREA_H
#define ACTIVEAREA_H

#include <QGraphicsSvgItem>
#include <QSet>

class QPointF;
class QString;
class KSvgRenderer;

/**
* @class ActiveArea
* @author David Capel <wot.narg@gmail.com>
* @brief Represents the active areas of theme files and coordinates widget positions.
* The theme files have active areas in them for each mode -- they are a box that contains the widgets
* relevant to that mode. These are then moved into the active_area element's confines in the main portion of theme.
* The represents that by giving the offset to individual widgets that would place them in the correct position.
* This class also allows themes to provide an image and a non-image layout for each mode. When the active area that
* represents the image version is shown (read: there is currently an image), it allows the widgets it coordinates to
* show themselves; otherwise it forces them to hide themselves. The opposite is true for the non-image version. Together
* this keeps the practice synced and sane.
*/
class ActiveArea : public QGraphicsSvgItem
{
    Q_OBJECT

    public:
        /// It attempts to find an active area that matches @param modeName, but falls back to @param fallbackModeName if it cannot.
        ActiveArea(KSvgRenderer * renderer, const QString& modeName, const QString& fallbackModeName = "");
        /// Provides the offset that is required to place the widget into the correct position on the canvas.
        /// If it sees that the widget is in the main portion of the theme (using the transedId), it gives a
        ///(0,0) offset to avoid skewing its position.
        QPointF offset(const QString& translatedId) const;
        /// Takes the widget's role's elementId ("image_box") and translates it into an id that fits the current mode ("multiple_choice_image_box").
        QString translateElementId(const QString& originalElementId);
        /// Returns true if the active area is valid. Image active areas will be invalid if there is no image version
        /// of an active area in the current theme. Inactivity prevents its coordinated widgets from doing anything.
        bool valid() const { return m_valid; };
        /// Sets the active flag, allowing its coordinated widgets to become active.
        void setActive(bool active) { m_active = active; };
        /// Access function for the active flag.
        bool active() const { return m_active; };

        void mousePressEvent ( QGraphicsSceneMouseEvent * event );

    signals:
        void signalClicked();
    private:
        KSvgRenderer * m_renderer;
        QPointF m_offset;
        QString m_mode_string; ///< The active area name.
        QRectF m_original;
        bool m_valid;
        bool m_active;
};

#endif
