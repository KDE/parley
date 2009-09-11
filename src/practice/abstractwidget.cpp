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

#include "abstractwidget.h"
#include <kdebug.h>

using namespace Practice;

AbstractModeWidget::AbstractModeWidget(AbstractBackend *backend, QWidget* parent)
    : QWidget(parent)
{
    Q_UNUSED(backend);
}
