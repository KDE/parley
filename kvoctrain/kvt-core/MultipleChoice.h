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


#ifndef MultipleChoice_included
#define MultipleChoice_included

#include <qstring.h>

#define MAX_MULTIPLE_CHOICE  5  // select one out of x

class MultipleChoice
{

public:

   MultipleChoice() {}

   MultipleChoice (
           const QString &mc1,
           const QString &mc2,
           const QString &mc3,
           const QString &mc4,
           const QString &mc5
          );

   void setMC1 (const QString &s) { muc1 = s; }
   void setMC2 (const QString &s) { muc2 = s; }
   void setMC3 (const QString &s) { muc3 = s; }
   void setMC4 (const QString &s) { muc4 = s; }
   void setMC5 (const QString &s) { muc5 = s; }

   QString mc1 () const { return muc1; }
   QString mc2 () const { return muc2; }
   QString mc3 () const { return muc3; }
   QString mc4 () const { return muc4; }
   QString mc5 () const { return muc5; }

   QString mc (unsigned idx) const;

   bool isEmpty() const;
   void normalize();
   void clear();
   unsigned size();

protected:

   QString  muc1, muc2, muc3, muc4, muc5;
};


#endif // MultipleChoice_included
