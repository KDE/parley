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
#include <QString>
#include <QRectF>


class TestEntryManager;

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
        TestEntryManager * m_manager;

        QRectF m_backgroundRect;
        
        bool eventFilter(QObject *obj, QEvent *event);

    public slots:
        void slotGetInput(const QString& input);
        void slotShowSolution();
        void slotToggleShowSolutionContinueActions();
        
    signals:
        void signalCheckInput(const QString&, const QString&);
        void signalShowSolution(const QString&);
};


//#include "moc_parleypracticemainwindow.cpp"

#endif
