/***************************************************************************

    $Id$

                   internal doc options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Nov 25 12:00:53 MET 1999
                                           
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

#ifndef DocOptionsPage_included
#define DocOptionsPage_included

#include "DocOptionsPageData.h"

#include <kvoctraindoc.h>

class DocOptionsPage : public DocOptionsPageData
{
    Q_OBJECT

public:

    DocOptionsPage
    (
        bool         sort,
        KV_Encoding  encoding,
	QWidget     *parent,
	const char  *name
    );

    bool getSorting () const { return sorter; }
    KV_Encoding getEncoding()  const { return encoder; }

public slots:
    void initFocus() const;

protected:
    void keyPressEvent( QKeyEvent * );

protected slots:
    void docSortToggled(bool);
    void slotEncSelected(int);

private:
    bool        sorter;
    KV_Encoding encoder;
};

#endif // DocOptionsPage_included
