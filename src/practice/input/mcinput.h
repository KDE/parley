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

class MCInput : public QGroupBox
{
    Q_OBJECT
    public:
        MCInput(KSvgRenderer * renderer, ActiveArea * area, const QString& elementId, QWidget * parent = 0);
        ~MCInput();

    public slots:
        virtual void slotEmitAnswer();
        void slotShortcutTriggered(int shortcutNumber);

    signals:
        void signalAnswer(const QString& answer);
        void triggered();
        void signalAnswerChanged(const QString& answer);

    protected:
        void setAvailableAnswers(QStringList answers);
    private:
        KSvgRenderer* m_renderer;
};

#endif
