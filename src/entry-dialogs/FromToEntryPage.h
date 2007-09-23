/***************************************************************************

                      edit properties from/to original

    -----------------------------------------------------------------------

    begin         : Mon Jun 28 21:02:16 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
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
    explicit FromToEntryPage(KEduVocDocument *doc, int fromIdentifier, int toIdentifier, QWidget *parent = 0);

public slots:
    void setData(const QList<int>& entries);
    void commitData();

signals:
    void sigModified();

private slots:
    void slotGradeSelected(int);
    void slotResetGrades();

    void slotToday();
    void slotNever();
    void slotFalseFriendChanged(const QString &);

    void slotDateChanged(const QDate &);
    void totalCountChanged(int);
    void badCountChanged(int);

private:
    KEduVocDocument     *m_doc;
    int m_translationFrom;
    int m_translationTo;
    QList<int> m_entries;

    // keep track of modifications (for multiple)
    bool m_gradeChanged;
    bool m_practiceDateChanged;
    bool m_totalCountChanged;
    bool m_wrongCountChanged;
};

#endif // FromToEntryPage_included
