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

#ifndef QueryManager_included
#define QueryManager_included

#include <vector>
using namespace std;

#include "kvoctraindoc.h"

class kvoctrainDoc;
class KConfig;
class kvoctrainExpr;

// internal types, indented are subtypes

#define QM_VERB           "v"    // go
#define   QM_VERB_IRR     "ir"
#define   QM_VERB_REG     "re"
#define QM_NOUN           "n"    // table, coffee
#define   QM_NOUN_F       "f"
#define   QM_NOUN_M       "m"
#define   QM_NOUN_S       "s"
#define QM_NAME           "nm"
#define QM_ART            "ar"   // article
#define   QM_ART_DEF      "def"  // definite    a/an
#define   QM_ART_IND      "ind"  // indefinite  the
#define QM_ADJ            "aj"   // adjective   expensive, good
#define QM_ADV            "av"   // adverb      today, strongly
#define QM_PRON           "pr"   // pronoun     you, she
#define   QM_PRON_POS     "pos"  // possessive  my, your
#define   QM_PRON_PER     "per"  // personal
#define QM_PHRASE         "ph"
#define QM_NUM            "num"  // numeral
#define   QM_NUM_ORD      "ord"  // ordinal     first, second
#define   QM_NUM_CARD     "crd"  // cardinal    one, two
#define QM_INFORMAL       "ifm"
#define QM_FIG            "fig"
#define QM_CON            "con"  // conjuncton  and, but
#define QM_PREP           "pre"  // preposition behind, between
#define QM_QUEST          "qu"   // question    who, what

// type delimiters

#define QM_USER_TYPE  "#"   // designates number of user type
#define QM_TYPE_DIV   ":"   // divide main from subtype

//enum kvq_timeout_t { kvq_notimeout, kvq_show, kvq_cont };

enum QueryType  { QT_Random, QT_Multiple,
                  QT_Articles, QT_Conjugation, QT_Comparison,
                  QT_Synonym, QT_Antonym, QT_Example, QT_Paraphrase };


class TypeRelation
{
 public:

  TypeRelation (const QString & _short, const QString & _long)
    : shortId (_short), longId(_long) {}

  QString shortStr() const { return shortId; }
  QString longStr()  const { return longId;  }

 protected:

  QString  shortId, longId;
};


struct QueryEntryRef {
 QueryEntryRef (kvoctrainExpr *_exp,
                int            _nr)
     : exp(_exp), nr(_nr) {}


   kvoctrainExpr *exp;
   int            nr;
};

typedef vector<vector<QueryEntryRef> > QuerySelection;

class QueryManager
{
 public:
                // don´t change the order/remove one of these,
                // just APPEND new types
 enum CompType { DontCare,
                  MoreEqThan, MoreThan,
                  Before, Within,
                  WorseThan, WorseEqThan,
                  EqualTo, NotEqual,
                  LessEqThan, LessThan,
                  BetterThan, BetterEqThan,
                  Current, NotAssigned, NotQueried,
                  Auto_Time, Auto_Count,
                  OneOf, NotOneOf
                };

  QueryManager ();

  //void loadConfig (KConfig *);
  //void saveConfig (KConfig *);

  static vector<TypeRelation> getRelation (bool only_maintypes);
  static void setTypeNames (vector<QString> names);
  static QString getSubType (const QString & type);
  static QString getMainType (const QString & type);

  static QString compStr (CompType type);
  static QString gradeStr (int i);
  static QString typeStr (const QString id);

  void setLessonItems (vector<int> indices) { lessonitems = indices; }
  void setLessonItemStr (const QString & indices);
  //void setDateItem (time_t time) { dateitem = time; }
  //void setQueryItem (int query) { queryitem = query; }
  //void setBadItem (int bad) { baditem = bad; }
  //void setTypeItem (const QString & type) { typeitem = type; }
  //void setGradeItem (grade_t grade) { gradeitem = grade; }
  //void setBlockItem (int item, int grade);
  //void setExpireItem (int item, int grade);

  vector<int> lessonItems() const { return lessonitems; }
  QString lessonItemStr() const;
  //time_t dateItem () const { return dateitem; }
  //int queryItem () const { return queryitem; }
  //int badItem () const { return baditem; }
  //QString typeItem () const { return typeitem; }
  //grade_t gradeItem () const { return gradeitem; }
  //int blockItem (int grade) const;
  //int expireItem (int grade) const;

  //void setDateComp (CompType time) { datecomp = time; }
  //void setLessonComp (CompType less) { lessoncomp = less; }
  //void setQueryComp (CompType query) { querycomp = query; }
  //void setBadComp (CompType bad) { badcomp = bad; }
  //void setTypeComp (CompType type) { typecomp = type; }
  //void setGradeComp (CompType grade) { gradecomp = grade; }

  //CompType dateComp () const { return datecomp; }
  //CompType lessonComp () const { return lessoncomp; }
  //CompType queryComp () const { return querycomp; }
  //CompType badComp () const { return badcomp; }
  //CompType typeComp () const { return typecomp; }
  //CompType gradeComp () const { return gradecomp; }

  bool validate(kvoctrainExpr *expr, int act_lesson,
                int oindex, int tindex,
                bool block, bool expire);

  QuerySelection select(kvoctrainDoc*, int act_lesson,
                        int oindex, int tindex,
                        bool swap, bool altlearn, bool block, bool expire);

  bool validate(kvoctrainExpr *expr, int act_lesson,
                int index, QString type);

  QuerySelection select(kvoctrainDoc*, int act_lesson,
                        int index, QString type);

  bool validate(kvoctrainExpr *expr, int act_lesson,
                int index, QueryType type);

  QuerySelection select(kvoctrainDoc*, int act_lesson,
                        int index, QueryType type);

 protected:

  bool compareBlocking (int grade, int limit, bool use_it);
  bool compareExpiring (int grade, int limit, bool use_it);
  bool compareDate (CompType, time_t, time_t);
  bool compareQuery (CompType, int, int);
  bool compareBad (CompType, int, int);
  bool compareGrade (CompType, grade_t, grade_t);
  bool compareType (CompType, const QString &, const QString &);
  bool compareLesson (CompType type, int less, const vector<int> &limit, int current);

 /* CompType    typecomp,
              querycomp,
              badcomp,
              gradecomp,
              lessoncomp,
              datecomp;*/

  //time_t      dateitem;
  //int         queryitem,
  //            baditem;
  //QString     typeitem;
  //grade_t     gradeitem;
  vector<int> lessonitems;

  //vector<int> blockItems,
  //            expireItems;

 private:
  static vector<QString> userTypes;
};

#endif // QueryManager_included
