/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/


#ifndef SUMMARYBARWIDGET_H
#define SUMMARYBARWIDGET_H

#include <QWidget>

class QLabel;
class QHBoxLayout;

namespace Practice
{

class SummaryBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryBarWidget(QWidget *parent = 0);

    void setStatistics(int correct, int wrong, int notAnswered);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

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
