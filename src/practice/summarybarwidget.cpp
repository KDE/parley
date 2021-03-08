/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "summarybarwidget.h"

#include <KColorScheme>
#include <KLocalizedString>

#include <QPainter>
#include <QEvent>
#include <QHelpEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainterPath>

using namespace Practice;

SummaryBarWidget::SummaryBarWidget(QWidget *parent)
    : QWidget(parent), m_correct(0), m_wrong(0), m_notAnswered(0), m_total(0)
{
    setMinimumHeight(30);
    m_layout = new QHBoxLayout(this);
    setLayout(m_layout);
    layout()->setContentsMargins(0, layout()->contentsMargins().top() + BAR_HEIGHT, 0, 0);
    setupCaption();
}

void SummaryBarWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    QLinearGradient linearGrad(QPoint(0, 0), QPoint(rect().width(), 0));

    KColorScheme scheme(QPalette::Active);
    QColor correctColor = scheme.foreground(KColorScheme::PositiveText).color();
    QColor wrongColor = scheme.foreground(KColorScheme::NegativeText).color();
    QColor notAnsweredColor = scheme.foreground(KColorScheme::NormalText).color();

    if (!m_total) {
        return;
    }
    double correctPos = double(m_correct) / m_total;
    double wrongPos = correctPos + double(m_wrong) / m_total;

    double margin = double(2) / rect().width();

    if (m_correct > 0) {
        linearGrad.setColorAt(qMax(correctPos - margin, 0.0), correctColor);
    }
    if (m_wrong > 0) {
        linearGrad.setColorAt(qMin(correctPos + margin, 1.0), wrongColor);
        linearGrad.setColorAt(qMax(wrongPos - margin, 0.0), wrongColor);
    }
    if (m_notAnswered > 0) {
        linearGrad.setColorAt(qMin(wrongPos + margin, 1.0), notAnsweredColor);
    }

    QRect r = rect();
    r.setHeight(BAR_HEIGHT);
    r.adjust(1, 1, -1, -1);

    QPainterPath path;
    path.addRoundedRect(r, 3.0, 3.0);
    painter.setBrush(QBrush(linearGrad));
    painter.drawPath(path);
}

void SummaryBarWidget::setStatistics(int correct, int wrong, int notAnswered)
{
    m_correct = correct;
    m_wrong = wrong;
    m_notAnswered = notAnswered;
    m_total = m_correct + m_wrong + m_notAnswered;

    m_correctCaption->setText(i18nc("test results", "%1 % correct", qRound(m_correct * 100.0 / m_total)));
    m_correctCaption->setToolTip(correctText());
    m_wrongCaption->setText(i18nc("test results", "%1 % wrong", qRound(m_wrong * 100.0 / m_total)));
    m_wrongCaption->setToolTip(wrongText());
    m_notAnsweredCaption->setText(i18nc("test results", "%1 % not answered", qRound(m_notAnswered * 100.0 / m_total)));
    m_notAnsweredCaption->setToolTip(notAnsweredText());

    update();
}

bool SummaryBarWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        if (!m_total)
            return QWidget::event(event);

        if (helpEvent->y() >= BAR_HEIGHT) {
            return false;
        }

        int correctPos = int(rect().width() * double(m_correct) / m_total);
        int wrongPos = correctPos + int(rect().width() * double(m_wrong) / m_total);
        if (helpEvent->x() <= correctPos) {
            setToolTip(correctText());
        } else if (helpEvent->x() <= wrongPos) {
            setToolTip(wrongText());
        } else {
            setToolTip(notAnsweredText());
        }
    }
    return QWidget::event(event);
}

void SummaryBarWidget::setupCaption()
{
    QLabel *correctColorLabel = new QLabel(this);
    m_layout->addWidget(correctColorLabel);
    m_correctCaption = new QLabel(this);
    m_layout->addWidget(m_correctCaption);
    m_layout->addSpacing(10);
    QLabel *wrongColorLabel = new QLabel(this);
    m_layout->addWidget(wrongColorLabel);
    m_wrongCaption = new QLabel(this);
    m_layout->addWidget(m_wrongCaption);
    m_layout->addSpacing(10);
    QLabel *notAnsweredColorLabel = new QLabel(this);
    m_layout->addWidget(notAnsweredColorLabel);
    m_notAnsweredCaption = new QLabel(this);
    m_layout->addWidget(m_notAnsweredCaption);
    m_layout->addStretch();

    KColorScheme scheme(QPalette::Active);
    correctColorLabel->setPixmap(captionPixmap(scheme.foreground(KColorScheme::PositiveText).color()));
    wrongColorLabel->setPixmap(captionPixmap(scheme.foreground(KColorScheme::NegativeText).color()));
    notAnsweredColorLabel->setPixmap(captionPixmap(scheme.foreground(KColorScheme::NormalText).color()));

}

QString SummaryBarWidget::correctText()
{
    if (!m_total)
        return QString();
    return i18n("Answered correctly on the first attempt: %1 of %2 (%3 %)", m_correct, m_total, qRound(m_correct * 100.0 / m_total));
}

QString SummaryBarWidget::wrongText()
{
    if (!m_total)
        return QString();
    return i18n("Answered wrong on the first attempt: %1 of %2 (%3 %)", m_wrong, m_total, qRound(m_wrong * 100.0 / m_total));
}

QString SummaryBarWidget::notAnsweredText()
{
    if (!m_total)
        return QString();
    return i18n("Not answered during this practice: %1 of %2 (%3 %)", m_notAnswered, m_total, qRound(m_notAnswered * 100.0 / m_total));
}

QPixmap SummaryBarWidget::captionPixmap(QColor color)
{
    QImage image(20, 20, QImage::Format_ARGB32_Premultiplied);
    image.fill(QColor(Qt::transparent).rgba());
    QPainter painter(&image);
    painter.setBrush(color);
    painter.setPen(Qt::black);

    QPainterPath path;
    path.addRoundedRect(image.rect().adjusted(0, 0, -1, -1), 3.0, 3.0);
    painter.drawPath(path);
    return QPixmap::fromImage(image);
}
