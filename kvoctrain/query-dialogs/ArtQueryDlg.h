/***************************************************************************

    $Id$

                     query dialog for articles

    -----------------------------------------------------------------------

    begin                : Fri Dec 3 18:28:18 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ArtQueryDlg_included
#define ArtQueryDlg_included

#include "ArtQueryDlgData.h"
#include <GrammerManager.h>

struct SpecFont_t;

class ArtQueryDlg : public ArtQueryDlgData
{
    Q_OBJECT

public:

    ArtQueryDlg(
                SpecFont_t *font,
                QString type,
                int entry,
                int col,
                int query_cycle,
                int query_num,
                int query_startnum,
                kvoctrainExpr *exp,
                kvoctrainApp  *kv_app,
                kvoctrainDoc  *doc,
                const Article &articles,
                int   mqtime,
                bool show,
                kvq_timeout_t type_timeout,
                QWidget *parent=0,
                char    *name=0);

    void setQuery (QString type,
                int entry,
                int col,
                int query_cycle,
                int query_num,
                int query_startnum,
                kvoctrainExpr *exp,
                kvoctrainApp  *kv_app,
                kvoctrainDoc  *doc,
                const Article &articles,
                int   mqtime,
                bool show,
                kvq_timeout_t type_timeout);

public slots:
    virtual void initFocus() const;

protected:
    void keyPressEvent( QKeyEvent *e );

protected slots:

    void stopItClicked();
    void showMoreClicked();
    void showAllClicked();
    void slotFemClicked();
    void editClicked();
    void knowItClicked();
    void slotNaturalClicked();
    void slotMaleClicked();
    void dontKnowClicked();
    void optionsClicked();
    void verifyClicked();
    void returnPressed();
    void timeoutReached();

protected:

    Article   articles;

};
#endif // ArtQueryDlg_included
