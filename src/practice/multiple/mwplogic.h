//
// C++ Interface: mwpcomparison
//
// Description:
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


#ifndef MWPLOGIC_H
#define MWPLOGIC_H



#include "../practiceentry.h"

#include <QStringList>
#include <QMap>
#include <QList>

class QLineEdit;

class MWPLogic
{
        public:

        virtual void setup(const QStringList& elementids, const QList<QLineEdit*> kids, const QMap<QString, QLineEdit*> map) = 0;
        virtual void clear() = 0;
        virtual void setTexts(const QStringList& texts) = 0;
        virtual void setEntry(const PracticeEntry* entry) = 0;
        virtual QStringList answer() = 0;
        virtual ~MWPLogic() {};
};

#endif
