/***************************************************************************

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin          : Mon Oct 29 18:09:29 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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

#ifndef MCEntryPage_included
#define MCEntryPage_included

#include "MCEntryPageForm.h"
#include <MultipleChoice.h>

class MCEntryPage : public QWidget, public Ui::MCEntryPageForm
{
  Q_OBJECT

public:
  MCEntryPage(bool multi_sel, const MultipleChoice &mc, QWidget *parent = 0);

  void setData(bool multi_sel, const MultipleChoice &mc);

  MultipleChoice getMultipleChoice() const { return multiplechoice; }

  bool isModified();
  void setModified(bool mod = true);
  void setEnabled(int enable_type);

signals:
  void sigModified();

protected slots:
  void mc1Changed(const QString&);
  void mc2Changed(const QString&);
  void mc3Changed(const QString&);
  void mc4Changed(const QString&);
  void mc5Changed(const QString&);

protected:
  MultipleChoice  multiplechoice;
  bool            modified;
};

#endif // MCEntryPage_included
