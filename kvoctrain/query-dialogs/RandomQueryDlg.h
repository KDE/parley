/***************************************************************************

    $Id$

                   dialog when in random query mode

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/28 09:17:06  arnold
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


#ifndef Query_Dlg_H
#define Query_Dlg_H

#include "RandomQueryDlgForm.h"
#include "QueryDlgBase.h"

/* Library Includes */

#include <qdialog.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qmultilineedit.h>

class kvoctrainDoc;

class RandomQueryDlg : public QueryDlgForm,
                       public QueryDlgBase
{
  Q_OBJECT

public:
	RandomQueryDlg(
                 QString org,
                 QString trans,
                 int entry,
                 int orgcol,
                 int transcol,
                 int query_cycle,
                 int query_num,
                 int query_startnum,
                 kvoctrainExpr *exp,
                 kvoctrainDoc  *,
                 int mqtime,
                 bool show,
                 kvq_timeout_t type_timeout,
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
                      kvoctrainDoc  *,
                      int   mqtime,
                      bool show,
                      kvq_timeout_t type_timeout);

public slots:
    virtual void initFocus() const;

protected:
    void keyPressEvent( QKeyEvent *e );

signals:
    void sigEditEntry(int row, int col);

public slots:
    void verifyClicked();
    void showMoreClicked();
    void showAllClicked();
    void knowItClicked();
    void dontKnowClicked();
    void stopItClicked();
    void timeoutReached();
    void editOrgClicked();
    void editTransClicked();
    void slotTransChanged(const QString&);
    void slotFFClicked();
    void slotTypeClicked();
    void slotRemClicked();

protected:
  kvoctrainDoc *kv_doc;
};

#endif
