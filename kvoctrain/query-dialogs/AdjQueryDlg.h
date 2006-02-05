/***************************************************************************

                   query dialog for adjectives

    -----------------------------------------------------------------------

    begin          : Sat Dec 4 15:09:18 1999

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


#ifndef AdjQueryDlg_included
#define AdjQueryDlg_included

#include <QKeyEvent>

#include "AdjQueryDlgForm.h"
#include "QueryDlgBase.h"

class AdjQueryDlg : public QueryDlgBase
{
  Q_OBJECT

public:
  AdjQueryDlg
  (
    QString type,
    int entry,
    int col,
    int query_cycle,
    int query_num,
    int query_startnum,
    KEduVocExpression *exp,
    KEduVocDocument *doc,
    const KEduVocComparison &comp);

  ~AdjQueryDlg();

  void setQuery(QString type,
              int entry,
              int col,
              int query_cycle,
              int query_num,
              int query_startnum,
              KEduVocExpression *exp,
              KEduVocDocument *doc,
              const KEduVocComparison &comp);

public slots:
  virtual void initFocus() const;

protected:
  void keyPressEvent(QKeyEvent *e);
  void resetAllFields();

protected slots:
  void lev1Changed(const QString&);
  void lev2Changed(const QString&);
  void lev3Changed(const QString&);
  void showAllClicked();
  void slotUser2();
  void knowItClicked();
  void returnPressed();
  void dontKnowClicked();
  void verifyClicked();
  void timeoutReached();

protected:
  KEduVocComparison  comp;

private:
  Ui::AdjQueryDlgForm * mw;
};
#endif // AdjQueryDlg_included
