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

#include <QtGui/QWidget>
#include "abstractbackend.h"

class QVariant;

namespace Practice {

class AbstractModeWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractModeWidget(QWidget* parent = 0);
    virtual ~AbstractModeWidget() {}
    
    virtual void setQuestion(const QVariant& question) = 0;
    virtual QVariant userInput() = 0;

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
