/***************************************************************************

                      threshold options for Parley

    -----------------------------------------------------------------------

    begin         : Tue Apr 5 2005

    copyright     : (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "thresholdoptions.h"

#include "practice/testentrymanager.h"
#include "prefs.h"

#include <keduvocdocument.h>
#include <keduvocwordtype.h>

ThresholdOptions::ThresholdOptions(KEduVocDocument* doc, QWidget* parent) : QWidget(parent)
{
    setupUi(this);
    m_doc = doc;

    connect(kcfg_WordTypesInPracticeEnabled, SIGNAL(toggled(bool)), PracticeWordTypesTreeWidget, SLOT(setEnabled(bool)));

    PracticeWordTypesTreeWidget->setEnabled(Prefs::wordTypesInPracticeEnabled());

    /// @todo the checking should move into updateWidgets() !
    QStringList activeWordTypes = Prefs::wordTypesInPractice();
    QStringList activeSubWordTypes = Prefs::subWordTypesInPractice();

    QTreeWidgetItem* typeItem;
    QTreeWidgetItem* subTypeItem;
    foreach ( QString typeName, m_doc->wordTypes().typeNameList() ) {
        typeItem = new QTreeWidgetItem(PracticeWordTypesTreeWidget);
        typeItem->setText(0, typeName);
        if ( activeWordTypes.contains( typeName ) ) {
            typeItem->setCheckState(0, Qt::Checked);
        } else {
            typeItem->setCheckState(0, Qt::Unchecked);
        }
        typeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
        PracticeWordTypesTreeWidget->addTopLevelItem( typeItem );
        // subtypes
        foreach ( QString subTypeName, m_doc->wordTypes().subTypeNameList(typeName) ) {
            subTypeItem = new QTreeWidgetItem(typeItem);
            subTypeItem->setText(0, subTypeName);
            if ( activeSubWordTypes.contains( subTypeName ) ) {
                subTypeItem->setCheckState(0, Qt::Checked);
            } else {
                subTypeItem->setCheckState(0, Qt::Unchecked);
            }
            subTypeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            typeItem->addChild(subTypeItem);
        }
    }
    connect(PracticeWordTypesTreeWidget, SIGNAL(  itemChanged ( QTreeWidgetItem *, int)), SLOT( wordTypeItemChanged ( QTreeWidgetItem *, int )));
}


void ThresholdOptions::updateWidgets()
{
}


bool ThresholdOptions::isDefault()
{
    return true;
    ///@todo
}


void ThresholdOptions::updateSettings()
{
    QStringList activeWordTypes;
    QStringList activeSubWordTypes;
    QTreeWidgetItem* parentItem = PracticeWordTypesTreeWidget->invisibleRootItem();
    for ( int i = 0; i < parentItem->childCount(); i++ ) {
        QTreeWidgetItem* typeItem = parentItem->child(i);
        if ( typeItem->checkState(0) == Qt::Checked ) {
            activeWordTypes.append(typeItem->text(0));
            // subtypes
            for ( int j = 0; j < typeItem->childCount(); j++ ) {
                QTreeWidgetItem* subTypeItem = typeItem->child(j);
                if ( subTypeItem->checkState(0) == Qt::Checked ) {
                    activeSubWordTypes.append(subTypeItem->text(0));
                }
            }
        }
    }
    Prefs::setWordTypesInPractice(activeWordTypes);
    Prefs::setSubWordTypesInPractice(activeSubWordTypes);
}

bool ThresholdOptions::hasChanged()
{
    return false;
}

void ThresholdOptions::wordTypeItemChanged ( QTreeWidgetItem * item, int column )
{
    // if it's a parent item, also change the check state of the children
    if ( item->parent() == 0 ) {
        for(int i = 0; i<item->childCount(); i++) {
            item->child(i)->setCheckState(0, item->checkState(0));
        }
    }
}


#include "thresholdoptions.moc"
