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


#include "kv_resource.h"
#include "kvoctrain.h"
#include "rowtable.h"
#include "entry-dialogs/EntryDlg.h"
#include "kvoctraindoc.h"
#include "kvoctrainview.h"

#include <kapp.h>
#include <kmessagebox.h>
#include <kstddirs.h>
#include <kconfig.h>
#include <klocale.h>

#include <qwidget.h>
#include <qlayout.h>
#include <qlistbox.h>

#include <ctype.h>

#define HEADER_MINSIZE  24

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

 parent->setMinimumSize(520, 300);

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
 connect( lb_list, SIGNAL(doubleClicked(int,int,int,const QPoint &)),
          parent, SLOT(slotEditEntry(int,int)) );
 connect( lb_list, SIGNAL(sigModified(bool)),
          parent, SLOT(slotModifiedDoc(bool)) );
 connect( lb_list, SIGNAL(sigCancelSelection()),
          parent, SLOT(slotCancelSelection()) );
 connect( lb_list, SIGNAL(cellMoved(int, int)),
          parent, SLOT(slotCurrentCellChanged(int, int)) );
// connect( lb_list, SIGNAL(selected(int,int,int)),
//          parent, SLOT(slotSelectEntry(int,int,int)) );

 list_layout = new QGridLayout( f_list, 2, 1, 4 );
 list_layout->addWidget( lb_list, 1, 0 );
 list_layout->setRowStretch( 1, 1 );
 list_layout->activate();

 setView(the_doc, ls, gradecols);
}


void kvoctrainView::resizeEvent ( QResizeEvent *r_ev )
{
  QWidget::resizeEvent(r_ev);

  if (r_ev == 0)
    return;

  f_list->resize (r_ev->size());
  QHeader *header = lb_list->horizontalHeader();
  int remain = lb_list->clipper()->width();
//  cout << "remain: " << remain << endl;
//  cout << "ev width: " << r_ev->size().width() << endl;
/*
  switch (header_resizer) {
    case Automatic: {
*/
    // lesson is only half as wide as a original/translation
    // exclude fixed size of "mark"-column
      int x = (remain -20) / ((lb_list->numCols()-1)*2-1);
//      cout << "x: " << remain << endl;
      header->resizeSection( KV_COL_LESS, x);
      lb_list->setColumnWidth (KV_COL_LESS, x);
      remain -= x;
      header->resizeSection( KV_COL_MARK, 20);
      lb_list->setColumnWidth (KV_COL_MARK, 20);
      remain -= 20;
      for (int i = KV_COL_ORG; i < lb_list->numCols()-1; i++) {
        remain -= 2*x;
        header->resizeSection(i, 2*x);
        lb_list->setColumnWidth (i, 2*x);
      }
      header->resizeSection(lb_list->numCols()-1, remain);
      lb_list->setColumnWidth (lb_list->numCols()-1, remain);
/*
    }
    break;

    case Percent: {
//      cout << "sz0.1 " << h_list->getHeaderSize(0) << endl;
//      cout << "sz1.1 " << h_list->getHeaderSize(1) << endl;
//      cout << "sz2.1 " << h_list->getHeaderSize(2) << endl;
      // header smaller than listbox to avoid scrollbar
      if (((QWidget*)lb_list->verticalScrollBar())->isVisible() )
        shrink += ((QWidget*)lb_list->verticalScrollBar())->width();
      h_list->resize(lb_list->width()-shrink,
                     h_list->height() );

      int all = 0;
      for (int i = 0; i < lb_list->numCols(); i++)
        all += h_list->getHeaderSize(i);

      if (lb_list->numCols() > 1) {
        int max = h_list->width();
        if (all != 0) {
          int rest = max;
          int i;
          for (i = 0; i < lb_list->numCols()-1; i++) {
            int size = h_list->getHeaderSize(i) * max / all;
            rest -= size;
            h_list->setHeaderSize( i, size );
          }
          h_list->setHeaderSize( i, rest );
        }
        else {
          int rest = max;
          int i;
          int size = max / lb_list->numCols();
          for (i = 0; i < lb_list->numCols()-1; i++) {
            rest -= size;
            h_list->setHeaderSize( i, size );
          }
          h_list->setHeaderSize( i, rest );
        }
      }
      else {
        h_list->setHeaderSize( KV_COL_ORG, h_list->width());
      }
//      cout << "sz0.2 " << h_list->getHeaderSize(0) << endl;
//      cout << "sz1.2 " << h_list->getHeaderSize(1) << endl;
//      cout << "sz2.2 " << h_list->getHeaderSize(2) << endl;
    }
    break;

    case Fixed:
      //
    break;
  }
*/
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
   setHeaderProp( KV_COL_MARK, QString::null,  QString::null); //locate("data", "kvoctrain/mark.png"));
   lb_list->setColumnWidth(KV_COL_MARK, 20);

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
     header->setLabel(id, pix, name);
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

  if (m_rows && !m_rows->isAllowedSorting() ) {
     KMessageBox::information(this, 
               i18n("Sorting is currently turned off for this document.\n"
                    "\n"
                    "Use the document properties dialog to turn sorting on."),
                    kapp->makeStdCaption(""), i18n("&OK"));
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
  updateContents(0, currentColumn());
  m_rows->setModified();
  QApplication::restoreOverrideCursor();
}


void kvoctrainTable::sortByColumn_index(int header)
{
  if (header == KV_COL_MARK)
   return;

  if (m_rows && !m_rows->isAllowedSorting() ) {
     KMessageBox::information(this, 
               i18n("Sorting is currently turned off for this document.\n"
                    "\n"
                    "Use the document properties dialog to turn sorting on."),
                    kapp->makeStdCaption(""), i18n("&OK"));
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
  updateContents(0, currentColumn());
  m_rows->setModified();
  QApplication::restoreOverrideCursor();
}


#include "kvoctrainview.moc"
