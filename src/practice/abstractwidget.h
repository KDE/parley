/***************************************************************************
    Copyright 2009 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PRACTICE_ABSTRACTWIDGET_H
#define PRACTICE_ABSTRACTWIDGET_H

#include "abstractfrontend.h"

#include <QtGui/QWidget>

class QVariant;

namespace Practice {

class AbstractModeWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractModeWidget(QWidget* parent = 0);
    virtual ~AbstractModeWidget() {}
    
    virtual void setQuestion(const QVariant& question) = 0;
    virtual void setSolution(const QVariant& solution) = 0;
    virtual QVariant userInput() = 0;
    virtual void setHint(const QVariant& hint) = 0;
    virtual void setFeedback(const QVariant& feedback) = 0;
    virtual void setResultState(AbstractFrontend::ResultState resultState) = 0;

    virtual void setQuestionSound(const KUrl& soundUrl) = 0;
    virtual void setSolutionSound(const KUrl& soundUrl) = 0;
    virtual void setSolutionPronunciation(const QString& pronunciationText) = 0;
    virtual void setQuestionPronunciation(const QString& pronunciationText) = 0;
    
public slots:
    virtual void showQuestion() = 0;
    virtual void showSolution() = 0;
    
signals:
    void continueAction();
    void hintAction();
    void skipAction();

};

}

#endif // PRACTICE_ABSTRACTWIDGET_H
