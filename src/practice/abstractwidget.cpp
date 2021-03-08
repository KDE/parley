/*
    SPDX-FileCopyrightText: 2009 Daniel Laidig <d.laidig@gmx.de>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "abstractwidget.h"

using namespace Practice;

AbstractModeWidget::AbstractModeWidget(GuiFrontend *frontend, QWidget* parent)
    : QWidget(parent), m_frontend(frontend)
{

}

void AbstractModeWidget::setResultPalettes(const QPalette &correctPalette, const QPalette &wrongPalette)
{
    m_correctPalette = correctPalette;
    m_wrongPalette = wrongPalette;
}
