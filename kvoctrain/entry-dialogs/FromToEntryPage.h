/***************************************************************************

                      edit properties from/to original

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
#include <keduvocgrammar.h>
#include <keduvocdocument.h>

class FromToEntryPage : public QWidget, public Ui::FromToEntryPageForm
{
    Q_OBJECT

public:
    FromToEntryPage(KEduVocDocument *doc, QWidget *parent = 0);

    void setData(const QList<int>& entries, int toTrans, int fromTrans);
    void commitData();
    void clear();

    bool isModified();

signals:
    void sigModified();

private slots:
    void slotGradeSelected(int);

    void slotToday();
    void slotNever();
    void slotFalseFriendChanged(const QString &);

    void slotDateChanged(const QDate &);
    void totalCountChanged(int);
    void badCountChanged(int);

private:
    /*QDateTime  getDate() const;


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
    void setModified(bool mod = true);
    void setEnabled(int enable_type);
    */

//     void validate();

//     QString fauxami;
//     bool    valid_date;
//     grade_t grade;
//     count_t qcount;
//     count_t bcount;


    KEduVocDocument     *m_doc;
    int m_translationFrom;
    int m_translationTo;
    QList<int> m_entries;
};

#endif // FromToEntryPage_included
