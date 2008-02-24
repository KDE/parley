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

#include <KLineEdit>
#include <KDebug>
#include <KStandardDirs>
// #include <QGraphicsSvgItem>
#include <QGraphicsProxyWidget>
#include <QResizeEvent>
#include <QSvgRenderer>

#include "questiondisplay.h"
#include "defaulttheme/vocabularycard.h"

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


    scene->addText("My word!");

    scene->addRect(0, 0, 10, 10);
    scene->addRect(10, 10, 100, 100);

    kDebug() << KStandardDirs::locate("data", "parley/images/card.svg");


    QuestionDisplay* question = new VocabularyCard();
    scene->addItem(question);
    question->setText("a question?");

    question->scale(75.0/question->boundingRect().width(), 75.0/question->boundingRect().width());
    question->setPos(0, 30);

    kDebug() << KStandardDirs::locate("data", "defaulttheme/layout.svg");

    m_layout = new QGraphicsSvgItem(KStandardDirs::locate("data", "parley/defaulttheme/layout.svg"));
    scene->addItem(m_layout);

    QuestionDisplay* card2 = new VocabularyCard(m_layout);
    card2->setText("a second card");
    card2->setPos( m_layout->renderer()->boundsOnElement( "card" ).topLeft() );

    double scale = qMin(
            m_layout->renderer()->boundsOnElement( "card" ).width()/card2->boundingRect().width(),
            m_layout->renderer()->boundsOnElement( "card" ).height()/card2->boundingRect().height());
    card2->setTransform(QTransform().scale(scale, scale));


    KLineEdit * line = new KLineEdit;
    line->setText("Hi WoC");
    QGraphicsProxyWidget * graphicsWidget = new QGraphicsProxyWidget(m_layout);
    graphicsWidget->setWidget(line);
    graphicsWidget->setPos( m_layout->renderer()->boundsOnElement( "userInput" ).topLeft() );
    graphicsWidget->resize(m_layout->renderer()->boundsOnElement( "userInput" ).width(), m_layout->renderer()->boundsOnElement( "userInput" ).height());
    line->show();

}


ParleyPracticeMainWindow::~ParleyPracticeMainWindow()
{
}

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


#include "parleypracticemainwindow.moc"
