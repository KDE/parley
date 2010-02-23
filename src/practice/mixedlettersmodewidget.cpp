/*
    Copyright (C) 2010  Daniel Laidig <d.laidig@gmx.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "mixedlettersmodewidget.h"
#include "ui_practice_widget_written.h"

#include <kdebug.h>
#include <krandomsequence.h>

using namespace Practice;

MixedLettersModeWidget::MixedLettersModeWidget(QWidget *parent)
    : WrittenPracticeWidget(parent)
{

}

void MixedLettersModeWidget::setQuestion(const QVariant& question)
{
    m_question = question.toString();
    WrittenPracticeWidget::setQuestion(question);
}

void MixedLettersModeWidget::showQuestion()
{
    WrittenPracticeWidget::showQuestion();
    m_ui->questionLabel->setText(m_question+"\n"+m_mixedSolution);
}

void MixedLettersModeWidget::setSolution(const QVariant& solution)
{
    WrittenPracticeWidget::setSolution(solution);
    QList<QChar> chars;
    Q_FOREACH(QChar ch, solution.toString()) {
        chars.append(ch);
    }
    KRandomSequence random = KRandomSequence();
    random.randomize(chars);
    m_mixedSolution.clear();
    Q_FOREACH(QChar ch, chars) {
        m_mixedSolution.append(ch);
    }
}

#include "mixedlettersmodewidget.moc"
