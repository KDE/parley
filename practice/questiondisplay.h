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

#ifndef QUESTIONDISPLAY_H
#define QUESTIONDISPLAY_H

#include <QObject>
#include <QGraphicsItem>

class QString;
class QGraphicsItem;

/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class QuestionDisplay :public QObject, public QGraphicsItem
{
Q_OBJECT
public:
    QuestionDisplay();

    ~QuestionDisplay();

public slots:
    virtual void setText(const QString& text) = 0;


};

#endif
