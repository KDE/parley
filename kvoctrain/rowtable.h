/***************************************************************************

    $Id$

                     table view to kvoctrain document

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

#ifndef rowtable_included
#define rowtable_included

#include <kv_resource.h>
#include <vector.h>

#include <qglobal.h>

#include <qtable.h>

#include "langset.h"

class QPainter;
class kvoctrainDoc;
class kvoctrainExpr;
class GradeCols;

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
                 const LangSet *ls=0, const GradeCols *gc = 0,
                 QWidget *parent = NULL, const char *name = NULL );
	~RowTable();

	kvoctrainExpr *selectedRow();
	void setNumCols( int cols );
	void setNumRows( int rows );

	void setCurrentRow( int row, int col );
	kvoctrainExpr *getRow( int row );
	void clear();
//	void setCellWidth( int width );
//	void setCellHeight( int height );
	void setSelectColumn( int col );
	int selectColumn() { return currentColumn(); }
	int currentCol() { return currentColumn(); }

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
        void repaintOriginal();
        virtual void paintCell( QPainter *p, int row, int col, const QRect &cr, bool selected);
	virtual void paletteChange( const QPalette &oldPalette );
        virtual QWidget *createEditor(int, int, bool) const;

signals:
	void selected(int row, int col, int key_state);
        void cellMoved(int, int, int);

protected:
	kvoctrainDoc    *m_rows;

private:
	void init(Flags flags);

	vector<SpecFont_t> m_colFonts;
	int                m_flags;

	const GradeCols   *gradecols;

  friend class kvoctrainView;
};

#endif
