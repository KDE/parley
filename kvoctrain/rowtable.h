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

#ifndef rowtable_included
#define rowtable_included

#include <vector.h>

#include <qtableview.h>

#include "langset.h"

#define KV_EXTRA_COLS    1   // add col for lesson number

#define KV_COL_LESS      0   // col: lesson number
#define KV_COL_ORG       1   // col: original
#define KV_COL_TRANS     2   // col: first translation

class QPainter;
class kvoctrainDoc;
class kvoctrainExpr;
class GradeCols;

/**
  * This class provides the container for your complete
  * vocabulary
  */

class RowTable : public QTableView
{
	Q_OBJECT

public:
	enum Flags {SelectRow, SelectCell, SelectFixed};

	RowTable(kvoctrainDoc *rows, Flags flags=SelectRow,
                 const LangSet *ls=0, const GradeCols *gc = 0,
                 QWidget *parent = NULL, const char *name = NULL );
	~RowTable();

	kvoctrainExpr *selectedRow();
	void setNumCols( int cols );
	void setNumRows( int rows );

	int  numCols() { return QTableView::numCols(); }
	int  numRows() { return QTableView::numRows(); }

        void updateCell( int row, int column, bool erase=TRUE )
          { QTableView::updateCell(row, column, erase); }

	void setCurrentRow( int row, int col );
	void setHighlightColumn( int col );
	kvoctrainExpr *getRow( int row );
	void clear();
	void setCellWidth( int width );
	void setCellHeight( int height );
	void setSelectColumn( int col );
	int selectColumn();
	int currentRow() { return (numRows() == 0 ? -1 : current_row); }
	int currentCol() { return current_col; }

        int numRows() const { return QTableView::numRows(); }
        void updateViewPort();
        void repaintCells(int firstRow=-1, int lastRow=-1,
                          int firstCol=-1, int lastCol=-1);
        void setDoc(kvoctrainDoc *rows, const LangSet  *langset,
                    const GradeCols *gc = 0);

        void setLangSet (const LangSet *ls);
        SpecFont_t getColFont(int index);

        static bool createMenuNames (QString forbidden_chars,
                                     vector<QString> names,
                                     int index,
                                     int &accel_index);

        static bool createMenuNames (vector<QString> forbidden_labels,
                                     vector<QString> names,
                                     int index,
                                     int &accel_index);

protected:
	int cellWidth( int col );
	int cellHeight( int row );

        void repaintOriginal();
	virtual void paintCell( QPainter *p, int row, int col );
	virtual void mousePressEvent( QMouseEvent *e );
	virtual void mouseDoubleClickEvent( QMouseEvent *e );
	virtual void keyPressEvent( QKeyEvent *e );
	virtual void focusInEvent( QFocusEvent *e );
	virtual void focusOutEvent( QFocusEvent *e );
	virtual void paletteChange( const QPalette &oldPalette );

signals:
	void selected(int row, int col, int key_state);
	void edited(int row, int col);
	void highlighted(int row, int col);
	void hSliderMoved(int value);
	void vSliderMoved(int value);
	void rightButtonClicked();
	void leftButtonClicked(int row, int col, int key);
        void vSliderPressed(bool state, int val);
        void cellMoved(int, int, int);

public slots:
	void setColumnWidth(int col, int width);
	void setRowHeight(int row, int height);

private slots:
	void hSliderMovedSlot(int value);
	void vSliderMovedSlot(int value);
        void vSliderPressedSlot();
        void vSliderReleasedSlot();

protected:
	kvoctrainDoc    *m_rows;

private:
	void init(Flags flags);

	vector<SpecFont_t> m_colFonts;
	QArray<int>        m_colWidths;
	QArray<int>        m_rowHeights;
	int                current_col;
	int	           current_row;
	int                m_flags;

	const GradeCols   *gradecols;

  friend class kvoctrainView;
};

#endif
