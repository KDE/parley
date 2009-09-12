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


#include "flashcardbackendmode.h"
 
using namespace Practice;
 
qreal FlashCardBackendMode::verifyAnswer ( const QVariant& answer )
{
    return 0.0;
}

QVariant FlashCardBackendMode::solution()
{
    return m_current->entry()->translation(m_options.languageTo())->text();
}

QVariant FlashCardBackendMode::question()
{
    return m_current->entry()->translation(m_options.languageFrom())->text();
}

