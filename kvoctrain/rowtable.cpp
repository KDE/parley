/***************************************************************************

    $Id$

                 table view to kvoctrain document

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : taken from klpq and modified as needed
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$

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
	: QTableView( parent, name )
{
	init(flags);
        setDoc (rows, ls, gc);
        setFocus();
}


RowTable::~RowTable()
{
}


void RowTable::repaintCells(int firstRow, int lastRow, int firstCol, int lastCol)
{
   int frow,
       lrow,
       fcol,
       lcol;

   int tc = topCell();
   if (   firstRow == -1
       || firstRow < tc)
     frow = tc;
   else
     frow = firstRow;

   int lrv = lastRowVisible();
   if (   lastRow == -1
       || lastRow > lrv)
     lrow = lrv;
   else
     lrow = lastRow;

   int lc = leftCell();
   if (   firstCol == -1
       || firstCol < lc)
     fcol = lc;
   else
     fcol = firstRow;

   int lcv = lastColVisible();
   if (   lastCol == -1
       || lastCol > lcv)
     lcol = lcv;
   else
     lcol = lastCol;

   if (   frow > lrow
       || fcol > lcol
      )
     return;

   for (int r = frow; r <= lrow; r++)
     for (int c = fcol; c <= lcol; c++)
        updateCell (r, c);
}


void RowTable::updateViewPort()
{
      int height = fontMetrics().lineSpacing();
      for (int i = 0; i < (int) m_colFonts.size(); i++) {
         if (m_colFonts[i].specfont) {
           QFontMetrics fm (m_colFonts[i].font);
           height= QMAX (height, fm.lineSpacing() );
         }
      }
      setCellHeight( height );

      if (m_rows) {
        setNumRows( m_rows->numEntries());
        setNumCols( QMAX (1, m_rows->numLangs()+KV_EXTRA_COLS) );
      }
      else {
        setNumRows( 0 );
        setNumCols( 1+KV_EXTRA_COLS );
      }
      current_row = QMIN (current_row, numRows()-1);
      cout << "stc: " << current_row << " " << topCell() << " " << lastRowVisible() << endl;
      int pageSize = lastRowVisible() - topCell();
      if( rowIsVisible( current_row ) ) {
        setCurrentRow (current_row, current_col);;
	setTopCell( QMIN( topCell(), (int) m_rowHeights.size()-pageSize ) );
        cout << "stc 1\n";
      }
      else {
        setCurrentRow (current_row, current_col);;
	setTopCell( QMIN( current_row, (int) m_rowHeights.size()-pageSize ) );
        cout << "stc 2\n";
      }
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
     // override charset
    if (m_rows->getCharSet(KV_COL_ORG-KV_EXTRA_COLS) != QFont::AnyCharSet)
      m_colFonts[KV_COL_ORG].font.setCharSet (m_rows->getCharSet(KV_COL_ORG-KV_EXTRA_COLS)); 
  }

  for (int i = KV_COL_TRANS; i < numCols(); i++) {
    id = ls->indexShortId(m_rows->getIdent(i-KV_EXTRA_COLS));
    if (id >= 0)
      ls->Font(id, m_colFonts[i].font, m_colFonts[i].specfont);
      // override charset
      if (m_rows->getCharSet(i-KV_EXTRA_COLS) != QFont::AnyCharSet)
        m_colFonts[i].font.setCharSet (m_rows->getCharSet(i-KV_EXTRA_COLS));
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
	current_row = -1;
	if( flags == SelectRow )
		current_col = -1;
	else
		current_col = 0;

	setFrameStyle( WinPanel | Sunken );
	setBackgroundColor( colorGroup().base() );

	setCellWidth( 0 );
	setCellHeight( 0 );

	setAutoUpdate( TRUE );
        setFocusPolicy( StrongFocus );
        setTableFlags( Tbl_autoScrollBars |
                       Tbl_clipCellPainting |
                       Tbl_snapToVGrid
                     );

	connect( (QObject*)horizontalScrollBar(), SIGNAL(sliderMoved(int)),
				SLOT(hSliderMovedSlot(int)) );
	connect( (QObject*)horizontalScrollBar(), SIGNAL(valueChanged(int)),
				SLOT(hSliderMovedSlot(int)) );

	connect( (QObject*)verticalScrollBar(), SIGNAL(sliderMoved(int)),
				SLOT(vSliderMovedSlot(int)) );
	connect( (QObject*)verticalScrollBar(), SIGNAL(sliderReleased()),
				SLOT(vSliderReleasedSlot()) );
	connect( (QObject*)verticalScrollBar(), SIGNAL(sliderPressed()),
				SLOT(vSliderPressedSlot()) );
	connect( (QObject*)verticalScrollBar(), SIGNAL(valueChanged(int)),
				SLOT(vSliderMovedSlot(int)) );

}


void RowTable::setCellWidth( int width )
{
	QTableView::setCellWidth( width );
}


void RowTable::setCellHeight( int height )
{
	QTableView::setCellHeight( height );
}


void RowTable::setNumCols( int cols )
{
        cols = QMAX (1, cols );
	int oldsize = m_colWidths.size();

	m_colWidths.resize( cols );
	for( int i=oldsize ; i<cols ; i++)
	  m_colWidths[i] = 0;

	oldsize = m_colFonts.size();
        m_colFonts.resize( cols );
	for( int i=oldsize ; i<cols ; i++) {
       	  m_colFonts[i].font = QFont();
       	  m_colFonts[i].specfont = false;
        }

	QTableView::setNumCols( cols );
}


void RowTable::setNumRows( int rows )
{
	int oldsize = m_rowHeights.size();
	m_rowHeights.resize( rows );
	for( int i=oldsize ; i<rows ; i++)
          m_rowHeights[i] = 0;

	QTableView::setNumRows( rows );
}


int RowTable::cellWidth( int col )
{
	int width = QTableView::cellWidth();
	if( width != 0 )
		return width;
        if (col < (int)m_colWidths.size() )
  	  return m_colWidths[col];
         else
           return 0;
}


void RowTable::setHighlightColumn( int col )
{
	current_col = col;
        setCurrentRow (current_row, current_col);;
}


void RowTable::setColumnWidth( int col, int width )
{
        if (col < (int)m_colWidths.size() )
   	  m_colWidths[col] = width;
	resizeEvent(0);
	repaint();
        if (m_rows) {
          if (col >= KV_COL_ORG)
            m_rows->setSizeHint(col-KV_COL_ORG, width);
          else
            m_rows->setSizeHint(-1, width); // lesson
        }
}


int RowTable::cellHeight( int row )
{
	int height = QTableView::cellHeight();
	if( height != 0 )
		return height;
        if (row < (int)m_rowHeights.size() )
  	  return m_rowHeights[row];
        else
          return 0;
}


void RowTable::setRowHeight( int row, int height )
{
        if (row < (int)m_rowHeights.size() )
  	  m_rowHeights[row] = height;
	resizeEvent(0);
	repaint();
}


void RowTable::vSliderMovedSlot(int value)
{
	emit(vSliderMoved(value / cellHeight(0) ));
}


void RowTable::vSliderPressedSlot()
{
  int val = verticalScrollBar()->value() / cellHeight(0);
  emit(vSliderPressed(true, val));
}


void RowTable::vSliderReleasedSlot()
{
  int val = verticalScrollBar()->value() / cellHeight(0);
  emit(vSliderPressed(false, val));
}


void RowTable::hSliderMovedSlot(int value)
{
	emit(hSliderMoved(value));
}


void RowTable::focusInEvent( QFocusEvent * )
{
  if( current_row != -1 && current_col != -1 )
    updateCell( current_row, current_col, FALSE );
    setCurrentRow (current_row, current_col);;
}


void RowTable::focusOutEvent( QFocusEvent * )
{
  if( current_row != -1 && current_col != -1 )
    updateCell( current_row, current_col, FALSE );
    setCurrentRow (current_row, current_col);;
}


void RowTable::paletteChange( const QPalette &) //oldPalette )
{
	setBackgroundColor( colorGroup().base() );
}


void RowTable::paintCell( QPainter *p, int row, int col )
{

  QPen oldPen = p->pen();
  QColor oldBackground = p->backgroundColor();
  
  QColorGroup g = colorGroup();
  p->setBackgroundColor( g.base() );
  p->setPen( g.text() );
  
  kvoctrainExpr *cell = getRow( row );
// FIXME: tagged in an other color ?
  bool sel = cell->isTagged();
  if( sel || current_row == row && ( current_col == col || current_col == -1 ) )
    {
      sel = true;
      QColor fc;
      if( style() == WindowsStyle )
	fc = darkBlue;
      else
	{
	  if( !hasFocus() )
	    fc = g.dark();
	  else
	    fc = g.text();
	}
      //if( hasFocus() )
      p->fillRect( 0, 0, cellWidth( col ), cellHeight( row ), fc );
      //else
      //p->fillRect( 1, 1, cellWidth( col )-2, cellHeight( row )-2, fc );
      p->setPen( g.base() );
      p->setBackgroundColor( g.text() );
    }
  
  if( cell != NULL ) {
                // with exactly 2 cols always keep org-col from translation
    cell->paint( p, col, cellWidth(col), sel, m_rows,
                 numCols() == KV_EXTRA_COLS+2 ? KV_COL_TRANS : current_col,
                 gradecols, &m_colFonts[col]);
  }
  if( current_row == row && ( current_col == col || current_col == -1 ) && hasFocus() )
    {
      if( style() == WindowsStyle )
	p->drawWinFocusRect( 1, 1, cellWidth( col )-2, cellHeight( row )-2 );
      else
	{
	  /*
	  p->setPen( g.base() );
	  p->setBrush( NoBrush );
	  p->drawRect( 1, 1, cellWidth( col )-2, cellHeight( row )-2 );
	  */
	}
    }
  
  p->setPen( oldPen );
  p->setBackgroundColor( oldBackground );
}


