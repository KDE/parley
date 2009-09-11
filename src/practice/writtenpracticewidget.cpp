/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#include "writtenpracticewidget.h"
#include "ui_practice_widget_written.h"
#include <kdebug.h>

using namespace Practice;

WrittenPracticeWidget::WrittenPracticeWidget(AbstractBackend *backend, QWidget *parent)
    : AbstractModeWidget(backend, parent), m_backend(backend)
{
    m_ui = new Ui::WrittenPracticeWidget();
    m_ui->setupUi(this);
}

QVariant WrittenPracticeWidget::userInput()
{
    return QVariant(m_ui->answerEdit->text());
}

void WrittenPracticeWidget::updateDisplay()
{
    kDebug() << "update display";
    if (m_backend->acceptUserInput()) {
        m_ui->answerEdit->setEnabled(true);
        m_ui->answerEdit->clear();
        m_ui->questionLabel->setText(m_backend->question().toString());
    } else {
        m_ui->answerEdit->setEnabled(false);
        m_ui->solutionLabel->setText(m_backend->solution().toString());
    }
}
