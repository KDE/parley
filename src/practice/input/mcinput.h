//
// C++ Interface: mcinput
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


#ifndef MCINPUT_H
#define MCINPUT_H

#include <QGroupBox>

class KSvgRenderer;
class QGraphicsView;
class QString;
class QWidget;
class QStringList;
class ActiveArea;

/**
* @class MCInput
* @author David Capel <wot.narg@gmail.com>
* @brief The widget that provides a multiple choice-style user input method.
*/
class MCInput : public QGroupBox
{
    Q_OBJECT
    public:
        MCInput(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent = 0);
        ~MCInput();

    public slots:
        /// Emits the currently selected answer using signalAnswer()
        void slotEmitAnswer();
        /// Checks to see if the given shortcut was valid, and if so, emits triggered()
        void slotShortcutTriggered(int shortcutNumber);
        /// Sets the QRadioButton choices to the @param choices.
        void slotSetChoices(const QStringList& choices);
        /// Shows a hint by disabling distrator choices.
        void slotShowHint(const QString& solution);
        /// Shows the solution by disabling all distractors.
        void slotShowSolution(const QString& solution);
    signals:
        /// Gives the users answer.
        void signalAnswer(const QString& answer);
        /// Signals that a shortcut was successfully triggered.
        void triggered();
    private:
        KSvgRenderer* m_renderer;
        ActiveArea* m_area;
};

#endif
