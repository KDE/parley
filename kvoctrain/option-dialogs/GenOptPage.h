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
    Revision 1.4  2001/11/02 10:18:58  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.3  2001/10/25 17:34:19  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:44:04  arnold
    import of version 0.7.0pre8 to kde-edu


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

#include "GenOptPageForm.h"
#include "../kvoctrainview.h"

class QValidator;

class GenOptPage : public GenOptPageForm
{
    Q_OBJECT

public:

    GenOptPage
    (
        int         _btime,
        kvoctrainView::Resizer res,
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
    void slotBTimeUsed(bool);

protected:
    QValidator              *validator;
    int                      btime;
    bool                     smart;
    bool                     autosaveopts;
    kvoctrainView::Resizer   resizer;
};

#endif // GenOptPage_included
