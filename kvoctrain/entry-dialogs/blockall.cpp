/***************************************************************************

                     dummy validator which blocks all

    -----------------------------------------------------------------------

    begin                : Sat Mar 25 15:02:16 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

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

#include "blockall.h"

BlockAllValidator::BlockAllValidator() : QValidator (0, 0)
{
}


BlockAllValidator::State BlockAllValidator::validate (QString&, int& ) const
{
  return Invalid;
}


