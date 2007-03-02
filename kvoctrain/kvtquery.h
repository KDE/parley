/***************************************************************************

                            manage queries

    -----------------------------------------------------------------------

    begin         : Sat Jul 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef kvtquery_included
#define kvtquery_included

#include <QVector>

#include <keduvocexpression.h>
#include "prefs.h"

class KEduVocDocument;
class KConfig;
class KEduVocExpression;
class Prefs;
class EnumCompType;

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

class TypeRelation
{
public:
  TypeRelation (const QString & _short, const QString & _long) : shortId (_short), longId(_long) {}

  QString shortStr() const { return shortId; }
  QString longStr()  const { return longId;  }

protected:
  QString  shortId;
  QString longId;
};


struct QueryEntryRef {
  QueryEntryRef (KEduVocExpression *_exp, int _nr) : exp(_exp), nr(_nr) {}

  KEduVocExpression *exp;
  int nr;
};


typedef QVector<QList<QueryEntryRef> > QuerySelection;


class KVTQuery
{
public:

enum QueryType { RandomQuery,
                 MultipleChoiceQuery,
                 ArticlesQuery,
                 ConjugationQuery,
                 ComparisonQuery,
                 SynonymQuery,
                 AntonymQuery,
                 ExampleQuery,
                 ParaphraseQuery
               };

                // don't change the order/remove one of these,
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

  KVTQuery();

  static QList<TypeRelation> getRelation(bool only_maintypes);
  static void setTypeNames(QStringList names);
  static QString getSubType(const QString & type);
  static QString getMainType(const QString & type);

  static QString compStr(Prefs::EnumCompType::type type);
  static QString gradeStr(int i);
  static QString typeStr(const QString id);

  void setLessonItems(QList<int> indices) { lessonitems = indices; }
  void setLessonItemStr(const QString & indices);

  QList<int> lessonItems() const { return lessonitems; }
  QString lessonItemStr() const;

  bool validate(KEduVocExpression *expr, int act_lesson, int oindex, int tindex);

  QuerySelection select(KEduVocDocument*, int act_lesson, int oindex, int tindex);

  bool validate(KEduVocExpression *expr, int act_lesson, int index, QString type);

  QuerySelection select(KEduVocDocument*, int act_lesson, int index, QString type);

  bool validate(KEduVocExpression *expr, int act_lesson, int index, QueryType type);

  QuerySelection select(KEduVocDocument*, int act_lesson, int index, QueryType type);

protected:
  bool compareBlocking(int grade, QDateTime limit, bool use_it);
  bool compareExpiring(int grade, QDateTime limit, bool use_it);
  bool compareDate(CompType, QDateTime);
  bool compareQuery(CompType, int, int);
  bool compareBad(CompType, int, int);
  bool compareGrade(CompType, grade_t, grade_t);
  bool compareType(CompType, const QString &, const QString &);
  bool compareLesson(CompType type, int less, const QList<int> &limit, int current);

  QList<int> lessonitems;

private:
  static QStringList userTypes;
};

#endif // kvtquery_included
