/***************************************************************************

    $Id$

                    display special spinbox

    -----------------------------------------------------------------------

    begin                : Sat Oct 21 18:02:16 1999
                                           
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


#include <qspinbox.h>

class QStringlist;

#ifndef MySpinbox_included
#define MySpinbox_included

  class MySpinBox : public QSpinBox {
  public:
    MySpinBox (QWidget* parent = 0, const char* name = 0);

    void setSpecial(const QString &str);
    void setData (QStringList *names, int minValue, int maxValue);
    virtual QString mapValueToText( int value );
    virtual int	    mapTextToValue( bool* ok );

  protected:
    QStringList    *spin_names;
    QString         special_str;
  };

#endif // MySpinbox_included
