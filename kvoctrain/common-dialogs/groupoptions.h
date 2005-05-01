/***************************************************************************

                      group options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter@peterandlinda.com

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

#ifndef GROUPOPTIONS_H
#define GROUPOPTIONS_H

#include "groupoptionsbase.h"

class GroupOptions : public GroupOptionsBase
{
  Q_OBJECT

public:
  GroupOptions(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

  void updateButtons();
};

#endif

