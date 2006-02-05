/***************************************************************************

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin          : Thu Nov 25 11:45:53 MET 1999

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

#ifndef MCQuery_Dlg_H
#define MCQuery_Dlg_H

#include <QKeyEvent>

#include "MCQueryDlgForm.h"
#include "QueryDlgBase.h"

class MCQueryDlg : public QueryDlgBase
{
  Q_OBJECT

public:
  MCQueryDlg(
            QString org,
            QString trans,
            int entry,
            int orgcol,
            int transcol,
            int query_cycle,
            int query_num,
            int query_startnum,
            KEduVocExpression *exp,
            KEduVocDocument  *doc);

  ~MCQueryDlg();

  void setQuery(QString org,
                QString trans,
                int entry,
                int orgcol,
                int transcol,
                int query_cycle,
                int query_num,
                int query_startnum,
                KEduVocExpression *exp,
                KEduVocDocument  *doc);

public slots:
  virtual void initFocus() const;

protected:
  void keyPressEvent(QKeyEvent *e);

public slots:
  void showItClicked();
  void knowItClicked();
  void dontKnowClicked();
  void timeoutReached();
  void slotUser2();
  void trans1clicked();
  void trans2clicked();
  void trans3clicked();
  void trans4clicked();
  void trans5clicked();
  void verifyClicked();

protected:
  int solution;
  vector<RB_Label> button_ref;

private:
  Ui::MCQueryDlgForm * mw;
};

#endif // MCQuery_Dlg_H
