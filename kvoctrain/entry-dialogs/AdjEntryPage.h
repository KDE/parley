/***************************************************************************

    $Id$

                   dialog page for adjectives (comparison)

    -----------------------------------------------------------------------

    begin                : Sat Dec 4 18:09:29 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef AdjEntryPage_included
#define AdjEntryPage_included

#include "AdjEntryPageForm.h"

#include <GrammerManager.h>

class EntryDlg;

class AdjEntryPage : public AdjEntryPageForm
{
    Q_OBJECT

public:

    AdjEntryPage
    (
        EntryDlg          *dlgbook,
        bool               multi_sel,
        const Comparison  &comp,
        QWidget           *parent = NULL,
        const char        *name = NULL
    );

    void setData (bool multi_sel,
                  const Comparison  &comp);

    Comparison getComparison() const { return comparisons; }

    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(int enable_type);

public slots:
    void initFocus() const;

signals:
    void sigModified();

protected:
    void keyPressEvent( QKeyEvent * );

protected slots:
    void lev1Changed(const QString&);
    void lev2Changed(const QString&);
    void lev3Changed(const QString&);
    void returnPressed();

protected:

    Comparison    comparisons;
    bool          modified;
    EntryDlg     *dlgbook;
};
#endif // AdjEntryPage_included
