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

#include <KDebug>
#include <KStandardDirs>
#include <QGraphicsSvgItem>

ParleyPracticeMainWindow::ParleyPracticeMainWindow(QWidget *parent)
 : KXmlGuiWindow(parent)
{
    m_view = new QGraphicsView;
    setCentralWidget(m_view);
    setupGUI();

    QGraphicsScene* scene = new QGraphicsScene(this);
    m_view->setScene(scene);

    scene->addText("My word!");

    scene->addRect(0, 0, 10, 10);
    scene->addRect(10, 10, 100, 100);

    kDebug() << KStandardDirs::locate("data", "parley/images/card.svg");

    QGraphicsSvgItem * card = new QGraphicsSvgItem(KStandardDirs::locate("data", "parley/images/card.svg"));
    scene->addItem(card);
}


ParleyPracticeMainWindow::~ParleyPracticeMainWindow()
{
}


#include "parleypracticemainwindow.moc"
