///\file stdbuttons.h
// C++ Interface: stdbuttons
//
// Description: The class the manages the standard buttons of a practice dialog (continue, skip, quit, etc).
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

#ifndef EDU_STD_BUTTONS_H
#define EDU_STD_BUTTONS_H

#include <QWidget>
#include <KPushButton>
#include <KSvgRenderer>

#include "activearea.h"
#include "statistics/statistics.h"
#include "practiceview.h"
#include "parleypracticemainwindow.h"

/// @class StdButton
/// @author David Capel <wot.narg@gmail.com>
/// @brief The standard set of actions on a practice dialog (contiue, skip, quit, etc)

class StdButton : public KPushButton
{
    Q_OBJECT

    public:
        explicit StdButton(const QString& text, KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget* parent = 0);
    public slots:
        void slotActivated();
        void slotToggleText(int state);

    signals:
        /// Emitted when the user wants to continue.
        /// The user should only be able to use the continue function
        /// after the question was been answered and corrected.
        /// This should be connected to the Continue Action
        void signalContinue();
        /// Emitted when the user wants to check their answer.
        /// Using this will make the continue option appears.
        /// This should be connected to the CheckAnswer Action
        void signalCheckAnswer();
    private:
            KSvgRenderer* m_renderer;
            int m_state;
};

class PracticeActionButton : public KPushButton
{
    public:
        explicit PracticeActionButton(const QString& text, KSvgRenderer * renderer, ActiveArea * area,  const QString& elementId, QWidget* parent = 0);

    private:
            KSvgRenderer* m_renderer;

};
#endif
