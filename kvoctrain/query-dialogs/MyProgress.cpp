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

#include <kstddirs.h>
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
}


void MyProgressBar::setData (int _all, int _current, bool x_from_y)
{
  if (_all >= 0 )
    all = _all;

  if (_current >= 0)
    current = _current;

  from = x_from_y;
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
