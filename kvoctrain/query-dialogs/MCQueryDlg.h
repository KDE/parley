/***************************************************************************

    $Id$

                    multiple choice query dialog

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 11:45:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.5  2001/11/24 17:16:08  arnold
    fixes for table view and query

    Revision 1.4  2001/11/09 10:41:18  arnold
    removed ability to display a different font for each column

    Revision 1.3  2001/10/28 09:17:06  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.2  2001/10/17 21:41:16  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:45:05  arnold
    import of version 0.7.0pre8 to kde-edu


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
                   kvoctrainExpr *exp,
                   kvoctrainDoc  *doc,
                   int mqtime,
                   bool show,
                   kvq_timeout_t type_to,
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
                      bool show,
                      kvq_timeout_t type_timeout);

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

  int            solution;
  vector<RB_Label> button_ref;
};

#endif // MCQuery_Dlg_H
