/***************************************************************************

    $Id$

                     table view to kvoctrain document

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : taken from klpq and modified as needed
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.6  2001/11/09 10:39:25  arnold
    removed ability to display a different font for each column

    Revision 1.5  2001/11/02 17:50:23  arnold
    fixed sorting basically

    Revision 1.4  2001/11/01 11:26:12  arnold
    fixed some editing actions

    Revision 1.3  2001/10/20 00:58:26  waba
    * Selection fixes
    * Compile fixes

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.1  2001/10/05 15:36:34  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

//
//  klpq
//
//  Copyright (C) 1997 Christoph Neerfeld
//  email:  Christoph.Neerfeld@home.ivm.de or chris@kde.org

#ifndef rowtable_included
#define rowtable_included

#include <kv_resource.h>
#include <vector.h>

#include <qglobal.h>

#include <qtable.h>

class QPainter;
class kvoctrainDoc;
class kvoctrainExpr;
class GradeCols;
class QKeyEvent;
class QTimer;

/**
  * This class provides the container for your complete
  * vocabulary
  */

class RowTable : public QTable
{
	Q_OBJECT

public:
	enum Flags {SelectRow, SelectCell, SelectFixed};

	RowTable(kvoctrainDoc *rows, Flags flags=SelectRow,
                 const GradeCols *gc = 0,
                 QWidget *parent = NULL, const char *name = NULL );
	~RowTable();

	void setNumCols( int cols );
	void setNumRows( int rows );

	void setCurrentRow( int row, int col );
	kvoctrainExpr *getRow( int row );
	
	void clear();
	void setSelectColumn( int col );

        void updateContents(int row = -1, int col = -1);
        void setDoc(kvoctrainDoc *rows,
                    const GradeCols *gc = 0);

protected:
        void repaintOriginal();
        virtual void paintCell( QPainter *p, int row, int col, const QRect &cr, bool selected);
	virtual void paletteChange( const QPalette &oldPalette );
        virtual QWidget *createEditor(int, int, bool) const;
        void setCellWidget ( int row, int col, QWidget * e );
        virtual void keyPressEvent( QKeyEvent *e );
        virtual void contentsMousePressEvent (QMouseEvent *e);
        virtual void contentsMouseDoubleClickEvent( QMouseEvent *e );

protected slots:
        void menuTriggerTimeout();
        void headerReleaseEvent(int sec);
        void headerPressEvent(int sec);

signals:
        void cellMoved(int, int);
        void edited(int, int);
	void selected(int row, int col, int key_state);
	void selected(int row);
        void rightButtonClicked(int header, int x, int y);

protected:
	kvoctrainDoc    *m_rows;

private:
	void init(Flags flags);

	int                m_flags;
	QTimer            *delayTimer;
        int                triggerSect;
	const GradeCols   *gradecols;

  friend class kvoctrainView;
};

#endif
