/***************************************************************************

    $Id$

                   user tense options dialog page

    -----------------------------------------------------------------------

    begin                : Sun May 28 12:14:31 2000
                                           
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


#ifndef TenseOptPage_included
#define TenseOptPage_included

#include "TenseOptPageData.h"

#include <vector.h>

class kvoctrainDoc;
class QStrList;

class TenseOptPage : public TenseOptPageData
{
    Q_OBJECT

public:

    TenseOptPage
    (
        const vector<QString> &tenses,
        kvoctrainDoc    *doc,
        QWidget         *parent = NULL,
        const char      *name = NULL,
        bool             modal = false
    );

//  virtual ~TenseOptPage();

    void getTenseNames (vector<QString> &ret_types,
                         vector<int> &ret_Index);

    static void cleanUnused(kvoctrainDoc *doc,
                            const vector<int> &tenseIndex,
                            int old_tenses);

public slots:
    void initFocus() const;

protected:
    void keyPressEvent( QKeyEvent * );

protected:

    void updateListBox(int start);

protected slots:

    void slotDeleteTense();
    void slotNewTense();
    void slotTenseChosen(int);
    void slotModifyTense();
    void slotCleanup();

private:
    kvoctrainDoc  *doc;
    int            act_tense;
    vector<int>    tenseIndex; // contains indices of tenses on exec()
                               // negative values are new tenses
};
#endif // TenseOptPage_included
