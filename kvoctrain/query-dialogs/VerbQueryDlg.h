/***************************************************************************

                    query dialog for verbs

    -----------------------------------------------------------------------

    begin          : Fri Dec 3 18:28:18 1999

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

#ifndef VerbQueryDlg_included
#define VerbQueryDlg_included

#include <QKeyEvent>

#include "VerbQueryDlgForm.h"
#include "QueryDlgBase.h"

class VerbQueryDlg : public QueryDlgBase
{
  Q_OBJECT

public:

  VerbQueryDlg(
                QString type,
                int entry,
                int col,
                int query_cycle,
                int query_num,
                int query_startnum,
                KEduVocExpression *exp,
                KEduVocDocument  *doc,
                const KEduVocConjugation &prefix,
                const KEduVocConjugation &conjug);

  ~VerbQueryDlg();

  void setQuery(QString type,
                int entry,
                int col,
                int query_cycle,
                int query_num,
                int query_startnum,
                KEduVocExpression *exp,
                KEduVocDocument  *doc,
                const KEduVocConjugation &prefix,
                const KEduVocConjugation &conjug);

public slots:
  virtual void initFocus() const;

protected:
  bool next();
  void keyPressEvent( QKeyEvent *e );
  void resetAllFields();

protected slots:
  void slotUser2();
  void slotP3pfChanged(const QString&);
  void slotP3snChanged(const QString&);
  void showAllClicked();
  void slotReturnPressed();
  void slotP3smChanged(const QString&);
  void knowItClicked();
  void dontKnowClicked();
  void slotP3pnChanged(const QString&);
  void slotP3sfChanged(const QString&);
  void slotP1sChanged(const QString&);
  void slotP2sChanged(const QString&);
  void slotP3pmChanged(const QString&);
  void slotP1pChanged(const QString&);
  void slotP2pChanged(const QString&);
  void verifyClicked();
  void timeoutReached();

protected:
  int current;
  KEduVocConjugation conjugations;
  bool all_known;

private:
  Ui::VerbQueryDlgForm * mw;
};

#endif // VerbQueryDlg_included
