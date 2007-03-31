/***************************************************************************

                   enter document title and author

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TitleDialog_included
#define TitleDialog_included

#include "ui_TitlePageForm.h"

class KEduVocDocument;

class TitlePage : public QWidget, public Ui::TitlePageForm
{
    Q_OBJECT

public:
    TitlePage(KEduVocDocument * doc, QWidget *parent = 0);

    QString getTitle()
    {
        return title;
    }
    QString getAuthor()
    {
        return author;
    }
    QString getLicense()
    {
        return license;
    }
    QString getDocRemark()
    {
        return doc_remark;
    }

protected slots:
    void slotTitleChanged(const QString&);
    void slotAuthorChanged();
    void slotLicenseChanged(const QString&);
    void slotDocRemarkChanged();

private:
    QString title;
    QString author;
    QString license;
    QString doc_remark;
};

#endif // TitlePage_included
