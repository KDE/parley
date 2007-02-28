/***************************************************************************
             profilesdialog.h  -  KVocTrain profiles dialog

                             -------------------
    begin                : Fri Apr 29 2005

    copyright            : (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *
 ***************************************************************************/

#ifndef PROFILESDIALOG_H
#define PROFILESDIALOG_H


#include <kdialog.h>

/**
@author Peter Hedlund
*/

class KVTQuery;
class GroupOptions;

class SettingsProfile
{
public:
  SettingsProfile();
  SettingsProfile (QString n, QString q, QString t, QString b);

  QString name;
  QString query_set;
  QString thresh_set;
  QString block_set;
};

class ProfilesDialog : public KDialog
{
  Q_OBJECT

public:
  ProfilesDialog(KVTQuery * m, QWidget *parent = 0, const char *name = 0, bool modal = true);

signals:
  void profileActivated();

protected slots:
  void slotDeleteGroup();
  void slotNewGroup();
  void slotStoreGroup();
  void slotRecallGroup();

private:
  void selectProfile(int profile);
  void modifyProfile(int profile);
  void saveProfiles();

  typedef QList<SettingsProfile> Profiles;
  Profiles profiles;
  KVTQuery * m_queryManager;
  GroupOptions * mw;
};

#endif
