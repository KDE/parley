/***************************************************************************

    $Id$

                     blocking options page class

    -----------------------------------------------------------------------

    begin                : Sat Oct 30 13:29:00 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/28 16:56:56  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.1  2001/10/05 15:46:01  arnold
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


#ifndef BlockOptPage_included
#define BlockOptPage_included

#include "BlockOptPageForm.h"

#include <time.h>
#include <vector>
using namespace std;

class QueryManager;

class BlockOptPage : public BlockOptPageForm
{
    Q_OBJECT

public:

    BlockOptPage
    (
        QueryManager *manager,
        bool          block,
        bool          expire,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    bool getExpire()       const { return expire; }
    bool getBlock()        const { return block; }

    void setStates(QueryManager *manager, bool block, bool expire);

public slots:
    void initFocus() const;

signals:
    void modifySetting(int = -1);
    void blockExpire (bool block, bool expire);

protected:
    void keyPressEvent( QKeyEvent * );
    void setExpireBox (int no, QComboBox*);
    void setBlockBox (int no, QComboBox*);
    void checkValidity();

protected slots:

    void slotSetExpire2(int);
    void slotSetBlock3(int);
    void slotSetExpire3(int);
    void slotSetExpire4(int);
    void slotSetExpire5(int);
    void slotSetExpire1(int);
    void slotSetSetBlock6(int);
    void slotSetSetBlock4(int);
    void slotSetSetBlock5(int);
    void slotSetSetBlock2(int);
    void slotSetExpire6(int);
    void slotSetBlock7(int);
    void slotSetSetBlock1(int);
    void slotSetExpire7(int);
    void slotCheckBlock(bool);
    void slotCheckExpire(bool);

protected:

  QueryManager  *manager;
  bool           block;
  bool           expire;
};
#endif // BlockOptPage_included
