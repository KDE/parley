//Added by qt3to4:
#include <QShowEvent>
#include <Q3Frame>
#include <QGridLayout>
#include <QResizeEvent>
/***************************************************************************

                         view to kvoctrain parts

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004-2005 Peter Hedlund <peter@peterandlinda.com>

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


#ifndef KVOCTRAINVIEW_H
#define KVOCTRAINVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "kvoctraindoc.h"

class QGridLayout;
class Q3Frame;
class KApplication;
class KPrinter;
class kvoctrainExpr;
class KVocTrainTable;
class kvoctrainApp;
class kvoctrainDoc;
class GradeCols;
class LangSet;

/**
  * This class provides an incomplete base for your application view.
  */

class kvoctrainView : public QWidget
{
  Q_OBJECT

 public:
  /** Constructor for the main view */
  kvoctrainView(kvoctrainDoc* doc, const LangSet &ls, kvoctrainApp *parent = 0);

  /** Destructor for the main view */
  ~kvoctrainView();

  /** sets view to another vocabulary */
  void setView(kvoctrainDoc *doc, const LangSet &ls);

  KVocTrainTable* getTable() { return m_table; }
  void setHeaderProp(int id, const QString &name, const QString &pixfile=QString::null);

  void adjustContent();
  void print(KPrinter *pPrinter);

private:
  void newPage(QPainter &, int, int, int);
  void endOfPage(QPainter &, int, int);

protected:
  KVocTrainTable  * m_table;
  bool              autoResize;
  kvoctrainDoc    * m_doc;
  QGridLayout     * list_layout;
  kvoctrainApp    * parent;

  /** resizes table when frame is resized */
  virtual void resizeEvent(QResizeEvent *);
  void showEvent(QShowEvent * s_ev);
};

#endif // KVOCTRAINVIEW_H
