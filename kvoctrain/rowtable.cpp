/***************************************************************************

    $Id$

                 table view to kvoctrain document

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : taken from klpq and modified as needed
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
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

//
//  klpq
//
//  Copyright (C) 1997 Christoph Neerfeld
//  email:  Christoph.Neerfeld@home.ivm.de or chris@kde.org

#include <qkeycode.h>
#include <qscrollbar.h>
#include <qpainter.h>
#include <kapp.h>

#include <iostream.h>

#include "kvoctraindoc.h"
#include "rowtable.h"
#include "kv_resource.h"
#include "langset.h"

RowTable::RowTable(kvoctrainDoc *rows, Flags flags,
                   const LangSet *ls, const GradeCols *gc,
                   QWidget *parent, const char *name )
	: QTable( parent, name )
{
	init(flags);
        setDoc (rows, ls, gc);
        setFocus();
}


RowTable::~RowTable()
{
}

void RowTable::updateContents(int row, int col)
{
      int current_row = row;
      if (current_row < 0)
         current_row = currentRow();
      int current_col = col;
      if (current_col < 0)
         current_col = currentColumn();
      int ncols, nrows;
      if (m_rows) {
        nrows = m_rows->numEntries();
        ncols = QMAX (1, m_rows->numLangs())+KV_EXTRA_COLS;
      }
      else {
        nrows = 0;
        ncols = 1+KV_EXTRA_COLS;
      }
      if (nrows != numRows())
        setNumRows(nrows);
      if (ncols != numCols())
        setNumCols(ncols);
      current_row = QMIN (current_row, numRows()-1);
      current_col = QMIN (current_col, numCols()-1); 
      bool b = signalsBlocked();
      blockSignals(true);
      setCurrentRow (current_row, current_col);
      blockSignals(b);
      update();
      emit currentChanged(current_row, current_col);
}


SpecFont_t RowTable::getColFont(int index)
{
  SpecFont_t font;
  if (index < (int) m_colFonts.size() && index >= 0)
    font = m_colFonts[index];
  else
    font.specfont = false;

  return font;
}


void RowTable::setLangSet (const LangSet *ls)
{
  if (m_colFonts.size() == 0 || ls == 0 || m_rows == 0)
    return;

  int id = ls->indexShortId (m_rows->getOriginalIdent());
  if (id >= 0) {
    ls->Font(id, m_colFonts[KV_COL_ORG].font,
            m_colFonts[KV_COL_ORG].specfont);
  }

  for (int i = KV_COL_TRANS; i < numCols(); i++) {
    id = ls->indexShortId(m_rows->getIdent(i-KV_EXTRA_COLS));
    if (id >= 0)
      ls->Font(id, m_colFonts[i].font, m_colFonts[i].specfont);
  }
}

void RowTable::setDoc(kvoctrainDoc *rows, const LangSet  *ls,
                      const GradeCols *gc)
{
  if (rows) {
     m_rows = rows;
     setNumRows( rows->numEntries() );
     setNumCols( QMAX (1+KV_EXTRA_COLS, m_rows->numLangs()+KV_EXTRA_COLS) );
     setCurrentRow (0, 0);
  }
  else {
    setNumRows( 0 );
    setNumCols( 1+KV_EXTRA_COLS );
    m_rows = 0;
  }
  setLangSet (ls);
  gradecols = gc;
}


void RowTable::init(Flags flags)
{
	m_flags = flags;

	setFrameStyle( WinPanel | Sunken );
	setBackgroundColor( colorGroup().base() );
	setLeftMargin(0);
#if QT_VERSION < 300
	setSelectionMode(Multi);
#else
	setSelectionMode(MultiRow);
#endif

        setFocusPolicy( StrongFocus );
}

void RowTable::setNumCols( int cols )
{
        cols = QMAX (1, cols );

	int oldsize = m_colFonts.size();
        m_colFonts.resize( cols );
	for( int i=oldsize ; i<cols ; i++) {
       	  m_colFonts[i].font = QFont();
       	  m_colFonts[i].specfont = false;
        }

	QTable::setNumCols( cols );
}


void RowTable::setNumRows( int rows )
{
	QTable::setNumRows( rows );
}

void RowTable::paletteChange( const QPalette &) //oldPalette )
{
	setBackgroundColor( colorGroup().base() );
}


void RowTable::paintCell( QPainter *p, int row, int col, const QRect &cr, bool selected)
{
    if ( cr.width() == 0 || cr.height() == 0 )
        return;

   QColorGroup cg = colorGroup();
  
#if 0  
   if ( /*(focusStl == SpreadSheet) && */ selected &&
	 (row == currentRow()) &&
	 (col == currentColumn()) && ( hasFocus() || viewport()->hasFocus() ) )
	selected = FALSE;
