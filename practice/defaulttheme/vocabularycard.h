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

#ifndef VOCABULARYCARD_H
#define VOCABULARYCARD_H

#include "questiondisplay.h"

#include <QGraphicsItem>

class QGraphicsSvgItem;
class QGraphicsTextItem;

/**
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class VocabularyCard : public QuestionDisplay
{
Q_OBJECT
public:
    VocabularyCard(QGraphicsItem* parent = 0);

    ~VocabularyCard();

    QRectF boundingRect() const;
//     QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void setText(const QString& text);

private:
    QGraphicsSvgItem* m_card;
    QGraphicsTextItem* m_text;
};

#endif
