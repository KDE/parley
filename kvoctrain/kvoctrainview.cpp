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
#include "header.h"
#include "entry-dialogs/EntryDlg.h"
#include "kvoctraindoc.h"
#include "kvoctrainview.h"

#include <kapp.h>
#include <kmessagebox.h>
#include <kstddirs.h>
#include <kconfig.h>

#include <qwidget.h>
#include <qlayout.h>
#include <qlistbox.h>

#include <ctype.h>

#define HEADER_MINSIZE  24

kvoctrainView::kvoctrainView(KApplication* a,kvoctrainDoc* doc,
                             const LangSet &ls, const GradeCols &gradecols,
                             kvoctrainApp *_parent)
	: QWidget(_parent)
{

 the_doc = doc;
 parent = _parent;
 // read the config file entries
 setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
 KConfig *config = EA_KappGetConfig;

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

 h_list = new KHeader( f_list, "Header_1", 1, KHeader::Resizable | KHeader::Buttons );
 h_list->setMinimumSize (100, HEADER_MINSIZE);
 if (the_doc->numLangs() == 0)
   the_doc->appendLang (i18n("Original"));

 connect( h_list, SIGNAL(selected(int)),
          lb_list, SLOT(sortByColumn_alpha(int)) );
 connect( h_list, SIGNAL(sizeChanged(int,int)),
          lb_list, SLOT(setColumnWidth(int,int)) );
 connect( h_list, SIGNAL(rightButtonClicked(int,int,int)),
          parent, SLOT(slotHeaderMenu(int,int,int)) );

 connect( lb_list, SIGNAL(hSliderMoved(int)),
          h_list, SLOT(setOrigin(int)) );
 connect( lb_list, SIGNAL(vSliderMoved(int)),
          this, SLOT(slotVSliderChanged(int)) );
 connect( lb_list, SIGNAL(vSliderPressed(bool, int)),
          this, SLOT(slotVSliderPressed(bool, int)) );

 connect( lb_list, SIGNAL(rightButtonClicked()),
          this, SLOT(popupMenu()) );
 connect( lb_list, SIGNAL(edited(int,int)),
          parent, SLOT(slotEditEntry(int,int)) );
 connect( lb_list, SIGNAL(selected(int,int,int)),
          parent, SLOT(slotSelectEntry(int,int,int)) );
 connect( lb_list, SIGNAL(leftButtonClicked(int, int, int)),
          parent, SLOT(slotTagEntry(int, int, int)) );
//connect( lb_list, SIGNAL(cellMoved(int, int, int)),
//          parent, SLOT(slotCellMoved(int, int, int)) );
 connect( lb_list, SIGNAL(sigModified(bool)),
          parent, SLOT(slotModifiedDoc(bool)) );
 connect( lb_list, SIGNAL(sigCancelSelection()),
          parent, SLOT(slotCancelSelection()) );

 list_layout = new QGridLayout( f_list, 2, 1, 4 );
 list_layout->addWidget( h_list, 0, 0 );
 list_layout->addWidget( lb_list, 1, 0 );
 list_layout->setRowStretch( 1, 1 );
 list_layout->activate();

 int lsize = config->readNumEntry(CFG_LESSONSIZE, 100);
 h_list->setHeaderSize( KV_COL_LESS, lsize);

 setView(the_doc, ls, gradecols);

}


void kvoctrainView::popupMenu()
{
// if (col-clicked == KV_COL_LESS) return;
}


void kvoctrainView::slotVSliderChanged (int val)
{
  emit vSliderTrack(val);
}


void kvoctrainView::slotVSliderPressed (bool state, int val)
{
  emit vSliderPressed(state, val);
}