kvoctrainExpr *RowTable::selectedRow()
{
	if( current_row == -1 )
		return NULL;
	return getRow( current_row );
}


kvoctrainExpr *RowTable::getRow( int row )
{
  if (m_rows)
    return m_rows->getEntry(row);
  else
    return 0;
}


void RowTable::mousePressEvent( QMouseEvent *e )
{
  int cr = findRow( e->pos().y() );
  int cc = findCol( e->pos().x() );

  // update color of original when column changes
  bool update_org = false;
  if (cc != current_col && numCols() > 2)
    update_org = true;

  if( m_flags & SelectCell ) {
    if (cc != KV_COL_LESS)
      setCurrentRow( cr, cc);
  }
  else if( m_flags & SelectFixed )
    setCurrentRow( cr, current_col );
  else
    setCurrentRow( cr, -1 );

  if (update_org)
    for (int i = topCell(); i <= lastRowVisible(); i++)
      updateCell(i, KV_COL_ORG);

  if( e->button() == RightButton )
    emit rightButtonClicked();

  if( e->button() == LeftButton ) {
    emit leftButtonClicked(cr, cc, e->state());
//    emit cellMoved(cr, cc, e->state());
  }
}


void RowTable::mouseDoubleClickEvent( QMouseEvent *e )
{
        int cc = findCol( e->pos().x() );
	if( current_col != -1 && current_row != -1 ) {
           if (cc != KV_COL_LESS)
		emit edited( current_row, cc );
	}	
        setCurrentRow (current_row, current_col);;
}


