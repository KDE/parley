/***************************************************************************

    $Id$

                   document properties dialog class

    -----------------------------------------------------------------------

    begin                : Fri Sep 10 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:38:38  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef _DocDlg_TYPES
#define _DocDlg_TYPES i18n("T&ypes")
#endif

#ifndef _DocDlg_TENSES
#define _DocDlg_TENSES i18n("Te&nses")
#endif

#ifndef _DocDlg_USAGES
#define _DocDlg_USAGES i18n("&Usage")
#endif

#ifndef _DocDlg_TITLE
#define _DocDlg_TITLE i18n("&General")
#endif

#ifndef _DocDlg_LESSONS
#define _DocDlg_LESSONS i18n("L&essons")
#endif

#ifndef _DocDlg_OPTIONS
#define _DocDlg_OPTIONS i18n("&Options" )
#endif

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
  _DocDlg_USAGES
  _DocDlg_TENSES
  _DocDlg_TITLE
  _DocDlg_TYPES
  _DocDlg_LESSONS
  _DocDlg_OPTIONS
#else


#ifndef DocPropsDlg_included
#define DocPropsDlg_included

#include <qtabdialog.h>
#include <qlabel.h>

#include "DocOptionsPage.h"
#include "LessOptPage.h"
#include "TypeOptPage.h"
#include "TenseOptPage.h"
#include "UsageOptPage.h"
#include "TitlePage.h"

#include <GrammerManager.h>
#include <kvoctraindoc.h>

#include <vector.h>

class kvoctrainDoc;
class LangPropPage;

class DocPropsDlg : public QTabDialog
{
    Q_OBJECT

public:

    DocPropsDlg
    (
        kvoctrainDoc    *doc,
        const char      *start_page,
        QComboBox       *lessons,
        QString          title,
        QString          author,
        QString          license,
        QString          doc_remark,
        vector<QString>  types,
        vector<QString>  tenses,
        vector<QString>  usages,
        QWidget         *parent = NULL,
        const char      *name = NULL
    );

    inline bool getSorting()  const { return docOptPage->getSorting(); }

    inline QString getTitle()      { return titleOptPage->getTitle(); }
    inline QString getAuthor()     { return titleOptPage->getAuthor(); }
    inline QString getLicense()    { return titleOptPage->getLicense(); }
    inline QString getDocRemark()  { return titleOptPage->getDocRemark(); }

    inline void getLesson (QComboBox *lessons, vector<int>& ret_index) const
      { lessOptPage->getLesson (lessons, ret_index); }

    inline void getTypeNames (vector<QString> &types, vector<int>& ret_index) const
      { typeOptPage->getTypeNames (types, ret_index); }

    inline void getTenseNames (vector<QString> &tenses, vector<int>& ret_index) const
      { tenseOptPage->getTenseNames (tenses, ret_index); }

    inline void getUsageLabels (vector<QString> &usages, vector<int>& ret_index) const
      { useOptPage->getUsageLabels (usages, ret_index); }

protected slots:

    void cancelButton();
    void okButton();

protected:

    UsageOptPage   *useOptPage;
    TypeOptPage    *typeOptPage;
    TenseOptPage   *tenseOptPage;
    LessOptPage    *lessOptPage;
    TitlePage      *titleOptPage;
    DocOptionsPage *docOptPage;
};

#endif // DocPropsDlg_included

#endif // __ONLY_TO_BE_SEEN_BY_XGETTEXT
