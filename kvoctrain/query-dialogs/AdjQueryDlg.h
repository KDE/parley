/***************************************************************************

    $Id$

                   query dialog for adjectives

    -----------------------------------------------------------------------

    begin                : Sat Dec 4 15:09:18 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2001/10/28 10:16:31  arnold
    quick 'n dirty fixes for new query dialogs

    Revision 1.3  2001/10/28 09:17:06  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.2  2001/10/17 21:41:15  waba
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


#ifndef AdjQueryDlg_included
#define AdjQueryDlg_included

#include "AdjQueryDlgForm.h"
#include "QueryDlgBase.h"
#include <GrammerManager.h>

class AdjQueryDlg : public AdjQueryDlgForm,
    		    public QueryDlgBase
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
        kvoctrainExpr *exp,
        kvoctrainDoc  *doc,
        const Comparison &comp,
        int   mqtime,
        bool show,
        kvq_timeout_t type_timeout,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    void setQuery (QString type,
                int entry,
                int col,
                int query_cycle,
                int query_num,
                int query_startnum,
                kvoctrainExpr *exp,
                kvoctrainDoc  *doc,
                const Comparison &comp,
                int   mqtime,
                bool show,
                kvq_timeout_t type_timeout);

public slots:
    virtual void initFocus() const;

protected:
    void keyPressEvent( QKeyEvent *e );
    void resetAllFields();

signals:
    void sigEditEntry(int row, int col);

protected slots:

    void lev1Changed(const QString&);
    void lev2Changed(const QString&);
    void lev3Changed(const QString&);
    void stopItClicked();
    void showAllClicked();
    void editClicked();
    void knowItClicked();
    void returnPressed();
    void dontKnowClicked();
    void verifyClicked();
    void timeoutReached();

protected:

    Comparison  comp;
};
#endif // AdjQueryDlg_included
