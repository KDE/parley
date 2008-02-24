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

#include "vocabularycard.h"

#include <KStandardDirs>
#include <KDebug>

#include <QGraphicsSvgItem>
#include <QGraphicsTextItem>

VocabularyCard::VocabularyCard()
 : QuestionDisplay()
{
    m_card = new QGraphicsSvgItem(KStandardDirs::locate("data", "parley/images/card.svg"), this);
    m_card->setElementId("Card");

    m_text = new QGraphicsTextItem(m_card);


}


VocabularyCard::~VocabularyCard()
{
}

void VocabularyCard::setText(const QString & text)
{
    m_text->setPlainText(text);

    kDebug() << "card height: " << m_card->boundingRect().height() << " text height" << m_text->boundingRect().height();

    qreal text_x = (m_card->boundingRect().width() - m_text->boundingRect().width()) /2.0;
    qreal text_y = (m_card->boundingRect().height() - m_text->boundingRect().height()) /2.0;

    m_text->setPos(text_x, text_y);
}

QRectF VocabularyCard::boundingRect() const
{
    return m_card->boundingRect();
}

void VocabularyCard::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    m_card->boundingRect();
}


#include "vocabularycard.moc"
