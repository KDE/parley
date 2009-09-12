/***************************************************************************
    Copyright 2009 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef FLASHCARDMODEWIDGET_H
#define FLASHCARDMODEWIDGET_H

#include "abstractwidget.h"


namespace Ui {
    class FlashCardPracticeWidget;
}

namespace Practice {
    
class FlashCardModeWidget: public AbstractModeWidget
{    
    Q_OBJECT
    
public:
    FlashCardModeWidget(AbstractBackend *backend, QWidget *parent = 0);
    virtual QVariant userInput();
    
public Q_SLOTS:
    virtual void updateDisplay();
    virtual void continueClicked();
    
private:
    Ui::FlashCardPracticeWidget* m_ui;
};

}

#endif // FLASHCARDMODEWIDGET_H

