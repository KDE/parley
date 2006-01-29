/***************************************************************************

                         maintain a kvoctrain expression

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#ifndef KVOCTRAINEXPR_H
#define KVOCTRAINEXPR_H

#define KV_MAX_GRADE       7
#define KV_MIN_GRADE       0

#define KV_NORM_GRADE      0       // not queried yet
#define KV_NORM_COLOR      Qt::black
#define KV_NORM_TEXT       I18N_NOOP("Not Queried Yet")

#define KV_LEV1_GRADE      1
#define KV_LEV1_TEXT       I18N_NOOP("Level 1")

#define KV_LEV2_GRADE      2
#define KV_LEV2_TEXT       I18N_NOOP("Level 2")

#define KV_LEV3_GRADE      3
#define KV_LEV3_TEXT       I18N_NOOP("Level 3")

#define KV_LEV4_GRADE      4
#define KV_LEV4_TEXT       I18N_NOOP("Level 4")

#define KV_LEV5_GRADE      5
#define KV_LEV5_TEXT       I18N_NOOP("Level 5")

#define KV_LEV6_GRADE      6
#define KV_LEV6_TEXT       I18N_NOOP("Level 6")

#define KV_LEV7_GRADE      7
#define KV_LEV7_TEXT       I18N_NOOP("Level 7")

#include <time.h>
#include <vector>
using namespace std;

#include <keduvocgrammar.h>
#include "keduvocmultiplechoice.h"

typedef signed char grade_t;
typedef unsigned short count_t;

/***************************************************************
  * This class contains one expression as original or in one
  * translations
  **************************************************************/

class kvoctrainExpr
{
 public:

  /** default constructor for an expression in different languages
   */
  ~kvoctrainExpr ();

  kvoctrainExpr ();

  kvoctrainExpr (QString &s, QString separator, int lesson = 0);

  /** Constructor for an expression in different languages
   *
   * @param expr             expression
   */
  kvoctrainExpr (QString &expr, int lesson = 0);

  /** adds a new translation of this entry
   * @param expr             translation
   * @param grade            grade of knowledge of this translation
   * @param rev_grade        dito, in opposite direction
   */
  void addTranslation (QString expr, grade_t grade=KV_NORM_GRADE,
                                     grade_t rev_grade=KV_NORM_GRADE);

  /** removes translation
   *
   * @param index            number of translation 1..x
   */
  void removeTranslation (int index);

  /** returns index of lesson (0 = none)
   */
  int getLesson () const;

  /** sets index of lesson (0 = none)
   */
  void setLesson (int l);

  /** returns original expression of this entry
   */
  QString getOriginal () const;

  /** sets original expression of this entry
   */
  void setOriginal (const QString & expr);

  /** returns number of max. translations of all expressions
   */
  int numTranslations() const;

  /** returns flag if entry is "selected" for queries
   */
  bool isInQuery() const {return inquery; }

  /** set entry "selected"
   */
  void setInQuery(bool flag = true) { inquery = flag; }

  /** returns flag if entry is activated for queries
   */
  bool isActive() const  {return active; }

  /** set entry active (enabled for queries)
   */
  void setActive(bool flag = true) { active = flag; }

  /** returns translation of this expression
   *
   * @param index            number of translation
   * @result                 expression or "" if no translation available
   */
  QString getTranslation (int index) const;

  /** sets translation of this expression
   *
   * @param index            number of translation
   * @param expr             expression of this index
   */
  void setTranslation (int index, const QString & expr);

  /** sets remark of this expression
   *
   * @param index            index of expression
   * @param expr             remark of this index
   */
  void setPronunce (int index, const QString & expr);

  /** returns pronunciation of this expression
   *
   * @param index            index of expression
   * @result                 pronunciation or "" if none available
   */
  QString getPronunce (int index) const;

  /** returns remarks of this expression
   *
   * @param index            index of expression
   * @result                 remark or "" if no remark available
   */
  QString getRemark (int index) const;

  /** sets remark of this expression
   *
   * @param index            index of expression
   * @param expr             remark of this index
   */
  void setRemark (int index, const QString & expr);


  /** sets false friend of this expression
   *
   * @param index            index of expression
   * @param expr             false friend of this index
   * @param rev_grade        dito, in opposite direction
   */
  void setFauxAmi (int index, const QString & expr, bool rev_ami = false);


  /** returns false friend of this expression
   *
   * @param index            index of expression
   * @param rev_grade        dito, in opposite direction
   * @result                 false friend or "" if no string available
   */
  QString getFauxAmi (int index, bool rev_ami = false) const;

  /** sets synonym this expression
   *
   * @param index            index of expression
   * @param expr             synonym of this index
   */
  void setSynonym (int index, const QString & expr);


  /** returns synonym of this expression
   *
   * @param index            index of expression
   * @result                 synonym or "" if no string available
   */
  QString getSynonym (int index) const;

  /** sets example this expression
   *
   * @param index            index of expression
   * @param expr             example of this index
   */
  void setExample (int index, const QString & expr);


  /** returns example of this expression
   *
   * @param index            index of expression
   * @result                 example or "" if no string available
   */
  QString getExample (int index) const;

  /** sets usage label this expression
   *
   * @param index            index of expression
   * @param usage            usage label of this index
   */
  void setUsageLabel (int index, const QString & usage);


  /** returns usage label of this expression
   *
   * @param index            index of expression
   * @result                 usage or "" if no string available
   */
  QString getUsageLabel (int index) const;

