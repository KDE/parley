/***************************************************************************

    $Id$

                    query dialog for properties

    -----------------------------------------------------------------------

    begin                : Sun Apr 9 18:55:34 2000

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


#ifndef SimpleQueryDlg_included
#define SimpleQueryDlg_included

#include "SimpleQueryDlgForm.h"
#include "QueryDlgBase.h"
#include "MyProgress.h"

class SimpleQueryDlg : public SimpleQueryDlgForm,
                       public QueryDlgBase
{
    Q_OBJECT

public:

    SimpleQueryDlg
    (
        QueryType querytpe,
        int entry,
        int column,
        int q_cycle,
        int q_num,
        int q_start,
	QFont & font,
        kvoctrainExpr *exp,
        kvoctrainDoc  *doc,
        int mqtime,
        bool showcounter,
        kvq_timeout_t type_to,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    void setQuery(QueryType querytpe,
                  int entry,
                  int column,
                  int q_cycle,
                  int q_num,
                  int q_start,
                  kvoctrainExpr *exp,
                  kvoctrainDoc  *doc,
                  int mqtime,
                  bool showcounter,
                  kvq_timeout_t type_to);

public slots:
    virtual void initFocus() const;

signals:
    void sigQueryChoice(QueryDlgBase::Result userchoice);
    void sigEditEntry(int row, int col);

protected:
    void keyPressEvent( QKeyEvent *e );

public slots:
    void stopItClicked();
    void showMoreClicked();
    void showAllClicked();
    void knowItClicked();
    void dontKnowClicked();
    void slotAnswerChanged();
    void editClicked();
    void verifyClicked();
    void timeoutReached();

protected:
    virtual void closeEvent (QCloseEvent*e);

    QString        answerstring;
    QueryType      querytype;
};
#endif // SimpleQueryDlg_included
