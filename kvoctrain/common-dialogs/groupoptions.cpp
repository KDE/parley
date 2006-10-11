/***************************************************************************

                      group options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

    copyright            :(C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <QPushButton>
#include <QComboBox>

#include "groupoptions.h"

GroupOptions::GroupOptions(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
}

void GroupOptions::updateButtons()
{
  ps_del->setEnabled (ps_name->count() > 0);
  ps_store->setEnabled (ps_name->count() > 0);
  ps_recall->setEnabled (ps_name->count() > 0);
  ps_name->setEnabled (ps_name->count() > 0);
}

#include "groupoptions.moc"
