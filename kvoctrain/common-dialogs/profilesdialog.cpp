/***************************************************************************
             profilesdialog.cpp  -  KVocTrain profiles dialog

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
 ***************************************************************************/

#include <QComboBox>
#include <QPushButton>

#include <kdialogbase.h>
#include <klocale.h>
#include <kinputdialog.h>
#include <klocale.h>

#include "profilesdialog.h"
#include "groupoptions.h"
#include "presettings.h"
#include "prefs.h"
#include "QueryManager.h"

SettingsProfile::SettingsProfile( )
{
}

SettingsProfile::SettingsProfile(QString n, QString q, QString t, QString b)
: name(n), query_set(q), thresh_set(t), block_set(b)
{
}

ProfilesDialog::ProfilesDialog(QueryManager * m, QWidget *parent, const char *name, bool modal)
 : KDialogBase(Swallow, i18n("Profiles"), Close, Close, parent, name, modal, false)
{
  m_queryManager = m;
  mw = new GroupOptions(this);
  setMainWidget(mw);

  connect(mw->ps_del, SIGNAL(clicked()), this, SLOT(slotDeleteGroup()));
  connect(mw->ps_new, SIGNAL(clicked()), this, SLOT(slotNewGroup()));
  connect(mw->ps_recall, SIGNAL(clicked()), this, SLOT(slotRecallGroup()));
  connect(mw->ps_store, SIGNAL(clicked()), this, SLOT(slotStoreGroup()));

  int ls = Prefs::numPreSetting();
  for (int i = 0 ; i < ls; i++)
  {
    PreSettings preSettings(QString::number(i));
    preSettings.readConfig();
    profiles.append(SettingsProfile(preSettings.name(), preSettings.query(),
      preSettings.threshold(), preSettings.blocking()));
    mw->ps_name->insertItem(preSettings.name());
  }

  mw->updateButtons();
}

void ProfilesDialog::slotDeleteGroup()
{
  if (mw->ps_name->count() != 0)
  {
    int curr = mw->ps_name->currentIndex();
    mw->ps_name->removeItem(curr);
    profiles.removeAt(curr);
    if (curr >= mw->ps_name->count()-1)
      mw->ps_name->setCurrentItem(mw->ps_name->count()-1);
  }
  saveProfiles();
  mw->updateButtons();
}

void ProfilesDialog::slotNewGroup()
{
  bool ok;
  QString newGroupName = KInputDialog::getText(i18n("Profile Description"),
    i18n("Enter profile description:"), QString(), &ok);

  if (ok)
  {
    newGroupName = newGroupName.simplified();
    mw->ps_name->insertItem(newGroupName);
    profiles.append(SettingsProfile(newGroupName, "", "", ""));
    modifyProfile(profiles.count()-1);
    mw->ps_name->setCurrentItem(mw->ps_name->count()-1);
    selectProfile(mw->ps_name->currentIndex());
    mw->updateButtons();
  }
}

void ProfilesDialog::slotRecallGroup()
{
  if (mw->ps_name->count() != 0)
    selectProfile(mw->ps_name->currentIndex());
}


void ProfilesDialog::slotStoreGroup()
{
  if (mw->ps_name->count() != 0)
    modifyProfile(mw->ps_name->currentIndex());
}



static bool extract (QString &line, QString &ret)
{
  int lim = line.find (",");
  if (lim < 0)
  {
    ret = "";
    line = "";
    return false;
  }
  else
  {
    ret = line.left(lim);
    line.remove(0, lim+1);
  }

  return true;
}

