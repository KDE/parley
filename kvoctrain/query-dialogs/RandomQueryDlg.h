/***************************************************************************

    $Id$

                   dialog when in random query mode

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
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


#ifndef Query_Dlg_H
#define Query_Dlg_H

#include "RandomQueryDlgForm.h"
#include "QueryDlgBase.h"

/* Library Includes */

#include <qcombobox.h>
#include <qlineedit.h>

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
		 QFont & font,
                 kvoctrainExpr *exp,
                 kvoctrainDoc  *,
                 int mqtime,
                 bool show,
                 kvq_timeout_t type_timeout,
                 bool _suggestions,
                 bool _split,
                 bool _periods,
                 bool _colons,
                 bool _semicolons,
                 bool _commas,
                 int  _fields,
                 bool _show_more,
                 bool _i_know,
                 bool _swap,
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

signals:
   void sigQueryChoice(QueryDlgBase::Result userchoice);
   void sigEditEntry(int row, int col);

public slots:
    void verifyClicked();
    void showMoreClicked();
    void showAllClicked();
    void knowItClicked();
    void dontKnowClicked();
    void stopItClicked();
    void timeoutReached();
    void editEntryClicked();
    void slotTransChanged(const QString&);
    void slotTransLostFocus();
    void slotFFClicked();
    void slotTypeClicked();
    void slotRemClicked();

protected:
    virtual void keyPressEvent( QKeyEvent *e );
    virtual void closeEvent (QCloseEvent*e);
    void setHintFields();
    QStringList extractTranslations (QString trans);

    kvoctrainDoc *kv_doc;

    QPtrList<QComboBox> transCombos;
    QPtrList<QLineEdit> transFields;

    QStringList translations;
    QStringList vocabulary;

    bool suggestion_hint;
    bool suggestions;
    bool split;
    bool periods;
    bool colons;
    bool semicolons;
    bool commas;
    int  fields;
};

#endif
