/***************************************************************************

    $Id$

                            about dialog

    -----------------------------------------------------------------------

    begin                : Thu Nov 8 20:50:53 MET 1999
                                           
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

/*   TAKEN FROM AND MODIFIED AS NEEDED:
 *
 *   khexedit - Versatile hex editor
 *   Copyright (C) 1999  Espen Sand, espensa@online.no
 */

#ifndef _ABOUT_DIALOG_H_
#define _ABOUT_DIALOG_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <qdialog.h>
#include <qframe.h>
#include <qstring.h>

#include <ktabctl.h>
#include <kurllabel.h>


class CURLLabel : public KURLLabel
{
  Q_OBJECT

  public:
    CURLLabel( QWidget *parent, const char *name = 0 );

  protected:
    bool eventFilter( QObject *object, QEvent *e );

  signals:
    void leftClick( const QString & );
};  



class CAboutDialog : public QDialog 
{
  Q_OBJECT
    
  public:
    enum Page
    {
      page_general = 0,
      page_contributor,
      page_licence,
      page_welcome,
      page_MAX
    };

  public:
    CAboutDialog( QWidget *parent, const char *name = 0 );

  private slots:
    void urlClick( const QString & );
    void emailClick( const QString &e );

  private:
    QFrame *addPage( const QString &title );
    void createGeneralPage( void );
    void createContributorPage( void );
    void createLicencePage( void );
    void createWelcomePage( void );

  private:
    KTabCtl *mTabControl;
    QFrame  *mPage[page_MAX];
    QString  about_version;
};


#endif
