//
// C++ Interface: textualinput
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


#ifndef TEXTUALINPUT_H
#define TEXTUALINPUT_H

#include <KLineEdit>

class KSvgRenderer;
class QGraphicsView;
class QString;
class ActiveArea;


/**
* @class TextualInput
* @author David Capel <wot.narg@gmail.com>
* @brief This widget provides the free-form written user input method.
*/
class TextualInput : public KLineEdit
{
    Q_OBJECT

    public:
        TextualInput(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId,  QWidget * parent = 0);

    public slots:
        /// Clears the input box.
        void slotClear();
        /// Shows the solution.
        void slotShowSolution(const QString& solution);
        /// Shows one (more) letter of the solution each time it is called.
        void slotShowHint(const QString& solution);
        /// Emits the current user answer with signalAnswer().
        void slotEmitAnswer();
    signals:
        /// Gives the current user answer.
        void signalAnswer(const QString& answer);
        /// Emitted whenever the user changes their answer
        void signalAnswerChanged(const QString& answer);
    private:
        KSvgRenderer* m_renderer;
        ActiveArea* m_area;
        // How many letters have been shown for the hint
        int m_size_hint_shown;
};

#endif
