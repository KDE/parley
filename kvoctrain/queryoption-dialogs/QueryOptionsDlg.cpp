/***************************************************************************

                     query options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team

    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "QueryOptionsDlg.h"
#include <prefs.h>

#include <kstandarddirs.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kcombobox.h>



#define Inherited QTabDialog

class kvoctraindoc;

QueryOptionsDlg::QueryOptionsDlg
(
        int           mqtime,
        bool          showcounter,
        //kvq_timeout_t type_timeout,
        kvoctrainDoc *,
        KComboBox    *lessons,
        QueryManager *_manager,
        bool          swapdir,
        bool          suggestions,
        bool          split,
        bool          periods,
        bool          colons,
        bool          semicolons,
        bool          commas,
        int           fields,
        bool          show_more,
        bool          i_know,
        bool          altlearn,
        bool          block,
        bool          expire,
        vector<PreSetting>  preset,
	QWidget      *parent,
	const char   *name
)
	:
	Inherited( parent, name, true ),
        settings(preset)
{
  manager = *_manager;
  setCaption(kapp->makeStdCaption(i18n("Query Options")));

  setCancelButton(i18n("&Cancel"));
  setOkButton(i18n("&OK"));

  groupOptPage = new GroupOptPage (settings, this, name);
  threshOptPage = new ThreshOptPage (&manager, lessons, this, name);
  queryOptPage = new QueryOptPage (mqtime, showcounter, &manager, swapdir,
    suggestions, split, periods, colons, semicolons, commas, fields, show_more, i_know, altlearn, this, name);
  blockOptPage = new BlockOptPage (&manager, block, expire, this, name);

  addTab( groupOptPage, i18n("&Groups"));
  connect(groupOptPage, SIGNAL(selectGroup(int)), SLOT(slotSelectGroup(int)) );
  connect(groupOptPage, SIGNAL(newGroup(const QString&)), SLOT(slotNewGroup(const QString&)) );
  connect(groupOptPage, SIGNAL(deleteGroup(int)), SLOT(slotDeleteGroup(int)) );
  connect(groupOptPage, SIGNAL(modifyGroup(int)), SLOT(slotModifyGroup(int)) );
  connect (this, SIGNAL(aboutToShow()), groupOptPage, SLOT(initFocus() ));

  addTab( queryOptPage, i18n("&Query"));
  connect (this, SIGNAL(aboutToShow()), queryOptPage, SLOT(initFocus() ));
  connect(queryOptPage, SIGNAL(modifySetting(int)), SLOT(slotModifyGroup(int)) );

  addTab( threshOptPage, i18n("&Thresholds"));
  connect (this, SIGNAL(aboutToShow()), threshOptPage, SLOT(initFocus() ));
  connect(threshOptPage, SIGNAL(modifySetting(int)), SLOT(slotModifyGroup(int)) );

  addTab( blockOptPage, i18n("&Blocking"));
  connect (this, SIGNAL(aboutToShow()), blockOptPage, SLOT(initFocus() ));
  connect(blockOptPage, SIGNAL(modifySetting(int)), SLOT(slotModifyGroup(int)) );
  connect(blockOptPage,  SIGNAL(blockExpire(bool, bool)),
          threshOptPage, SLOT(slotBlockExpire(bool, bool)) );

  connect( this, SIGNAL(applyButtonPressed()), SLOT(okButton()) );
  connect( this, SIGNAL(cancelButtonPressed()), SLOT(cancelButton()) );

  threshOptPage->slotBlockExpire (getBlock(), getExpire());
}

void QueryOptionsDlg::showEvent (QShowEvent* event)
{
  Inherited::showEvent (event);
  resize (width(), minimumSize().height());
}


void QueryOptionsDlg::cancelButton()
{
  emit reject();
}


void QueryOptionsDlg::okButton()
{
  emit accept();
}


static bool extract (QString &line, QString &ret)
{
   int lim = line.find (",");
   if (lim < 0) {
     ret = "";
     line = "";
     return false;
   }
   else {
     ret = line.left(lim);
     line.remove (0, lim+1);
   }

   return true;
}


void QueryOptionsDlg::slotSelectGroup(int grp)
{
  if (grp < (int) settings.size() ) {
    QString line = settings[grp].query_set;
    QString s;
    int mqtime = 0;
    bool swap = false;
    bool altlearn = false;
    bool show = false;
    //kvq_timeout_t type_to = kvq_notimeout;
    bool suggestions = false;
    bool split = false;
    bool periods = true;
    bool colons = false;
    bool semicolons = true;
    bool commas = false;
    int  fields = 5;
    bool show_more = true;
    bool i_know = true;
    if (extract (line, s))
      mqtime = s.toInt();
    if (extract (line, s))
      swap = (bool) s.toInt();
    if (extract (line, s))
      altlearn = (bool) s.toInt();
    if (extract (line, s))
      show = (bool) s.toInt();
    if (extract (line, s))
      Prefs::setQueryTimeout(s.toInt());
    if (extract (line, s))
      suggestions = (bool) s.toInt();
    if (extract (line, s))
      split = (bool) s.toInt();
    if (extract (line, s))
      periods = (bool) s.toInt();
    if (extract (line, s))
      colons = (bool) s.toInt();
    if (extract (line, s))
      semicolons = (bool) s.toInt();
    if (extract (line, s))
      commas = (bool) s.toInt();
    if (extract (line, s))
      fields = s.toInt();
    if (extract (line, s))
      show_more = (bool) s.toInt();
    if (extract (line, s))
      i_know = (bool) s.toInt();
    queryOptPage->setStates (mqtime, swap, altlearn, show, suggestions,
      split, periods, colons, semicolons, commas, fields, show_more, i_know);

#define QCT(x)  QueryManager::CompType(x)

    line = settings[grp].thresh_set;

    line.stripWhiteSpace();
    if (line.length() != 0 && line[0] == '(') { // new style: multiple lessons
      int pos;
      line.remove(0, 1);
      if ((pos = line.find(')')) > 0) {
        s = line.left (pos);
        line.remove (0, pos+1);
      }
      else {
        s = line;
        line = "";
      }
      manager.setLessonItemStr(s);
    }
    else if (extract (line, s))
      manager.setLessonItemStr(s);

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
    threshOptPage->setStates(&manager);

#undef QCT

    line = settings[grp].block_set;
    bool block = false;
    bool expire = false;

    if (extract (line, s))
      block = s.toInt();
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
      expire = (bool) s.toInt();
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

    blockOptPage->setStates(&manager, block, expire);
  }
}


void QueryOptionsDlg::slotModifyGroup(int grp)
{
  if (grp == -1)
    return;

  if (grp < (int) settings.size() ) {
    QString s, line;

    s.setNum(getMQueryTime());
    line += s + ',';
    s.setNum(getSwapDir());
    line += s + ',';
    s.setNum(getAltLearn());
    line += s + ',';
    s.setNum((int) getShowCounter());
    line += s + ',';
    s.setNum( (int) Prefs::queryTimeout() /*getTypeQueryTimeout()*/);
    line += s + ',';
    s.setNum((int) getSuggestions());
    line += s + ',';
    s.setNum((int) getSplit());
    line += s + ',';
    s.setNum((int) getPeriods());
    line += s + ',';
    s.setNum((int) getColons());
    line += s + ',';
    s.setNum((int) getSemicolons());
    line += s + ',';
    s.setNum((int) getCommas());
    line += s + ',';
    s.setNum(getFields());
    line += s + ',';
    s.setNum((int) getShowMore());
    line += s + ',';
    s.setNum((int) getIKnow());
    line += s + ',';
    settings[grp].query_set = line;

    line = '(' + manager.lessonItemStr() + ')';
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
    settings[grp].thresh_set = line;

    s.setNum(getBlock());
    line = s + ',';
    for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++) {
      s.setNum(Prefs::blockItem(i));
      line += s + ',';
    }

    s.setNum(getExpire());
    line += s + ',';
    for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++) {
      s.setNum(Prefs::expireItem(i));
      line += s + ',';
    }
    settings[grp].block_set = line;
  }

}


void QueryOptionsDlg::slotDeleteGroup(int grp)
{
  if (grp < (int) settings.size() ) {
    vector<PreSetting>::iterator start(&settings[grp]);
    vector<PreSetting>::iterator end(&settings[grp+1]);
    settings.erase (start, end);
  }
}


void QueryOptionsDlg::slotNewGroup(const QString &name)
{
  settings.push_back(PreSetting(name, "", "", ""));
  slotModifyGroup(settings.size()-1);
}


void QueryOptionsDlg::selectPage(int index)
{
  switch (index) {
    case 0: showPage(groupOptPage);
    break;

    case 1: showPage(queryOptPage);
    break;

    case 2: showPage(threshOptPage);
    break;

    case 3: showPage(blockOptPage);
    break;

   default:
     kdError() << "QueryOptionsDlg::selectPage(int) index out of range" << endl;
  }
}


#include "QueryOptionsDlg.moc"
