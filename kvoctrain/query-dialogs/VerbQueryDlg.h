/***************************************************************************

    $Id$

                    query dialog for verbs

    -----------------------------------------------------------------------

    begin                : Fri Dec 3 18:28:18 1999

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


#ifndef VerbQueryDlg_included
#define VerbQueryDlg_included

#include "VerbQueryDlgForm.h"
#include "QueryDlgBase.h"

#include <GrammerManager.h>

class VerbQueryDlg : public VerbQueryDlgForm,
                     public QueryDlgBase
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
                     kvoctrainExpr *exp,
                     kvoctrainDoc  *doc,
                     const Conjugation &prefix,
                     const Conjugation &conjug,
                     int   mqtime,
                     bool show,
                     kvq_timeout_t type_timeout,
                     QWidget *parent=0,
                     char    *name=0);

	void setQuery(QString type,
                      int entry,
                      int col,
                      int query_cycle,
                      int query_num,
                      int query_startnum,
                      kvoctrainExpr *exp,
                      kvoctrainDoc  *doc,
                      const Conjugation &prefix,
                      const Conjugation &conjug,
                      int   mqtime,
                      bool show,
                      kvq_timeout_t type_timeout);

public slots:
    virtual void initFocus() const;

signals:
   void sigQueryChoice(QueryDlgBase::Result userchoice);
   void sigEditEntry(int row, int col);

protected:
    bool next();
    void keyPressEvent( QKeyEvent *e );
    void resetAllFields();

protected slots:
    void editClicked();
    void stopItClicked();
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
    virtual void closeEvent (QCloseEvent*e);

    int         current;
    Conjugation conjugations;
    bool        all_known;
    int         query_time;
};
#endif // VerbQueryDlg_included