void RowTable::setCurrentRow( int row, int col )
{
	int old_row = current_row;
	current_row = row;
	int old_col = current_col;
	current_col = col;
	updateCell( old_row, old_col );
	updateCell( current_row, current_col, FALSE );
	emit highlighted( current_row, current_col );
}


void RowTable::keyPressEvent( QKeyEvent *e )
{
  if( m_colWidths.size() == 0 )
    return;
  if( current_row == -1 )
    setCurrentRow( topCell(), current_col );
  
  int pageSize;
  
  switch( e->key() )
    {
      
    case Key_Up:
      if( current_row > 0 )
	{
	  setCurrentRow( current_row-1, current_col );
	  if(    current_row < topCell()
	      || current_row > lastRowVisible() )
	    setTopCell( current_row );
          emit cellMoved(current_row, current_col, e->state());
	}
      break;

    case Key_Space:
      if( current_row != -1 && current_col != -1 ) {
        emit selected ( current_row, current_col, e->state() );
      }
      break;

    case Key_Left:
      if( current_col > KV_COL_ORG )
	{
	  setCurrentRow( current_row, current_col-1 );
	  if( current_col < leftCell() )
	    setLeftCell( current_col );
          if (numCols() > 2)
            for (int i = topCell(); i <= lastRowVisible(); i++)
              updateCell(i, KV_COL_ORG);
          emit cellMoved(current_row, current_col, e->state());
	}
      break;

    case Key_Down:
      if( current_row < (int) m_rowHeights.size()-1 )
	{
	  setCurrentRow( current_row + 1, current_col );
	  if(current_row >= lastRowVisible() )
	    setTopCell( topCell() + current_row - lastRowVisible() );
          else if (current_row < topCell() )
	    setTopCell( current_row);
          emit cellMoved(current_row, current_col, e->state());
	}
      else {
        if ((lastRowVisible() - topCell()+1 ) * cellHeight(0) > viewHeight() )
          setTopCell(topCell()+1); // was only partially visible
      }
      break;
      
    case Key_Right:
      if( current_col < (int) m_colWidths.size()-1 )
	{
	  setCurrentRow( current_row, current_col + 1 );
	  if( current_col > lastColVisible() )
	    setLeftCell( leftCell() + current_col - lastColVisible() );
          if (numCols() > 2)
            for (int i = topCell(); i <= lastRowVisible(); i++)
              updateCell(i, KV_COL_ORG);
          emit cellMoved(current_row, current_col, e->state());
	}
      break;

    case Key_Next: {
      int cl = current_col;
      int cr = current_row;
      setCurrentRow(-1, -1);
      pageSize = lastRowVisible() - topCell();
      if( rowIsVisible( cr ) )
	setTopCell( QMIN( topCell() + pageSize, (int) m_rowHeights.size()-pageSize ) );
      else
	setTopCell( QMIN( cr + pageSize, (int) m_rowHeights.size()-pageSize ) );
      setCurrentRow( QMIN( cr + pageSize, (int) m_rowHeights.size()-1 ), cl );
      }
      emit cellMoved(current_row, current_col, e->state());
      break;
      
    case Key_Prior: {
      int cl = current_col;
      int cr = current_row;
      setCurrentRow(-1, -1);
      pageSize = lastRowVisible() - topCell();
      if( rowIsVisible( cr ) )
	setTopCell( QMAX( topCell() - pageSize, 0 ) );
      else
	setTopCell( QMAX( cr - pageSize, 0 ) );
      setCurrentRow( QMAX( cr - pageSize, 0 ), cl );
      emit cellMoved(current_row, current_col, e->state());
      }
      break;
      
    case Key_Return:
    case Key_Enter:
      if( current_row != -1 && current_col != -1 ) {
	emit edited( current_row, current_col );
      }
      break;
      
	default:
	  e->ignore();
	  break;
    }
}


void RowTable::setSelectColumn( int col )
{
	if( m_flags == SelectRow )
		col = -1;
	current_col = col;
        setCurrentRow (current_row, current_col);;
}


int RowTable::selectColumn()
{
	return current_col;
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

