//
// C++ Implementation: kvtlanguageview
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <KDebug>

#include "kvtlanguageview.h"

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
    emit signalActivated ();
}




#include "kvtlanguageview.moc"
