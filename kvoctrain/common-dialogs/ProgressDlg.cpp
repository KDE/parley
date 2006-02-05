/***************************************************************************

                              progress dialog

    -----------------------------------------------------------------------

    begin         : Mon Aug 16 17:41:11 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <QProgressBar>
#include <QLabel>

#include <kstandarddirs.h>

#include <keduvocdocument.h>
#include "ProgressDlg.h"

ProgressDlg::ProgressDlg(const QString &doctitle, const QString &filename, const QString &title, QWidget* parent)
  : QWidget(parent)
{
  setupUi(this);
  doc = 0;
  setCaption(title);
  l_title->setText(doctitle);
  l_file->setText(filename);
  progress-> setMaximum(100);
}


void ProgressDlg::setValue(KEduVocDocument *new_doc, int val)
{
  progress->setValue(val);
  if (doc == 0 && new_doc != 0)
  {
    doc = new_doc;
    l_title->setText(doc->title());
    l_file->setText(doc->URL().fileName());
  }
}

#include "ProgressDlg.moc"
