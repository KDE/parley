/***************************************************************************

    $Id$

                      language options dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 18:09:06 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.5  2001/12/10 21:00:15  mueller
    fix compilation

    Revision 1.4  2001/11/10 17:35:03  arnold
    fixed language property dialog page

    Revision 1.3  2001/11/09 10:40:46  arnold
    removed ability to display a different font for each column

    Revision 1.2  2001/10/25 17:34:19  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.1  2001/10/05 15:44:04  arnold
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

#ifndef LangOptPage_included
#define LangOptPage_included

#include "LangOptPageForm.h"
#include <qvalidator.h>

#include <langset.h>

class QPopupMenu;

class LangOptPage : public LangOptPageForm
{
    Q_OBJECT

public:

    LangOptPage
    (
        QString    deflang,
        LangSet    &langset,
        QString    &lastpix,
        QWidget* parent = NULL,
        const char* name = NULL
    );

    QString getDefaultLang () const { return deflang; }
    LangSet getLangSet ()     const;

    virtual ~LangOptPage ();

    struct LangRef {
      LangRef (const QString& reg, const LangSet &ls)
        : region(reg), langs(ls) {}
       QString region;
       LangSet langs;
    };

protected:
    void keyPressEvent( QKeyEvent *e );

public slots:
    void initFocus() const;

protected slots:
    void slotDeleteClicked();
    void slotNewClicked();
    void slotPixmapClicked();
    void slotLangChanged(const QString&);
    void slotShort2Changed(const QString&);
    void slotShortActivated(const QString&);
    void slotNewNameChanged(const QString&);
    void slotLangFromGlobalActivated(int);

protected:
    bool setPixmap(QString pm);
    void enableLangWidgets();
    void loadCountryData();

    QString     deflang;
    LangSet    &langset;

    QValidator *validator;
    QString    &lastPixName;

    QStringList      countrylist;
    QStringList      regionlist;


    vector <LangRef> globalLangs;
    LangSet          global_langset;
    QPopupMenu      *langset_popup;

    friend class sortByRegion;
};
#endif // LangOptPage_included
