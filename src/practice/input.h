///\file input.h
// C++ Interface: input
//
// Description: Accepts user input and checks for correctness.
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

#ifndef EDU_INPUT_H
#define EDU_INPUT_H

#include <QObject>
#include <QLineEdit>
#include <QGroupBox>

#include <KSvgRenderer>
#include <QGraphicsView>

class QString;
class PracticeEntry;

class TextualInput : public QLineEdit
{
    Q_OBJECT

    public:
        TextualInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent = 0);

    public slots:
        void slotChangeAnswerColor(float correct);
        void slotClear();
        void slotShowSolution(const QString& solution);
        void slotEmitAnswer();

    signals:
        void signalAnswer(const QString& answer);

    private:
        KSvgRenderer* m_renderer;
};


class MultipleChoiceInput : public QGroupBox
{
    Q_OBJECT;
    public:
        MultipleChoiceInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent = 0);
        ~MultipleChoiceInput();

    public slots:
        void slotEmitAnswer();
        void slotSetAnswers(PracticeEntry*, QList<PracticeEntry*>);

    signals:
        void signalAnswer(const QString& answer);

    private:
        KSvgRenderer* m_renderer;
};

#endif
