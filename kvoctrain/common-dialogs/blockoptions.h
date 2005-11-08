/***************************************************************************

                      blocking options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Tue Apr 5 2005

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

#ifndef BLOCKOPTIONS_H
#define BLOCKOPTIONS_H

#include <Q3ValueList>

#include "blockoptionsbase.h"
#include "QueryManager.h"

class QComboBox;

class BlockOptions : public BlockOptionsBase
{
  Q_OBJECT

public:
  BlockOptions(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
  void updateWidgets();
  bool hasChanged();
  bool isDefault();
  void updateSettings();

signals:
  void widgetModified();
  void blockExpireChanged(bool block, bool expire);

protected slots:
  void slotBlockToggled(bool);
  void slotExpireToggled(bool);
  void slotComboBoxActivated(int);

private:
  void fillWidgets();
  void fillComboBox(QComboBox *);
  void updateComboBox(int, QComboBox *);
  void checkValidity();

  Q3ValueList<QComboBox *> m_blockComboList;
  Q3ValueList<QComboBox *> m_expireComboList;
};

#endif

