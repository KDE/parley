/***************************************************************************

    $Id$

                   types options dialog page

    -----------------------------------------------------------------------

    begin                : Sun Sep 12 15:38:31 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:38:38  arnold
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

#ifndef TypeOptPage_included
#define TypeOptPage_included

#include "TypeOptPageForm.h"

#include <vector.h>

class kvoctrainDoc;
class QStrList;

class TypeOptPage : public TypeOptPageForm
{
    Q_OBJECT

public:

    TypeOptPage
    (
        const vector<QString> &types,
        kvoctrainDoc    *doc,
        QWidget         *parent = NULL,
        const char      *name = NULL,
        bool             modal = false
    );

    void getTypeNames (vector<QString> &ret_types,
                       vector<int> &ret_Index);

    static void cleanUnused(kvoctrainDoc *doc,
                            const vector<int> &lessonIndex,
                            int old_lessons);

public slots:
    void initFocus() const;

protected:
    void keyPressEvent( QKeyEvent * );

protected:

    void updateListBox(int start);

protected slots:

    void slotDeleteType();
    void slotNewType();
    void slotTypeChosen(int);
    void slotModifyType();
    void slotCleanup();

private:
    kvoctrainDoc  *doc;
    int            act_type;
    vector<int>    typeIndex; // contains indices of types on exec()
                              // negative values are new lessons
};
#endif // TypeOptPage_included
