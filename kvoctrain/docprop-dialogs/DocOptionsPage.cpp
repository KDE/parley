/***************************************************************************

                   internal doc options dialog class

    -----------------------------------------------------------------------

    begin          : Thu Nov 25 12:00:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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


#include "DocOptionsPage.h"

#include <qcheckbox.h>

DocOptionsPage::DocOptionsPage(bool sort, QWidget *parent, const char *name):DocOptionsPageForm(parent, name)
{
  sorter = sort;
  docsort->setChecked(sort);

  connect( docsort, SIGNAL(toggled(bool)), SLOT(docSortToggled(bool)) );
}

void DocOptionsPage::docSortToggled(bool state)
{
  sorter = state;
}

#include "DocOptionsPage.moc"
