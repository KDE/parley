/***************************************************************************

    $Id$

                   lesson properties dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 22:03:50 1999
                                           
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

#ifndef LessOptPage_included
#define LessOptPage_included

#include "LessOptPageForm.h"

#include <vector.h>

class kvoctrainDoc;
class QComboBox;

class LessOptPage : public LessOptPageForm
{
    Q_OBJECT

public:

    LessOptPage
    (
        QComboBox     *lessons,
        kvoctrainDoc  *doc,
        QWidget       *parent = NULL,
        const char    *name = NULL,
        bool           modal = false
    );

    void getLesson (QComboBox *ret_lesson, vector<int>& ret_Index);

    static void cleanUnused(kvoctrainDoc *doc,
                            const QComboBox *lessons,
                            const vector<int> &lessonIndex,
                            int old_lessons,
                            vector<int> &lessons_in_use);

public slots:
    void initFocus() const;

protected:
    void updateListBox(int start);
    void keyPressEvent( QKeyEvent *e );

protected slots:

    void slotDeleteLesson();
    void slotNewLesson();
    void slotLessonChosen(int);
    void slotModifyLesson();
    void slotCleanup();

private:
    kvoctrainDoc  *doc;
    int            act_lesson;
    vector<int>    lessonIndex; // contains indices of lesson on exec()
                               // negative values are new lessons
};
#endif // LessOptPage_included
