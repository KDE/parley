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
// #include <QGraphicsProxyWidget>
#include <QResizeEvent>

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

    scene->setSceneRect(0.0, 0.0, 100.0, 100.0);

    m_view->setSceneRect(scene->sceneRect());

    m_view->installEventFilter(this);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    scene->addText("My word!");

    scene->addRect(0, 0, 10, 10);
    scene->addRect(10, 10, 100, 100);

    kDebug() << KStandardDirs::locate("data", "parley/images/card.svg");


//     KLineEdit * line = new KLineEdit(this);
//     QGraphicsProxyWidget * graphicsWidget = scene->addWidget(line);
//     graphicsWidget->setPos(300, 300);




    QuestionDisplay* question = new VocabularyCard();
    scene->addItem(question);
    question->setText("a question?");

    question->scale(75.0/question->boundingRect().width(), 75.0/question->boundingRect().width());
    question->setPos(0, 30);

//     QGraphicsSvgItem * card = new QGraphicsSvgItem(KStandardDirs::locate("data", "parley/images/card.svg"));
//     scene->addItem(card);
}


ParleyPracticeMainWindow::~ParleyPracticeMainWindow()
{
}

bool ParleyPracticeMainWindow::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::Resize) {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        QMatrix matrix;
        matrix.scale(resizeEvent->size().width()/100.0, resizeEvent->size().height()/100.0);
        m_view->setMatrix(matrix, false);
        return QObject::eventFilter(obj, event);
    }
    return QObject::eventFilter(obj, event);
}





#include "parleypracticemainwindow.moc"
