/***************************************************************************

    $Id$

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 11:45:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

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

#include "MCQueryDlgForm.h"
#include "QueryDlgBase.h"

/* Library Includes */

class kvoctrainExpr;
class kvoctrainDoc;

class MCQueryDlg : public MCQueryDlgForm,
		   public QueryDlgBase
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
                   QFont font,
                   kvoctrainExpr *exp,
                   kvoctrainDoc  *doc,
                   int mqtime,
                   bool show,
                   QWidget *parent=0,
                   char *name=0);

	void setQuery(QString org,
                      QString trans,
                      int entry,
                      int orgcol,
                      int transcol,
                      int query_cycle,
                      int query_num,
                      int query_startnum,
                      kvoctrainExpr *exp,
                      kvoctrainDoc  *doc,
                      int   mqtime,
                      bool show);

public slots:
        virtual void initFocus() const;

signals:
   void sigQueryChoice(QueryDlgBase::Result userchoice);
   void sigEditEntry(int row, int col);

protected:
        void keyPressEvent( QKeyEvent *e );

public slots:
        void showItClicked();
        void knowItClicked();
        void dontKnowClicked();
        void stopItClicked();
        void timeoutReached();
        void editEntryClicked();
        void trans1clicked();
        void trans2clicked();
        void trans3clicked();
        void trans4clicked();
        void trans5clicked();
        void verifyClicked();

protected:
    virtual void closeEvent (QCloseEvent*e);

  int            solution;
  vector<RB_Label> button_ref;
};

#endif // MCQuery_Dlg_H
