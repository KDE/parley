/***************************************************************************

    $Id$

                   enter document title and author

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$

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

#include "TitlePageData.h"

class TitlePage : public TitlePageData
{
    Q_OBJECT

public:

    TitlePage
    (
        QString      title,
        QString      author,
        QString      license,
        QString      doc_remark,
        QWidget     *parent = NULL,
        const char  *name = NULL
    );

    QString getTitle() { return title; }
    QString getAuthor() { return author; }
    QString getLicense() { return license; }
    QString getDocRemark() { return doc_remark; }

public slots:
    void initFocus() const;

protected:
  void keyPressEvent( QKeyEvent *e );

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
