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
    Revision 1.15  2002/01/20 11:41:01  arnold
    fixed issues with modeless dialogs

    Revision 1.14  2002/01/19 10:33:09  arnold
    made entry dialog modeless

    Revision 1.13  2001/12/30 12:12:57  arnold
    fixed smart appending and editing

    Revision 1.12  2001/12/26 15:11:29  mueller
    CVSSILINT: fixincludes

    Revision 1.11  2001/12/13 18:39:29  arnold
    added phonetic alphabet stuff

    Revision 1.10  2001/11/17 17:58:55  arnold
    added inline editing of all columns

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
#include <kvoctraindoc.h>
#include <kmainwindow.h>

#include <iostream.h>

#include <qpixmap.h>
#include <qlayout.h>
#include <qframe.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qapplication.h>

#include <kvoctraindoc.h>
#include <kv_resource.h>
#include <langset.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kapplication.h>

EntryDlg::EntryDlg(
        KMainWindow   *main,
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
        const QFont&   ipafont,
	QWidget       *parent,
	const char    *name)
	:
	EntryDlgForm( parent, name, false)
{
        setAutoApply (false);
        mainwin = main;
        docked = false;
        edit_row = -1;
        edit_col = -1;

	setCaption (kapp->makeStdCaption(title));

        QString s;
        if (langset.findLongId(lang).isEmpty() )
          s = lang;
        else          
          s = langset.findLongId(lang);

        from_page = 0;
        to_page = 0;

        if (origin) {
          comm_page = new CommonEntryPage (this, doc, multi_sel, expr, lesson, lessonbox,
                                           lang, type, pronunce, usagelabel, 
                                           i18n("Original &expression in %1:").arg(s), querymanager, active,
                                           ipafont);
          aux_page = new AuxInfoEntryPage (this, multi_sel, synonym, antonym, example, rem, paraphrase);
          mc_page = new MCEntryPage (this, multi_sel, mc, 0, QString(_EntryDlg_MULTIPLECHOICE).local8Bit());
          tense_page = new TenseEntryPage (this, multi_sel, con_prefix, conjugations, 0, QString(_EntryDlg_CONJUGATION).local8Bit());
          adj_page = new AdjEntryPage (this, multi_sel, comp, 0, QString(_EntryDlg_ADJECTIVE).local8Bit());
        }
        else {
          comm_page = new CommonEntryPage (this, doc, multi_sel, expr, lesson, lessonbox,
                                           lang, type, pronunce, usagelabel, 
                                           i18n("Translated &expression in %1:").arg(s), querymanager, active,
                                           ipafont);
          aux_page = new AuxInfoEntryPage (this, multi_sel, synonym, antonym, example, rem, paraphrase);
          mc_page = new MCEntryPage (this, multi_sel, mc, 0, QString(_EntryDlg_MULTIPLECHOICE).local8Bit());
          tense_page = new TenseEntryPage (this, multi_sel, con_prefix, conjugations, 0, QString(_EntryDlg_CONJUGATION).local8Bit());
          adj_page = new AdjEntryPage (this, multi_sel, comp, 0, QString(_EntryDlg_ADJECTIVE).local8Bit());
        }

        from_page = new FromToEntryPage (this, multi_sel, f_grd, f_qdate, f_qcount, f_bcount,
                                         f_faux_ami,
                                         i18n("Properties from original"));
        to_page   = new FromToEntryPage (this, multi_sel, t_grd, t_qdate, t_qcount, t_bcount,
                                         t_faux_ami,
                                         i18n("Properties to original"));

        QVBoxLayout *tablay = new QVBoxLayout(tabframe);
        tabber = new QTabWidget(tabframe);
        tablay->addWidget(tabber);

        tabber->addTab( comm_page,  i18n( "Co&mmon" ));
        tabber->addTab( aux_page,   i18n( "&Additional" ));
        tabber->addTab( mc_page,   _EntryDlg_MULTIPLECHOICE);
        tabber->addTab( tense_page, _EntryDlg_CONJUGATION);
        tabber->addTab( adj_page,   _EntryDlg_ADJECTIVE);
        tabber->addTab( from_page, i18n( "&From original" ));
        tabber->addTab( to_page, i18n( "&To original" ));

        dock_vert->setPixmap(KGlobal::iconLoader()->loadIcon("view_left_right", KIcon::Small));
        dock_horiz->setPixmap(KGlobal::iconLoader()->loadIcon("view_top_bottom", KIcon::Small));

        updatePages (type);

        connect(comm_page, SIGNAL(typeSelected(const QString&)),
                SLOT(updatePages(const QString&)) );

        connect( chk_autoapply, SIGNAL(toggled(bool)), this, SLOT(slotAutoApplyChecked(bool)) );
        connect( undoButton, SIGNAL(clicked()), this, SLOT(slotUndo()) );
        connect( applyButton, SIGNAL(clicked()), this, SLOT(slotApply()) );
        connect( cancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()) );
        connect( dock_vert, SIGNAL(clicked()), this, SLOT(slotDockVertical()) );
        connect( dock_horiz, SIGNAL(clicked()), this, SLOT(slotDockHorizontal()) );

        connect (comm_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
        connect (aux_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
        connect (adj_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
        connect (mc_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
        connect (tense_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));

        if (from_page != 0)
          connect (from_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
        if (to_page != 0)
          connect (to_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));

        chk_autoapply->setChecked(false);
        applyButton->setEnabled(false);
        undoButton->setEnabled(false);
        applyButton->setDefault(true);
        initFocus();
        setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void EntryDlg::setData(
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
        bool           active)
{
	setCaption (kapp->makeStdCaption(title));

        QString s;
        if (langset.findLongId(lang).isEmpty() )
          s = lang;
        else
          s = langset.findLongId(lang);

        if (origin)
          comm_page->setData(multi_sel, expr, lesson, lessonbox,
                             lang, type, pronunce, usagelabel,
                             i18n("Original &expression in %1:").arg(s), querymanager,
                             active);
        else
          comm_page->setData(multi_sel, expr, lesson, lessonbox,
                             lang, type, pronunce, usagelabel,
                             i18n("Translated &expression in %1:").arg(s), querymanager,
                             active);

        adj_page->setData(multi_sel, comp);
        aux_page->setData(multi_sel, synonym, antonym, example, rem, paraphrase);
        mc_page->setData(multi_sel, mc);
        tense_page->setData(multi_sel, conjugations);
        if (from_page != 0)
          from_page->setData(multi_sel, f_grd, f_qdate, f_qcount, f_bcount,
                             f_faux_ami,
                             i18n("Properties from original"));
        if (to_page != 0)
          to_page->setData(multi_sel, t_grd, t_qdate, t_qcount, t_bcount,
                           t_faux_ami,
                           i18n("Properties to original"));

        setModified(false);
        updatePages (type);
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
     tense_page->setEnabled(EntryDlg::EnableAll);
     adj_page->setEnabled(EntryDlg::EnableNone);
   }
   else if (main == QM_ADJ) {
     tense_page->setEnabled(EntryDlg::EnableNone);
     adj_page->setEnabled(EntryDlg::EnableAll);
   }
   else {
     tense_page->setEnabled(EntryDlg::EnableNone);
     adj_page->setEnabled(EntryDlg::EnableNone);
   }
}


void EntryDlg::initFocus()
{

  comm_page->initFocus();
  aux_page->initFocus();
  mc_page->initFocus();
  tense_page->initFocus();
  mc_page->initFocus();
  adj_page->initFocus();
  if (from_page != 0)
    from_page->initFocus();
  if (to_page != 0)
    to_page->initFocus();
}


void EntryDlg::setModified(bool mod)
{
  comm_page->setModified(mod);
  aux_page->setModified(mod);
  mc_page->setModified(mod);
  tense_page->setModified(mod);
  mc_page->setModified(mod);
  adj_page->setModified(mod);
  if (from_page != 0)
    from_page->setModified(mod);
  if (to_page != 0)
    to_page->setModified(mod);
  applyButton->setEnabled(false);
  undoButton->setEnabled(false);
}


void EntryDlg::setEnabled(int enable)
{
   QString type = comm_page->getType();
   QString main;
   int pos;
   if ((pos = type.find (QM_TYPE_DIV)) < 0)  // only use main type
     main = type;
   else
     main = type.left(pos);

  if (enable == EnableOnlyOriginal) {
    comm_page->setEnabled(EnableAll);
    aux_page->setEnabled(EnableAll);
    mc_page->setEnabled(EnableAll);
    tense_page->setEnabled(main == QM_VERB ? EnableAll : EnableNone);
    mc_page->setEnabled(EnableAll);
    adj_page->setEnabled(main == QM_ADJ ? EnableAll : EnableNone);
    if (from_page != 0)
      from_page->setEnabled(EnableNone);
    if (to_page != 0)
      to_page->setEnabled(EnableNone);
  }
  else {
    comm_page->setEnabled(enable);
    aux_page->setEnabled(enable);
    mc_page->setEnabled(enable);
    tense_page->setEnabled(main == QM_VERB ? enable : EnableNone);
    mc_page->setEnabled(enable);
    adj_page->setEnabled(main == QM_ADJ ? enable : EnableNone);
    if (from_page != 0)
      from_page->setEnabled(enable);
    if (to_page != 0)
      to_page->setEnabled(enable);
  }
}


void EntryDlg::slotCancel()
{
  emit sigEditChoice(EditCancel);
}


void EntryDlg::slotApply()
{
  emit sigEditChoice(EditApply);
}


void EntryDlg::slotUndo()
{
  emit sigEditChoice(EditUndo);
}


bool EntryDlg::isModified()
{
  bool mod = comm_page->isModified()
          || aux_page->isModified()
          || tense_page->isModified()
          || mc_page->isModified()
          || adj_page->isModified();

  if (from_page != 0)
    mod |= from_page->isModified();

  if (to_page != 0)
    mod |= to_page->isModified();

  return mod;
}


void EntryDlg::slotDisplayModified()
{
  applyButton->setEnabled(true);
  undoButton->setEnabled(true);
}


void EntryDlg::setCell(int row, int col, const vector<QTableSelection>& sel)
{
  edit_row = row;
  edit_col = col;
//  selections = sel;
}


void EntryDlg::getCell(int &row, int &col, vector<QTableSelection>& sel) const
{
  row = edit_row;
  col = edit_col;
//  sel = selections;
}


void EntryDlg::slotDockVertical()
{
   if (!docked) {
     oldMainPos = mainwin->pos();
     oldMainSize = mainwin->size();
     docked = true;
   }

   QWidget *desk = QApplication::desktop();

   resize(minimumWidth(), desk->height());
   move (0, 0);
   mainwin->resize(desk->width()-width(), desk->height());
   mainwin->move(width(), 0);
}


void EntryDlg::slotDockHorizontal()
{
   if (!docked) {
     oldMainPos = mainwin->pos();
     oldMainSize = mainwin->size();
     docked = true;
   }

   QWidget *desk = QApplication::desktop();

   resize(desk->width(), minimumHeight());
   mainwin->resize(desk->width(), desk->height()-height());
   mainwin->move (0, 0);
   move(0, mainwin->height());
}


void EntryDlg::setAutoApply (bool appl)
{
  autoapply = appl;
  chk_autoapply->setChecked(autoapply);
}


void EntryDlg::slotAutoApplyChecked(bool ena)
{
  autoapply = ena;
}



EntryDlg::~EntryDlg()
{
   if (docked) {
     docked = false;
     mainwin->resize(oldMainSize);
     mainwin->move(oldMainPos);
   }
}


void EntryDlg::closeEvent (QCloseEvent*e)
{
  emit sigEditChoice(EditCancel);
}


#include "EntryDlg.moc"

