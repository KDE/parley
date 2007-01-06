/***************************************************************************

                      paste options for kvoctrain

    -----------------------------------------------------------------------

    begin                : Thu Mar 31 2005

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

#ifndef PASTEOPTIONS_H
#define PASTEOPTIONS_H

#include "ui_pasteoptionsbase.h"
#include "kvtlanguages.h"

class KEduVocDocument;

class PasteOptions : public QWidget, public Ui::PasteOptionsBase
{
  Q_OBJECT
public:
  PasteOptions(KVTLanguages & langset, KEduVocDocument * doc, QWidget* parent = 0);

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
  KEduVocDocument * m_doc;
  KVTLanguages m_langSet;
};

#endif
