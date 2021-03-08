/*
    edit properties from/to original
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FromToEntryPage_included
#define FromToEntryPage_included

#include <QDateTime>

#include "ui_FromToEntryPageForm.h"
#include <KEduVocExpression>
#include <keduvocgrammar.h>
#include <KEduVocDocument>

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
    /// get a text representation of a grade
    static QString gradeStr(int i);

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
