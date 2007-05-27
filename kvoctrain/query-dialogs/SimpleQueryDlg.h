/***************************************************************************

                    query dialog for properties

    -----------------------------------------------------------------------

    begin          : Sun Apr 9 18:55:34 2000

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include "ui_SimpleQueryDlgForm.h"
#include "QueryDlgBase.h"

class SimpleQueryDlg : public QueryDlgBase
{
    Q_OBJECT

public:
    SimpleQueryDlg(QWidget *parent);

    ~SimpleQueryDlg();

    void setQuery(KVTQuery::QueryType querytpe,
                  int entry,
                  int column,
                  int q_cycle,
                  int q_num,
                  int q_start,
                  KEduVocDocument  *doc);

public slots:
    virtual void initFocus() const;

public slots:
    void showMoreClicked();
    void showSolution();
    void knowItClicked();
    void dontKnowClicked();
    void slotAnswerChanged();
    void slotUser1();
    void verifyClicked();
    void timeoutReached();

protected:
    QString answerstring;
    KVTQuery::QueryType querytype;

private:
    Ui::SimpleQueryDlgForm * mw;
    void setQueryFieldWordwrap();
};

#endif // SimpleQueryDlg_included
