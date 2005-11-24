/***************************************************************************

                            progress dialog

    -----------------------------------------------------------------------

    begin          : Mon Aug 16 17:41:11 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef ProgressDlg_included
#define ProgressDlg_included

#include "ProgressDlgForm.h"

class kvoctrainDoc;

class ProgressDlg : public QWidget, public Ui::ProgressDlgForm
{
  Q_OBJECT
public:
  ProgressDlg(const QString &doctitle, const QString &filename, const QString &title, QWidget* parent = 0);

public slots:
  void setValue(kvoctrainDoc *doc, int);

protected:
  kvoctrainDoc *doc;
};

#endif // ProgressDlg_included
