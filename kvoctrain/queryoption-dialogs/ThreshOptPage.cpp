/***************************************************************************

    $Id$

                     threshhold options dialog page

    -----------------------------------------------------------------------

    begin                : Sat Jul 10 18:03:22 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:46:01  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#include "ThreshOptPage.h"
#include <iostream.h>
#include <vector.h>

#include <qkeycode.h>

#define Inherited ThreshOptPageData

#define _QM  QueryManager
#define _QT(x) QueryManager::CompType(x)

////////////////////////////////////
// don`t care == 0

static QueryManager::CompType
     bad_complist[]   =  {_QM::DontCare, _QM::MoreThan, _QM::MoreEqThan,
                          _QM::EqualTo, _QM::NotEqual,
                          _QM::LessEqThan, _QM::LessThan,  _QT(-1)};

static QueryManager::CompType
     query_complist[] =  {_QM::DontCare, _QM::MoreThan, _QM::MoreEqThan,
                          _QM::EqualTo, _QM::NotEqual,
                          _QM::LessEqThan, _QM::LessThan, _QT(-1)};

static QueryManager::CompType
     grade_complist[] =  {_QM::DontCare, _QM::WorseThan, _QM::WorseEqThan,
                          _QM::EqualTo, _QM::NotEqual,
                          _QM::BetterEqThan, _QM::BetterThan, _QT(-1)};

static QueryManager::CompType
     type_complist[] =   {_QM::DontCare, _QM::EqualTo, _QM::NotEqual, _QT(-1)};

static QueryManager::CompType
     date_complist[] =   {_QM::DontCare,
                          _QM::Before, _QM::Within, _QM::NotQueried, _QT(-1)};

static QueryManager::CompType
     less_complist[] =   {_QM::DontCare,
                          _QM::OneOf, _QM::NotOneOf, _QM::Current,
                          _QM::NotAssigned, _QT(-1)};

struct ListRef {
          const char *text;
          long int num;
       };

static ListRef bad_itemlist [] =
      { {"0",   0},
        {"1",   1},
        {"2",   2},
        {"3",   3},
        {"4",   4},
        {"5",   5},
        {"6",   6},
        {"7",   7},
        {"8",   8},
        {"9",   9},
        {"10", 10},
        {"11", 11},
        {"12", 12},
        {"13", 13},
        {"14", 14},
        {"15", 15},
        {  0 ,  0}
      };

static ListRef query_itemlist [] =
      { {"0", 0},
        {"1", 1},
        {"2", 2},
        {"3", 3},
        {"4", 4},
        {"5", 5},
        {"6", 6},
        {"7", 7},
        {"8", 8},
        {"9", 9},
        {"10", 10},
        {"11", 11},
        {"12", 12},
        {"13", 13},
        {"14", 14},
        {"15", 15},
        {0 ,  0}
      };

#ifndef i18n_noop
# define i18n_noop(x) (x)
#endif

static ListRef date_itemlist [] =
      {
        {i18n_noop("30 min"),        30*60   },
        {i18n_noop("1 hour"),    1 * 60*60   },
        {i18n_noop("2 hours"),   2 * 60*60   },
        {i18n_noop("4 hours"),   4 * 60*60   },
        {i18n_noop("8 hours"),   8 * 60*60   },
        {i18n_noop("12 hours"), 12 * 60*60   },
        {i18n_noop("18 hours"), 18 * 60*60   },

        {i18n_noop("1 day"),    1 * 60*60*24},
        {i18n_noop("2 days"),   2 * 60*60*24},
        {i18n_noop("3 days"),   3 * 60*60*24},
        {i18n_noop("4 days"),   4 * 60*60*24},
        {i18n_noop("5 days"),   5 * 60*60*24},
        {i18n_noop("6 days"),   6 * 60*60*24},

        {i18n_noop("1 week"),   1 * 60*60*24*7},
        {i18n_noop("2 weeks"),  2 * 60*60*24*7},
        {i18n_noop("3 weeks"),  3 * 60*60*24*7},
        {i18n_noop("4 weeks"),  4 * 60*60*24*7},

        {i18n_noop("1 month"),  1 * 60*60*24*7*30},
        {i18n_noop("2 months"), 2 * 60*60*24*7*30},
        {i18n_noop("3 months"), 3 * 60*60*24*7*30},
        {i18n_noop("4 months"), 4 * 60*60*24*7*30},
        {i18n_noop("5 months"), 5 * 60*60*24*7*30},
        {i18n_noop("6 months"), 6 * 60*60*24*7*30},
        {0 ,  0}
      };

#undef _QM
#undef _QT

#include <kapp.h>

ThreshOptPage::ThreshOptPage
(
        QueryManager *_manager,
        QComboBox    *_lessons,
	QWidget      *parent,
	const char   *name
)
	:
	Inherited( parent, name )
{
   connect( lessoncomp, SIGNAL(activated(int)), SLOT(slotSetLessonComp(int)) );
   connect( datelist, SIGNAL(activated(int)), SLOT(slotSetDateItem(int)) );
   connect( datecomp, SIGNAL(activated(int)), SLOT(slotSetDateComp(int)) );
   connect( badlist, SIGNAL(activated(int)), SLOT(slotSetBadItem(int)) );
   connect( badcomp, SIGNAL(activated(int)), SLOT(slotSetBadComp(int)) );
   connect( querylist, SIGNAL(activated(int)), SLOT(slotSetQueryItem(int)) );
   connect( querycomp, SIGNAL(activated(int)), SLOT(slotSetQueryComp(int)) );
   connect( gradelist, SIGNAL(activated(int)), SLOT(slotSetGradeItem(int)) );
   connect( gradecomp, SIGNAL(activated(int)), SLOT(slotSetGradeComp(int)) );
   connect( typelist, SIGNAL(activated(int)), SLOT(slotSetTypeItem(int)) );
   connect( typecomp, SIGNAL(activated(int)), SLOT(slotSetTypeComp(int)) );
   connect( lessonlist, SIGNAL(selectionChanged()), SLOT(slotSetLessonItems()) );
   connect( b_all_less, SIGNAL(clicked()), SLOT(slotSelectAll()) );
   connect( b_none_less, SIGNAL(clicked()), SLOT(slotSelectNone()) );

   lessons = _lessons;

   label_grade->setBuddy(gradecomp);
   label_qdate->setBuddy(datecomp);
   label_type->setBuddy(typecomp);
   label_qcount->setBuddy(querycomp);
   label_bcount->setBuddy(badcomp);
   label_lesson->setBuddy(lessoncomp);
   setStates(_manager);
}


void ThreshOptPage::setStates(QueryManager *_manager)
{
   manager = _manager;
   QueryManager::CompType *ct;
   ListRef *ref;
   int index;

   // fill in all listboxes and select last choice

   // first in lessons is trash !
   lessonlist->clear();
   for (int i = 1; i < (int) lessons->count(); i++ )
     lessonlist->insertItem (lessons->text(i));

   vector<int> sel = manager->lessonItems();
   if (sel.size() != 0) {
     for (int i = 0; i < (int) sel.size(); i++)  {
       // 0 = not assigned lesson
       if (sel[i] > 0 && sel[i]-1 < (int) lessonlist->count() ) {
         lessonlist->setCurrentItem(sel[i]-1);  // important with qt1.x!
         lessonlist->setSelected(sel[i]-1, true);
       }
     }
   }
   slotSetLessonItems(); 

   // limits and threshholds
   index = 0;
   gradelist->clear();
   for (int i = 1; i <= KV_MAX_GRADE; i++) {
     gradelist->insertItem (manager->gradeStr(i));
     if (manager->gradeItem() == i)
       index = i-1;
   }
   gradelist->setCurrentItem (index);
   slotSetGradeItem(index);

   index = 0;
   typelist->clear();
   all_maintypes = QueryManager::getRelation(true); // collect main types
   for (int i = 0; i < (int) all_maintypes.size(); i++) {
     typelist->insertItem (all_maintypes[i].longStr());
     if (manager->typeItem() == all_maintypes[i].shortStr())
       index = i;
   }
   typelist->setCurrentItem (index);

   index = 0;
   ref = bad_itemlist;
   badlist->clear();
   while (ref->text != 0 ) {
     badlist->insertItem (ref->text);
     if (manager->badItem() == ref->num)
       index = ref - bad_itemlist;
     ref++;
   }
   badlist->setCurrentItem (index);

   index = 0;

   ref = query_itemlist;
   querylist->clear();
   while (ref->text != 0 ) {
     querylist->insertItem (ref->text);
     if (manager->queryItem() == ref->num)
       index = ref - query_itemlist;
     ref++;
   }
   querylist->setCurrentItem (index);

   index = 0;
   ref = date_itemlist;
   datelist->clear();
   while (ref->text != 0 ) {
     datelist->insertItem (i18n(ref->text));
     if (manager->dateItem() == ref->num)
       index = ref - date_itemlist;
     ref++;
   }
   datelist->setCurrentItem (index);

   // compare-functions

   index = 0;
   ct = type_complist;
   typecomp->clear();
   while (*ct != QueryManager::CompType(-1) ) {
     typecomp->insertItem (manager->compStr(*ct));
     if (manager->typeComp() == *ct)
       index = ct - type_complist;
     ct++;
   }
   typecomp->setCurrentItem (index);
   typelist->setEnabled(index != 0); // don`t care == 0

   index = 0;
   ct = query_complist;
   querycomp->clear();
   while (*ct != QueryManager::CompType(-1) ) {
     querycomp->insertItem (manager->compStr(*ct));
     if (manager->queryComp() == *ct)
       index = ct - query_complist;
     ct++;
   }
   querycomp->setCurrentItem (index);
   querylist->setEnabled(index != 0); // don`t care == 0

   index = 0;
   ct = bad_complist;
   badcomp->clear();
   while (*ct != QueryManager::CompType(-1) ) {
     badcomp->insertItem (manager->compStr(*ct));
     if (manager->badComp() == *ct)
       index = ct - bad_complist;
     ct++;
   }
   badcomp->setCurrentItem (index);
   badlist->setEnabled(index != 0); // don`t care == 0

   index = 0;
   ct = less_complist;
   lessoncomp->clear();
   while (*ct != QueryManager::CompType(-1) ) {
     lessoncomp->insertItem (manager->compStr(*ct));
     if (manager->lessonComp() == *ct)
       index = ct - less_complist;
     ct++;
   }
   lessoncomp->setCurrentItem (index);
   slotSetLessonComp(index);

   index = 0;
   ct = grade_complist;
   gradecomp->clear();
   while (*ct != QueryManager::CompType(-1) ) {
     gradecomp->insertItem (manager->compStr(*ct));
     if (manager->gradeComp() == *ct)
       index = ct - grade_complist;
     ct++;
   }
   gradecomp->setCurrentItem (index);
   gradelist->setEnabled(index != 0); // don`t care == 0

   index = 0;
   ct = date_complist;
   datecomp->clear();
   while (*ct != QueryManager::CompType(-1) ) {
     datecomp->insertItem (manager->compStr(*ct));
     if (manager->dateComp() == *ct)
       index = ct - date_complist;
     ct++;
   }
   datecomp->setCurrentItem (index);
   if (   date_complist[index] == QueryManager::Before
       || date_complist[index] == QueryManager::Within)
     datelist->setEnabled(true);
   else
     datelist->setEnabled(false);
}


void ThreshOptPage::initFocus() const
{
  lessonlist->setFocus();
}


void ThreshOptPage::slotSelectAll()
{
   for (int i = 0; i < (int) lessonlist->count(); i++)
     lessonlist->setSelected(i, true);

   slotSetLessonItems();
}


void ThreshOptPage::slotSelectNone()
{
   for (int i = 0; i < (int) lessonlist->count(); i++)
     lessonlist->setSelected(i, false);

   slotSetLessonItems();
}


void ThreshOptPage::slotSetLessonComp(int i)
{
   if (   less_complist[i] == QueryManager::OneOf
       || less_complist[i] == QueryManager::NotOneOf) {
     lessonlist->setEnabled(true);
     b_all_less->setEnabled(true);
     b_none_less->setEnabled(true);
     l_count->setEnabled(true);
     l_lesson->setEnabled(true);
   }
   else {
     l_count->setEnabled(false);
     lessonlist->setEnabled(false);
     b_all_less->setEnabled(false);
     b_none_less->setEnabled(false);
     l_lesson->setEnabled(false);
   }

   if (i < 0)
     return;

   manager->setLessonComp(less_complist[i]);

   emit modifySetting();
}


void ThreshOptPage::slotSetGradeComp(int i)
{
   manager->setGradeComp (grade_complist[i]);
   gradelist->setEnabled(i != 0); // don`t care == 0
   emit modifySetting();
}


void ThreshOptPage::slotSetTypeComp(int i)
{
   manager->setTypeComp (type_complist[i]);
   typelist->setEnabled(i != 0); // don`t care == 0
}


void ThreshOptPage::slotSetQueryComp(int i)
{
   manager->setQueryComp (query_complist[i]);
   querylist->setEnabled(i != 0); // don`t care == 0
   emit modifySetting();
}


void ThreshOptPage::slotSetBadComp(int i)
{
   manager->setBadComp (bad_complist[i]);
   badlist->setEnabled(i != 0); // don`t care == 0
   emit modifySetting();
}


void ThreshOptPage::slotSetDateComp(int i)
{
   manager->setDateComp (date_complist[i]);
   if (   date_complist[i] == QueryManager::Before
       || date_complist[i] == QueryManager::Within)
     datelist->setEnabled(true);
   else
     datelist->setEnabled(false);
   emit modifySetting();
}


void ThreshOptPage::slotSetTypeItem(int i)
{
   manager->setTypeItem (all_maintypes[i].shortStr() );
   emit modifySetting();
}


void ThreshOptPage::slotSetGradeItem(int i)
{
   manager->setGradeItem (i+1);
   emit modifySetting();
}


void ThreshOptPage::slotSetQueryItem(int i)
{
   manager->setQueryItem (query_itemlist[i].num);
   emit modifySetting();
}


void ThreshOptPage::slotSetDateItem(int i)
{
   manager->setDateItem (date_itemlist[i].num);
   emit modifySetting();
}


void ThreshOptPage::slotSetBadItem(int i)
{
   manager->setBadItem (bad_itemlist[i].num);
   emit modifySetting();
}


void ThreshOptPage::slotSetLessonItems()
{
   vector<int> sel;
   int cnt = 0;

   for (int i = 0; i < (int) lessonlist->count(); i++)  {
     if (lessonlist->isSelected(i)) {
        sel.push_back(i+1);   // 0 = not assigned lesson
        cnt++;
     }
   }

   QString s;
   s.setNum (cnt);
   l_count->setText (s);

   manager->setLessonItems(sel);
   emit modifySetting();
}


void ThreshOptPage::slotBlockExpire (bool block, bool expire)
{
  bool enable = ! (block | expire);

  gradecomp->setEnabled(enable);
  gradelist->setEnabled(enable);
  label_grade->setEnabled(enable);

  datecomp->setEnabled(enable);
  datelist->setEnabled(enable);
  label_qdate->setEnabled(enable);

  querycomp->setEnabled(enable);
  querylist->setEnabled(enable);
  label_qcount->setEnabled(enable);

  badcomp->setEnabled(enable);
  badlist->setEnabled(enable);
  label_bcount->setEnabled(enable);
}


void ThreshOptPage::keyPressEvent( QKeyEvent *e )
{
   if (e->state() & AltButton & ControlButton & ShiftButton == 0) {
     if (  e->key() == Key_Escape )
       emit reject();
     else if (  e->key() == Key_Enter
              ||e->key() == Key_Return)
       emit accept();
     else
       e->ignore();
   }
   else
     e->ignore();
}

#include "ThreshOptPage.moc"
