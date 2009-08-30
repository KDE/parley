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

#include "guifrontend.h"

#include <kxmlguiwindow.h>

namespace Practice {
class MainWindow : public KXmlGuiWindow
{
public:
    MainWindow(QWidget* parent = 0, Qt::WindowFlags f = 0)
        : KXmlGuiWindow(parent, f) {};
};
}

using namespace Practice;

GuiFrontend::GuiFrontend(AbstractBackend* backend, QObject* parent)
    : AbstractFrontend(backend, parent)
{
    m_mainWindow = new MainWindow();
}

QVariant GuiFrontend::userInput()
{
    return QVariant("Baum");
}

KXmlGuiWindow* GuiFrontend::getWindow()
{
    return m_mainWindow;
}
