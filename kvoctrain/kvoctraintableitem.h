/***************************************************************************

                      table item for the main view

    -----------------------------------------------------------------------

    begin                : Mon Dec 27 17:05:53 PST 2004

    copyright            : (C) 2004-2005 Peter Hedlund

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

#ifndef KVOCTRAINTABLEITEM_H
#define KVOCTRAINTABLEITEM_H

#include <qtable.h>

#include "kvoctraindoc.h"
#include "kv_resource.h"

/**
@author Peter Hedlund
 */
class KVocTrainTableItem : public QTableItem
{
  public:
    KVocTrainTableItem( QTable *t, EditType et, kvoctrainDoc *doc );
    QWidget *createEditor() const;
    void setContentFromEditor( QWidget *w );
    void setPosition(int row, int col);
    void setDoc(kvoctrainDoc *doc);

  private:
    kvoctrainDoc * m_doc;
};

#endif

