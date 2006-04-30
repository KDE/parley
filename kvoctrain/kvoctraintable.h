/***************************************************************************

                      table for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Jan 4 19:04:53 PST 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter.hedlund@kdemail.net

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

#include <Q3Table>
#include <QTimer>
#include <QMouseEvent>
#include <QPixmap>
#include <QKeyEvent>

#include <keduvocdocument.h>
#include <keduvocexpression.h>
#include "kvt-core/langset.h"
#include "kvoctraintableitem.h"

/**
@author Peter Hedlund
*/
class KVocTrainTable : public Q3Table
{
Q_OBJECT
public:
  public:
    KVocTrainTable( KEduVocDocument *_doc, const LangSet *ls, QWidget *parent = NULL, const char *name = NULL );

    friend class kvoctrainView;

    int currentItem() { return currentRow(); }
    void setCurrentItem(int row);
    int count() { return numRows(); }

    void setModified(bool _dirty = true) { m_doc->setModified(_dirty); }
    QString indentifier(int i) const { return m_doc->identifier(i); }
    QString originalIdentifier() const { return m_doc->originalIdentifier(); }
    int findIdent(QString id) const { return m_doc->findIdentifier(id); }
    void setCurrentRow(int row, int col);
    void updateContents(int row = -1, int col = -1);
    KEduVocExpression *getRow(int row);
    void setDoc(KEduVocDocument *rows);
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
    void paintCell(QPainter *p, int row, int col, const QRect &cr, bool selected, const QPalette &cg);
    void setItem(int row, int col, Q3TableItem * item);
    Q3TableItem* item (int row, int col) const;
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void contentsMousePressEvent(QMouseEvent *e);
    void contentsMouseDoubleClickEvent(QMouseEvent *e);
    void activateNextCell();

  private:
    KEduVocDocument * m_doc;
    void sortByColumn(int, bool);
    const LangSet* langs;
    //const GradeCols *gradecols;
    KVocTrainTableItem *defaultItem;
    QTimer *delayTimer;
    int triggerSect;
    QPixmap m_pixInactive;
    QPixmap m_pixInQuery;
};

#endif
