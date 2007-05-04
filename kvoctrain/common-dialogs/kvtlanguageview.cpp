/***************************************************************************

                             kvtlanguageview

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@gmx.de>

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

#include "kvtlanguageview.h"

#include <KDebug>

KVTLanguageView::KVTLanguageView(QWidget *parent)
 : QTreeView(parent)
{
    KVTLanguageModel *languageModel;
    languageModel = new KVTLanguageModel();
    setModel(languageModel);
    setRootIsDecorated(false);
    sortByColumn(0, Qt::AscendingOrder);
    connect(this, SIGNAL( activated( const QModelIndex& )), 
        this, SLOT(  emitSignalActivated(const QModelIndex&) ));
}

void KVTLanguageView::currentChanged ( const QModelIndex & current, const QModelIndex & previous )
{
    QTreeView::currentChanged ( current, previous );
    emit signalLanguageChanged(current.data().toString(), current.data(KVTLanguageModel::LanguageTwoLetterRole).toString());
}


void KVTLanguageView::emitSignalActivated ( const QModelIndex & activatedItem )
{
    Q_UNUSED(activatedItem);
    emit signalActivated ();
}




#include "kvtlanguageview.moc"
