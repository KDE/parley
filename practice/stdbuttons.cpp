//
// C++ Implementation: stdbuttons
//
// Description: Implements a set of standard buttons to control the the practice session
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <KDebug>
#include <QAbstractButton>
#include <QPushButton>
#include <QList>

#include "stdbuttons.h"
#include "statistics.h"

StdButtons::StdButtons(QWidget * parent)
        : KDialogButtonBox(parent)
{
    // I realize I'm kinda abusing the ButtonRoles
    addButton("Check Answer", QDialogButtonBox::ActionRole);
    addButton("Skip (Answer Known)", QDialogButtonBox::YesRole);
    addButton("Skip (Answer Not Known)", QDialogButtonBox::NoRole);
    connect(this, SIGNAL(clicked(QAbstractButton*)), this, SLOT(slotButtonClicked(QAbstractButton*)));
}

void StdButtons::slotButtonClicked(QAbstractButton * button)
{
    if (button->text() == "Check Answer")
    {
        // Continue only shows up after they checked the answer
        button->setText("Continue");
        emit signalCheckAnswer();
    }
    else if (button->text() == "Continue")
    {
        button->setText("Check Answer");
        emit signalContinue();
    }
    else if (button->text() == "Skip (Answer Known)")
    {
        emit signalSkipped(Statistics::Known);
    }
    else if (button->text() == "Skip (Answer Not Known)")
    {
        emit signalSkipped(Statistics::Unknown);
    }
    else
    {
        kDebug() << "slotButtonClicked recieved unhandled button " << button->text();
    }
}

void StdButtons::slotReturnPressed()
{
    QList<QAbstractButton*> qlb = buttons();
    QAbstractButton* b =  qlb[0];
    if (b->text() == "Check Answer")
    {
        b->setText("Continue");
        emit signalCheckAnswer();
    }
    else if (b->text() == "Continue")
    {
        b->setText("Check Answer");
        emit signalContinue();
    }
    else
    {
        kDebug() << "unknown button text " << b->text();
    }
}

void StdButtons::slotAnswerShown()
{
    QList<QAbstractButton*> qlb = buttons();
    QAbstractButton* b =  qlb[0];
    if (b->text() == "Check Answer")
    {
        // showing the answer removes their ability to provide an answer (duh!)
        b->setText("Continue");
    }
}
