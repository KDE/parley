/***************************************************************************

                             manage queries

    -----------------------------------------------------------------------

    begin          : Sat Jul 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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

#include <klocale.h>
#include <kconfig.h>

#include <iostream>
#include <vector>
using namespace std;

#include "QueryManager.h"
#include "kvoctraincore.h"
#include <prefs.h>

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
  { QM_ADJ,                           I18N_NOOP("Adjective") },

  { QM_ADV,                           I18N_NOOP("Adverb") },

  { QM_ART,                           I18N_NOOP("Article") },
  { QM_ART QM_TYPE_DIV QM_ART_DEF,    I18N_NOOP("Article Definite") },
  { QM_ART QM_TYPE_DIV QM_ART_IND,    I18N_NOOP("Article Indefinite") },

  { QM_CON,                           I18N_NOOP("Conjunction") },

  { QM_NAME,                          I18N_NOOP("Name") },   // old type "3"

  { QM_NOUN,                          I18N_NOOP("Noun") },   // old type "2"
  { QM_NOUN QM_TYPE_DIV QM_NOUN_M,    I18N_NOOP("Noun Male") },
  { QM_NOUN QM_TYPE_DIV QM_NOUN_F,    I18N_NOOP("Noun Female") },
  { QM_NOUN QM_TYPE_DIV QM_NOUN_S,    I18N_NOOP("Noun Neutral") },

  { QM_NUM,                           I18N_NOOP("Numeral") },
  { QM_NUM QM_TYPE_DIV QM_NUM_ORD,    I18N_NOOP("Numeral Ordinal") },
  { QM_NUM QM_TYPE_DIV QM_NUM_CARD,   I18N_NOOP("Numeral Cardinal") },

  { QM_PHRASE,                        I18N_NOOP("Phrase") },

  { QM_PREP,                          I18N_NOOP("Preposition") },

  { QM_PRON,                          I18N_NOOP("Pronoun") },
  { QM_PRON QM_TYPE_DIV QM_PRON_POS,  I18N_NOOP("Pronoun Possessive") },
  { QM_PRON QM_TYPE_DIV QM_PRON_PER,  I18N_NOOP("Pronoun Personal") },

  { QM_QUEST,                         I18N_NOOP("Question") },

  { QM_VERB,                          I18N_NOOP("Verb") },   // old type "1"
  { QM_VERB  QM_TYPE_DIV QM_VERB_IRR, I18N_NOOP("Verb Irregular") },
  { QM_VERB  QM_TYPE_DIV QM_VERB_REG, I18N_NOOP("Verb Regular") },

  { 0, 0 }  // the end
};


QString QueryManager::getSubType (const QString & type)
{
  int i;
  QString t = type;
  if ((i = t.find(QM_TYPE_DIV)) >= 0) {
    t.remove(0, i+1);
    return t;
  }
  else
    return QString::null;
}


QString QueryManager::getMainType (const QString & type)
{
  int i;
  if ((i = type.find(QM_TYPE_DIV)) >= 0)
    return type.left(i);
  else
    return type;
}


QueryManager::QueryManager()
{
  lessonitems.clear();
}


QuerySelection QueryManager::select(kvoctrainDoc *doc, int act_lesson, int oindex, int tindex)
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

   //Note that Leitner style learning (altlearn) normally only uses 20
   //entries, we just ignore that here
   for (int i = 0; i < doc->numEntries(); i++) {
     ent_no++;
     if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
       emit doc->progressChanged(doc, int (ent_no / f_ent_percent));

     kvoctrainExpr *expr = doc->getEntry(i);
     unsigned int lessonno;
     if (Prefs::altLearn())
       lessonno = 0; //We only use a single array in Leitner style
     else
       lessonno = expr->getLesson();

     if (expr->isActive() ){
       if (Prefs::swapDirection()) {
         if (  validate (expr, act_lesson, oindex, tindex)
             || validate (expr, act_lesson, tindex, oindex)) {
           random[lessonno].push_back (QueryEntryRef(expr, i));
           expr->setInQuery(true);
         }
       }
       else {
         if (validate (expr, act_lesson, oindex, tindex)) {
           random[lessonno].push_back (QueryEntryRef(expr, i));
           expr->setInQuery(true);
         }
       }
     }
   }

   // remove empty lesson elements
   for (int i = (int) random.size()-1; i >= 0; i--)
     if (random[i].size() == 0)
       random.erase(random.begin() + i);
   return random;
}


bool QueryManager::validate(kvoctrainExpr *expr, int act_lesson, int oindex, int tindex)
{
   int index = tindex ? tindex : oindex;
   if ((compareExpiring(expr->getGrade(index, oindex != 0),
                        expr->getQueryDate(index, oindex != 0), Prefs::expire())
        ||

        (
            compareGrade ((CompType) Prefs::compType(Prefs::EnumType::Grade), expr->getGrade(index, oindex != 0), Prefs::gradeItem())
         && compareQuery ((CompType) Prefs::compType(Prefs::EnumType::Query), expr->getQueryCount(index, oindex != 0), Prefs::queryItem())
         && compareBad ((CompType) Prefs::compType(Prefs::EnumType::Bad), expr->getBadCount(index, oindex != 0), Prefs::badItem())
         && compareDate ((CompType) Prefs::compType(Prefs::EnumType::Date), expr->getQueryDate(index, oindex != 0), Prefs::dateItem())

         && compareBlocking(expr->getGrade(index, oindex != 0),
                            expr->getQueryDate(index, oindex != 0), Prefs::block())
        )
       )
//     lesson + word type must ALWAYS match
//     (and there must be a word on both sides)
       && compareLesson ((CompType) Prefs::compType(Prefs::EnumType::Lesson), expr->getLesson(), lessonitems, act_lesson)
       && compareType ((CompType) Prefs::compType(Prefs::EnumType::WordType), expr->getType(index), Prefs::typeItem())
       && !expr->getOriginal().stripWhiteSpace().isEmpty()
       && !expr->getTranslation(index).stripWhiteSpace().isEmpty()
      )
     return true;
   else
     return false;

}


QuerySelection QueryManager::select(kvoctrainDoc *doc, int act_lesson, int idx, QString type)
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
       emit doc->progressChanged(doc, int (ent_no / f_ent_percent));

     kvoctrainExpr *expr = doc->getEntry(i);
     if (expr->isActive() && validate (expr, act_lesson, idx, type)) {
       random[expr->getLesson()].push_back (QueryEntryRef(expr, i));
       expr->setInQuery(true);
     }
   }

   // remove empty lesson elements
   for (int i = (int) random.size()-1; i >= 0; i--)
     if (random[i].size() == 0)
       random.erase(random.begin() + i);

   return random;
}


bool QueryManager::validate(kvoctrainExpr *expr, int act_lesson, int idx, QString query_type)
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

   if (compareLesson ((CompType) Prefs::compType(Prefs::EnumType::Lesson), expr->getLesson(),
                      lessonitems, act_lesson)
       && type_ok) {
     return true;
     }
   else {
     return false;
   }
}


QuerySelection QueryManager::select(kvoctrainDoc *doc, int act_lesson, int idx, QueryType type)
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
       emit doc->progressChanged(doc, int (ent_no / f_ent_percent));

     kvoctrainExpr *expr = doc->getEntry(i);
     if (expr->isActive() && validate (expr, act_lesson, idx, type)) {
       random[expr->getLesson()].push_back (QueryEntryRef(expr, i));
       expr->setInQuery(true);
     }
   }

   // remove empty lesson elements
   for (int i = (int) random.size()-1; i >= 0; i--)
     if (random[i].size() == 0)
       random.erase(random.begin() + i);

   return random;
}


bool QueryManager::validate(kvoctrainExpr *expr, int act_lesson, int idx, QueryType query_type)
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

   if (compareLesson ((CompType) Prefs::compType(Prefs::EnumType::Lesson), expr->getLesson(),
                      lessonitems, act_lesson)
       && type_ok) {
     return true;
     }
   else {
     return false;
   }
}


QString QueryManager::compStr(Prefs::EnumCompType::type type)
{
   QString str = "???";
   switch (type)
   {
    case DontCare: str = i18n("Do not Care"); break;
    case WorseThan: str = i18n("Worse Than"); break;
    case WorseEqThan: str = i18n("Equal/Worse Than"); break;
    case MoreThan: str = i18n(">"); break;
    case MoreEqThan: str = i18n(">="); break;
    case BetterEqThan: str = i18n("Equal/Better Than"); break;
    case BetterThan: str = i18n("Better Than"); break;
    case LessEqThan: str = i18n("<="); break;
    case LessThan: str = i18n("<"); break;

    case EqualTo : str = i18n("Equal To"); break;
    case NotEqual: str = i18n("Not Equal"); break;

    case OneOf   : str = i18n("Contained In"); break;
    case NotOneOf: str = i18n("Not Contained In"); break;

    case Within    : str = i18n("Within Last"); break;
    case Before    : str = i18n("Before"); break;
    case NotQueried: str = i18n("Not Queried"); break;

    case Current    : return i18n("Current Lesson"); break;
    case NotAssigned: return i18n("Not Assigned"); break;
    default:
      ;
   }
   return str;
}


QString QueryManager::gradeStr (int i)
{
  switch (i) {
    case KV_NORM_GRADE:    return i18n(KV_NORM_TEXT); break;
    case KV_LEV1_GRADE:    return i18n(KV_LEV1_TEXT); break;
    case KV_LEV2_GRADE:    return i18n(KV_LEV2_TEXT); break;
    case KV_LEV3_GRADE:    return i18n(KV_LEV3_TEXT); break;
    case KV_LEV4_GRADE:    return i18n(KV_LEV4_TEXT); break;
    case KV_LEV5_GRADE:    return i18n(KV_LEV5_TEXT); break;
    case KV_LEV6_GRADE:    return i18n(KV_LEV6_TEXT); break;
    case KV_LEV7_GRADE:    return i18n(KV_LEV7_TEXT); break;
    default:               return i18n(KV_LEV1_TEXT); break;
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
      return QString::null;
  }
  else {
    t_type_rel *type = InternalTypeRelations;
    while (type->short_ref != 0) {
      if (type->short_ref == id)
        return i18n(type->long_ref);
      type++;
    }
  }
  return QString::null;
}


bool QueryManager::compareBlocking (int grade, int date, bool use_it)
{
   time_t cmp = Prefs::blockItem(grade);
   if (grade == KV_NORM_GRADE || cmp == 0 || !use_it) // don't care || all off
     return true;
   else {
     time_t now = time(0);
     return date+cmp < now;
   }
}


bool QueryManager::compareExpiring (int grade, int date, bool use_it)
{
   time_t cmp = Prefs::expireItem(grade);
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


bool QueryManager::compareType (CompType type, const QString & exprtype, const QString & limit)
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


void QueryManager::setLessonItemStr (const QString & indices)
{
  int pos;
  QString indices_copy = indices;
  lessonitems.clear();
  while ((pos = indices_copy.find(' ')) >= 0) {
     QString s = indices_copy.left(pos);
     indices_copy.remove(0, pos+1);
     lessonitems.push_back(s.toInt());
  }
  if (indices_copy.length() != 0) {
    lessonitems.push_back(indices_copy.toInt());
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

