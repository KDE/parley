/***************************************************************************

    $Id$

                         table header class

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


#ifndef kheader_included
#define kheader_included

#include <qframe.h>
#include <qtimer.h>

class KHeaderItem;

/**
  * This Class provides the buttons on the top to sort
  * your vocabulary rows
  */

class KHeader : public QFrame {
  Q_OBJECT
public:
  enum { Buttons = 1,
	 Vertical = 2,
	 Resizable = 4
  };

  KHeader( QWidget *parent = 0, const char *name = 0,
	   int numHeaders = 0, int flags = 0 );
  ~KHeader();
  
  void setNumHeaders( int numHeaders );
  int  numHeaders();
  void setHeaderSize( int header, int width );
  int  getHeaderSize( int header );
  void setHeaderAccel( int header, const QString &accel );
  void setHeaderAccel( int header, char accel );
  char getHeaderAccel( int header );
  void setHeaderLabel( int header, const QString &text, int align = AlignCenter );
  void getHeaderLabel( int header, QString &name);
  void setHeaderPixmap( int header, QPixmap *pix);
  void setHeaderFlags( int header, int flags );
  void setGeometry( int x, int y, int w, int h );
  void resize( int w, int h );
  QPoint getAbsHeaderPos(int index);

protected:
  virtual void mouseDoubleClickEvent( QMouseEvent *e );
  virtual void paintEvent( QPaintEvent *pev );
  virtual void mousePressEvent( QMouseEvent *mev );
  virtual void mouseReleaseEvent( QMouseEvent *mev );
  virtual void enterEvent( QEvent *ev );
  virtual void leaveEvent( QEvent *ev );
  virtual void fontChange( const QFont &oldFont );
  
signals:
  void sizeChanged( int header, int size );
  void sizeChanging( int header, int size );
  void selected( int header );
  void rightButtonClicked(int header, int x, int y);

public slots:
  void setOrigin( int pos );
  void menuTriggerTimeout();

private:
  bool eventFilter( QObject *, QEvent * );
  void adjustHeaderSize( int start, int delta );

  QArray<KHeaderItem*> labels;
  int divider;
  int divstart;
  int m_offset;
  int m_selected;
  int m_temp_sel;
  int m_flags;
  int m_resizing;
  QCursor m_defCursor;
  QTimer  *delayTimer;
};


#endif /* kheader_included */
