/***************************************************************************

    $Id$

         display special spinbox (only meaningful for QT2x)

    -----------------------------------------------------------------------

    begin                : Sat Oct 21 18:02:16 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:40:37  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#include <qstringlist.h>

#include "MySpinBox.h"

MySpinBox::MySpinBox (QWidget* parent, const char* name ) :
    QSpinBox(parent, name ),
    spin_names (0)
{
  setWrapping(true);
}


void MySpinBox::setData (QStringList *names, int minValue, int maxValue)
{
   spin_names = names;
   setRange (minValue, maxValue);
}


QString MySpinBox::mapValueToText( int value )
{
  if (special_str.length() != 0 )
    return special_str;

  if (spin_names != 0)
    return (*spin_names)[value];
  else
    return QString("%1").arg(value);
}


int MySpinBox::MySpinBox::mapTextToValue( bool* ok )
{
  return 0;
}


void MySpinBox::setSpecial(const QString &str)
{
   special_str = str;
   setSuffix("");  // update display
}



