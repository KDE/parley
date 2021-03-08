/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mixedlettersmodewidget.h"
#include "ui_practice_widget_written.h"

#include <QFontMetrics>
#include <QPainter>

#include <KColorScheme>


using namespace Practice;

MixedLettersModeWidget::MixedLettersModeWidget(GuiFrontend *frontend, QWidget *parent)
    : WrittenPracticeWidget(frontend, parent)
{
    m_ui->mixedSolutionLabel->show();
    connect(m_ui->answerEdit, &QLineEdit::textChanged, this, &MixedLettersModeWidget::updatePixmap);
}

void MixedLettersModeWidget::setSolutionFont(const QFont& font)
{
    m_solutionFont = font;
    WrittenPracticeWidget::setSolutionFont(font);
}

void MixedLettersModeWidget::setQuestion(const QVariant& question)
{
    m_question = question.toString();
    WrittenPracticeWidget::setQuestion(question);
}

void MixedLettersModeWidget::showQuestion()
{
    WrittenPracticeWidget::showQuestion();
    updatePixmap();
}

void MixedLettersModeWidget::updatePixmap()
{
    QFontMetrics fm(m_solutionFont);
    int charHeight = fm.height();
    int charWidth = fm.averageCharWidth();
    m_pixmap = QPixmap(charWidth * m_mixedSolution.length() * 2 + charWidth, charHeight * 3);
    m_pixmap.fill(QColor(0, 0, 0, 0));

    QPainter p(&m_pixmap);
    p.setFont(m_solutionFont);
    KColorScheme scheme(QPalette::Active);
    QPen defaultPen = p.pen();
    defaultPen.setColor(palette().color(QPalette::WindowText));
    QString enteredChars = m_ui->answerEdit->text();
    int i = 0;
    Q_FOREACH(QChar ch, m_mixedSolution) {
        int pos = enteredChars.indexOf(ch);
        if (pos != -1) {
            p.setPen(scheme.foreground(KColorScheme::InactiveText).color());
            enteredChars.remove(pos, 1);
        } else {
            p.setPen(defaultPen);
        }
        p.drawText(charWidth + charWidth * i * 2, charHeight + int(m_positions.at(i)*charHeight * 0.25), ch);
        i++;
    }
    m_ui->mixedSolutionLabel->setPixmap(m_pixmap);
    m_ui->mixedSolutionLabel->setMinimumSize(m_pixmap.size());
}

void MixedLettersModeWidget::setSolution(const QVariant& solution)
{
    WrittenPracticeWidget::setSolution(solution);
    m_solution = solution.toString();
    QList<QChar> chars;
    Q_FOREACH(QChar ch, solution.toString()) {
        chars.append(ch);
    }
    m_randomSequence.randomize(chars);
    m_mixedSolution.clear();
    m_positions.clear();
    Q_FOREACH(QChar ch, chars) {
        m_mixedSolution.append(ch);
        m_positions.append(m_randomSequence.getInt(8));
    }
}
