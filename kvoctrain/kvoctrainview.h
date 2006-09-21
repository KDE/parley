/***************************************************************************

                         view to kvoctrain parts

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2004-2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <config.h>

#include <keduvocdocument.h>
//Added by qt3to4:
#include <QShowEvent>
#include <QGridLayout>
#include <QResizeEvent>


class QGridLayout;
class KApplication;
class KPrinter;
class KEduVocExpression;
class KVocTrainTable;
class kvoctrainApp;
class KEduVocDocument;
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
  kvoctrainView(KEduVocDocument* doc, const LangSet &ls, kvoctrainApp *parent = 0);

  /** Destructor for the main view */
  ~kvoctrainView();

  /** sets view to another vocabulary */
  void setView(KEduVocDocument *doc, const LangSet &ls);

  KVocTrainTable* getTable() { return m_table; }
  void setHeaderProp(int id, const QString &name, const QString &pixfile=QString());

  void adjustContent();
  void print(KPrinter *pPrinter);

private:
  void newPage(QPainter &, int, int, int);
  void endOfPage(QPainter &, int, int);

protected:
  KVocTrainTable  * m_table;
  bool              autoResize;
  KEduVocDocument    * m_doc;
  QGridLayout     * list_layout;
  kvoctrainApp    * parent;

  /** resizes table when frame is resized */
  virtual void resizeEvent(QResizeEvent *);
  void showEvent(QShowEvent * s_ev);
};

#endif // KVOCTRAINVIEW_H
