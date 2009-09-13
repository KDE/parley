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


#ifndef WRITTENBACKENDMODE_H
#define WRITTENBACKENDMODE_H

#include "abstractbackendmode.h"

namespace Practice {

class WrittenBackendMode : public AbstractBackendMode
{
    Q_OBJECT
public:
    WrittenBackendMode(const PracticeOptions& practiceOptions, AbstractFrontend *frontend, QObject *parent);
    
    virtual qreal verifyAnswer(const QVariant& answer);
    virtual QVariant solution();
    virtual QVariant question();
};

}

#endif // WRITTENBACKENDMODE_H
