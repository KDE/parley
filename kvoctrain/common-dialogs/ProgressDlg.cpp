/***************************************************************************

                              progress dialog

    -----------------------------------------------------------------------

    begin                : Mon Aug 16 17:41:11 1999

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

#undef stack
#include <kvoctraindoc.h>
#include "ProgressDlg.h"

#include <qprogressbar.h>
#include <qlabel.h>

#include <kstandarddirs.h>


ProgressDlg::ProgressDlg(const QString &doctitle, const QString &filename, const QString &title, QWidget* parent, const char* name)
  : ProgressDlgForm( parent, name )
{
  doc = 0;
  setCaption (title);
  l_title->setText (doctitle);
  l_file->setText (filename);
  progress-> setTotalSteps(100);
}


void ProgressDlg::setValue( kvoctrainDoc *new_doc, int val)
{
  progress->setProgress(val);
  if (doc == 0 && new_doc != 0)
  {
    doc = new_doc;
    l_title->setText (doc->getTitle());
    l_file->setText (doc->URL().fileName());
  }
}


#include "ProgressDlg.moc"