void kvoctrainView::setView(kvoctrainDoc *doc,
                            const LangSet& ls, const GradeCols &gc)
{
 // set header
 the_doc = doc;
 lb_list->setDoc(the_doc, &ls, &gc);
 if (the_doc) {
   h_list->setNumHeaders(lb_list->numCols() );

   int id = ls.indexShortId (the_doc->getOriginalIdent());

//   h_list->setHeaderLabel( KV_COL_LESS, i18n("Lesson"), EA_QtNS(AlignHCenter) | EA_QtNS(AlignVCenter) );
   setHeaderProp( KV_COL_LESS, i18n("Lesson"),  EA_KDEDATADIR("", "kvoctrain/lesson.xpm"));

   if (id < 0 )
     h_list->setHeaderLabel( KV_COL_ORG, the_doc->getOriginalIdent(), EA_QtNS(AlignHCenter) | EA_QtNS(AlignVCenter) );
   else
     setHeaderProp( KV_COL_ORG, ls.longId(id),  ls.PixMapFile(id));

   for (int i = KV_COL_TRANS; i < lb_list->numCols(); i++) {
     int id = ls.indexShortId(the_doc->getIdent(i-KV_EXTRA_COLS));

     if (id < 0 )
       h_list->setHeaderLabel( i, the_doc->getIdent(i-KV_EXTRA_COLS), EA_QtNS(AlignHCenter) | EA_QtNS(AlignVCenter) );
     else
       setHeaderProp( i, ls.longId(id),  ls.PixMapFile(id));
   }

   if (the_doc->getSizeHint(-1) > 0)
     h_list->setHeaderSize( KV_COL_LESS, the_doc->getSizeHint(-1) );
//   cout << "setview::sz0 " << h_list->getHeaderSize(KV_COL_LESS) << endl;

   for (int i = 0; i < the_doc->numLangs(); i++) {
     h_list->setHeaderSize( i+KV_COL_ORG, the_doc->getSizeHint(i) );
//     cout << "setview::sz" << i+1 << " " << h_list->getHeaderSize(i+KV_COL_ORG) << endl;
   }

   if (getTable()->currentCol() < KV_COL_ORG)
     getTable()->setCurrentRow(getTable()->currentRow(), KV_COL_ORG);
   emit getTable()->cellMoved (getTable()->currentRow(),
                               getTable()->currentCol(), 0);
 }

 if (header_resizer !=  Fixed) {
    showEvent (0);
 }
}


kvoctrainView::~kvoctrainView()
{
  // write the config file entries
  KConfig *config = EA_KappGetConfig;
  config->setGroup(CFG_WINPROP);
  config->writeEntry(CFG_WINWIDTH, width());
  config->writeEntry(CFG_WINHEIGHT, height());
  config->writeEntry(CFG_LESSONSIZE, h_list->getHeaderSize(KV_COL_LESS));
}


void kvoctrainView::showEvent( QShowEvent * )
{
  QResizeEvent r_ev (size(), size());
  resizeEvent ( &r_ev );
}


