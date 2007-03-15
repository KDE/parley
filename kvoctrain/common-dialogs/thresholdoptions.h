/***************************************************************************

                      threshold options for kvoctrain

    -----------------------------------------------------------------------

    begin         : Tue Apr 5 2005

    copyright     : (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef THRESHOLDOPTIONS_H
#define THRESHOLDOPTIONS_H

#include "ui_thresholdoptionsbase.h"
#include "kvtquery.h"

class KComboBox;

class ThresholdOptions : public QWidget, public Ui::ThresholdOptionsBase
{
  Q_OBJECT
public:
  ThresholdOptions(KVTQuery * m, QWidget* parent = 0);

  void updateWidgets();
  bool hasChanged();
  bool isDefault();
  void updateSettings();

signals:
  void widgetModified();

protected slots:
  void slotComboActivated(int);
  void slotSetGradeComp(int);
  void slotSetBadComp(int);
  void slotSetQueryComp(int);
  void slotSetTypeComp(int);
  void slotSetDateComp(int);
  void slotBlockExpire(bool block, bool expire);

private:
  void fillWidgets();

  KVTQuery * m_queryManager;
  QList<TypeRelation> all_maintypes;
};

#endif
