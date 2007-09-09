/***************************************************************************
             profilesdialog.h  -  KVocTrain profiles dialog

                             -------------------
    begin                : Fri Apr 29 2005

    copyright            : (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                           (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

class KEduVocDocument;
class GroupOptions;

class SettingsProfile
{
public:
    SettingsProfile();
    SettingsProfile(const QString &n, const QString &q, const QString &t, const QString &b);

    QString name;
    QString query_set;
    QString thresh_set;
    QString block_set;
};

class ProfilesDialog : public KDialog
{
    Q_OBJECT

public:
    ProfilesDialog(KEduVocDocument *doc, QWidget *parent);

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
    GroupOptions * mw;
    KEduVocDocument *m_doc; // This is used to make changes to the lesson selection.
};

#endif
