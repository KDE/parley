/***************************************************************************

    $Id$

                 table view to kvoctrain document

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : taken from klpq and modified as needed
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.13  2001/11/19 20:05:16  arnold
    fixed warning message

    Revision 1.12  2001/11/18 12:28:25  arnold
    provided menu entry for example files

    Revision 1.11  2001/11/17 17:58:22  arnold
    added inline editing of all columns

    Revision 1.10  2001/11/16 18:52:59  arnold
    added possibility to disable expressions

    Revision 1.9  2001/11/09 11:06:40  arnold
    removed ability to display a different font for each column

    Revision 1.8  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.7  2001/11/02 17:50:23  arnold
    fixed sorting basically

    Revision 1.6  2001/11/02 10:17:48  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.5  2001/11/01 11:26:12  arnold
    fixed some editing actions

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

#include <qkeycode.h>
#include <qscrollbar.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>

#include <kapp.h>
#include <klocale.h>
#include <kdebug.h>

#include <iostream.h>

#include "kvoctraindoc.h"
#include "rowtable.h"
#include "kv_resource.h"

// delay im ms (microsoft seconds) before delayed popup pops up
#define POPUP_DELAY 500


class KvoctrainItem : public QTableItem
{
public:
    KvoctrainItem( QTable *t, EditType et, kvoctrainDoc *doc );
    QWidget *createEditor() const;
    void setContentFromEditor( QWidget *w );
    void setPosition(int row, int col);

private:
    kvoctrainDoc       *kv_doc;
};


KvoctrainItem::KvoctrainItem( QTable *t, EditType et, kvoctrainDoc *doc)
    : QTableItem( t, et, QString::null )
{
    kv_doc = doc;
    // we do not want that this item can be replaced
    setReplaceable( FALSE );
}


void KvoctrainItem::setPosition(int curr_row, int curr_col)
{
   setRow(curr_row);
   setCol(curr_col);
}


QWidget *KvoctrainItem::createEditor() const
{
  kdDebug() << "crea     " << endl;
   if (kv_doc != 0 && kv_doc->numEntries() != 0 && row() >= 0 && col() >= 0) {
     switch (col()) {
       case KV_COL_LESS: {
         QComboBox *lessonbox = new QComboBox(table()->viewport() );
         lessonbox->insertItem (kv_doc->getLessonDescr(0));
         for (unsigned i = 1; i <= kv_doc->numLessons(); ++i)
           lessonbox->insertItem (kv_doc->getLessonDescr(i));
         lessonbox->setCurrentItem(kv_doc->getEntry(row())->getLesson());
         return lessonbox;
       }
       break;

       case KV_COL_MARK: {
         QComboBox *statebox = new QComboBox(table()->viewport() );
         statebox->insertItem (i18n("state of a row", "active, not in query"));
         statebox->insertItem (i18n("state of a row", "in query"));
         statebox->insertItem (i18n("state of a row", "inactive"));
         QSize sz = statebox->sizeHint();
         sz.setHeight(table()->rowHeight(row()));
         statebox->setMinimumSize(sz);
         if (!kv_doc->getEntry(row())->isActive() )
           statebox->setCurrentItem(2);
         else if (kv_doc->getEntry(row())->isInQuery() )
           statebox->setCurrentItem(1);
         else
           statebox->setCurrentItem(0);
         return statebox;
       }
       break;

       default: {
  kdDebug() << "edit1 " << endl;
         QLineEdit *edit = new QLineEdit(table()->viewport() );
         if (col() == KV_COL_ORG)
           edit->setText(kv_doc->getEntry(row())->getOriginal());
         else
           edit->setText(kv_doc->getEntry(row())->getTranslation(col()-KV_COL_ORG));
         return edit;
        }
     }
   }
   return 0;
}


void KvoctrainItem::setContentFromEditor( QWidget *w )
{
   if (kv_doc != 0) {
     if ( w->inherits( "QComboBox" ) ) {
       if (col() == KV_COL_MARK) {
        QComboBox *statebox = (QComboBox*) w;
        kvoctrainExpr *expr = kv_doc->getEntry(row());
        bool inq = false;
        bool act = true;
        if (statebox->currentItem() == 0) {
          inq = false;
          act = true;
        }
        else if (statebox->currentItem() == 1) {
          inq = true;
          act = true;
        }
        else if (statebox->currentItem() == 2) {
          inq = false;
          act = false;
        }
        if (inq != expr->isInQuery() ||
            act != expr->isActive() )
          kv_doc->setModified();
        expr->setInQuery(inq);
        expr->setActive(act);
       }
       else if (col() == KV_COL_LESS) {
        QComboBox *lessonbox = (QComboBox*) w;
        if (kv_doc->getEntry(row())->getLesson() != lessonbox->currentItem())
          kv_doc->setModified();
        kv_doc->getEntry(row())->setLesson(lessonbox->currentItem());
       }
     }
     else {
       QLineEdit *edit = (QLineEdit*) w;
       if (col() == KV_COL_ORG) {
         if (kv_doc->getEntry(row())->getOriginal() != edit->text())
           kv_doc->setModified();
         kv_doc->getEntry(row())->setOriginal(edit->text());
       }
       else {
         if (kv_doc->getEntry(row())->getTranslation(col()-KV_COL_ORG) != edit->text())
           kv_doc->setModified();
         kv_doc->getEntry(row())->setTranslation(col()-KV_COL_ORG, (edit->text()));
       }
     }
   }
}	


RowTable::RowTable(kvoctrainDoc *rows, Flags flags,
                   const GradeCols *gc,
                   QWidget *parent, const char *name )
	: QTable( parent, name )
{
	init(flags);
        setDoc (rows, gc);
        setFocus();
        triggerSect = -1;
        delayTimer = new QTimer (this);
        connect (delayTimer, SIGNAL(timeout ()), this, SLOT(menuTriggerTimeout()));
        QHeader *header = horizontalHeader();
        connect (header, SIGNAL(pressed(int)), this, SLOT(headerPressEvent(int)));
        connect (header, SIGNAL(released(int)), this, SLOT(headerReleaseEvent(int)));
}


RowTable::~RowTable()
{
  delete defaultItem;
  defaultItem = 0;
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


void RowTable::setDoc(kvoctrainDoc *rows,  const GradeCols *gc)
{
  if (defaultItem)
    endEdit(defaultItem->row(), defaultItem->col(), true, false);

  delete defaultItem;
  defaultItem = 0;
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

// QTableItem::Never
// QTableItem::WhenCurrent
// QTableItem::OnTyping
  defaultItem = new KvoctrainItem(this, QTableItem::OnTyping, rows);
  gradecols = gc;
}


void RowTable::init(Flags flags)
{
        defaultItem = 0;
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
        setColumnMovingEnabled(false);
        setRowMovingEnabled(false);
        setSorting(false);
//        horizontalHeader()->setClickEnabled(false);
}


void RowTable::setNumCols( int cols )
{
        cols = QMAX (1, cols );
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
       cell->paint( p, col, w, selected, m_rows,
                    numCols() == KV_EXTRA_COLS+2 ? KV_COL_TRANS : currentColumn(),
                    gradecols);
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


void RowTable::setItem ( int row, int col, QTableItem * item )
{
  // ignore item!
  updateContents(row, col);
}


QTableItem* RowTable::item ( int row, int col ) const
{
   if (defaultItem)
     defaultItem->setPosition(row, col);
   return defaultItem;
}


void RowTable::contentsMouseDoubleClickEvent( QMouseEvent *e )
{
  delayTimer->stop();
  QTable::contentsMouseDoubleClickEvent(e);
  emit edited( currentRow(), currentColumn() );
}



void RowTable::contentsMousePressEvent( QMouseEvent *e )
{
  delayTimer->stop();
  int cc = columnAt(e->x());
  int cr = rowAt(e->y());

  QTable::contentsMousePressEvent(e);

  if (cc >= KV_EXTRA_COLS) {
    // update color of original when column changes and more than 1 translation
    bool update_org = false;
    if (cc != currentColumn() && numCols() > 2)
      update_org = true;

    int topCell = rowAt(0);
    int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
    if (update_org)
      for (int i = topCell; i <= lastRowVisible; i++)
        updateCell(i, KV_COL_ORG);

    if( e->button() == LeftButton )
      emit cellMoved(cr, cc);
  }
}


void RowTable::keyPressEvent( QKeyEvent *e )
{
  delayTimer->stop();
  switch( e->key() ) {
      case Key_Right: {
        int topCell = rowAt(0);
        int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
        if (numCols() > 2)
          for (int i = topCell; i <= lastRowVisible; i++)
            updateCell(i, KV_COL_ORG);
      }  // fallthrough
      case Key_Up:
      case Key_Down:
      case Key_Next:
      case Key_Prior:
      QTable::keyPressEvent(e);
      if (currentColumn() > KV_EXTRA_COLS)
        emit cellMoved(currentRow(), currentColumn());
    break;

    case Key_Left: {
        QTable::keyPressEvent(e);
        int topCell = rowAt(0);
        int lastRowVisible = QMIN(numRows(), rowAt(contentsHeight()));
        if (numCols() > 2)
          for (int i = topCell; i <= lastRowVisible; i++)
            updateCell(i, KV_COL_ORG);
        if (currentColumn() > KV_EXTRA_COLS)
          emit cellMoved(currentRow(), currentColumn());
    }
    break;

    case Key_Return:
    case Key_Enter:
      QTable::keyPressEvent(e);
      emit edited( currentRow(), currentColumn() );
    break;

    default:
      QTable::keyPressEvent(e);
    break;
  }
}


void RowTable::headerPressEvent(int sect)
{
   triggerSect = sect;
   delayTimer->stop();
   delayTimer->start(POPUP_DELAY, true);
}


void RowTable::headerReleaseEvent(int sect)
{
   delayTimer->stop();
   if(triggerSect == -1 ) {             // long enough pressed for popup menu
     return;
   }
   int mt = triggerSect;
   triggerSect = -1;
   emit selected(mt);
}


void RowTable::menuTriggerTimeout()
{
   delayTimer->stop();
   if(triggerSect == -1 )
     return;

   int mt = triggerSect;
   triggerSect = -1;

//   emit header->released(mt);

   QHeader *header = horizontalHeader();
   int x = leftMargin();
   for (int i = 0; i < mt; ++i )
     x += header->sectionSize(i);
   QPoint mpos = mapToGlobal(QPoint(x, topMargin()));
   emit rightButtonClicked(mt, mpos.x(), mpos.y() );
}

#include "rowtable.moc"
