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
    explicit SummaryBarWidget(QWidget *parent = nullptr);

    void setStatistics(int correct, int wrong, int notAnswered);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;

private:
    void setupCaption();
    QString correctText();
    QString wrongText();
    QString notAnsweredText();

    QPixmap captionPixmap(QColor color);

    int m_correct{0};
    int m_wrong{0};
    int m_notAnswered{0};
    int m_total{0};

    QHBoxLayout *m_layout{nullptr};
    QLabel *m_correctCaption{nullptr};
    QLabel *m_wrongCaption{nullptr};
    QLabel *m_notAnsweredCaption{nullptr};

    static const int BAR_HEIGHT = 30;
};

}

#endif // SUMMARYBARWIDGET_H