void ProfilesDialog::selectProfile(int profile)
{
  if (profile < (int) profiles.count())
  {
    QString line = profiles[profile].query_set;
    QString s;
    Prefs::setMaxTimePer(0);
    Prefs::setSwapDirection(false);
    Prefs::setAltLearn(false);
    Prefs::setShowCounter(false);
    //kvq_timeout_t type_to = kvq_notimeout;
    Prefs::setSuggestions(false);
    Prefs::setSplit(false);
    Prefs::setPeriods(true);
    Prefs::setColons(false);
    Prefs::setSemicolons(true);
    Prefs::setCommas(false);
    Prefs::setFields(5);
    Prefs::setShowMore(true);
    Prefs::setIKnow(true);
    if (extract (line, s))
      Prefs::setMaxTimePer(s.toInt());
    if (extract (line, s))
      Prefs::setSwapDirection((bool) s.toInt());
    if (extract (line, s))
      Prefs::setAltLearn((bool) s.toInt());
    if (extract (line, s))
      Prefs::setShowCounter((bool) s.toInt());
    if (extract (line, s))
      Prefs::setQueryTimeout(s.toInt());
    if (extract (line, s))
      Prefs::setSuggestions((bool) s.toInt());
    if (extract (line, s))
      Prefs::setSplit((bool) s.toInt());
    if (extract (line, s))
      Prefs::setPeriods((bool) s.toInt());
    if (extract (line, s))
      Prefs::setColons((bool) s.toInt());
    if (extract (line, s))
      Prefs::setSemicolons((bool) s.toInt());
    if (extract (line, s))
      Prefs::setCommas((bool) s.toInt());
    if (extract (line, s))
      Prefs::setFields(s.toInt());
    if (extract (line, s))
      Prefs::setShowMore((bool) s.toInt());
    if (extract (line, s))
      Prefs::setIKnow((bool) s.toInt());

#define QCT(x)  Prefs::EnumCompType::type(x)

    line = profiles[profile].thresh_set;

    line.simplified();
    if (line.length() != 0 && line[0] == '(')
    { // new style: multiple lessons
      int pos;
      line.remove(0, 1);
      if ((pos = line.find(')')) > 0)
      {
        s = line.left (pos);
        line.remove (0, pos+1);
      }
      else
      {
        s = line;
        line = "";
      }
      m_queryManager->setLessonItemStr(s);
    }
    else if (extract (line, s))
      m_queryManager->setLessonItemStr(s);

    if (extract (line, s))
      Prefs::setCompType(Prefs::EnumType::Lesson, QCT(s.toInt()));
    if (extract (line, s))
      Prefs::setTypeItem(s);   // s.toInt()
    if (extract (line, s))
      Prefs::setCompType(Prefs::EnumType::WordType, QCT(s.toInt())) /*manager.setTypeComp(QCT(s.toInt()))*/;
    if (extract (line, s))
      Prefs::setGradeItem(s.toInt());
    if (extract (line, s))
      Prefs::setCompType(Prefs::EnumType::Grade, QCT(s.toInt()));
    if (extract (line, s))
      Prefs::setQueryItem(s.toInt());
    if (extract (line, s))
      Prefs::setCompType(Prefs::EnumType::Query, QCT(s.toInt()));
    if (extract (line, s))
      Prefs::setBadItem(s.toInt());
    if (extract (line, s))
      Prefs::setCompType(Prefs::EnumType::Bad, QCT(s.toInt()));
    if (extract (line, s))
      Prefs::setDateItem(s.toInt());
    if (extract (line, s))
      Prefs::setCompType(Prefs::EnumType::Date, QCT(s.toInt()));

#undef QCT

    line = profiles[profile].block_set;
    Prefs::setBlock(false);
    Prefs::setExpire(false);

    if (extract (line, s))
      Prefs::setBlock((bool) s.toInt());
    if (extract (line, s))
      Prefs::setBlockItem(KV_LEV1_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setBlockItem(KV_LEV2_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setBlockItem(KV_LEV3_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setBlockItem(KV_LEV4_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setBlockItem(KV_LEV5_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setBlockItem(KV_LEV6_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setBlockItem(KV_LEV7_GRADE, s.toInt());

    if (extract (line, s))
      Prefs::setExpire((bool) s.toInt());
    if (extract (line, s))
      Prefs::setExpireItem(KV_LEV1_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setExpireItem(KV_LEV2_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setExpireItem(KV_LEV3_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setExpireItem(KV_LEV4_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setExpireItem(KV_LEV5_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setExpireItem(KV_LEV6_GRADE, s.toInt());
    if (extract (line, s))
      Prefs::setExpireItem(KV_LEV7_GRADE, s.toInt());
  }
  emit profileActivated();
}

void ProfilesDialog::modifyProfile(int profile)
{
  if (profile == -1)
    return;

  if (profile < (int) profiles.count())
  {
    QString s;
    QString line;

    s.setNum(Prefs::maxTimePer());
    line += s + ',';
    s.setNum(Prefs::swapDirection());
    line += s + ',';
    s.setNum(Prefs::altLearn());
    line += s + ',';
    s.setNum((int) Prefs::showCounter());
    line += s + ',';
    s.setNum( (int) Prefs::queryTimeout() /*getTypeQueryTimeout()*/);
    line += s + ',';
    s.setNum((int) Prefs::suggestions());
    line += s + ',';
    s.setNum((int) Prefs::split());
    line += s + ',';
    s.setNum((int) Prefs::periods());
    line += s + ',';
    s.setNum((int) Prefs::colons());
    line += s + ',';
    s.setNum((int) Prefs::semicolons());
    line += s + ',';
    s.setNum((int) Prefs::commas());
    line += s + ',';
    s.setNum(Prefs::fields());
    line += s + ',';
    s.setNum((int) Prefs::showMore());
    line += s + ',';
    s.setNum((int) Prefs::iKnow());
    line += s + ',';
    profiles[profile].query_set = line;

    line = '(' + m_queryManager->lessonItemStr() + ')';
    s.setNum((int) Prefs::compType(Prefs::EnumType::Lesson));
    line += s + ',';
    s = Prefs::typeItem();
    line += s + ',';
    s.setNum((int) Prefs::compType(Prefs::EnumType::WordType) /*manager.typeComp()*/);
    line += s + ',';
    s.setNum(Prefs::gradeItem());
    line += s + ',';
    s.setNum((int) Prefs::compType(Prefs::EnumType::Grade));
    line += s + ',';
    s.setNum(Prefs::queryItem());
    line += s + ',';
    s.setNum((int) Prefs::compType(Prefs::EnumType::Query));
    line += s + ',';
    s.setNum(Prefs::badItem());
    line += s + ',';
    s.setNum((int) Prefs::compType(Prefs::EnumType::Bad));
    line += s + ',';
    s.setNum(Prefs::dateItem());
    line += s + ',';
    s.setNum((int) Prefs::compType(Prefs::EnumType::WordType));
    line += s + ',';
    profiles[profile].thresh_set = line;

    s.setNum(Prefs::block());
    line = s + ',';
    for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++)
    {
      s.setNum(Prefs::blockItem(i));
      line += s + ',';
    }

    s.setNum(Prefs::expire());
    line += s + ',';
    for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++)
    {
      s.setNum(Prefs::expireItem(i));
      line += s + ',';
    }
    profiles[profile].block_set = line;
  }
  saveProfiles();
}

void ProfilesDialog::saveProfiles( )
{
  Prefs::setNumPreSetting(profiles.count());
  for (int i = 0 ; i < (int) profiles.count(); i++)
  {
    PreSettings preSettings(QString::number(i));
    preSettings.setName(profiles[i].name);
    preSettings.setQuery(profiles[i].query_set);
    preSettings.setThreshold(profiles[i].thresh_set);
    preSettings.setBlocking(profiles[i].block_set);
    preSettings.writeConfig();
  }
}

#include "profilesdialog.moc"
