/***************************************************************************

                      general options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Mar 29 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter.hedlund@kdemail.net

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GENERALOPTIONS_H
#define GENERALOPTIONS_H

#include "generaloptionsbase.h"

class GeneralOptions : public GeneralOptionsBase
{
  Q_OBJECT

public:
  GeneralOptions(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
};

#endif

