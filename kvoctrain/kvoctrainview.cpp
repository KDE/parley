/***************************************************************************

    $Id$

                      view to kvoctrain parts

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                           (C) 2004 Peter Hedlund

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

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
#include "kvoctrain.h"
#include "kvoctrainview.h"

#include <kstandarddirs.h>
#include <kconfig.h>
#include <klocale.h>
#include <kdebug.h>
#include <dcopclient.h>
#include <kapplication.h>

#include <qlayout.h>
#include <qbitmap.h>
#include <qfile.h>

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

 setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));

 lb_list = new kvoctrainTable( the_doc, &ls, &gradecols, this, "ListBox_1" );

 lb_list->setLineWidth( 2 );

 if (the_doc->numLangs() == 0)
   the_doc->appendLang (i18n("Original"));

 connect( lb_list, SIGNAL(selected(int)),
          lb_list, SLOT(sortByColumn_alpha(int)) );

 connect( lb_list, SIGNAL(rightButtonClicked(int,int,int)),
          parent, SLOT(slotHeaderMenu(int,int,int)) );

 connect( lb_list, SIGNAL(edited(int,int)),
          parent, SLOT(slotEditEntry(int,int)) );

 connect( lb_list, SIGNAL(currentChanged(int, int)),
          parent, SLOT(slotCurrentCellChanged(int, int)) );

 connect( lb_list, SIGNAL(selectionChanged()),
          lb_list, SLOT(slotSelectionChanged()) );

 connect( lb_list, SIGNAL(forwardKeyPressEvent (QKeyEvent*)),
          parent, SLOT(keyPressEvent(QKeyEvent *)) );

 connect( lb_list, SIGNAL(forwardKeyReleaseEvent (QKeyEvent*)),
          parent, SLOT(keyReleaseEvent(QKeyEvent *)) );

 list_layout = new QGridLayout( this, 2, 1/*, 4 */);
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
    QPixmap pix;     
    if (QFile::exists(pixfile))
      pix.load(pixfile);
    else
    {
      //this is the code used by kxkb when a flag image can't be found
      //see kdebase/kxkb/pixmap.cpp/LayoutIcon::findPixmap()
      pix.resize(21, 14);
      pix.fill(Qt::white);
      
      QPainter p(&pix);
      
      QFont font("sans");
      font.setPixelSize(10);
      font.setWeight(QFont::Bold);
      p.setFont(font);
      p.setPen(Qt::red);
      p.drawText(2, 1, pix.width(), pix.height()-2, Qt::AlignCenter, "err");
      p.setPen(Qt::blue);
      p.drawText(1, 0, pix.width(), pix.height()-2, Qt::AlignCenter, "err");     
    }
     
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
                               const LangSet *ls, const GradeCols *gc,
                               QWidget *parent, const char *name )
  : RowTable( doc, SelectCell, gc, parent, name ), langs(ls)
{
  setNumCols( doc->numLangs() );
  setNumRows( doc->numEntries() );
}


void kvoctrainTable::setCurrentItem(int row)
{
  setCurrentRow( row, currentColumn() );
}

QWidget* kvoctrainTable::beginEdit(int row, int col, bool replace)
{
  if (KApplication::dcopClient()->isApplicationRegistered("kxkb")) {
  
    if (m_rows) {
      QString id = (col == KV_COL_ORG) ? m_rows->getOriginalIdent()
	: m_rows->getIdent(col - KV_EXTRA_COLS);
      
      if (langs) {
	QString kbLayout(langs->keyboardLayout(langs->indexShortId(id)));
	if (!kbLayout.isEmpty()) {
	  QByteArray data, replyData;
	  QCString replyType;
	  QDataStream arg(data, IO_WriteOnly);
	  arg << kbLayout;

	  if (!KApplication::dcopClient()->call("kxkb", "kxkb", 
						"setLayout(QString)",
						data, replyType, replyData)) {
	    kdDebug() << "kskb dcop error" << endl;
	  }
	}
      }
    }
  }
  return RowTable::beginEdit(row, col, replace);
}

void kvoctrainTable::endEdit(int row, int col, bool accept, bool replace)
{
//   if (KApplication::dcopClient()->isApplicationRegistered("kxkb")) {
//     QByteArray data, replyData;
//     QCString replyType;
    
//     if (!KApplication::dcopClient()->call("kxkb", "kxkb", 
// 					  "setLayout(QString)",
// 					  data, replyType, replyData)) {
//   }
  RowTable::endEdit(row, col, accept, replace);
}

void kvoctrainTable::sortByColumn(int header, bool alpha) {
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

  QApplication::setOverrideCursor( waitCursor );

  clearSelection();

  bool hasinlineenabled = getInlineEnabled();
  setInlineEnabled(false);

  bool sortdir = false;
  if (m_rows) {
    if (header >= KV_COL_ORG)
      sortdir = m_rows->sort (header-KV_EXTRA_COLS);
    else
      if (alpha)
        sortdir = m_rows->sortByLesson_alpha();
      else
        sortdir = m_rows->sortByLesson_index();
  }
  horizontalHeader()->setSortIndicator ( header, sortdir);
  repaintContents();
  m_rows->setModified();
  emit currentChanged(currentRow(), currentColumn());
  setInlineEnabled(hasinlineenabled);
  QApplication::restoreOverrideCursor();
}

void kvoctrainTable::sortByColumn_alpha(int header)
{
  sortByColumn(header, true);
}


void kvoctrainTable::slotSelectionChanged()
{
  emit currentChanged(currentRow(), currentColumn());
}


void kvoctrainTable::sortByColumn_index(int header)
{
  sortByColumn(header, false);
}


#include "kvoctrainview.moc"
