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

#ifndef PRACTICE_WRITTENPRACTICEWIDGET_H
#define PRACTICE_WRITTENPRACTICEWIDGET_H

#include "abstractwidget.h"

namespace Ui {
class WrittenPracticeWidget;
}


namespace Practice {

class WrittenPracticeWidget : public Practice::AbstractModeWidget
{
    Q_OBJECT

public:
    WrittenPracticeWidget(GuiFrontend *frontend, QWidget *parent = 0);
    virtual QVariant userInput();

    virtual void setQuestion(const QVariant& question);
    virtual void setSolution(const QVariant& solution);
    virtual void setHint(const QVariant& hint);
    virtual void setFeedback(const QVariant& feedback);
    virtual void setResultState(AbstractFrontend::ResultState resultState);

    virtual void setQuestionSound(const KUrl& soundUrl);
    virtual void setSolutionSound(const KUrl& soundUrl);
    virtual void setSolutionPronunciation(const QString& pronunciationText);
    virtual void setQuestionPronunciation(const QString& pronunciationText);
    
public slots:
    void showQuestion();
    void showSolution();
    void continueClicked();

protected:
    Ui::WrittenPracticeWidget* m_ui;

private:
    QPalette m_correctPalette;
    QPalette m_wrongPalette;
    QString m_solution;
    AbstractFrontend::ResultState m_resultState;
};

}

#endif // PRACTICE_WRITTENPRACTICEWIDGET_H
