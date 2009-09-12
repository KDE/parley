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


#ifndef FLASHCARDBACKENDMODE_H
#define FLASHCARDBACKENDMODE_H

#include "abstractbackendmode.h"

namespace Practice {

class FlashCardBackendMode : public AbstractBackendMode
{
public:
    FlashCardBackendMode(const PracticeOptions& options) : AbstractBackendMode(options) {}
    virtual qreal verifyAnswer ( const QVariant& answer );
    virtual QVariant solution();
    virtual QVariant question();
};

}

#endif // FLASHCARDBACKENDMODE_H
