/***************************************************************************

    $Id$

                    slightly modified QProgressBar

    -----------------------------------------------------------------------

    begin                : Sun Apr 22 18:55:34 2000

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2001/12/07 19:21:36  arnold
    included multiple choice fields and false friend into query

    Revision 1.3  2001/11/24 17:16:08  arnold
    fixes for table view and query

    Revision 1.2  2001/11/10 22:29:40  arnold
    removed compatibility for kde1

    Revision 1.1  2001/10/05 15:45:05  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "MyProgress.h"

#include <qpainter.h>
#include <qframe.h>

#include <kstandarddirs.h>
#include <klocale.h>

#include <iostream.h>

#define Inherited QFrame

MyProgressBar::MyProgressBar
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name)
{
   QFrame::setFrameStyle( WinPanel | Sunken  );
   QFrame::setLineWidth( 2 );
   all = 100;
   current = 0;
   from = false;
   setMinimumWidth(70);
   setMinimumHeight(30);
}


void MyProgressBar::setData (int _all, int _current, bool x_from_y)
{
  if (_all >= 0 )
    all = _all;

  if (_current >= 0)
    current = _current;

  from = x_from_y;
}


unsigned MyProgressBar::getPercentage() const
{
  if (from) {
    if (current > 0 && all > 0)
      return (current/all) * 100;
    else
      return 0;
  }
  else
    return 0;
}


void MyProgressBar::paintEvent (QPaintEvent *pev)
{
  QFrame::paintEvent (pev);

  if (!isEnabled() )
    return;

  QString s, s2;
  s.setNum (current);
  if (from) {
    s2.setNum (all);
    s += "/"+s2;
  }

  const QRect bar = contentsRect();
  QPainter painter (this);

  QFont ft = painter.font();
  ft.setWeight(QFont::DemiBold);
  painter.setFont(ft);

  if (all) { // Sanity check
      int pw = bar.width() * current / all;

      painter.setPen( colorGroup().highlightedText() );
      painter.setClipRect( bar.x(), bar.y(), pw, bar.height() );

      painter.fillRect( bar, colorGroup().brush( QColorGroup::Highlight ) );
      painter.drawText( bar, AlignCenter, s );
      painter.setClipRect( bar.x()+pw, bar.y(), bar.width()-pw, bar.height() );
  }
  if ( current != all ) {
      painter.fillRect( bar, colorGroup().brush( QColorGroup::Base ) );
  }
  painter.setPen( colorGroup().text() );

  painter.drawText( bar, AlignCenter, s );
}
