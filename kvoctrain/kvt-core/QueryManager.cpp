/***************************************************************************

    $Id$

                             manage queries

    -----------------------------------------------------------------------

    begin                : Sat Jul 11 20:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.8  2001/12/29 10:40:24  arnold
    merged fixes from POST-branch

    Revision 1.7  2001/12/26 15:11:53  mueller
    CVSSILINT: fixincludes

    Revision 1.6  2001/12/01 11:28:34  arnold
    fixed flickering in query dialogs

    Revision 1.5  2001/11/25 11:11:23  arnold
    switch for inline edit, splitted kv_resource.h

    Revision 1.4  2001/11/16 18:53:21  arnold
    added possibility to disable expressions

    Revision 1.3  2001/11/10 22:28:46  arnold
    removed compatibility for kde1

    Revision 1.2  2001/11/09 14:19:13  arnold
    fixed and improved some dialog pages

    Revision 1.1  2001/10/05 15:42:01  arnold
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

#include "QueryManager.h"
#include "kvoctraincore.h"

#include <klocale.h>
#include <kapplication.h>
#include <kconfig.h>

#include <kvoctraindoc.h>

#include <iostream.h>
#include <vector>
using namespace std;

vector<QString> QueryManager::userTypes;

struct t_type_rel
{
   const char *short_ref;
   const char *long_ref;
};

// types are hierarchical !
// - . divides main from sub type
// - sub types must follow main type

// user types are strings-references like this: #1

static t_type_rel InternalTypeRelations [] =
{
  { QM_ADJ,                           I18N_NOOP("adjective") },

  { QM_ADV,                           I18N_NOOP("adverb") },

  { QM_ART,                           I18N_NOOP("article") },
  { QM_ART QM_TYPE_DIV QM_ART_DEF,    I18N_NOOP("article definite") },
  { QM_ART QM_TYPE_DIV QM_ART_IND,    I18N_NOOP("article indefinite") },

  { QM_CON,                           I18N_NOOP("conjunction") },

  { QM_NAME,                          I18N_NOOP("name") },   // old type "3"

  { QM_NOUN,                          I18N_NOOP("noun") },   // old type "2"
  { QM_NOUN QM_TYPE_DIV QM_NOUN_M,    I18N_NOOP("noun male") },
  { QM_NOUN QM_TYPE_DIV QM_NOUN_F,    I18N_NOOP("noun female") },
  { QM_NOUN QM_TYPE_DIV QM_NOUN_S,    I18N_NOOP("noun neutral") },

  { QM_NUM,                           I18N_NOOP("numeral") },
  { QM_NUM QM_TYPE_DIV QM_NUM_ORD,    I18N_NOOP("numeral ordinal") },
  { QM_NUM QM_TYPE_DIV QM_NUM_CARD,   I18N_NOOP("numeral cardinal") },

  { QM_PHRASE,                        I18N_NOOP("phrase") },

  { QM_PREP,                          I18N_NOOP("preposition") },

  { QM_PRON,                          I18N_NOOP("pronoun") },
  { QM_PRON QM_TYPE_DIV QM_PRON_POS,  I18N_NOOP("pronoun possessive") },
  { QM_PRON QM_TYPE_DIV QM_PRON_PER,  I18N_NOOP("pronoun personal") },

  { QM_QUEST,                         I18N_NOOP("question") },

  { QM_VERB,                          I18N_NOOP("verb") },   // old type "1"
  { QM_VERB  QM_TYPE_DIV QM_VERB_IRR, I18N_NOOP("verb irregular") },
  { QM_VERB  QM_TYPE_DIV QM_VERB_REG, I18N_NOOP("verb regular") },

  { 0, 0 }  // the end
};


QString QueryManager::getSubType (QString type)
{
  int i;
  if ((i = type.find(QM_TYPE_DIV)) >= 0) {
    type.remove(0, i+1);
    return type;
  }
  else
    return "";
}


QString QueryManager::getMainType (QString type)
{
  int i;
  if ((i = type.find(QM_TYPE_DIV)) >= 0)
    return type.left(i);
  else
    return type;
}


QueryManager::QueryManager ()
{
  datecomp = DontCare;
  badcomp = DontCare;
  querycomp = DontCare;
  gradecomp = DontCare;
  typecomp = DontCare;
  lessoncomp = DontCare;

  dateitem = 0;
  queryitem = 0;
  baditem = 0;
  typeitem = "";
  gradeitem = 0;
  lessonitems.clear();
}


void QueryManager::loadConfig (KConfig *config)
{
  datecomp = (CompType) config->readNumEntry(CFG_QM_DATE_COMP, DontCare);
  badcomp = (CompType) config->readNumEntry(CFG_QM_BAD_COMP, DontCare);
  querycomp = (CompType) config->readNumEntry(CFG_QM_QUERY_COMP, DontCare);
  gradecomp = (CompType) config->readNumEntry(CFG_QM_GRADE_COMP, DontCare);
  typecomp = (CompType) config->readNumEntry(CFG_QM_TYPE_COMP, DontCare);
  lessoncomp = (CompType) config->readNumEntry(CFG_QM_LESSON_COMP, Current);

  dateitem = (CompType) config->readNumEntry(CFG_QM_DATE_ITEM, 0);
  baditem = (CompType) config->readNumEntry(CFG_QM_BAD_ITEM, 0);
  queryitem = (CompType) config->readNumEntry(CFG_QM_QUERY_ITEM, 0);
  gradeitem = (CompType) config->readNumEntry(CFG_QM_GRADE_ITEM, 0);
  typeitem = config->readEntry(CFG_QM_TYPE_ITEM, "");
//  setLessonItemStr (config->readEntry(CFG_QM_LESSON_ITEM, ""));

  blockItems.clear();
  blockItems.push_back(config->readNumEntry(CFG_QM_BLOCK_ITEM"1", 1 *      60*60*24));
  blockItems.push_back(config->readNumEntry(CFG_QM_BLOCK_ITEM"2", 2 *      60*60*24));
  blockItems.push_back(config->readNumEntry(CFG_QM_BLOCK_ITEM"3", 4 *      60*60*24));
  blockItems.push_back(config->readNumEntry(CFG_QM_BLOCK_ITEM"4", 1 *    7*60*60*24));
  blockItems.push_back(config->readNumEntry(CFG_QM_BLOCK_ITEM"5", 2 *    7*60*60*24));
  blockItems.push_back(config->readNumEntry(CFG_QM_BLOCK_ITEM"6", 1 * 30*7*60*60*24));
  blockItems.push_back(config->readNumEntry(CFG_QM_BLOCK_ITEM"7", 2 * 30*7*60*60*24));

  expireItems.clear();
  expireItems.push_back(config->readNumEntry(CFG_QM_EXPIRE_ITEM"1", 2 *      60*60*24));
  expireItems.push_back(config->readNumEntry(CFG_QM_EXPIRE_ITEM"2", 4 *      60*60*24));
  expireItems.push_back(config->readNumEntry(CFG_QM_EXPIRE_ITEM"3", 1 *    7*60*60*24));
  expireItems.push_back(config->readNumEntry(CFG_QM_EXPIRE_ITEM"4", 2 *    7*60*60*24));
  expireItems.push_back(config->readNumEntry(CFG_QM_EXPIRE_ITEM"5", 1 * 30*7*60*60*24));
  expireItems.push_back(config->readNumEntry(CFG_QM_EXPIRE_ITEM"6", 2 * 30*7*60*60*24));
  expireItems.push_back(config->readNumEntry(CFG_QM_EXPIRE_ITEM"7", 4 * 30*7*60*60*24));
}


void QueryManager::saveConfig (KConfig *config)
{
  config->writeEntry(CFG_QM_DATE_COMP, (int) datecomp);
  config->writeEntry(CFG_QM_BAD_COMP, (int) badcomp);
  config->writeEntry(CFG_QM_QUERY_COMP, (int) querycomp);
  config->writeEntry(CFG_QM_GRADE_COMP, (int) gradecomp);
  config->writeEntry(CFG_QM_TYPE_COMP, (int) typecomp);
  config->writeEntry(CFG_QM_LESSON_COMP, (int) lessoncomp);

  config->writeEntry(CFG_QM_DATE_ITEM, (int) dateitem);
  config->writeEntry(CFG_QM_BAD_ITEM, (int) baditem);
  config->writeEntry(CFG_QM_QUERY_ITEM, (int) queryitem);
  config->writeEntry(CFG_QM_GRADE_ITEM, (int) gradeitem);
  config->writeEntry(CFG_QM_TYPE_ITEM, typeitem);
//  config->writeEntry(CFG_QM_LESSON_ITEM, lessonItemStr());

  QString s;
  for (int i = KV_LEV1_GRADE;
       i <= KV_MAX_GRADE && i <= (int) blockItems.size(); i++) {
    s.setNum(i);
    s.insert(0, CFG_QM_BLOCK_ITEM);
    config->writeEntry(s, blockItems[i-1]);
  }

  for (int i = KV_LEV1_GRADE;
       i <= KV_MAX_GRADE && i <= (int) expireItems.size(); i++) {
    s.setNum(i);
    s.insert(0, CFG_QM_EXPIRE_ITEM);
    config->writeEntry(s, expireItems[i-1]);
  }
}


int QueryManager::blockItem (int grade) const
{
   if (grade <= (int) blockItems.size() && grade > 0)
    return  blockItems[grade-1];
   else
    return 0;
}


int QueryManager::expireItem (int grade) const
{
   if (grade <= (int) expireItems.size() && grade > 0)
    return expireItems[grade-1];
   else
    return 0;
}


void QueryManager::setBlockItem (int item, int grade)
{
   if (grade > KV_MAX_GRADE || grade <= 0)
     return;

   for (int i = (int) blockItems.size(); i <= grade; i++)
     blockItems.push_back(0);
   blockItems[grade-1] = item;
}


void QueryManager::setExpireItem (int item, int grade)
{
   if (grade > KV_MAX_GRADE || grade <= 0)
     return;

   for (int i = (int) expireItems.size(); i <= grade; i++)
     expireItems.push_back(0);
   expireItems[grade-1] = item;
}


QuerySelection QueryManager::select(kvoctrainDoc *doc, int act_lesson,
                                    int oindex, int tindex,
                                    bool swap, bool block, bool expire)
{
   QuerySelection random;
   random.resize(doc->numLessons()+1);
   for (int i = 0; i < doc->numEntries(); i++)
     doc->getEntry(i)->setInQuery(false);

   // selecting might take rather long
   int ent_no = 0;
   int ent_percent = doc->numEntries() / 100;
   float f_ent_percent = doc->numEntries() / 100.0;
   emit doc->progressChanged(doc, 0);

   for (int i = 0; i < doc->numEntries(); i++) {
     ent_no++;
     if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
       emit doc->progressChanged(doc, ent_no / f_ent_percent);

     kvoctrainExpr *expr = doc->getEntry(i);
     if (expr->isActive() ){
       if (swap) {
         if (  validate (expr, act_lesson,
                       oindex, tindex,
                       block, expire)
             ||validate (expr, act_lesson,
                       tindex, oindex,
                       block, expire)) {
           random[expr->getLesson()].push_back (QueryEntryRef(expr, i));
           expr->setInQuery(true);
         }
       }
       else {
         if (validate (expr, act_lesson,
                       oindex, tindex,
                       block, expire)) {
           random[expr->getLesson()].push_back (QueryEntryRef(expr, i));
           expr->setInQuery(true);
         }
       }
     }
   }

   // remove empty lesson elements
   for (int i = (int) random.size()-1; i >= 0; i--)
     if (random[i].size() == 0)
       random.erase(&random[i], &random[i+1]);
   return random;
}


bool QueryManager::validate(kvoctrainExpr *expr, int act_lesson,
                            int oindex, int tindex,
                            bool block, bool expire)
{
   int index = tindex ? tindex : oindex;
   if ((compareExpiring(expr->getGrade(index, oindex != 0),
                        expr->getQueryDate(index, oindex != 0), expire)
        ||

        (
            compareGrade (gradecomp, expr->getGrade(index, oindex != 0), gradeitem)
         && compareQuery (querycomp, expr->getQueryCount(index, oindex != 0), queryitem)
         && compareBad (badcomp, expr->getBadCount(index, oindex != 0), baditem)
         && compareDate (datecomp, expr->getQueryDate(index, oindex != 0), dateitem)

         && compareBlocking(expr->getGrade(index, oindex != 0),
                            expr->getQueryDate(index, oindex != 0), block)
        )
       )
//     lesson + word type must ALWAYS match
//     (and there must be a word on both sides)
       && compareLesson (lessoncomp, expr->getLesson(), lessonitems, act_lesson)
       && compareType (typecomp, expr->getType(index), typeitem)
       && !expr->getOriginal().stripWhiteSpace().isEmpty()
       && !expr->getTranslation(index).stripWhiteSpace().isEmpty()
      )
     return true;
   else
     return false;

}


QuerySelection QueryManager::select(kvoctrainDoc *doc, int act_lesson,
                                    int idx, QString type)
{
   QuerySelection random;
   random.resize(doc->numLessons()+1);
   for (int i = 0; i < doc->numEntries(); i++)
     doc->getEntry(i)->setInQuery(false);

   // selecting might take rather long
   int ent_no = 0;
   int ent_percent = doc->numEntries() / 100;
   float f_ent_percent = doc->numEntries() / 100.0;
   emit doc->progressChanged(doc, 0);

   for (int i = 0; i < doc->numEntries(); i++) {
     ent_no++;
     if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
       emit doc->progressChanged(doc, ent_no / f_ent_percent);

     kvoctrainExpr *expr = doc->getEntry(i);
     if (expr->isActive() && validate (expr, act_lesson, idx, type)) {
       random[expr->getLesson()].push_back (QueryEntryRef(expr, i));
       expr->setInQuery(true);
     }
   }

   // remove empty lesson elements
   for (int i = (int) random.size()-1; i >= 0; i--)
     if (random[i].size() == 0)
       random.erase(&random[i], &random[i+1]);

   return random;
}


bool QueryManager::validate(kvoctrainExpr *expr, int act_lesson,
                            int idx, QString query_type)
{
   QString qtype;
   int pos = query_type.find (QM_TYPE_DIV);
   if (pos >= 0)
     qtype = query_type.left (pos);
   else
     qtype = query_type;

   QString expr_type = expr->getType(idx);
   bool type_ok = false;
   if (qtype == QM_NOUN) {
     type_ok =    expr_type == QM_NOUN  QM_TYPE_DIV  QM_NOUN_S
               || expr_type == QM_NOUN  QM_TYPE_DIV  QM_NOUN_M
               || expr_type == QM_NOUN  QM_TYPE_DIV  QM_NOUN_F;

   }
   else if (qtype == QM_VERB) {
     type_ok = (   expr_type == QM_VERB
                || expr_type == QM_VERB  QM_TYPE_DIV  QM_VERB_IRR
                || expr_type == QM_VERB  QM_TYPE_DIV  QM_VERB_REG
               )
               && expr->getConjugation(idx).numEntries() > 0;

   }
   else if (qtype == QM_ADJ) {
     type_ok =    expr_type == QM_ADJ
               && !expr->getComparison(idx).isEmpty();
   }
   else
     return false;

   if (compareLesson (lessoncomp, expr->getLesson(),
                      lessonitems, act_lesson)
       && type_ok) {
     return true;
     }
   else {
     return false;
   }
}


QuerySelection QueryManager::select(kvoctrainDoc *doc, int act_lesson,
                                    int idx, QueryType type)
{
   QuerySelection random;
   random.resize(doc->numLessons()+1);
   for (int i = 0; i < doc->numEntries(); i++)
     doc->getEntry(i)->setInQuery(false);

   // selecting might take rather long
   int ent_no = 0;
   int ent_percent = doc->numEntries() / 100;
   float f_ent_percent = doc->numEntries() / 100.0;
   emit doc->progressChanged(doc, 0);

   for (int i = 0; i < doc->numEntries(); i++) {
     ent_no++;
     if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
       emit doc->progressChanged(doc, ent_no / f_ent_percent);

     kvoctrainExpr *expr = doc->getEntry(i);
     if (expr->isActive() && validate (expr, act_lesson, idx, type)) {
       random[expr->getLesson()].push_back (QueryEntryRef(expr, i));
       expr->setInQuery(true);
     }
   }

   // remove empty lesson elements
   for (int i = (int) random.size()-1; i >= 0; i--)
     if (random[i].size() == 0)
       random.erase(&random[i], &random[i+1]);

   return random;
}


bool QueryManager::validate(kvoctrainExpr *expr, int act_lesson,
                            int idx, QueryType query_type)
{
   bool type_ok = false;
   if (query_type == QT_Synonym) {
     type_ok = !expr->getSynonym(idx).stripWhiteSpace().isEmpty();
   }
   else if (query_type == QT_Antonym) {
     type_ok = !expr->getAntonym(idx).stripWhiteSpace().isEmpty();
   }
   else if (query_type == QT_Paraphrase) {
     type_ok = !expr->getParaphrase(idx).stripWhiteSpace().isEmpty();
   }
   else if (query_type == QT_Example) {
     type_ok = !expr->getExample(idx).stripWhiteSpace().isEmpty();
   }

   if (compareLesson (lessoncomp, expr->getLesson(),
                      lessonitems, act_lesson)
       && type_ok) {
     return true;
     }
   else {
     return false;
   }
}


QString QueryManager::compStr (CompType type)
{
   QString str = "???";
   switch (type) {
    case DontCare: str = i18n("don`t care"); break;
    case WorseThan: str = i18n("worse than"); break;
    case WorseEqThan: str = i18n("equal/worse than"); break;
    case MoreThan: str = i18n(">"); break;
    case MoreEqThan: str = i18n(">="); break;
    case BetterEqThan: str = i18n("equal/better than"); break;
    case BetterThan: str = i18n("better than"); break;
    case LessEqThan: str = i18n("<="); break;
    case LessThan: str = i18n("<"); break;

    case EqualTo : str = i18n("equal to"); break;
    case NotEqual: str = i18n("not equal"); break;

    case OneOf   : str = i18n("contained in"); break;
    case NotOneOf: str = i18n("not contained in"); break;

    case Within    : str = i18n("within last"); break;
    case Before    : str = i18n("before"); break;
    case NotQueried: str = i18n("not queried"); break;

    case Current    : return i18n("current lesson"); break;
    case NotAssigned: return i18n("not assigned"); break;
    default:
      ;
   }
   return str;
}


QString QueryManager::gradeStr (int i)
{
  switch (i) {
    case KV_NORM_GRADE:    return KV_NORM_TEXT; break;
    case KV_LEV1_GRADE:    return KV_LEV1_TEXT; break;
    case KV_LEV2_GRADE:    return KV_LEV2_TEXT; break;
    case KV_LEV3_GRADE:    return KV_LEV3_TEXT; break;
    case KV_LEV4_GRADE:    return KV_LEV4_TEXT; break;
    case KV_LEV5_GRADE:    return KV_LEV5_TEXT; break;
    case KV_LEV6_GRADE:    return KV_LEV6_TEXT; break;
    case KV_LEV7_GRADE:    return KV_LEV7_TEXT; break;
    default:               return KV_LEV1_TEXT; break;
  }
}


vector<TypeRelation> QueryManager::getRelation (bool only_maintypes)
{
  vector<TypeRelation> vec;
  for (int i = 0; i < (int) userTypes.size(); i++) {
    QString s;
    s.setNum(i+1);
    s.insert(0, QM_USER_TYPE);
    vec.push_back(TypeRelation(s, userTypes[i]));
  }


  t_type_rel *type = InternalTypeRelations;
  while (type->short_ref != 0) {
    if (!only_maintypes || strstr (type->short_ref, QM_TYPE_DIV) == 0)
      vec.push_back(TypeRelation(type->short_ref, i18n(type->long_ref)));
    type++;
  }

  return vec;
}


QString QueryManager::typeStr (const QString id)
{
  if (id.left(1) == QM_USER_TYPE) {
    QString num = id;
    num.remove (0, 1);
    int i = num.toInt()-1;
    if (i >= 0 && i < (int) userTypes.size() )
      return userTypes[i];
    else
      return "";
  }
  else {
    t_type_rel *type = InternalTypeRelations;
    while (type->short_ref != 0) {
      if (type->short_ref == id)
        return i18n(type->long_ref);
      type++;
    }
  }
  return "";
}


bool QueryManager::compareBlocking (int grade, int date, bool use_it)
{
   time_t cmp = blockItem(grade);
   if (grade == KV_NORM_GRADE || cmp == 0 || !use_it) // don't care || all off
     return true;
   else {
     time_t now = time(0);
     return date+cmp < now;
   }
}


bool QueryManager::compareExpiring (int grade, int date, bool use_it)
{
   time_t cmp = expireItem(grade);
   if (grade == KV_NORM_GRADE || cmp == 0 || !use_it) // don't care || all off
     return false;
   else {
     time_t now = time(0);
     return date+cmp < now;
   }
}


bool QueryManager::compareDate (CompType type, time_t qd, time_t limit)
{
   time_t now = time(0);
   bool erg = true;
   switch (type) {
    case DontCare: erg = true;
    break;

    case Before: erg = qd == 0 || qd < now-limit; // never queried or older date
    break;

    case Within: erg = qd >= now-limit;           // newer date
    break;

    case NotQueried: erg = qd == 0;    
    break;

    default:
      ;
   }
   return erg;
}


bool QueryManager::compareQuery (CompType type, int qgrade, int limit)
{
   bool erg = true;
   switch (type) {
    case DontCare: erg = true;
    break;

    case MoreThan: erg = qgrade > limit;    // sel has higher query count
    break;

    case MoreEqThan: erg = qgrade >= limit;
    break;

    case EqualTo: erg = qgrade == limit;
    break;

    case NotEqual: erg = qgrade != limit;
    break;

    case LessEqThan: erg = qgrade <= limit;   // sel has less count
    break;

    case LessThan: erg = qgrade < limit;   
    break;

    default:
      ;
   }
   return erg;
}


bool QueryManager::compareBad (CompType type, int bcount, int limit)
{
   bool erg = true;
   switch (type) {
    case DontCare: erg = true;
    break;

    case MoreThan: erg = bcount > limit;    // sel has higher bad count
    break;

    case MoreEqThan: erg = bcount >= limit; 
    break;

    case EqualTo: erg = bcount == limit;
    break;

    case NotEqual: erg = bcount != limit;
    break;

    case LessEqThan: erg = bcount <= limit;   // sel has less count
    break;

    case LessThan: erg = bcount < limit; 
    break;

    default:
      ;
   }
   return erg;
}


bool QueryManager::compareGrade (CompType type, grade_t qgrade, grade_t limit)
{
   bool erg = true;
   switch (type) {
    case DontCare: erg = true;
    break;

    case WorseThan: erg = qgrade < limit;    // sel has worse grade
    break;

    case WorseEqThan: erg = qgrade <= limit;
    break;

    case EqualTo: erg = qgrade == limit;
    break;

    case NotEqual: erg = qgrade != limit;
    break;

    case BetterEqThan: erg = qgrade >= limit;   // sel has better grade
    break;

    case BetterThan: erg = qgrade > limit;
    break;

    default:
      ;
   }
   return erg;
}


bool QueryManager::compareType (CompType type, QString exprtype, QString limit)
{
   bool erg = true;
   switch (type) {
    case DontCare: erg = true;
    break;

    case EqualTo: erg = getMainType(exprtype) == getMainType(limit);       // type is same
    break;

    case NotEqual: erg = getMainType(exprtype) != getMainType(limit);      // other type
    break;

    default:
      ;
   }
   return erg;
}


bool QueryManager::compareLesson (CompType type, int less, const vector<int> &limit, int current)
{
   bool erg = true;
   switch (type) {
    case DontCare:
      erg = true;
    break;

    case OneOf:
    {
      erg = false;
      for (int i = 0; !erg && i < (int) limit.size(); i++)
        if (limit[i] == less)
          erg = true;
    }
    break;

    case NotOneOf:
    {
      erg = true;
      for (int i = 0; erg && i < (int) limit.size(); i++)
        if (limit[i] == less)
          erg = false;
    }
    break;

    case Current:
      erg = less == current;
    break;

    case NotAssigned:
      erg = less == 0;
    break;

    default:
      ;
   }
   return erg;
}


void QueryManager::setTypeNames (vector<QString> names)
{
  userTypes = names;
}


void QueryManager::setLessonItemStr (QString indices)
{
  int pos;
  lessonitems.clear();
  while ((pos = indices.find(' ')) >= 0) {
     QString s = indices.left(pos);
     indices.remove(0, pos+1);
     lessonitems.push_back(s.toInt());
  }
  if (indices.length() != 0) {
    lessonitems.push_back(indices.toInt());
  }
}


QString QueryManager::lessonItemStr() const
{
   QString s, ret;
   for (int i = 0; i < (int) lessonitems.size(); i++) {
     s.setNum (lessonitems[i]);
     if (i != 0)
       ret += ' ';
     ret += s;
   }
   return ret;
}

