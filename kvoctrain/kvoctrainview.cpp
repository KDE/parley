/***************************************************************************

    $Id$

                      view to kvoctrain parts

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.26  2002/01/19 10:33:08  arnold
    made entry dialog modeless

    Revision 1.25  2002/01/05 13:59:20  arnold
    added drop-down arrow to table buttons

    Revision 1.24  2002/01/02 11:38:27  arnold
    fixed displaying of modified doc

    Revision 1.23  2001/12/29 15:49:12  arnold
    fixed kmessagebox calls

    Revision 1.22  2001/12/29 10:40:03  arnold
    merged fixes from POST-branch

    Revision 1.21  2001/12/26 15:10:25  mueller
    CVSSILINT: fixincludes

    Revision 1.20  2001/12/24 14:29:46  arnold
    fixed sorting

    Revision 1.19  2001/12/16 16:51:25  arnold
    fixed keyboard handling in main view

    Revision 1.18  2001/12/14 16:05:49  arnold
    fixed handling of table font

    Revision 1.17  2001/11/25 11:11:03  arnold
    switch for inline edit, splitted kv_resource.h

    Revision 1.16  2001/11/24 17:15:45  arnold
    fixes for table view and query

    Revision 1.15  2001/11/19 20:05:16  arnold
    fixed warning message

    Revision 1.14  2001/11/18 12:28:25  arnold
    provided menu entry for example files

    Revision 1.13  2001/11/16 18:52:59  arnold
    added possibility to disable expressions

    Revision 1.12  2001/11/11 12:51:45  arnold
    fixed some strings for i18n purposes

    Revision 1.11  2001/11/10 22:27:08  arnold
    removed compatibility for kde1

    Revision 1.10  2001/11/10 21:11:57  arnold
    removed icons and compatibilty for kde1

    Revision 1.9  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.8  2001/11/02 17:50:23  arnold
    fixed sorting basically

    Revision 1.7  2001/11/02 10:17:48  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.6  2001/11/01 11:26:12  arnold
    fixed some editing actions

    Revision 1.5  2001/10/22 06:38:58  waba
    Show a cute little marker to indicate marked entries.
    (Instead of using a bold type-face)

    Revision 1.4  2001/10/20 00:58:26  waba
    * Selection fixes
    * Compile fixes

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:36:34  arnold
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


#include <kvoctraincore.h>
#include "kv_resource.h"
#include "kvoctrain.h"
#include "rowtable.h"
#include "entry-dialogs/EntryDlg.h"
#include "kvoctraindoc.h"
#include "kvoctrainview.h"

#include <kapplication.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kconfig.h>
#include <klocale.h>
#include <kdebug.h>

#include <qwidget.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qiconset.h>
#include <qbitmap.h>
#include <qstyle.h>

#include <ctype.h>

#define HEADER_MINSIZE   24
#define KV_COLWIDTH_MARK 20

kvoctrainView::kvoctrainView(kvoctrainDoc* doc,
                             const LangSet &ls, const GradeCols &gradecols,
                             kvoctrainApp *_parent)
	: QWidget(_parent)
{

 the_doc = doc;
 parent = _parent;
 // read the config file entries
 setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
 KConfig *config = KApplication::kApplication()->config();

 config->setGroup(CFG_WINPROP);
 parent->resize (config->readNumEntry(CFG_WINWIDTH, 520),
                 config->readNumEntry(CFG_WINHEIGHT, 300));

 f_list = new QFrame( this, "Frame_2" );
 f_list->setFrameStyle( 50 );
 f_list->resize (width(), height() );

 lb_list = new kvoctrainTable( the_doc, &ls, &gradecols, f_list, "ListBox_1" );

 lb_list->setFrameStyle( 51 );
 lb_list->setLineWidth( 2 );

 if (the_doc->numLangs() == 0)
   the_doc->appendLang (i18n("Original"));

 connect( lb_list, SIGNAL(selected(int)),
          lb_list, SLOT(sortByColumn_alpha(int)) );

 connect( lb_list, SIGNAL(rightButtonClicked(int,int,int)),
          parent, SLOT(slotHeaderMenu(int,int,int)) );

 connect( lb_list, SIGNAL(edited(int,int)),
          parent, SLOT(slotEditEntry(int,int)) );

 connect( lb_list, SIGNAL(sigCancelSelection()),
          parent, SLOT(slotCancelSelection()) );

 connect( lb_list, SIGNAL(currentChanged(int, int)),
          parent, SLOT(slotCurrentCellChanged(int, int)) );

 connect( lb_list, SIGNAL(forwardKeyPressEvent (QKeyEvent*)),
          parent, SLOT(keyPressEvent(QKeyEvent *)) );

 connect( lb_list, SIGNAL(forwardKeyReleaseEvent (QKeyEvent*)),
          parent, SLOT(keyReleaseEvent(QKeyEvent *)) );

 list_layout = new QGridLayout( f_list, 2, 1, 4 );
 list_layout->addWidget( lb_list, 1, 0 );
 list_layout->setRowStretch( 1, 1 );
 list_layout->activate();

 setView(the_doc, ls, gradecols);
}


void kvoctrainView::showEvent (  QShowEvent * s_ev)
{
  QWidget::showEvent(s_ev);
  QResizeEvent r_ev(size(), size());
  resizeEvent (&r_ev);
}


void kvoctrainView::adjustContent()
{
  QResizeEvent r_ev(size(), size());
  resizeEvent (&r_ev);
}


void kvoctrainView::resizeEvent ( QResizeEvent *r_ev )
{
  QWidget::resizeEvent(r_ev);

  if (r_ev == 0)
    return;

  QHeader *header = lb_list->horizontalHeader();
  unsigned oldwidth = 0;
  for (int i = 0; i < lb_list->numCols(); ++i )
    oldwidth += header->sectionSize(i);

  f_list->resize (r_ev->size());
  unsigned newwidth = lb_list->clipper()->width();
  int remain = newwidth;

  switch (header_resizer) {
    case Automatic: {
    // lesson is only half as wide as a original/translation
    // exclude fixed size of "mark"-column
      int x = (remain -KV_COLWIDTH_MARK) / ((lb_list->numCols()-1)*2-1);
      header->resizeSection( KV_COL_LESS, x);
      lb_list->setColumnWidth (KV_COL_LESS, x);
      remain -= x;
      header->resizeSection( KV_COL_MARK, KV_COLWIDTH_MARK);
      lb_list->setColumnWidth (KV_COL_MARK, KV_COLWIDTH_MARK);
      remain -= KV_COLWIDTH_MARK;
      for (int i = KV_COL_ORG; i < lb_list->numCols()-1; i++) {
        remain -= 2*x;
        header->resizeSection(i, 2*x);
        lb_list->setColumnWidth (i, 2*x);
      }
      header->resizeSection(lb_list->numCols()-1, remain);
      lb_list->setColumnWidth (lb_list->numCols()-1, remain);
    }
    break;

    case Percent: {
      float grow = float(newwidth) / float(oldwidth);
      header->resizeSection( KV_COL_MARK, KV_COLWIDTH_MARK);
      lb_list->setColumnWidth (KV_COL_MARK, KV_COLWIDTH_MARK);
      int remain = newwidth - KV_COLWIDTH_MARK;
      int x = QMAX(20, (int)((header->sectionSize(KV_COL_LESS) * grow)+0.5));
      header->resizeSection( KV_COL_LESS, x);
      lb_list->setColumnWidth (KV_COL_LESS, x);
      remain -= x;

      for (int i = KV_COL_ORG; i < lb_list->numCols()-1; i++) {
        x = QMAX(20, (int)((header->sectionSize(i) * grow)+0.5));
        remain -= x;
        header->resizeSection(i, x);
        lb_list->setColumnWidth (i, x);
      }
      header->resizeSection(lb_list->numCols()-1, QMAX(20,remain));
      lb_list->setColumnWidth (lb_list->numCols()-1, QMAX(20,remain));
    }
    break;

    case Fixed:
      // nix
    break;
  }

  if (the_doc != 0) {
    for (int i = KV_COL_ORG; i < lb_list->numCols(); i++) {
      the_doc->setSizeHint (i-KV_EXTRA_COLS, header->sectionSize(i));
    }
    the_doc->setSizeHint (-1, header->sectionSize(KV_COL_LESS));
  }
}


void kvoctrainView::setView(kvoctrainDoc *doc,
                            const LangSet& ls, const GradeCols &gc)
{
 // set header
 the_doc = doc;
 lb_list->setDoc(the_doc, &gc);
 if (the_doc) {
   int id = ls.indexShortId (the_doc->getOriginalIdent());

   setHeaderProp( KV_COL_LESS, i18n("Lesson"),  locate("data", "kvoctrain/lesson.xpm"));
   setHeaderProp( KV_COL_MARK, QString::null,  QString::null);
   lb_list->setColumnWidth(KV_COL_MARK, KV_COLWIDTH_MARK);

   if (id < 0 )
     setHeaderProp( KV_COL_ORG, the_doc->getOriginalIdent());
   else
     setHeaderProp( KV_COL_ORG, ls.longId(id),  ls.PixMapFile(id));

   for (int i = KV_COL_TRANS; i < lb_list->numCols(); i++) {
     int id = ls.indexShortId(the_doc->getIdent(i-KV_EXTRA_COLS));

     if (id < 0 )
       setHeaderProp( i, the_doc->getIdent(i-KV_EXTRA_COLS));
     else
       setHeaderProp( i, ls.longId(id),  ls.PixMapFile(id));
   }

   if (lb_list->currentColumn() < KV_COL_ORG)
     lb_list->setCurrentRow(lb_list->currentRow(), KV_COL_ORG);
 }
 lb_list->updateContents();
}


void kvoctrainView::setInlineEnabled(bool state)
{
  lb_list->setInlineEnabled(state);
}


kvoctrainView::~kvoctrainView()
{
  // write the config file entries
  KConfig *config = KApplication::kApplication()->config();
  config->setGroup(CFG_WINPROP);
  config->writeEntry(CFG_WINWIDTH, width());
  config->writeEntry(CFG_WINHEIGHT, height());
}


void kvoctrainView::setHeaderProp (int id, const QString &name,
                                   const QString &pixfile)
{
  QHeader *header = lb_list->horizontalHeader();

  if (pixfile.isEmpty())
  {
     header->setLabel(id, name);
  }
  else
  {
     QPixmap pix(pixfile);
     int w = pix.width();
     int h = pix.height();

     QPixmap arrow(w+14, h);
     QBitmap mask(arrow.width(), arrow.height());
     arrow.fill(Qt::white);
     mask.fill(Qt::black);

     bitBlt(&arrow, 0, 0, &pix, 0, 0);
     if (pix.mask() != 0)
       bitBlt(&mask, 0, 0, pix.mask(), 0, 0);
     else {
       QBitmap bmp (pix.createHeuristicMask());
       bitBlt(&mask, 0, 0, &bmp, 0, 0);
     }

     QPainter p, pm;
     p.begin( &arrow);
     pm.begin( &mask);

     p.setPen(QColor(0,0,0));
     p.drawLine(w+5, h-2, w+5+1, h-2);
     p.drawLine(w+4, h-3, w+6+1, h-3);
     p.drawLine(w+3, h-4, w+7+1, h-4);
     p.drawLine(w+2, h-5, w+8+1, h-5);
     p.drawLine(w+2, h-6, w+8+1, h-6);

     pm.setPen(QColor(255,255,255));
     pm.drawLine(w+5, h-2, w+5+1, h-2);
     pm.drawLine(w+4, h-3, w+6+1, h-3);
     pm.drawLine(w+3, h-4, w+7+1, h-4);
     pm.drawLine(w+2, h-5, w+8+1, h-5);
     pm.drawLine(w+2, h-6, w+8+1, h-6);

     pm.end();
     p.end();
     arrow.setMask(mask);

     QIconSet set(arrow, QIconSet::Small);
     header->setLabel(id, set, name);
  }
}


///////////////////////////////////////////////////////////


kvoctrainTable::kvoctrainTable(kvoctrainDoc *doc,
                               const LangSet */*ls*/, const GradeCols *gc,
                               QWidget *parent, const char *name )
  : RowTable( doc, SelectCell, gc, parent, name )
{
  setNumCols( doc->numLangs() );
  setNumRows( doc->numEntries() );
}


