/***************************************************************************

    $Id$

                   entry dialog for table cell contents

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.9  2001/11/10 22:28:25  arnold
    removed compatibility for kde1

    Revision 1.8  2001/11/10 21:13:00  arnold
    removed icons and compatibilty for kde1

    Revision 1.7  2001/11/09 14:18:49  arnold
    fixed and improved some dialog pages

    Revision 1.6  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.5  2001/10/30 14:10:32  arnold
    added property 'multiple choice'

    Revision 1.4  2001/10/23 18:15:39  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:40:37  arnold
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


#include "EntryDlg.h"

#define Inherited QTabDialog

#include <iostream.h>
#include <kapp.h>

#include <qpixmap.h>

#include <kvoctraindoc.h>
#include <kv_resource.h>
#include <langset.h>
#include <klocale.h>
#include <kstddirs.h>

EntryDlg::EntryDlg
(
        kvoctrainDoc  *doc,
        bool           multi_sel,
        bool           origin,
        grade_t        f_grd,
        grade_t        t_grd,
        count_t        f_qcount,
        count_t        t_qcount,
        count_t        f_bcount,
        count_t        t_bcount,
        time_t         f_qdate,
        time_t         t_qdate,
        QString        f_faux_ami,
        QString        t_faux_ami,
        QString        expr,
        int            lesson,
        QComboBox     *lessonbox,
        QString        lang,
        LangSet       &langset,
        QString        rem,
        QString        type,
        QString        pronunce,
        QString        synonym,
        QString        antonym,
        QString        example,
        QString        usagelabel,
        QString        paraphrase,
        const          Conjugation &con_prefix,
        const          Conjugation &conjugations,
        const          Article &article,
        const          Comparison &comp,
        const          MultipleChoice &mc,
        QueryManager  &querymanager,
	const QString &title,
        bool           active,
	QWidget    *parent,
	const char *name
)
	:
	Inherited( parent, name, true )
{
	setCaption (kapp->makeStdCaption( name));

        QString s;
        if (langset.findLongId(lang).isEmpty() )
          s = lang;
        else          
          s = langset.findLongId(lang);

        if (origin) {
          from_page = 0;
          to_page = 0;
          comm_page = new CommonEntryPage (this, doc, multi_sel, expr, lesson, lessonbox,
                                           lang, type, pronunce, usagelabel, 
                                           i18n("Original &expression in %1:").arg(s), querymanager, active);
          aux_page = new AuxInfoEntryPage (this, multi_sel, synonym, antonym, example, rem, paraphrase);
          mc_page = new MCEntryPage (this, multi_sel, mc, 0, QString(_EntryDlg_MULTIPLECHOICE).local8Bit());
          tense_page = new TenseEntryPage (this, multi_sel, con_prefix, conjugations, 0, QString(_EntryDlg_CONJUGATION).local8Bit());
          adj_page = new AdjEntryPage (this, multi_sel, comp, 0, QString(_EntryDlg_ADJECTIVE).local8Bit());
        }
        else {
          comm_page = new CommonEntryPage (this, doc, multi_sel, expr, lesson, lessonbox,
                                           lang, type, pronunce, usagelabel, 
                                           i18n("Translated &expression in %1:").arg(s), querymanager, active);
          aux_page = new AuxInfoEntryPage (this, multi_sel, synonym, antonym, example, rem, paraphrase);
          mc_page = new MCEntryPage (this, multi_sel, mc, 0, QString(_EntryDlg_MULTIPLECHOICE).local8Bit());
          tense_page = new TenseEntryPage (this, multi_sel, con_prefix, conjugations, 0, QString(_EntryDlg_CONJUGATION).local8Bit());
          adj_page = new AdjEntryPage (this, multi_sel, comp, 0, QString(_EntryDlg_ADJECTIVE).local8Bit());
          from_page = new FromToEntryPage (this, multi_sel, f_grd, f_qdate, f_qcount, f_bcount,
                                           f_faux_ami,
                                           i18n("Properties from original"));
          to_page   = new FromToEntryPage (this, multi_sel, t_grd, t_qdate, t_qcount, t_bcount,
                                           t_faux_ami,
                                           i18n("Properties to original"));
        }

        setCancelButton(i18n("&Cancel"));
        setOkButton(i18n("&OK"));
          
        addTab( comm_page,  i18n( "Co&mmon" ));
        addTab( aux_page,   i18n( "&Additional" ));
        addTab( mc_page,   _EntryDlg_MULTIPLECHOICE);
        addTab( tense_page, _EntryDlg_CONJUGATION);
        addTab( adj_page,   _EntryDlg_ADJECTIVE);
        if (!origin) {
          addTab( from_page, i18n( "&From original" ));
          addTab( to_page, i18n( "&To original" ));
        }
      
        updatePages (type);

        connect(comm_page, SIGNAL(typeSelected(const QString&)),
                SLOT(updatePages(const QString&)) );

        connect( this, SIGNAL(applyButtonPressed()), SLOT(accept()) );
        connect( this, SIGNAL(cancelButtonPressed()), SLOT(reject()) );

        connect (this, SIGNAL(aboutToShow()), comm_page, SLOT(initFocus() ));
        connect (this, SIGNAL(aboutToShow()), aux_page, SLOT(initFocus() ));
        connect (this, SIGNAL(aboutToShow()), adj_page, SLOT(initFocus() ));
        connect (this, SIGNAL(aboutToShow()), mc_page, SLOT(initFocus() ));
        connect (this, SIGNAL(aboutToShow()), tense_page, SLOT(initFocus() ));
        if (from_page != 0)
          connect (this, SIGNAL(aboutToShow()), from_page, SLOT(initFocus() ));
        if (to_page != 0)
          connect (this, SIGNAL(aboutToShow()), to_page, SLOT(initFocus() ));

        setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void EntryDlg::updatePages(const QString &type)
{
   QString main;
   int pos;
   if ((pos = type.find (QM_TYPE_DIV)) < 0)  // only use main type
     main = type;
   else
     main = type.left(pos);

   if (main == QM_VERB) {
     setTabEnabled (QString(_EntryDlg_CONJUGATION).local8Bit(), true);
     setTabEnabled (QString(_EntryDlg_ADJECTIVE).local8Bit(), false);
   }                  
   else if (main == QM_ADJ) {
     setTabEnabled (QString(_EntryDlg_CONJUGATION).local8Bit(), false);
     setTabEnabled (QString(_EntryDlg_ADJECTIVE).local8Bit(), true);
   }
   else {
     setTabEnabled (QString(_EntryDlg_CONJUGATION).local8Bit(), false);
     setTabEnabled (QString(_EntryDlg_ADJECTIVE).local8Bit(), false);
   }
}


#include "EntryDlg.moc"
