/***************************************************************************
    Copyright 2010 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef SUMMARYBARWIDGET_H
#define SUMMARYBARWIDGET_H

#include <QtGui/QWidget>

class QLabel;
class QHBoxLayout;

namespace Practice
{

class SummaryBarWidget : public QWidget
{
    Q_OBJECT

public:
    SummaryBarWidget(QWidget *parent = 0);

    void setStatistics(int correct, int wrong, int notAnswered);

protected:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);

private:
    void setupCaption();
    QString correctText();
    QString wrongText();
    QString notAnsweredText();

    QPixmap captionPixmap(QColor color);

    int m_correct;
    int m_wrong;
    int m_notAnswered;
    int m_total;

    QHBoxLayout *m_layout;
    QLabel *m_correctCaption;
    QLabel *m_wrongCaption;
    QLabel *m_notAnsweredCaption;

    static const int BAR_HEIGHT = 30;
};

}

#endif // SUMMARYBARWIDGET_H
