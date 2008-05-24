/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2008 David Capel <wot.narg@gmail.com>

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PARLEYPRACTICEMAINWINDOW_H
#define PARLEYPRACTICEMAINWINDOW_H

#include <KXmlGuiWindow>

#include <QGraphicsView>
#include <QGraphicsSvgItem>

class ParleyPracticeMainWindow : public KXmlGuiWindow
{
        Q_OBJECT

    public:
        ParleyPracticeMainWindow(QWidget *parent = 0);
        /*
            ~ParleyPracticeMainWindow();*/

    private:
        QGraphicsView* m_view;
        QGraphicsSvgItem* m_layout;

        bool eventFilter(QObject *obj, QEvent *event);

};


//#include "moc_parleypracticemainwindow.cpp"

#endif