void kvoctrainTable::setCurrentItem(int row)
{
  setCurrentRow( row, currentColumn() );
}


void kvoctrainTable::sortByColumn_alpha(int header)
{
  if (header == KV_COL_MARK)
   return;

  if (header >= numRows() ) {
    kdError() << "header >= numRows()\n";
    return;
  }

  if (m_rows && !m_rows->isAllowedSorting() ) {
     KMessageBox::information(this, 
               i18n("Sorting is currently turned off for this document.\n"
                    "\n"
                    "Use the document properties dialog to turn sorting on."),
                    kapp->makeStdCaption(""));
    return;
  }

//  emit sigCancelSelection();
  bool sortdir = false;
  QApplication::setOverrideCursor( waitCursor );
  if (m_rows) {
    if (header >= KV_COL_ORG)
      sortdir = m_rows->sort (header-KV_EXTRA_COLS);
    else
      sortdir = m_rows->sortByLesson_alpha();
  }
  horizontalHeader()->setSortIndicator ( header, sortdir);
  repaintContents();
  m_rows->setModified();
  QApplication::restoreOverrideCursor();
}


void kvoctrainTable::sortByColumn_index(int header)
{
  if (header == KV_COL_MARK)
   return;

  if (header >= numRows() ) {
    kdError() << "header >= numRows()\n";
    return;
  }

  if (m_rows && !m_rows->isAllowedSorting() ) {
     KMessageBox::information(this, 
               i18n("Sorting is currently turned off for this document.\n"
                    "\n"
                    "Use the document properties dialog to turn sorting on."),
                    kapp->makeStdCaption(""));
    return;
  }

//  emit sigCancelSelection();
  bool sortdir = false;
  QApplication::setOverrideCursor( waitCursor );
  if (m_rows) {
    if (header >= KV_COL_ORG)
      sortdir = m_rows->sort (header-KV_EXTRA_COLS);
    else
      sortdir = m_rows->sortByLesson_index();
  }
  horizontalHeader()->setSortIndicator ( header, sortdir);
  repaintContents();
  m_rows->setModified();
  QApplication::restoreOverrideCursor();
}


#include "kvoctrainview.moc"
