/***************************************************************************

                      table item for the main view

    -----------------------------------------------------------------------

    begin                : Mon Dec 27 17:05:53 PST 2004

    copyright            : (C) 2004-2005 Peter Hedlund

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

#ifndef KVOCTRAINTABLEITEM_H
#define KVOCTRAINTABLEITEM_H

#include <Q3Table>

#include <keduvocdocument.h>
#include "kv_resource.h"

/**
@author Peter Hedlund
 */
class KVocTrainTableItem : public Q3TableItem
{
  public:
    KVocTrainTableItem( Q3Table *t, EditType et, KEduVocDocument *doc );
    QWidget *createEditor() const;
    void setContentFromEditor( QWidget *w );
    void setPosition(int row, int col);
    void setDoc(KEduVocDocument *doc);

  private:
    KEduVocDocument * m_doc;
};

#endif

