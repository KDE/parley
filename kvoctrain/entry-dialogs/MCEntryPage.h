/***************************************************************************

    $Id$

              dialog page for multiple choice suggestions

    -----------------------------------------------------------------------

    begin                : Mon Oct 29 18:09:29 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.1  2001/10/30 14:53:34  arnold
    added property 'multiple choice'

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

class MCEntryPage : public MCEntryPageForm
{
    Q_OBJECT

public:

    MCEntryPage
    (
        QDialog              *dlgbook,
        bool                  multi_sel,
        const MultipleChoice &mc,
        QWidget              *parent = NULL,
        const char           *name = NULL
    );

    void setData(bool multi_sel,
                 const MultipleChoice &mc);

    MultipleChoice getMultipleChoice() const { return multiplechoice; }

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
    void mc1Changed(const QString&);
    void mc2Changed(const QString&);
    void mc3Changed(const QString&);
    void mc4Changed(const QString&);
    void mc5Changed(const QString&);
    void returnPressed();

protected:
    MultipleChoice  multiplechoice;
    bool            modified;
};

#endif // MCEntryPage_included
