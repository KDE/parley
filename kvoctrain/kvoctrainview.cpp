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

 if (the_doc->numLangs() == 0)
   the_doc->appendLang (i18n("Original"));

 connect( lb_list, SIGNAL(doubleClicked(int,int,int,const QPoint &)),
          parent, SLOT(slotEditEntry(int,int)) );
//connect( lb_list, SIGNAL(currentChanged(int, int)),
//          parent, SLOT(slotCurrentChanged(int, int)) );
 connect( lb_list, SIGNAL(sigModified(bool)),
          parent, SLOT(slotModifiedDoc(bool)) );
 connect( lb_list, SIGNAL(sigCancelSelection()),
          parent, SLOT(slotCancelSelection()) );

 list_layout = new QGridLayout( f_list, 2, 1, 4 );
 list_layout->addWidget( lb_list, 1, 0 );
 list_layout->setRowStretch( 1, 1 );
 list_layout->activate();

 setView(the_doc, ls, gradecols);

}

void kvoctrainView::setView(kvoctrainDoc *doc,
                            const LangSet& ls, const GradeCols &gc)
{
 // set header
 the_doc = doc;
 lb_list->setDoc(the_doc, &ls, &gc);
 if (the_doc) {
   int id = ls.indexShortId (the_doc->getOriginalIdent());

   setHeaderProp( KV_COL_LESS, i18n("Lesson"),  EA_KDEDATADIR("", "kvoctrain/lesson.xpm"));
   setHeaderProp( KV_COL_MARK, QString::null,  EA_KDEDATADIR("", "kvoctrain/mark.png"));
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
  KConfig *config = EA_KappGetConfig;
  config->setGroup(CFG_WINPROP);
  config->writeEntry(CFG_WINWIDTH, width());
  config->writeEntry(CFG_WINHEIGHT, height());
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
                               const LangSet *ls, const GradeCols *gc,
                               QWidget *parent, const char *name )
  : RowTable( doc, SelectCell, ls, gc, parent, name )
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
  updateContents(0, currentColumn());
  m_rows->setModified();
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
  updateContents(0, currentColumn());
  m_rows->setModified();
  QApplication::restoreOverrideCursor();
}


#include "kvoctrainview.moc"
