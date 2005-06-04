/***************************************************************************

              manage multiple choice suggestions for queries

    -----------------------------------------------------------------------

    begin                : Mon Oct 29 18:09:29 1999

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

#include "MultipleChoice.h"

MultipleChoice::MultipleChoice (
           const QString &mc1,
           const QString &mc2,
           const QString &mc3,
           const QString &mc4,
           const QString &mc5
          )
{
   setMC1 (mc1);
   setMC2 (mc2);
   setMC3 (mc3);
   setMC4 (mc4);
   setMC5 (mc5);
}


bool MultipleChoice::isEmpty() const
{
  return   muc1.stripWhiteSpace().isEmpty()
        && muc2.stripWhiteSpace().isEmpty()
        && muc3.stripWhiteSpace().isEmpty()
        && muc4.stripWhiteSpace().isEmpty()
        && muc5.stripWhiteSpace().isEmpty();
}


void MultipleChoice::clear()
{
   muc1 = "";
   muc2 = "";
   muc3 = "";
   muc4 = "";
   muc5 = "";
}


QString MultipleChoice::mc (unsigned idx) const
{
   switch (idx) {
     case 0: return muc1;
     case 1: return muc2;
     case 2: return muc3;
     case 3: return muc4;
     case 4: return muc5;
   }
   return "";
}


unsigned MultipleChoice::size()
{
   normalize();
   unsigned num = 0;
   if (!muc1.isEmpty() )
     ++num;
   if (!muc2.isEmpty() )
     ++num;
   if (!muc3.isEmpty() )
     ++num;
   if (!muc4.isEmpty() )
     ++num;
   if (!muc5.isEmpty() )
     ++num;
   return num;
}


void MultipleChoice::normalize()
{
  // fill from first to last

  if (muc1.isEmpty()) {
    muc1 = muc2;
    muc2 = "";
  }

  if (muc2.isEmpty()) {
    muc2 = muc3;
    muc3 = "";
  }

  if (muc3.isEmpty()) {
    muc3 = muc4;
    muc4 = "";
  }

  if (muc4.isEmpty()) {
    muc4 = muc5;
    muc5 = "";
  }

}