void kvoctrainView::resizeEvent ( QResizeEvent *r_ev )
{
  if (r_ev == 0)
    return;

  f_list->resize (r_ev->size());
  int shrink = 0;

  switch (header_resizer) {
    case Automatic: {
      // header smaller than listbox to avoid scrollbar
      if (((QWidget*)lb_list->verticalScrollBar())->isVisible() )
        shrink += ((QWidget*)lb_list->verticalScrollBar())->width();
      h_list->resize(lb_list->width()-shrink,
                     h_list->height() );
    
      // lesson is only half as wide as a original/translation
      if (lb_list->numCols() > 1) {
        int x = (h_list->width()) / ((lb_list->numCols())*2-1);
        h_list->setHeaderSize( KV_COL_LESS, x);
        for (int i = KV_COL_ORG; i < lb_list->numCols(); i++)
          h_list->setHeaderSize( i, 2*x);
      }
      else {
        h_list->setHeaderSize( KV_COL_ORG, h_list->width());
      }
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

  if (the_doc != 0) {
    for (int i = KV_COL_ORG; i < lb_list->numCols(); i++) {
      the_doc->setSizeHint (i-KV_EXTRA_COLS, h_list->getHeaderSize(i));
    }
    the_doc->setSizeHint (-1, h_list->getHeaderSize(KV_COL_LESS));
  }

}


void kvoctrainView::setHeaderProp (int id, const QString &name,
                                   const QString &pixfile)
{
  QPixmap pix(pixfile);
//  if (pixfile.length() == 0)
//     pix.resize(1, HEADER_MINSIZE-2*3);
  h_list->setHeaderLabel (id, name);
  h_list->setHeaderPixmap (id, &pix);
  h_list->setHeaderSize( 0, h_list->getHeaderSize( 0 ) );

  vector<QString> labels;
  QString s;
  for (int i = 0; i < getTable()->numCols(); i++) {
    h_list->getHeaderLabel(i, s);
    labels.push_back(s);
  }

  QString forbidden;
//  h_list->setHeaderAccel( KV_COL_LESS, i18n("Lesson"));
//  forbidden += h_list->getHeaderAccel(KV_COL_LESS);
  int accel;
  for (int i = KV_COL_LESS; i < (int) labels.size(); i++) {
    if (RowTable::createMenuNames(parent->used_Menus, labels, i, accel)) {
      h_list->setHeaderAccel(i, (char) labels[i][accel].lower() );
    }
    else
      h_list->setHeaderAccel(i, 0);
  }
  h_list->repaint();
}


///////////////////////////////////////////////////////////

kvoctrainTable::kvoctrainTable(kvoctrainDoc *doc,
                               const LangSet *ls, const GradeCols *gc,
                               QWidget *parent, const char *name )
  : RowTable( doc, SelectCell, ls, gc, parent, name )
{
  bool update = autoUpdate();
  setAutoUpdate(false);
  setNumCols( doc->numLangs() );
  setNumRows( doc->numEntries() );
  setAutoUpdate(update);
  setCellHeight( fontMetrics().lineSpacing() );
}


void kvoctrainTable::appendItem(kvoctrainExpr *expr)
{
  if (m_rows) {
    m_rows->appendEntry (expr);
    m_rows->setModified();
    emit sigModified(true);
  }
  updateViewPort();
  repaintCells();
}


void kvoctrainTable::appendCol()
{
  if (m_rows) {
    m_rows->appendLang ("");
    m_rows->setModified();
    emit sigModified(true);
  }
  updateViewPort();
  repaintCells();
}


void kvoctrainTable::removeCurrentItem()
{
  if (m_rows) {
    m_rows->removeEntry (currentRow() );
    m_rows->setModified();
    emit sigModified(true);
  }
  updateViewPort();
  repaintCells();
}


void kvoctrainTable::setCurrentItem(int row)
{
  setCurrentRow( row, currentCol() );
}


void kvoctrainTable::sortByColumn_alpha(int header)
{
  if (m_rows && !m_rows->isAllowedSorting() ) {
     KMessageBox::information(this, 
               i18n("Sorting is currently turned off for this document.\n"
                    "\n"
                    "Use the document properties dialog to turn sorting on."),
                    kvoctrainApp::generateCaption(""), i18n("&OK"));
    return;
  }

//  emit sigCancelSelection();
  QApplication::setOverrideCursor( waitCursor );
  if (m_rows) {
    if (header >= KV_COL_ORG)
      m_rows->sort (header-KV_EXTRA_COLS);
    else
      m_rows->sortByLesson_alpha();
  }
  repaintCells();
  m_rows->setModified();
  setCurrentRow(0, currentCol());
  QApplication::restoreOverrideCursor();
}


void kvoctrainTable::sortByColumn_index(int header)
{
  if (m_rows && !m_rows->isAllowedSorting() ) {
     KMessageBox::information(this, 
               i18n("Sorting is currently turned off for this document.\n"
                    "\n"
                    "Use the document properties dialog to turn sorting on."),
                    kvoctrainApp::generateCaption(""), i18n("&OK"));
    return;
  }

//  emit sigCancelSelection();
  QApplication::setOverrideCursor( waitCursor );
  if (m_rows) {
    if (header >= KV_COL_ORG)
      m_rows->sort (header-KV_EXTRA_COLS);
    else
      m_rows->sortByLesson_index();
  }
  repaintCells();
  m_rows->setModified();
  setCurrentRow(0, currentCol());
  QApplication::restoreOverrideCursor();
}


#include "kvoctrainview.moc"
