/***************************************************************************

    $Id$

                  viewing options dialog page

    -----------------------------------------------------------------------

    begin                : Fri Sep 10 16:42:01 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/25 17:34:19  arnold
    replaced qtarch dialog files by qtdesigner

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


#ifndef ViewOptPage_included
#define ViewOptPage_included

#include "ViewOptPageForm.h"

#include <kvoctrainexpr.h>

class QueryManager;

class ViewOptPage : public ViewOptPageForm
{
    Q_OBJECT

public:

    ViewOptPage
    (
        QFont        &_tablefont,
        QFont        &_ipa_font,
        GradeCols    &cols,
        QueryManager *manager,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    QFont     getIPAFont()    const { return ipa_font; }
    QFont     getFont()       const { return tablefont; }
    GradeCols getGradeCols()  const { return gc; }

protected:
    void keyPressEvent( QKeyEvent * );

public slots:
    void initFocus() const;

protected slots:
    void slotChooseFont();
    void slotChooseIPAFont();
    void slotCol0(const QColor&);
    void slotCol1(const QColor&);
    void slotCol2(const QColor&);
    void slotCol3(const QColor&);
    void slotCol4(const QColor&);
    void slotCol5(const QColor&);
    void slotCol6(const QColor&);
    void slotCol7(const QColor&);
    void slotColUsed(bool used);


protected:
    GradeCols     gc;
    QFont         tablefont;
    QFont         ipa_font;
};
#endif // ViewOptPage_included
