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


#include "abstractbackendmode.h"

using namespace Practice;

AbstractBackendMode::AbstractBackendMode(const PracticeOptions& practiceOptions, Practice::AbstractFrontend* frontend, QObject *parent)
    :QObject(parent)
    ,m_practiceOptions(practiceOptions)
    ,m_frontend(frontend)
{}

#include "abstractbackendmode.moc"
