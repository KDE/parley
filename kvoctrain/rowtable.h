/***************************************************************************

    $Id$

                     table view to kvoctrain document

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : taken from klpq and modified as needed
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************/

//
//  klpq
//
//  Copyright (C) 1997 Christoph Neerfeld
//  email:  Christoph.Neerfeld@home.ivm.de or chris@kde.org

#ifndef rowtable_included
#define rowtable_included

#include "kv_resource.h"

#include <qtable.h>

#include <vector>
using namespace std;

class QPainter;
class kvoctrainDoc;
class kvoctrainExpr;
class GradeCols;
class QKeyEvent;
class QTimer;
class KvoctrainItem;

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

	void setFont( const QFont &);
	void setNumCols( int cols );
	void setNumRows( int rows );

	void setCurrentRow( int row, int col );
	kvoctrainExpr *getRow( int row );
	
	void clear();
	void setSelectColumn( int col );

        void updateContents(int row = -1, int col = -1);
        void setDoc(kvoctrainDoc *rows,
                    const GradeCols *gc = 0);
        void setEditorBlocked(bool block);

protected:
        void repaintOriginal();
        virtual void paintCell( QPainter *p, int row, int col, const QRect &cr, bool selected);
	virtual void paletteChange( const QPalette &oldPalette );
        virtual void keyPressEvent( QKeyEvent *e );
        virtual void keyReleaseEvent( QKeyEvent *e );
        virtual void contentsMousePressEvent (QMouseEvent *e);
        virtual void contentsMouseDoubleClickEvent( QMouseEvent *e );
        virtual void setItem ( int row, int col, QTableItem * item );
        void setInlineEnabled(bool state);
        bool getInlineEnabled();

protected slots:
        void menuTriggerTimeout();
        void headerReleaseEvent(int sec);
        void headerPressEvent(int sec);

signals:
        void edited(int, int);
	void selected(int row, int col, int key_state);
	void selected(int row);
        void rightButtonClicked(int header, int x, int y);
        void forwardKeyPressEvent (QKeyEvent *);
        void forwardKeyReleaseEvent (QKeyEvent *);

protected:
        virtual QTableItem* item ( int row, int col ) const;
	kvoctrainDoc    *m_rows;

private:
	void init(Flags flags);

	int                m_flags;
	QTimer            *delayTimer;
        int                triggerSect;
	const GradeCols   *gradecols;
        KvoctrainItem     *defaultItem;

  friend class kvoctrainView;
};

#endif