  /** sets paraphrase of this expression
   *
   * @param index            index of expression
   * @param usage            paraphrase of this index
   */
  void setParaphrase (int index, const QString & usage);


  /** returns paraphrase of this expression
   *
   * @param index            index of expression
   * @result                 paraphrase or "" if no string available
   */
  QString getParaphrase (int index) const;

  /** sets antonym this expression
   *
   * @param index            index of expression
   * @param expr             antonym of this index
   */
  void setAntonym (int index, const QString & expr);


  /** returns antonym of this expression
   *
   * @param index            index of expression
   * @result                 antonym or "" if no string available
   */
  QString getAntonym (int index) const;

  /** returns type of this expression
   *
   * @result                 type or "" if no type available
   */
  QString getType (int index) const;

  /** all langs have same type ?
   *
   * @result                 true if all have same type
   */
  bool uniqueType () const;

  /** sets type of this expression
   *
   * @param index            index of type
   * @param type             type of this expression ("" = none)
   */
  void setType (int index, const QString &type);

  /** returns grade of given translation as string
   *
   * @param index            index of expression
   * @param rev_grade        dito, in opposite direction
   * @result                 number of knowlegde: 0=known, x=numbers not knows
   */
  QString gradeStr (int index, bool rev_grade = false) const;

  /** sets grade of given translation
   *
   * @param index            index of translation
   * @param grade            number of knowlegde: 0=known, x=numbers not knows
   */
  void setGrade (int index, grade_t grade, bool rev_grade = false);

  /** returns grade of given translation as int
   *
   * @param index            index of translation
   * @param rev_grade        dito, in opposite direction
   * @result                 number of knowlegde: 0=known, x=numbers not knows
   */
  grade_t getGrade (int index, bool rev_grade = false) const;

  /** increments grade of given translation
   *
   * @param index            index of translation
   * @param rev_grade        dito, in opposite direction
   */
  void incGrade (int index, bool rev_grade = false);

  /** decrements grade of given translation
   *
   * @param index            index of translation
   * @param rev_grade        dito, in opposite direction
   */
  void decGrade (int index, bool rev_grade = false);

  /** returns last query date of given translation as int
   *
   * @param index            index of translation
   * @param rev_date         dito, in opposite direction
   */
  time_t getQueryDate (int index, bool rev_date = false) const;

  /** set last query date of given translation as int
   *
   * @param index            index of translation
   * @param rev_date         dito, in opposite direction
   */
  void setQueryDate (int index, time_t date, bool rev_date = false);

  /** returns conjugations if available
   *
   * @param index            index of translation
   */
  KEduVocConjugation getConjugation(int index) const;

  /** sets conjugations
   *
   * @param index            index of translation
   * @param con              conjugation block
   */
  void setConjugation(int index, const KEduVocConjugation &con);

  /** returns comparison if available
   *
   * @param index            index of translation
   */
  KEduVocComparison getComparison(int index) const;

  /** sets comparison
   *
   * @param index            index of translation
   * @param con              comparison block
   */
  void setComparison(int index, const KEduVocComparison &comp);

  /** returns multiple choice if available
   *
   * @param index            index of multiple choice
   */
  KEduVocMultipleChoice getMultipleChoice(int index) const;

  /** sets multiple choice
   *
   * @param index            index of translation
   * @param con              multiple choice block
   */
  void setMultipleChoice(int index, const KEduVocMultipleChoice &mc);

  /** returns query count of given translation as int
   *
   * @param index            index of translation
   * @param rev_count        dito, in opposite direction
   */
  count_t getQueryCount (int index, bool rev_count = false) const;

  /** set query count of given translation as int
   *
   * @param index            index of translation
   * @param rev_count        dito, in opposite direction
   */
  void setQueryCount (int index, count_t count, bool rev_count = false);

  /** returns bad query count of given translation as int
   *
   * @param index            index of translation
   * @param rev_count        dito, in opposite direction
   */
  count_t getBadCount (int index, bool rev_count = false) const;

  /** set bad query count of given translation as int
   *
   * @param index            index of translation
   * @param rev_count        dito, in opposite direction
   */
  void setBadCount (int index, count_t count, bool rev_count = false);

  /** increment bad query count of given translation by 1
   *
   * @param index            index of translation
   * @param rev_count        dito, in opposite direction
   */
  void incBadCount (int index, bool rev_count = false);

  /** increment query count of given translation by 1
   *
   * @param index            index of translation
   * @param rev_count        dito, in opposite direction
   */
  void incQueryCount (int index, bool rev_count = false);

 protected:

  void Init();

 private:
  QString            origin;

  // all these vectors must be deleted in removeTranslation()
  vector<QString>     exprtypes;
  vector<QString>     translations;
  vector<QString>     remarks;
  vector<QString>     usageLabels;
  vector<QString>     paraphrases;
  vector<QString>     fauxAmi;
  vector<QString>     rev_fauxAmi;
  vector<QString>     synonym;
  vector<QString>     example;
  vector<QString>     antonym;
  vector<QString>     pronunces;
  vector<grade_t>     grades;
  vector<grade_t>     rev_grades;
  vector<count_t>     qcounts;
  vector<count_t>     rev_qcounts;
  vector<count_t>     bcounts;
  vector<count_t>     rev_bcounts;
  vector<time_t>      qdates;
  vector<time_t>      rev_qdates;
  vector<KEduVocConjugation> conjugations;
  vector<KEduVocComparison>  comparisons;
  vector<KEduVocMultipleChoice> mcs;

  int                lesson;
  bool               inquery;
  bool               active;
};

#endif // KVOCTRAINEXPR_H

