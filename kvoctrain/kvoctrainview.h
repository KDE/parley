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

class QGridLayout;
class QFrame;
class KApplication;
class kvoctrainExpr;
class KVocTrainTable;
class kvoctrainApp;
class kvoctrainDoc;
class GradeCols;

#include "kvoctraindoc.h"

/**
  * This class provides an incomplete base for your application view.
  */

class kvoctrainView : public QWidget
{
  Q_OBJECT

 public:

   enum Resizer { Automatic,  // guess something meaningful for resizing
                  Fixed,      // keep users sizes
                  Percent};   // keep percentage of size

  void setResizer (Resizer res) { header_resizer = res; }

  /** Constructor for the main view */
  kvoctrainView(kvoctrainDoc* doc,
                const LangSet &ls, const GradeCols &gradecols,
                kvoctrainApp *parent = 0);

  /** Destructor for the main view */
  ~kvoctrainView();

  /** sets view to another vocabulary */
  void setView (kvoctrainDoc *doc, const LangSet &ls, const GradeCols &gc);

  KVocTrainTable* getTable() { return lb_list; }
  void setHeaderProp (int id, const QString &name, const QString &pixfile=QString::null);

  void adjustContent();

 private:

 protected:
  KVocTrainTable  *lb_list;
  bool             autoResize;
  kvoctrainDoc    *the_doc;
  QGridLayout     *list_layout;
  Resizer          header_resizer;
  kvoctrainApp    *parent;

  /** resizes table when frame is resized */
  virtual void resizeEvent ( QResizeEvent * );
  void showEvent (  QShowEvent * s_ev);
};

#endif // KVOCTRAINVIEW_H
