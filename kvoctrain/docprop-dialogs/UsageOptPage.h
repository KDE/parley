/***************************************************************************

                   usage labels dialog page

    -----------------------------------------------------------------------

    begin         : Sun Sep 12 15:38:31 1999

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

#ifndef UsageOptPage_included
#define UsageOptPage_included

#include "ui_optionlistform.h"
#include <keduvocdocument.h>

class KEduVocDocument;

class UsageOptPage : public QWidget, public Ui::OptionListForm
{
    Q_OBJECT
public:
    UsageOptPage(KEduVocDocument *doc, QWidget *parent);

    static void cleanUnused(KEduVocDocument *doc, const QList<int> &lessonIndex, int old_lessons);

    void commitData();

private:
    void updateListBox(int start);

private slots:
    void slotDeleteUsage();
    void slotNewUsage();
    void slotUsageChosen(int);
    void slotModifyUsage();
    void slotCleanup();

private:
    KEduVocDocument  *m_doc;
    int               m_currentUsage;

    QStringList            m_newUsages;
    QStringList            m_deletedUsages;
    QList<QPair<QString, QString> > m_renameList;
};

#endif // UsageOptPage_included
