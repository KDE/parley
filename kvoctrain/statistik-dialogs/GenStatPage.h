/***************************************************************************

    $Id$

                      general statistics dialog page

    -----------------------------------------------------------------------

    begin                : Thu Sep 21 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:47:12  arnold
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


#ifndef GenStatPage_included
#define GenStatPage_included

#include "GenStatPageForm.h"

class kvoctrainDoc;

class GenStatPage : public GenStatPageForm
{
    Q_OBJECT

public:

    GenStatPage
    (
        kvoctrainDoc *doc,
        QWidget* parent = NULL,
        const char* name = NULL
    );

protected:
  void keyPressEvent( QKeyEvent *e );

};
#endif // GenStatPage_included
