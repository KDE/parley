/***************************************************************************

    $Id$

                       query groups dialog page

    -----------------------------------------------------------------------

    begin                : Mon Nov 1 13:10:21 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:46:01  arnold
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

#ifndef GroupOptPage_included
#define GroupOptPage_included

#include "GroupOptPageForm.h"

#include <vector.h>

struct PreSetting {
 PreSetting (QString n, QString q, QString t, QString b)
    : name(n),
      query_set(q),
      thresh_set(t),
      block_set(b) {}

   QString name,
           query_set,
           thresh_set,
           block_set;
};


class GroupOptPage : public GroupOptPageForm
{
    Q_OBJECT

public:

    GroupOptPage
    (
        vector<PreSetting>  preset,
        QWidget            *parent = NULL,
        const char         *name = NULL
    );

public slots:
    void initFocus() const;

protected slots:
    void slotGroupSelected(int item);
    void slotDeleteGroup();
    void slotNewGroup();
    void slotStoreGroup();
    void slotRecallGroup();

protected:
    void keyPressEvent( QKeyEvent * );
    void updateButtons();

signals:
    void selectGroup(int);
    void deleteGroup(int);
    void modifyGroup(int);
    void newGroup (const QString&);

};
#endif // GroupOptPage_included
