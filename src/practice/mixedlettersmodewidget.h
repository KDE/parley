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

#ifndef PRACTICE_MIXEDLETTERSMODEWIDGET_H
#define PRACTICE_MIXEDLETTERSMODEWIDGET_H

#include "writtenpracticewidget.h"

#include <KRandomSequence>

namespace Ui
{
class WrittenPracticeWidget;
}

namespace Practice
{

class MixedLettersModeWidget : public Practice::WrittenPracticeWidget
{
    Q_OBJECT

public:
    explicit MixedLettersModeWidget(GuiFrontend *frontend, QWidget *parent = 0);

    void setQuestion(const QVariant& question) Q_DECL_OVERRIDE;
    void setSolution(const QVariant& solution) Q_DECL_OVERRIDE;
    void setSolutionFont(const QFont& font) Q_DECL_OVERRIDE;

public slots:
    void showQuestion() Q_DECL_OVERRIDE;

private slots:
    void updatePixmap();

private:
    QString m_solution;
    QString m_mixedSolution;
    QString m_question;
    QList<int> m_positions;
    QPixmap m_pixmap;
    KRandomSequence m_randomSequence;
    QFont m_solutionFont;
};

}

#endif // PRACTICE_MIXEDLETTERSMODEWIDGET_H
