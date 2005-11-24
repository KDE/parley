/***************************************************************************

                      view options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 31 2005

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

#ifndef VIEWOPTIONS_H
#define VIEWOPTIONS_H

#include "viewoptionsbase.h"

class ViewOptions : public QWidget, public Ui::ViewOptionsBase
{
  Q_OBJECT
public:
  ViewOptions(QWidget* parent = 0);
};

#endif
