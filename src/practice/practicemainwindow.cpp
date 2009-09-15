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

#include "practicemainwindow.h"

#include <KDebug>

using namespace Practice;

void PracticeMainWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        if (focusWidget()->metaObject()->className() == QLatin1String("QRadioButton")) {
            kDebug() << "enter on radio";
            emit enterPressed();
            return;
        }
    }
    KXmlGuiWindow::keyPressEvent(e);
}

#include "practicemainwindow.moc"