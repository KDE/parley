/***************************************************************************

    $Id$

                     query options dialog class

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2001/10/28 16:56:56  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.3  2001/10/17 21:41:16  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:46:01  arnold
    import of version 0.7.0pre8 to kde-edu


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

#include <kv_resource.h>
#include <compat_2x.h>

#include <kapp.h>

#include <kcombobox.h>

#include <qpixmap.h>


#define Inherited QTabDialog

class kvoctraindoc;

QueryOptionsDlg::QueryOptionsDlg
(
        int           mqtime,
        bool          showcounter,
        kvq_timeout_t type_timeout,
        kvoctrainDoc *_doc,
        KComboBox    *lessons,
        QueryManager *_manager,
        bool          swapdir,
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
  queryOptPage = new QueryOptPage (mqtime, showcounter, type_timeout, &manager,
                                   swapdir, this, name);
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

  addTab( threshOptPage, i18n("&Threshholds"));
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

  setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
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
    bool show = false;
    kvq_timeout_t type_to = kvq_notimeout;
    if (extract (line, s))
      mqtime = s.toInt();
    if (extract (line, s))
      swap = (bool) s.toInt();
    if (extract (line, s))
      show = (bool) s.toInt();
    if (extract (line, s))
      type_to = (kvq_timeout_t) s.toInt();
    queryOptPage->setStates (mqtime, swap, show, type_to);

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
      manager.setLessonComp(QCT(s.toInt()));
    if (extract (line, s))
      manager.setTypeItem(s);   // s.toInt()
    if (extract (line, s))
      manager.setTypeComp(QCT(s.toInt()));
    if (extract (line, s))
      manager.setGradeItem(s.toInt());
    if (extract (line, s))
      manager.setGradeComp(QCT(s.toInt()));
    if (extract (line, s))
      manager.setQueryItem(s.toInt());
    if (extract (line, s))
      manager.setQueryComp(QCT(s.toInt()));
    if (extract (line, s))
      manager.setBadItem(s.toInt());
    if (extract (line, s))
      manager.setBadComp(QCT(s.toInt()));
    if (extract (line, s))
      manager.setDateItem(s.toInt());
    if (extract (line, s))
      manager.setDateComp(QCT(s.toInt()));
    threshOptPage->setStates(&manager);

#undef QCT

    line = settings[grp].block_set;
    bool block = false;
    bool expire = false;

    if (extract (line, s))
      block = s.toInt();

    if (extract (line, s))
      manager.setBlockItem(KV_LEV1_GRADE, s.toInt());
    if (extract (line, s))
      manager.setBlockItem(KV_LEV2_GRADE, s.toInt());
    if (extract (line, s))
      manager.setBlockItem(KV_LEV3_GRADE, s.toInt());
    if (extract (line, s))
      manager.setBlockItem(KV_LEV4_GRADE, s.toInt());
    if (extract (line, s))
      manager.setBlockItem(KV_LEV5_GRADE, s.toInt());
    if (extract (line, s))
      manager.setBlockItem(KV_LEV6_GRADE, s.toInt());
    if (extract (line, s))
      manager.setBlockItem(KV_LEV7_GRADE, s.toInt());

    if (extract (line, s))
      expire = (bool) s.toInt();
    if (extract (line, s))
      manager.setExpireItem(KV_LEV1_GRADE, s.toInt());
    if (extract (line, s))
      manager.setExpireItem(KV_LEV2_GRADE, s.toInt());
    if (extract (line, s))
      manager.setExpireItem(KV_LEV3_GRADE, s.toInt());
    if (extract (line, s))
      manager.setExpireItem(KV_LEV4_GRADE, s.toInt());
    if (extract (line, s))
      manager.setExpireItem(KV_LEV5_GRADE, s.toInt());
    if (extract (line, s))
      manager.setExpireItem(KV_LEV6_GRADE, s.toInt());
    if (extract (line, s))
      manager.setExpireItem(KV_LEV7_GRADE, s.toInt());

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
    s.setNum((int) getShowCounter());
    line += s + ',';
    s.setNum( (int) getTypeQueryTimeout());
    line += s + ',';
    settings[grp].query_set = line;

    line = '(' + manager.lessonItemStr() + ')';
    s.setNum((int) manager.lessonComp());
    line += s + ',';
    s = manager.typeItem();
    line += s + ',';
    s.setNum((int) manager.typeComp());
    line += s + ',';
    s.setNum(manager.gradeItem());
    line += s + ',';
    s.setNum((int) manager.gradeComp());
    line += s + ',';
    s.setNum(manager.queryItem());
    line += s + ',';
    s.setNum((int) manager.queryComp());
    line += s + ',';
    s.setNum(manager.badItem());
    line += s + ',';
    s.setNum((int) manager.badComp());
    line += s + ',';
    s.setNum(manager.dateItem());
    line += s + ',';
    s.setNum((int) manager.dateComp());
    line += s + ',';
    settings[grp].thresh_set = line;

    s.setNum(getBlock());
    line = s + ',';
    for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++) {
      s.setNum(manager.blockItem(i));
      line += s + ',';
    }
  
    s.setNum(getExpire());
    line += s + ',';
    for (int i = KV_LEV1_GRADE; i <= KV_MAX_GRADE; i++) {
      s.setNum(manager.expireItem(i));
      line += s + ',';
    }
    settings[grp].block_set = line;
  }

}


void QueryOptionsDlg::slotDeleteGroup(int grp)
{
  if (grp < (int) settings.size() )
    settings.erase (&settings[grp], &settings[grp+1]);
}


void QueryOptionsDlg::slotNewGroup(const QString &name)
{
  settings.push_back(PreSetting(name, "", "", ""));
  slotModifyGroup(settings.size()-1);
}
#include "QueryOptionsDlg.moc"
