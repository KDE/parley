/***************************************************************************

    $Id$

                       a table header class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : taken from klpq and modified as needed
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:36:34  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

//
//  klpq
//
//  Copyright (C) 1997 Christoph Neerfeld
//  email:  Christoph.Neerfeld@home.ivm.de or chris@kde.org

#include "header.h"
#include "qpainter.h"
#include "qdrawutil.h"
#include "qpixmap.h"
#include "qbitmap.h"

#include <iostream.h>
#include <ctype.h>

#include <compat_2x.h>
#include <qstyle.h>

// delay im ms (microsoft seconds) before delayed popup pops up
#define POPUP_DELAY 500

//=======================================================================
// KHeaderItem internal class

class KHeaderItem {
public:
  KHeaderItem( QWidget *parent, int flags, int size = 0, QString *label = 0,
	       int alignment = EA_QtNS(AlignCenter), QPixmap *pix = 0 );
  ~KHeaderItem();
  void    setSize( int size );
  int     size();
  void    setText( QString label, int alignment = EA_QtNS(AlignCenter) );
  QString getText();
  void    setAccel(char accel) { accelchar = accel; }
  char    getAccel() const { return accelchar; }
  QString text();
  void    setFlags( int flags );
  int     flags();
  void    setPixmap (QPixmap *pixmap);
  void    paint(QPainter *paint, QColorGroup *g, int style, int pos, int width,
		int height, int sunken);
private:
  int      m_size;
  int      m_flags;
  QString  m_label;
  int      m_labelAlignment;
  QWidget *m_parent;
  QPixmap  m_pix;
  QPixmap  m_arrow;
  QBitmap  m_mask;
  int      accelchar;
};


KHeaderItem::KHeaderItem( QWidget *parent, int flags,
                          int size, QString *label, int alignment, QPixmap *pixmap )
{
	m_size = size;
	if( label != 0 ) m_label = *label;
	if( pixmap != 0 ) m_pix = *pixmap;
	m_labelAlignment = alignment;
	m_flags = flags;
	m_parent = parent;
        accelchar = 0;

        m_mask.resize(12, 10);
        m_arrow.resize(12, 10);

        QPainter p;
        p.begin( &m_arrow);
        p.fillRect(0, 0, m_arrow.width(), m_arrow.height(), m_parent->backgroundColor());

        QPainter pm;
        pm.begin( &m_mask);
        pm.fillRect (0, 0, m_mask.width(), m_mask.height(), EA_QtNS(color0));
//        pm.fillRect (0, 0, m_mask.width()/2, m_mask.height(), EA_QtNS(color1));


        m_parent->style().drawArrow( &p, Qt::DownArrow, false,
           2, 0, m_arrow.width(), m_arrow.height(), m_parent->colorGroup(), true);
//        m_parent->style().drawArrow( &pm, Qt::DownArrow, false,
  //         2, 0, m_mask.width(), m_mask.height(), maskgroup, true);

        p.end();
        pm.end();
//        m_arrow.setMask(m_mask);
}


KHeaderItem::~KHeaderItem()
{
}


void KHeaderItem::setPixmap (QPixmap *pixmap)
{
	if( pixmap != 0 && !pixmap->isNull() ) {
          m_pix = *pixmap;
        }
        else {
          m_pix.resize(0, 0);
        }
}


void KHeaderItem::setSize( int size )
{
	m_size = size;
}


int KHeaderItem::size()
{
	return m_size;
}


void KHeaderItem::setText( QString label, int alignment )
{
	m_label = label;
	m_labelAlignment = alignment;
}


QString KHeaderItem::getText()
{
	return m_label;
}


QString KHeaderItem::text()
{
	return m_label;
}


void KHeaderItem::setFlags( int flags )
{
	m_flags = ( m_flags & KHeader::Vertical ) | ( flags & ~KHeader::Vertical );
}


int KHeaderItem::flags()
{
	return m_flags;
}


void KHeaderItem::paint(QPainter *paint, QColorGroup *g,
        int style, int pos, int width, int height, int state)
{
	if( m_flags & KHeader::Vertical )
	{
		if( pos>height || pos+m_size<0 )
			return;
			qDrawWinPanel(paint,0,pos,width,m_size,*g,state,NULL);
// FIXME: correct painting when pressed (and button too small)
                if (state) {
                    pos+=2;
                    height+=2;
                }
		if( !m_label.isEmpty() )
			paint->drawText(2,pos+2,width-4,m_size-4,
                                        m_labelAlignment, m_label);
	}
	else
	{
		if( pos>width || pos+m_size<0 )
			return;
			qDrawWinPanel(paint,pos,0,m_size,height,*g,state,NULL);
                if (state) {
                    pos+=2;
                    height+=2;
                }

                QString label = m_label;
		if( !m_label.isEmpty() && getAccel() != 0) {
                  int offset;
                  if ((offset = m_label.find(getAccel(), 0, false)) >= 0)
                    label.insert(offset, "&");
                }


		if( !m_pix.isNull() ) {
                  QRect tbox;
		  if( !m_label.isEmpty() ) {
                    tbox = paint->boundingRect(pos+4+m_pix.width()+m_arrow.width(),2,
                                               m_size-4-m_pix.width()-m_arrow.width(),
                                               height-4,m_labelAlignment,m_label);
                    tbox.setRight(tbox.right()+10);
                    int x = QMAX (pos+4, pos+4+((m_size-m_pix.width()-m_arrow.width()-tbox.width()-8)/2));
                    paint->drawPixmap(x,
                                      (height-m_pix.height())/2,
                                      m_pix,
                                      0, 0,
                                      QMIN(QMAX(0, m_size-8), m_pix.width()), -1
                                     );
                    paint->drawPixmap(x+m_pix.width(),
                                      (height-m_arrow.height())-3,
                                      m_arrow,
                                      0, 0,
                                      QMIN(QMAX(0, m_size-8), m_arrow.width()), -1
                                     );
                    if (m_size - m_pix.width()-m_arrow.width() -8-10 > 0) {
         	      paint->drawText(x+m_pix.width()+m_arrow.width()+10,
                                      (height-tbox.height())/2,
                                      QMAX(0, m_size-4-m_arrow.width()-m_pix.width()-10),
                                      height-4,
                                      EA_QtNS(AlignLeft) | EA_QtNS(ShowPrefix),
                                      label
                                     );
                    }
                  }
                  else {
                    int x = QMAX (pos+4, pos+4+((m_size-m_pix.width()-m_arrow.width()-8)/2));
                    paint->drawPixmap(x,
                                      (height-m_pix.height())/2,
                                      m_pix,
                                      0, 0,
                                      QMIN(QMAX(0, m_size-8), m_pix.width()), -1
                                     );
                    paint->drawPixmap(x+m_pix.width(),
                                      (height-m_arrow.height())-3,
                                      m_arrow,
                                      0, 0,
                                      QMIN(QMAX(0, m_size-8), m_arrow.width()), -1
                                     );
                  }
                }
                else {
                  QRect tbox;
                  tbox = paint->boundingRect(pos+4+m_arrow.width(),2,
                                             m_size-4-m_arrow.width(),height-4,
                                             m_labelAlignment,m_label);
                  tbox.setRight(tbox.right()+10);
                  int x = QMAX (pos+4, pos+4+((m_size-m_arrow.width()-tbox.width()-8)/2));
                  paint->drawPixmap(x,
                                    (height-m_arrow.height())-3,
                                    m_arrow,
                                    0, 0,
                                    QMIN(QMAX(0, m_size-8), m_arrow.width()), -1
                                   );
  		  if( !m_label.isEmpty() )
                    paint->drawText(x+m_arrow.width()+10, (height-tbox.height())/2,
                                    QMAX(0, m_size-4-m_arrow.width()), height-4,
                                    EA_QtNS(AlignLeft)  | EA_QtNS(ShowPrefix),label);
               }
	}
}

//=======================================================================

KHeader::KHeader( QWidget *parent, const char *name, int numHeaders, int flags )
	: QFrame( parent, name )
{
	labels.resize( 0 );
	m_offset = 0;
	m_selected = -1;
	m_temp_sel = -1;
	m_flags = flags;
	divider = -1;
	m_resizing = FALSE;
	if( numHeaders != 0 )
        setNumHeaders( numHeaders );
	if( flags & Resizable )
	{
		installEventFilter(this);
		setMouseTracking(TRUE);
		m_defCursor=cursor();
	}
        delayTimer = new QTimer (this);
        connect (delayTimer, SIGNAL(timeout ()), this, SLOT(menuTriggerTimeout()));
}


KHeader::~KHeader()
{
	int i;
	for( i=0 ; i < (int) labels.size() ; i++ )
		delete labels[ i ];
}


void KHeader::setGeometry( int x, int y, int w, int h )
{
	if( m_flags & Vertical )
		QFrame::setGeometry( x, y, w, h );
	else
		QFrame::setGeometry( x, y, w, fontMetrics().height() + 4 );
}


void KHeader::resize( int w, int h )
{
	if( m_flags & Vertical )
		QFrame::resize( w, h );
	else
		QFrame::resize( w, fontMetrics().height() + 4 );
}


void KHeader::fontChange( const QFont & ) // oldfont )
{
	if( !(m_flags & Vertical) )
		resize( width(), fontMetrics().height() + 4 );
}


bool KHeader::eventFilter( QObject *, QEvent *ev )
{
	if( ev->type()==QEvent::MouseButtonPress && divider!=-1 ) {
		m_resizing=TRUE;
		return TRUE;
	}

	if( ev->type()==QEvent::MouseMove ) {
		QMouseEvent *mev = (QMouseEvent*)ev;
		if(m_resizing) {
			if( m_flags & Vertical ) {
				adjustHeaderSize( divider, mev->pos().y()-divstart );
				divstart = mev->pos().y();
			}
			else {
				adjustHeaderSize( divider, mev->pos().x()-divstart );
				divstart = mev->pos().x();
			}
			return TRUE;
		}
		else if( mev->button()==NoButton ) {
			// search labels to see if we are in range of a divider
			int pos = m_offset;
			int curs = m_flags&Vertical ? mev->y()+3 : mev->x()+3;
			divider = -1;
			for( int i=0 ; i < (int) labels.size() ; i++ ) {
				pos += labels[i]->size();
				if( curs>=pos && curs<pos+6 ) {
					setCursor(m_flags&Vertical ? sizeVerCursor : sizeHorCursor);
					divider = i;
					divstart = pos;
				}
			}
			if(divider==-1) setCursor(m_defCursor);
			return TRUE;
		}
	}

	if( ev->type()==QEvent::MouseButtonRelease && m_resizing ) {
		emit sizeChanged( divider, labels[divider]->size() );
		m_resizing = FALSE;
		return TRUE;
	}
	return FALSE;
}


int KHeader::numHeaders()
{
	return labels.size();
}


void KHeader::setNumHeaders( int numHeaders )
{
	ASSERT( numHeaders >= 0 );
	if( numHeaders < (int) labels.size() )
	{
		for( int i=numHeaders ; i < (int) labels.size() ; i++ )
		{
			delete labels[i];
		}
		labels.resize( numHeaders );
	}
	else if( numHeaders > (int) labels.size() )
	{
		int oldCount = labels.size();
		labels.resize( numHeaders );
		
		for( int i=oldCount ; i < numHeaders ; i++ )
		{
			labels[i] = new KHeaderItem( this, m_flags );
		}
	}
}


void KHeader::setHeaderFlags( int header, int flags )
{
	ASSERT( header >= 0 );
	ASSERT( header < (int) labels.size() );

	labels[header]->setFlags( flags );
}


void KHeader::setHeaderSize( int header, int size )
{
	ASSERT( header >= 0 );
	ASSERT( header < (int) labels.size() );

	labels[header]->setSize( size );
	repaint();
	emit sizeChanged( header, size );
}


int KHeader::getHeaderSize( int header )
{
  ASSERT( header >= 0 );
  ASSERT( header < (int) labels.size() );
  return labels[header]->size();
}


void KHeader::adjustHeaderSize( int start, int delta )
{
	int pos = labels[start]->size()+delta;
	if( pos < 0 ) pos = 0;
	labels[start]->setSize( pos );
// There must be a better way!
// calculate the rectangle?
	repaint();
	emit sizeChanging( start, pos );
}


void KHeader::setOrigin( int pos )
{
	m_offset = -pos;
	repaint();
}


void KHeader::setHeaderLabel( int header, const QString &text,
                               int align )
{
	labels[header]->setText( text, align );
}


void KHeader::getHeaderLabel( int header, QString &name )
{
	name = labels[header]->getText();
}


void KHeader::setHeaderPixmap(int header, QPixmap *pix)
{
	labels[header]->setPixmap( pix);
}


void KHeader::paintEvent( QPaintEvent * ) // pev )
{
	QColorGroup g = colorGroup();

	QPainter paint;
	paint.begin( this );

	int pos = m_offset;
	for( int i=0 ; i < (int) labels.size(); i++ )
	{
		labels[i]->paint( &paint, &g, style(), pos, width(), height(), i==m_selected);
		pos += labels[i]->size();
	}

	paint.end();
}


void KHeader::mouseDoubleClickEvent( QMouseEvent *mev )
{
	int curs;
	if( m_flags & Vertical )
		curs = mev->y();
	else
		curs = mev->x();
	int pos = m_offset;
	for( int i=0 ; i < (int) labels.size() ; i++ )
	{
		pos += labels[i]->size();
		if( pos >= curs )
		{
			if( !(labels[i]->flags() & Buttons) )
				return;
			break;
		}
	}
}


void KHeader::mousePressEvent( QMouseEvent *mev )
{
	int curs;
	if( m_flags & Vertical )
		curs = mev->y();
	else
		curs = mev->x();
	int pos = m_offset;
	for( int i=0 ; i < (int) labels.size() ; i++ )
	{
		pos += labels[i]->size();
		if( pos >= curs )
		{
			if( !(labels[i]->flags() & Buttons) )
				return;
			m_selected = i;

                        if (mev->button() == LeftButton ) {
                          delayTimer->stop(); // just in case?
                          delayTimer->start(POPUP_DELAY, true);
                        }

			break;
		}
	}
	repaint();
}


void KHeader::mouseReleaseEvent( QMouseEvent *mev )
{
        delayTimer->stop();
	if( m_selected == -1 )
	{
		m_temp_sel = -1;
		return;
	}
        if (mev->button() == LeftButton )
  	  emit selected(m_selected);

	m_selected = -1;
	m_temp_sel = -1;
	repaint();
}


QPoint KHeader::getAbsHeaderPos(int index)
{
      int x = 0;
      for( int i=0 ; i < index && i < (int) labels.size(); i++ )
        x += labels[i]->size();
      return mapToGlobal (QPoint (x, height()));
}


void KHeader::menuTriggerTimeout()
{
      delayTimer->stop();
      QPoint pt = getAbsHeaderPos(m_selected);
      emit rightButtonClicked(m_selected, pt.x(), pt.y());
      m_selected = -1;
      m_temp_sel = -1;
      repaint();
}


void KHeader::enterEvent( QEvent * ) // ev )
{
	m_selected = m_temp_sel;
	if( m_selected != -1 )
		repaint();
}


void KHeader::leaveEvent( QEvent * ) // ev )
{
	m_temp_sel = m_selected;
	m_selected = -1;
	if( m_temp_sel != -1 )
		repaint();
}


void KHeader::setHeaderAccel( int header, const QString &accel )
{
  if (accel.length() > 0)
    labels[header]->setAccel( accel.lower().local8Bit()[0] );
  else
    labels[header]->setAccel( 0 );
}


void KHeader::setHeaderAccel( int header, char accel )
{
    labels[header]->setAccel( tolower(accel));

}


char KHeader::getHeaderAccel( int header )
{
    return labels[header]->getAccel();
}
#include "header.moc"
