/***************************************************************************

    $Id$

                      edit properties from/to original

    -----------------------------------------------------------------------

    begin                : Mon Jun 28 21:02:16 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2002/01/19 10:33:09  arnold
    made entry dialog modeless

    Revision 1.3  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.2  2001/10/23 18:15:39  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.1  2001/10/05 15:40:37  arnold
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


#ifndef FromToEntryPage_included
#define FromToEntryPage_included

#include "FromToEntryPageForm.h"
#include <time.h>
#include <qdatetime.h>

#include <kvoctraindoc.h>

class EntryDlg;

class FromToEntryPage : public FromToEntryPageForm
{
    Q_OBJECT

public:

    FromToEntryPage
    (
        EntryDlg   *dlgbook,
        bool        multi_sel,
        grade_t     _grade,
        time_t      _date,
        count_t     _qcount,
        count_t     _bcount,
        QString     faux,
        QString     label,
        QWidget    *parent = NULL,
        const char *name = NULL
    );

    void setData(
        bool        multi_sel,
        grade_t     _grade,
        time_t      _date,
        count_t     _qcount,
        count_t     _bcount,
        QString     faux,
        QString     label);

    time_t  dateDirty   () const { return date_dirty; }
    grade_t gradeDirty  () const { return grade_dirty; }
    count_t qCountDirty () const { return qcount_dirty; }
    count_t bCountDirty () const { return bcount_dirty; }

    time_t  getDate   () const;
    grade_t getGrade  () const { return grade; }
    count_t getQCount () const { return qcount; }
    count_t getBCount () const { return bcount; }

    QString getFauxAmi () const { return fauxami; }

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
    void slotGradeSelected(int);
    void slotQCount(const QString&);
    void slotBCount(const QString&);

    void slotIncYear();
    void slotIncMonth();
    void slotIncDay();
    void slotDecYear();
    void slotDecMonth();
    void slotDecDay();
    void slotToday();
    void slotNever();
    void slotFauxAmiSelected(const QString&);

    void slotDayChanged(int);
    void slotMonthChanged(int);
    void slotYearChanged(int);

protected:
    void validate();

    QString    fauxami;
    bool       valid_date;
    bool       date_dirty,
               qcount_dirty,
               bcount_dirty,
               grade_dirty;
    grade_t    grade;
    count_t    qcount;
    count_t    bcount;
    int        year,
               month,
               day;
    EntryDlg  *dlgbook;
    bool       modified;
};

#endif // FromToEntryPage_included
