/***************************************************************************

    $Id$

                   document language dialog class

    -----------------------------------------------------------------------

    begin                : Sat Jun 2 20:50:53 MET 1999
                                           
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

#ifndef DocPropsLangDlg_included
#define DocPropsLangDlg_included

#include <qtabdialog.h>

#include <GrammerManager.h>

#include <vector.h>

class kvoctrainDoc;
class LangPropPage;
class LangSet;

class DocPropsLangDlg : public QTabDialog
{
    Q_OBJECT

public:

    DocPropsLangDlg
    (
        kvoctrainDoc    *doc,
        LangSet         *langset,
        QWidget         *parent = NULL,
        const char      *name = NULL
    );

    Conjugation getConjugation(int idx) const;
    Article     getArticle(int idx) const;
    QFont::CharSet getCharSet(int idx) const;

protected slots:

    void cancelButton();
    void okButton();

protected:

    vector<LangPropPage*> langPages;
};

#endif // DocPropsLangDlg_included

