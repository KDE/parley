/***************************************************************************

                      table for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Jan 4 19:04:53 PST 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter@peterandlinda.com

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

#ifndef KVOCTRAINTABLE_H
#define KVOCTRAINTABLE_H

#include <qtable.h>
#include <qtimer.h>

#include "kvt-core/kvoctraindoc.h"
#include "kvt-core/kvoctrainexpr.h"
#include "kvoctraintableitem.h"

/**
@author Peter Hedlund
*/
class KVocTrainTable : public QTable
{
Q_OBJECT
public:
  public:
    KVocTrainTable( kvoctrainDoc *_doc, const LangSet *ls, const GradeCols *gc = 0, QWidget *parent = NULL, const char *name = NULL );

    int currentItem() { return currentRow(); }
    void setCurrentItem(int row);
    int count() { return numRows(); }

    void setModified(bool _dirty = true) { m_doc->setModified(_dirty); }
    QString getIdent(int i) const { return m_doc->getIdent(i); }
    QString getOriginalIdent() const { return m_doc->getOriginalIdent(); }
    int findIdent(QString id) const { return m_doc->findIdent(id); }
    void setCurrentRow(int row, int col);
    void updateContents(int row = -1, int col = -1);
    kvoctrainExpr *getRow(int row);
    void setDoc(kvoctrainDoc *rows, const GradeCols *gc = 0);
    void setFont(const QFont &);

  signals:
    void selected(int row);
    void rightButtonClicked(int header, int x, int y);
    void forwardKeyPressEvent(QKeyEvent *);
    void forwardKeyReleaseEvent(QKeyEvent *);
    void edited(int, int);

  public slots:
    void sortByColumn_alpha(int index);
    void sortByColumn_index(int index);
    void slotSelectionChanged();

  protected slots:
    void menuTriggerTimeout();
    void headerReleaseEvent(int sec);
    void headerPressEvent(int sec);

  protected:
    int current;
    QWidget* beginEdit(int row, int col, bool replace);
    void endEdit(int row, int col, bool accept, bool replace);
    void paintCell(QPainter *p, int row, int col, const QRect &cr, bool selected);
    void setItem(int row, int col, QTableItem * item);
    QTableItem* item (int row, int col) const;
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void contentsMousePressEvent(QMouseEvent *e);
    void contentsMouseDoubleClickEvent(QMouseEvent *e);
    void activateNextCell();

  private:
    kvoctrainDoc * m_doc;
    void sortByColumn(int, bool);
    const LangSet* langs;
    const GradeCols *gradecols;
    KVocTrainTableItem *defaultItem;
    QTimer *delayTimer;
    int triggerSect;
};

#endif
