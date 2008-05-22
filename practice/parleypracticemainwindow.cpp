/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "parleypracticemainwindow.h"

#include <KXmlGuiWindow>
#include <KLineEdit>
#include <KDebug>
#include <KStandardDirs>
// #include <QGraphicsSvgItem>
#include <QGraphicsProxyWidget>
#include <QResizeEvent>
#include <QSvgRenderer>
#include <QLCDNumber>

//#include "questiondisplay.h"
//#include "defaulttheme/vocabularycard.h"

#include "input.h"
#include "prompt.h"
#include "statistics.h"
#include "stdbuttons.h"



ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
 : KXmlGuiWindow(parent)
{
    m_view = new QGraphicsView;
    setCentralWidget(m_view);
    setupGUI();

    QGraphicsScene* scene = new QGraphicsScene(this);
    m_view->setScene(scene);

    scene->setSceneRect(0.0, 0.0, 800.0, 600.0);

    m_view->setSceneRect(scene->sceneRect());

    m_view->installEventFilter(this);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //kDebug() << KStandardDirs::locate("data", "parley/images/card.svg");

    TextualPrompt * prompt = new TextualPrompt();
    prompt->resize(100, 30);
    QGraphicsProxyWidget * gprompt = scene->addWidget(prompt);
    
    TextualInput * input = new TextualInput();
    QGraphicsProxyWidget * ginput = scene->addWidget(input);

    LCDStatistics * stats = new LCDStatistics();
    QGraphicsProxyWidget * gstats = scene->addWidget(stats);
    stats->refresh();

    StdButtons * stdbuttons = new StdButtons();
    QGraphicsProxyWidget * gstdbuttons = scene->addWidget(stdbuttons);
    
    connect(prompt, SIGNAL(signalAnswerChanged(const QString&)), input, SLOT(slotSetAnswer(const QString&)));
    connect(input, SIGNAL(signalCorrect()), stats, SLOT(slotCorrect()));
    connect(input, SIGNAL(signalIncorrect(Statistics::ErrorType)), stats, SLOT(slotIncorrect(Statistics::ErrorType)));
    connect(stdbuttons, SIGNAL(signalCheckAnswer()), input, SLOT(slotCheckAnswer()));
    connect(stdbuttons, SIGNAL(signalSkipped(Statistics::SkipReason)), stats, SLOT(slotSkipped(Statistics::SkipReason)));
    connect(stdbuttons, SIGNAL(signalSkipped(Statistics::SkipReason)), prompt, SLOT(slotNewPrompt()));
    connect(stdbuttons, SIGNAL(signalContinue()), prompt, SLOT(slotNewPrompt()));

    
    gprompt->setPos(30, 100);
    ginput->setPos(60, 60);
    gstdbuttons->setPos(200, 200);
    gstats->setPos(200, 100);
    
    //kDebug() << KStandardDirs::locate("data", "defaulttheme/layout.svg");

    prompt->slotNewPrompt();

    m_view->show();
}

/*
ParleyPracticeMainWindow::~ParleyPracticeMainWindow()
{
}*/

bool ParleyPracticeMainWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Resize) {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        QMatrix matrix;
        matrix.scale(resizeEvent->size().width()/800.0, resizeEvent->size().height()/600.0);
        m_view->setMatrix(matrix, false);
        return QObject::eventFilter(obj, event);
    }
    return QObject::eventFilter(obj, event);
}


