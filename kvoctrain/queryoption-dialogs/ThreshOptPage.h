/***************************************************************************

    $Id$

                 threshold options dialog page

    -----------------------------------------------------------------------

    begin                : Sat Jul 10 18:03:22 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/28 16:56:56  arnold
    replaced qtarch dialog files with qtdesigner

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

#ifndef ThreshOptPage_included
#define ThreshOptPage_included

#include "ThreshOptPageForm.h"
#include <QueryManager.h>

class ThreshOptPage : public ThreshOptPageForm
{
    Q_OBJECT

public:

    friend class QueryOptionsDlg;

    ThreshOptPage
    (
        QueryManager *_manager,
        QComboBox    *lessons,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    void setStates(QueryManager *manager);

protected:
    void keyPressEvent( QKeyEvent * );

signals:
     void modifySetting(int = -1);

public slots:
    void initFocus() const;

protected slots:
    void slotSelectAll();
    void slotSelectNone();
    void slotSetGradeComp(int);
    void slotSetBadComp(int);
    void slotSetGradeItem(int);
    void slotSetBadItem(int);
    void slotSetQueryComp(int);
    void slotSetTypeItem(int);
    void slotSetDateItem(int);
    void slotSetTypeComp(int);
    void slotSetQueryItem(int);
    void slotSetLessonItems();
    void slotSetLessonComp(int);
    void slotSetDateComp(int);
    void slotBlockExpire (bool block, bool expire);

 protected:

    QueryManager *manager;
    QComboBox    *lessons;
    vector<TypeRelation> all_maintypes;
};
#endif // ThreshOptPage_included
