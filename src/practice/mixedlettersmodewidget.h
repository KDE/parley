/*
    SPDX-FileCopyrightText: 2010 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
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
