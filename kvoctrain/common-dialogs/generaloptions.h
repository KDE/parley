/***************************************************************************

                      general options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Mar 29 2005

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

#ifndef GENERALOPTIONS_H
#define GENERALOPTIONS_H

#include "ui_generaloptionsbase.h"

class GeneralOptions : public QWidget, public Ui::GeneralOptionsBase
{
  Q_OBJECT
public:
  GeneralOptions(QWidget* parent = 0);

  void updateWidgets();
  bool hasChanged();
  bool isDefault();
  void updateSettings();

signals:
  void widgetModified();

private slots:
  void fillWidgets();
  void slotSeparatorComboActivated(int);
};

#endif

