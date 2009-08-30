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

#ifndef PRACTICE_GUIFRONTEND_H
#define PRACTICE_GUIFRONTEND_H

#include "abstractfrontend.h"

class KXmlGuiWindow;

namespace Practice {

class MainWindow;

class GuiFrontend : public Practice::AbstractFrontend
{
    Q_OBJECT

public:
    GuiFrontend(AbstractBackend* backend, QObject* parent = 0);
    virtual QVariant userInput();
    QWidget* getWidget();
    KXmlGuiWindow* getWindow();

private:
    MainWindow* m_mainWindow;
};

}

#endif // PRACTICE_GUIFRONTEND_H
