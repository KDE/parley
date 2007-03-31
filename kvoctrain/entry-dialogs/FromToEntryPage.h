/***************************************************************************

                      edit properties from/to original

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

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

#include <QDateTime>

#include "ui_FromToEntryPageForm.h"
#include <keduvocexpression.h>

class FromToEntryPage : public QWidget, public Ui::FromToEntryPageForm
{
    Q_OBJECT

public:
    FromToEntryPage(QWidget *parent = 0);

    void setData(bool multi_sel, grade_t _grade, QDateTime _date, count_t _qcount, count_t _bcount, QString faux, QString label);

    QDateTime  getDate() const;
    grade_t   getGrade() const
    {
        return grade;
    }
    count_t  getQCount() const
    {
        return qcount;
    }
    count_t  getBCount() const
    {
        return bcount;
    }
    QString getFauxAmi() const
    {
        return fauxami;
    }

    bool dateIsModified()
    {
        return m_dateIsModified;
    }
    bool gradeIsModified()
    {
        return m_gradeIsModified;
    }
    bool queryCountIsModified()
    {
        return m_queryCountIsModified;
    }
    bool badCountIsModified()
    {
        return m_badCountIsModified;
    }
    bool isModified();
    void setModified(bool mod = true);
    void setEnabled(int enable_type);

signals:
    void sigModified();

protected slots:
    void slotGradeSelected(int);

    void slotToday();
    void slotNever();
    void slotFauxAmiChanged(const QString &);

    void slotDateChanged(const QDate &);
    void totalCountChanged(int);
    void badCountChanged(int);

protected:
    void validate();

    QString fauxami;
    bool    valid_date;
    grade_t grade;
    count_t qcount;
    count_t bcount;
    bool    modified;
    bool    m_largeSelection;
    bool    m_gradeIsModified;
    bool    m_queryCountIsModified;
    bool    m_badCountIsModified;
    bool    m_dateIsModified;
};

#endif // FromToEntryPage_included
