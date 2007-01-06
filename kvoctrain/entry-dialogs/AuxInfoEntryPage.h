/***************************************************************************

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 17:29:44 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef AuxInfoEntryPage_included
#define AuxInfoEntryPage_included

#include "ui_AuxInfoEntryPageForm.h"

class AuxInfoEntryPage : public QWidget, public Ui::AuxInfoEntryPageForm
{
  Q_OBJECT

public:
  AuxInfoEntryPage(QWidget *parent = 0);

  void setData(bool multi_sel, QString syno, QString anto, QString example, QString remark, QString para);

  QString getSynonym ();
  QString getAntonym ();
  QString getExample ();
  QString getRemark  ();
  QString getParaphrase();

  bool isModified();
  void setModified(bool mod = true);
  void setEnabled(int enable_type);

signals:
  void sigModified();

protected:
  void normalize(QString &str);

protected slots:
  void slotAntonymSelected();
  void slotSynonymSelected();
  void slotExampSelected();
  void slotRemarkSelected();
  void slotParaSelected();

protected:
  QString       synonym;
  QString       antonym;
  QString       example;
  QString       remark;
  QString       paraphrase;
  bool          modified;
};

#endif // AuxInfoEntryPage_included
