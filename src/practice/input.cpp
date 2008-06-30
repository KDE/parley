//
// C++ Implementation: input
//
// Description: Implementation of the input-gathering class
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "input.h"
#include "statistics.h"
#include "practiceentry.h"
#include "prefs.h"

#include <KDebug>
#include <KSvgRenderer>
#include <KRandom>
#include <QRadioButton>
#include <QVBoxLayout>
#include <KRandomSequence>

TextualInput::TextualInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget* parent)
        : QLineEdit(parent),
        m_renderer(renderer)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

     QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());
}

void TextualInput::slotEmitAnswer()
{
    emit signalAnswer(text());
}

void TextualInput::slotChangeAnswerColor(float correct)
{
    QPalette pal;
    if (correct == 1.0)
        pal.setColor(QPalette::Text, Qt::green);
    else
        pal.setColor(QPalette::Text, Qt::red);

    setPalette(pal);
    setText(text());
}


void TextualInput::slotShowSolution(const QString& solution)
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::green);
    setPalette(pal);
    setText(solution);
}

void TextualInput::slotClear()
{
    QPalette pal;
    pal.setColor(QPalette::Text, Qt::black);
    setPalette(pal);
    setText("");
}


MultipleChoiceInput::MultipleChoiceInput(KSvgRenderer * renderer, QGraphicsView * view, const QString& elementId, QWidget* parent)
        : QGroupBox(parent),
        m_renderer(renderer)
{
    if (!renderer->elementExists(elementId))
    {
        setVisible(false);
        kDebug() << "!! Element id doesn't exist:";
        kDebug() << elementId << ":" << renderer->elementExists(elementId);
    }

     QRect bounds = m_renderer->boundsOnElement(elementId).toRect();
     setGeometry(view->mapToScene(bounds).boundingRect().toRect());
}



void MultipleChoiceInput::slotSetAnswers(PracticeEntry* currentEntry, const QList<PracticeEntry*> source)
{
    if (source.size() < 4)
    {
        kDebug() << "source list too short with " << source.size() << "entries. Aborted.";
        return;
    }


    // clean up from last time
    delete layout();

    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        delete b;
    }


    // start fresh and new!
    QVBoxLayout *vbox = new QVBoxLayout;
    QString s;
    QList<QString> list;
    // TODO should size of question set be configurable?
    int timeout = 0;
    long r;

    list.append(currentEntry->expression()->translation(Prefs::solutionLanguage())->text());

    while (list.size() < 4 && timeout < 50) // prevent infinite loop
    {
        ++timeout;
        r = KRandom::random() % source.size();
        s = source[r]->expression()->translation(Prefs::solutionLanguage())->text();
        if (!list.contains(s))
            list.append(s);
    }

    KRandomSequence(0).randomize(list);

    foreach(s, list)
    {
        vbox->addWidget(new QRadioButton(s));
    }

     vbox->addStretch(1);
     setLayout(vbox);

}

MultipleChoiceInput::~MultipleChoiceInput()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
    {
        delete b;
    }
}

void MultipleChoiceInput::slotEmitAnswer()
{
    foreach(QRadioButton* b, findChildren<QRadioButton*>())
        if (b->isChecked())
        {
            kDebug() << "found it";
            emit signalAnswer(b->text());
        }
    emit signalAnswer(""); // none were selected.
}

#include "input.moc"
