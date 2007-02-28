/***************************************************************************

              maintain list of lines from QMultiLineEdit

    -----------------------------------------------------------------------

    begin                : Sun Aug 13 10:00:53 MET 2000

    copyright            : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                           (C) 2001 The KDE-EDU team

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

#ifndef LineList_h
#define LineList_h

#include <QString>
#include <QStringList>

class LineList {
 public:
  LineList (const QString &multilines = "");

  void setLines(const QString &lines);
  void normalizeWS();
  int count() const;
  QString getLine (int index) const;
  QString allLines() const;

 protected:
  QStringList  multilines;
};

#endif // LineList_h
