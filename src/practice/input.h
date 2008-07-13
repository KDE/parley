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
        void signalAnswerChanged(const QString& answer);
    private:
        KSvgRenderer* m_renderer;
};

class MCInput : public QGroupBox
{
    Q_OBJECT
    public:
        MCInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent = 0);
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


class MultipleChoiceMCInput : public MCInput
{
    Q_OBJECT
    public:
        MultipleChoiceMCInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget * parent = 0);

    public slots:
        void slotSetAnswers(PracticeEntry*, QList<PracticeEntry*>);
};


class ArticleMCInput : public MCInput
{
    Q_OBJECT
     public:
        ArticleMCInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, class KEduVocDocument * doc, QWidget * parent = 0);

    public slots:
        void slotSetAnswers(PracticeEntry*);
        void slotEmitAnswer();
    private:
        KEduVocDocument* m_doc;
};
#endif
