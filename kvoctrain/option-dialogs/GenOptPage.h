/***************************************************************************

    $Id$

                     general options dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 18:08:56 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef GenOptPage_included
#define GenOptPage_included

#include "GenOptPageData.h"
#include "../kvoctrainview.h"

#include <compat_2x.h>

class GenOptPage : public GenOptPageData
{
    Q_OBJECT

public:

    GenOptPage
    (
        int         _btime,
        kvoctrainView::Resizer resizer,
        bool        _smartAppend,
        bool        _autosaveopts,
        QWidget    *parent = NULL,
        const char *name = NULL
    );

    ~GenOptPage ();

    int getBackupTime()       const { return btime; }
    int getSmartAppend()      const { return smart; }
    int getAutoSaveOpts()     const { return autosaveopts; }
    kvoctrainView::Resizer getResizer() const { return resizer; }

public slots:
    void initFocus() const;

protected:
    void keyPressEvent( QKeyEvent * );

protected slots:

    void slotHBfixed();
    void slotHBpercent();
    void slotHBauto();
    void slotChangeBTime(const QString&);
    void slotSmartAppend(bool);
    void slotAutoSaveOpts(bool);

protected:

    QValidator              *validator;
    int                      btime;
    kvoctrainView::Resizer   resizer;
    bool                     smart;
    bool                     autosaveopts;
};

#endif // GenOptPage_included
