/***************************************************************************

    $Id: $

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin                : Mon Oct 29 18:09:29 1999

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

#ifndef MCEntryPage_included
#define MCEntryPage_included

#include "MCEntryPageForm.h"

#include <MultipleChoice.h>

struct SpecFont_t;

class MCEntryPage : public MCEntryPageForm
{
    Q_OBJECT

public:

    MCEntryPage
    (
        SpecFont_t           *font,
        QDialog              *dlgbook,
        bool                  multi_sel,
        const MultipleChoice &mc,
        QWidget              *parent = NULL,
        const char           *name = NULL
    );

    MultipleChoice getMultipleChoice() const { return multiplechoice; }

protected:
    void keyPressEvent( QKeyEvent * );

protected slots:
    void initFocus() const;
    void mc1Changed(const QString&);
    void mc2Changed(const QString&);
    void mc3Changed(const QString&);
    void mc4Changed(const QString&);
    void mc5Changed(const QString&);
    void returnPressed();

protected:

    MultipleChoice  multiplechoice;
};

#endif // MCEntryPage_included
