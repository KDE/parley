/***************************************************************************

                      query options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

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

#ifndef QUERYOPTIONS_H
#define QUERYOPTIONS_H

#include "queryoptionsbase.h"

class QueryOptions : public QueryOptionsBase
{
  Q_OBJECT

public:
  QueryOptions(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );

  void updateWidgets();
};

#endif

