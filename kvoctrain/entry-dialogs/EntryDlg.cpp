/***************************************************************************

                   entry dialog for table cell contents

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

#include <QLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFrame>
#include <QCloseEvent>

#include <kmainwindow.h>
#include <kapplication.h>
#include <kwinmodule.h>

#include "EntryDlg.h"
#include <langset.h>
#include <klocale.h>

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
  const          Article &/*article*/,
  const          Comparison &comp,
  const          MultipleChoice &mc,
  QueryManager  &querymanager,
  const QString &title,
  bool           active,
  const QFont&   ipafont,
  QWidget       *parent,
  const char    *name,
  bool           modal)
  :
  KDialogBase(Tabbed, title, User1|User2|User3|Apply|Close, Apply, parent, name, modal, false,
    KGuiItem(i18n("&Reset")),
    KGuiItem(QString::null, "view_left_right"),
    KGuiItem(QString::null, "view_top_bottom"))

{
  mainwin = main;
  docked = false;
  edit_row = -1;
  edit_col = -1;
  from_page = 0;
  to_page = 0;

  QString s;
  if (langset.findLongId(lang).isEmpty() )
    s = lang;
  else
    s = langset.findLongId(lang);

  QFrame *page;
  QVBoxLayout *topLayout;

  if (origin)
  {
    page = addPage( i18n("Co&mmon") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    comm_page = new CommonEntryPage (doc, multi_sel, expr, lesson, lessonbox,
                                      lang, type, pronunce, usagelabel,
                                      i18n("Original &expression in %1:").arg(s), querymanager, active,
                                      ipafont, page);
    topLayout->addWidget(comm_page);

    page = addPage( i18n("A&dditional") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    aux_page = new AuxInfoEntryPage (multi_sel, synonym, antonym, example, rem, paraphrase, page);
    topLayout->addWidget(aux_page);

    page = addPage( i18n("&Multiple Choice") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    mc_page = new MCEntryPage (multi_sel, mc, page);
    topLayout->addWidget(mc_page);

    page = addPage( i18n("Con&jugation") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    tense_page = new TenseEntryPage (multi_sel, con_prefix, conjugations, page);
    topLayout->addWidget(tense_page);

    page = addPage( i18n("Compar&ison") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    adj_page = new AdjEntryPage (multi_sel, comp, page);
    topLayout->addWidget(adj_page);
  }
  else
  {
    page = addPage( i18n("Co&mmon") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    comm_page = new CommonEntryPage (doc, multi_sel, expr, lesson, lessonbox,
                                      lang, type, pronunce, usagelabel,
                                      i18n("Translated &expression in %1:").arg(s), querymanager, active,
                                      ipafont, page);
    topLayout->addWidget(comm_page);

    page = addPage( i18n("A&dditional") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    aux_page = new AuxInfoEntryPage (multi_sel, synonym, antonym, example, rem, paraphrase, page);
    topLayout->addWidget(aux_page);

    page = addPage( i18n("&Multiple Choice") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    mc_page = new MCEntryPage (multi_sel, mc, page);
    topLayout->addWidget(mc_page);

    page = addPage( i18n("Con&jugation") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    tense_page = new TenseEntryPage (multi_sel, con_prefix, conjugations, page);
    topLayout->addWidget(tense_page);

    page = addPage( i18n("Compar&ison") );
    topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
    adj_page = new AdjEntryPage (multi_sel, comp, page);
    topLayout->addWidget(adj_page);
  }

  page = addPage( i18n("&From Original") );
  topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
  from_page = new FromToEntryPage (multi_sel, f_grd, f_qdate, f_qcount, f_bcount, f_faux_ami,
                                   i18n("Properties From Original"), page);
  topLayout->addWidget(from_page);

  page = addPage( i18n("&To Original") );
  topLayout = new QVBoxLayout( page, KDialog::marginHint(), KDialog::spacingHint() );
  to_page   = new FromToEntryPage (multi_sel, t_grd, t_qdate, t_qcount, t_bcount, t_faux_ami,
                                   i18n("Properties to Original"), page);
  topLayout->addWidget(to_page);

  updatePages (type);

  connect(comm_page, SIGNAL(typeSelected(const QString&)), SLOT(updatePages(const QString&)) );

  connect( this, SIGNAL(user1Clicked()), this, SLOT(slotUndo()) );
  connect( this, SIGNAL(applyClicked()), this, SLOT(slotApply()) );
  connect( this, SIGNAL(user2Clicked()), this, SLOT(slotDockVertical()) );
  connect( this, SIGNAL(user3Clicked()), this, SLOT(slotDockHorizontal()) );

  connect (comm_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
  connect (aux_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
  connect (adj_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
  connect (mc_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
  connect (tense_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));

  if (from_page != 0)
    connect (from_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));
  if (to_page != 0)
    connect (to_page, SIGNAL(sigModified()), this, SLOT(slotDisplayModified() ));

  enableButtonApply(false);
  enableButton(User1, false);
  comm_page->expr_line->setFocus();
}


void EntryDlg::setData(
  kvoctrainDoc  */*doc*/,
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
  const          Conjugation &/*con_prefix*/,
  const          Conjugation &conjugations,
  const          Article &/*article*/,
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
    comm_page->setData(multi_sel, expr, lesson, lessonbox, lang, type, pronunce, usagelabel,
      i18n("Original &expression in %1:").arg(s), querymanager, active);
  else
    comm_page->setData(multi_sel, expr, lesson, lessonbox, lang, type, pronunce, usagelabel,
      i18n("Translated &expression in %1:").arg(s), querymanager, active);

  adj_page->setData(multi_sel, comp);
  aux_page->setData(multi_sel, synonym, antonym, example, rem, paraphrase);
  mc_page->setData(multi_sel, mc);
  tense_page->setData(multi_sel, conjugations);
  if (from_page != 0)
    from_page->setData(multi_sel, f_grd, f_qdate, f_qcount, f_bcount, f_faux_ami, i18n("Properties From Original"));
  if (to_page != 0)
    to_page->setData(multi_sel, t_grd, t_qdate, t_qcount, t_bcount, t_faux_ami, i18n("Properties to Original"));

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
  enableButtonApply(false);
  enableButton(User1, false);
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

  if (enable == EnableOnlyOriginal)
  {
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
  else
  {
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
  enableButtonApply(true);
  enableButton(User1, true);
}


void EntryDlg::setCell(int row, int col, const vector<Q3TableSelection>& sel)
{
  edit_row = row;
  edit_col = col;
  selections = sel;
}


void EntryDlg::getCell(int &row, int &col, vector<Q3TableSelection>& sel) const
{
  row = edit_row;
  col = edit_col;
  sel = selections;
}


void EntryDlg::slotDockVertical()
{
  if (!docked) {
    oldMainPos = mainwin->pos();
    oldMainSize = mainwin->size();
    docked = true;
  }

  KWinModule info;
  QRect rect = info.workArea();

  int diff_x = frameGeometry().width()-width();
  int diff_y = frameGeometry().height()-height();
  resize(minimumWidth(), rect.height()-diff_y);
  mainwin->resize(rect.width()-frameGeometry().width()-diff_x,
                  rect.height()-diff_y);
  move (0, 0);
  mainwin->move(frameGeometry().width(), 0);
}


void EntryDlg::slotDockHorizontal()
{
  if (!docked) {
    oldMainPos = mainwin->pos();
    oldMainSize = mainwin->size();
    docked = true;
  }

  KWinModule info;
  QRect rect = info.workArea();

  int diff_x = frameGeometry().width()-width();
  int diff_y = frameGeometry().height()-height();

  resize(rect.width()-diff_x, minimumHeight());
  mainwin->resize(rect.width()-diff_x,
                  rect.height()-frameGeometry().height()-diff_y);
  move(0, 0);
  mainwin->move (0, frameGeometry().height());
}


EntryDlg::~EntryDlg()
{
  if (docked) {
    docked = false;
    mainwin->resize(oldMainSize);
    mainwin->move(oldMainPos);
  }
}


void EntryDlg::reject ()
{
  emit sigEditChoice(EditCancel);
}


void EntryDlg::closeEvent (QCloseEvent * /*e*/)
{
  emit sigEditChoice(EditCancel);
}


#include "EntryDlg.moc"

