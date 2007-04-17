//
// C++ Interface: kvtlanguageview
//
// Description: 
//
//
// Author: Frederik Gladhorn <frederik.gladhorn@gmx.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KVTLANGUAGEVIEW_H
#define KVTLANGUAGEVIEW_H

#include <QTreeView>

#include "kvtlanguagemodel.h"

/**
	@author Frederik Gladhorn <frederik.gladhorn@gmx.de>
*/
class KVTLanguageView : public QTreeView
{
Q_OBJECT

public:
    KVTLanguageView(QWidget *parent = 0);

public slots:
    void currentChanged ( const QModelIndex & current, const QModelIndex & previous );
    /** Only emit signalActivated without QModelIndex for the wizard.
     * @param activatedItem
     */
    void emitSignalActivated ( const QModelIndex & activatedItem );

signals:
    void signalLanguageChanged(const QString &languageName, const QString &languageTwoLetter);
    /** To let the wizard know that the next page is requested. */
    void signalActivated();
};

#endif
