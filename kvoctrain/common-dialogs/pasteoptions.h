/***************************************************************************

                      paste options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 31 2005

    copyright            :(C) 2005 Peter Hedlund

    email                : peter@peterandlinda.com

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

#ifndef PASTEOPTIONS_H
#define PASTEOPTIONS_H

#include "pasteoptionsbase.h"
#include "langset.h"

class kvoctrainDoc;

class PasteOptions : public PasteOptionsBase
{
  Q_OBJECT

public:
  PasteOptions(LangSet & langset, kvoctrainDoc * doc, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

  void updateWidgets();
  bool hasChanged();
  bool isDefault();
  void updateSettings();

signals:
  void widgetModified();
  void syncPasteOrderList();

private slots:
  void fillWidgets();
  void syncButtons();
  void slotUpButtonClicked();
  void slotSkipButtonClicked();
  void slotDownButtonClicked();
  void slotSeparatorComboActivated(int);
  void slotUseCurrentDocToggled(bool);

private:
  void slotSyncPasteOrderList();
  QStringList preparePasteOrderList();
  kvoctrainDoc * m_doc;
  LangSet m_langSet;

};

#endif

