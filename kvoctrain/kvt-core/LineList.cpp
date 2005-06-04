/***************************************************************************

                 maintain list of lines from QMultiLineEdit

    -----------------------------------------------------------------------

    begin                : Sun Aug 13 10:00:53 MET 2000

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "LineList.h"

#include <kdebug.h>

LineList::LineList (const QString &multilines)
{
   setLines (multilines);
}


void LineList::setLines(const QString &the_lines )
{
  multilines.clear();
  QString lines = the_lines;
  int pos;
  if ((pos = lines.find ('\n')) >= 0) {
    while (pos >= 0) {
      multilines.push_back(lines.left(pos));
      lines.remove (0, pos+1);
      pos = lines.find ('\n');
    }
  }

  if (lines.length() > 0)   // empty string at end ==> not an entry!
    multilines.push_back(lines);

  normalizeWS();
}


void LineList::normalizeWS()
{
  for (int i = 0; i < (int) multilines.size(); i++) {
    multilines[i] = multilines[i].stripWhiteSpace();
// FIXME: more tolerance with WS?  2*SPC = 1*SPC...
  }
}


int LineList::count() const
{
  return  multilines.size();
}


QString LineList::getLine (int index) const
{
   if (index < (int) multilines.size())
     return multilines[index];
   else {
     kdError() << "LineList::getLine: index too big";
     return "";
   }
}


QString LineList::allLines() const
{
   QString ret;
   if (multilines.size() > 0)
     ret = multilines[0];

   if (multilines.size() > 1) {
     for (int i = 1; i < (int) multilines.size(); i++)
       ret += "\n" + multilines[i];
   }
   return ret;
}