#endif

    int w = cr.width();
    int h = cr.height();
    int x2 = w - 1;
    int y2 = h - 1;

    kvoctrainExpr *cell = getRow( row );

    p->fillRect( 0, 0, w, h, selected ? cg.brush( QColorGroup::Highlight ) : cg.brush( QColorGroup::Base ) );

    if( cell ) {
       p->save();
       cell->paint( p, col, w, selected, m_rows, col, gradecols, &m_colFonts[col]);
       p->restore();
    }

    if ( true ) {
	// Draw our lines
	QPen pen( p->pen() );
	p->setPen( colorGroup().mid() );
	p->drawLine( x2, 0, x2, y2 );
	p->drawLine( 0, y2, x2, y2 );
	p->setPen( pen );
    }
}

QWidget *RowTable::createEditor(int, int, bool) const
{
   return 0; // No inline editing
}

kvoctrainExpr *RowTable::getRow( int row )
{
  if (m_rows)
    return m_rows->getEntry(row);
  else
    return 0;
}

void RowTable::setCurrentRow( int row, int col )
{
        QTable::setCurrentCell(row, col);
}

void RowTable::setSelectColumn( int col )
{
        setCurrentRow(currentRow(), col);;
}

bool RowTable::createMenuNames (vector<QString> forbidden_labels,
                                vector<QString> names,
                                int index,
                                int &accel_index)
{
   QString forbidden;
   for (int i = 0; i < (int) forbidden_labels.size(); i++) {
     int pos;
     if ((pos = forbidden_labels[i].find('&'))>= 0)
       if ((int) forbidden_labels[i].length() > pos+1)
          forbidden += forbidden_labels[i][pos+1];
   }
   return createMenuNames (forbidden, names, index, accel_index);
}


bool RowTable::createMenuNames (QString forbidden_chars,
                                vector<QString> names,
                                int index,
                                int &accel_index)
{
   accel_index = 0;
   if (names.size() == 0
       || index > (int) names.size())
     return false;
   else if (names.size() == 1)
     return true;

   QString pool = "0123456789abcdefghijklmnopqrstuvwxyz";
   int pos;
   forbidden_chars = forbidden_chars.lower()+"&";

   for (int i = 0; i < (int) forbidden_chars.length(); i++)
     if ((pos = pool.find(forbidden_chars[i])) >= 0)
        pool.remove(pos, 1);

   vector<int> key_index;
   for (int i = 0; i < (int) names.size(); i++) {
     names[i] = names[i].lower();
     if (names[i].isEmpty() )
       names[i] = " ";
     key_index.push_back(0);
   }

   while (true) {

     QString s;
     for (int i = 0; i < (int) names.size(); i++) {
       char c = names[i].local8Bit()[key_index[i]];
// FIXME: problem with empty names and names with only invalid chars
       if (s.find(c) < 0            // x different chars from pool
           && pool.find(c) >= 0)
         s += c;
     }

     // return when first possibility found
     if (s.length() == names.size() ) {
       if ((int) names[index].length() > key_index[index]) {
         accel_index = key_index[index];
         return true;
       }
     }

   // count up
     int n = 0;
     bool over = false;
     while (n < (int) names.size() && !over) {
       if (++key_index[n] >= (int) names[n].length()) {
         key_index[n] = 0;
         if (n+1 >= (int) names.size() ) //tried all posiblities?
           return false;
       }
       else
         over = true;
       n++;
     }
   }
   return false;
}

#include "rowtable.moc"
