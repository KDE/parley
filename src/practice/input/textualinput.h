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

#include <QLineEdit>

class KSvgRenderer;
class QGraphicsView;
class QString;
class ActiveArea;

class TextualInput : public QLineEdit
{
    Q_OBJECT

    public:
        TextualInput(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId,  QWidget * parent = 0);

    public slots:
        void slotClear();
        void slotShowSolution(const QString& solution);
        void slotShowHint(const QString& solution);
        void slotEmitAnswer();
    signals:
        void signalAnswer(const QString& answer);
        void signalAnswerChanged(const QString& answer);
    private:
        KSvgRenderer* m_renderer;
        ActiveArea* m_area;
        int m_size_hint_shown;
};

#endif
