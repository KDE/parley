/***************************************************************************

    $Id$

                  general options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2001/12/13 18:40:12  arnold
    added phonetic alphabet stuff

    Revision 1.3  2001/11/02 10:18:58  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:44:04  arnold
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

#ifndef _OptDlg_CLIPBOARD 
#define _OptDlg_CLIPBOARD i18n("&Clipboard")
#endif

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
  _OptDlg_CLIPBOARD
#else

#ifndef GeneralOptionsDlg_included
#define GeneralOptionsDlg_included

#include <qtabdialog.h>
#include <qstrlist.h>

#include "LangOptPage.h"
#include "ViewOptPage.h"
#include "GenOptPage.h"
#include "PasteOptPage.h"

#include <langset.h>

class kvoctraindoc;

class GeneralOptionsDlg : public QTabDialog
{
    Q_OBJECT

public:

    GeneralOptionsDlg
    (   QString       _deflang,
        QString       _sep,
        int           _btime,
        LangSet      &_langset,
        QString      &lastpix,
        QComboBox    *lessons,
        QStringList  &list,
        bool          useCurrent,
        kvoctrainDoc *_doc,
        QFont        &table_font,
        QFont        &ipa_font,
        QueryManager *manager,
        GradeCols    &gradecols,
        kvoctrainView::Resizer resizer,
        bool          smartAppend,
        bool          autosaveopts,
        bool          _autoapply,
        QWidget      *parent = NULL,
        const char   *name = NULL
    );

    QFont getFont()           const { return viewOptPage->getFont(); }
    QFont getIPAFont()        const { return viewOptPage->getIPAFont(); }
    GradeCols getGradeCols()  const { return viewOptPage->getGradeCols(); }

    int getBackupTime()       const { return genOptPage->getBackupTime(); }
    int getSmartAppend()      const { return genOptPage->getSmartAppend(); }
    int getAutoSaveOpts()     const { return genOptPage->getAutoSaveOpts(); }
    bool getAutoApply()       const { return genOptPage->getAutoApply(); }
    kvoctrainView::Resizer getResizer() const { return genOptPage->getResizer(); }

    QString getDefaultLang () const { return langOptPage->getDefaultLang(); }
    LangSet getLangSet ()     const { return langOptPage->getLangSet(); }

    QStringList getPasteOrder()  const { return pasteOptPage->getPasteOrder(); }
    QString getSeparator ()   const { return pasteOptPage->getSeparator(); }
    bool getUseCurrent()      const { return pasteOptPage->getUseCurrent(); }

    void selectPage(int index);

protected slots:

    void cancelButton();
    void okButton();

protected:

    GenOptPage    *genOptPage;
    LangOptPage   *langOptPage;
    ViewOptPage   *viewOptPage;
    PasteOptPage  *pasteOptPage;
    LangSet        langset;
};

#endif // GeneralOptionsDlg_included

#endif  // __ONLY_TO_BE_SEEN_BY_XGETTEXT
