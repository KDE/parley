/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "entryfilter.h"

#include "testentrymanager.h"

EntryFilter::EntryFilter(QWidget * parent, TestEntryManager* manager) :KDialog(parent)
{
    setCaption(i18n("Start Practice"));
    QWidget *widget = new QWidget( this );
    ui.setupUi(widget);
    setMainWidget(widget);

    m_manager = manager;
}

EntryFilter::~ EntryFilter()
{
}


#include "entryfilter.moc"
