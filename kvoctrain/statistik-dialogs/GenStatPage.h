/***************************************************************************

                      general statistics dialog page

    -----------------------------------------------------------------------

    begin          : Thu Sep 21 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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


#ifndef GenStatPage_included
#define GenStatPage_included

#include "GenStatPageForm.h"

class KEduVocDocument;

class GenStatPage : public QWidget, public Ui::GenStatPageForm
{
  Q_OBJECT
public:
  GenStatPage(KEduVocDocument *doc, QWidget* parent = 0);
};

#endif // GenStatPage_included
