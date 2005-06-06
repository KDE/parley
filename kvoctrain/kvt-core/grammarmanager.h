/***************************************************************************

              manage grammer parts (articles, conjugation)

    -----------------------------------------------------------------------

    begin                : Sat Nov 27 09:50:53 MET 1999

    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

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


#ifndef grammarmanager_included
#define grammarmanager_included

#include <qstring.h>

#include <vector>
using namespace std;

#define CONJ_SIMPLE_PRESENT    "PrSi" // I live at home  what you frequently do
#define CONJ_PRESENT_PROGR     "PrPr" // I am working    what you currently are doing
#define CONJ_PRESENT_PERFECT   "PrPe" // I have cleaned  tell, #that# something has happened

#define CONJ_SIMPLE_PAST       "PaSi" // the train left 2 min ago  when did it happen
#define CONJ_PAST_PROGR        "PaPr" // it was raining  what happen at a given time in the past
#define CONJ_PAST_PARTICIPLE   "PaPa" // I cleaned       tell, #that# it happened

#define CONJ_FUTURE            "FuSi"

#define CONJ_PREFIX            "--"   // definition of prefixes (I, you, ..)

#define UL_USER_TENSE  "#"   // designates number of user tense

class Article
{

public:

   Article() {}

   Article (
           const QString &fem_def, const QString &fem_indef,
           const QString &mal_def, const QString &mal_indef,
           const QString &nat_def, const QString &nat_indef
          );

   void setFemale  (const QString &def, const QString &indef);
   void setMale    (const QString &def, const QString &indef);
   void setNatural (const QString &def, const QString &indef);

   void female  (QString &def, QString &indef) const;
   void male    (QString &def, QString &indef) const;
   void natural (QString &def, QString &indef) const;

protected:

   QString  fem_def, fem_indef,
            mal_def, mal_indef,
            nat_def, nat_indef;
};


class Comparison
{

public:

   Comparison() {}

   Comparison (
           const QString &l1,
           const QString &l2,
           const QString &l3
          );

   void setL1 (const QString &s) { ls1 = s; }
   void setL2 (const QString &s) { ls2 = s; }
   void setL3 (const QString &s) { ls3 = s; }

   QString l1 () const { return ls1; }
   QString l2 () const { return ls2; }
   QString l3 () const { return ls3; }

   bool isEmpty() const;
   void clear();

protected:

   QString  ls1, ls2, ls3;
};


class TenseRelation
{
 public:

  TenseRelation (const QString & _short, const QString & _long)
    : shortId (_short), longId(_long) {}

  inline QString shortStr() const { return shortId; }
  inline QString longStr()  const { return longId;  }

 protected:

  QString  shortId, longId;
};


class Conjugation
{

public:

   Conjugation () {}

   int numEntries() const;

   static vector<TenseRelation> getRelation ();
   static void setTenseNames (vector<QString> names);

   static QString getName (const QString &abbrev);
   static QString getName (int index);
   static QString getAbbrev (const QString &name);
   static QString getAbbrev (int index);
   static int numInternalNames();
   static int numTenses();

   QString getType (int index);
   void setType (int index, const QString & type);
   void cleanUp();
   bool isEmpty (int idx);

   QString pers1Singular(const QString &type) const;
   QString pers2Singular(const QString &type) const;
   bool    pers3SingularCommon(const QString &type) const;
   QString pers3FemaleSingular(const QString &type) const;
   QString pers3MaleSingular(const QString &type) const;
   QString pers3NaturalSingular(const QString &type) const;

   QString pers1Plural(const QString &type) const;
   QString pers2Plural(const QString &type) const;
   bool    pers3PluralCommon(const QString &type) const;
   QString pers3FemalePlural(const QString &type) const;
   QString pers3MalePlural(const QString &type) const;
   QString pers3NaturalPlural(const QString &type) const;

   void setPers1Singular(const QString &type, const QString &str);
   void setPers2Singular(const QString &type, const QString &str);
   void setPers3SingularCommon(const QString &type, bool f);
   void setPers3FemaleSingular(const QString &type, const QString &str);
   void setPers3MaleSingular(const QString &type, const QString &str);
   void setPers3NaturalSingular(const QString &type, const QString &str);

   void setPers1Plural(const QString &type, const QString &str);
   void setPers2Plural(const QString &type, const QString &str);
   void setPers3PluralCommon(const QString &type, bool f);
   void setPers3FemalePlural(const QString &type, const QString &str);
   void setPers3MalePlural(const QString &type, const QString &str);
   void setPers3NaturalPlural(const QString &type, const QString &str);

private:

   struct conjug_t {

      conjug_t() {
        p3common = false;
        s3common = false;
      }

      QString type;
      bool    p3common,
              s3common;
      QString pers1_sing,
              pers2_sing,
              pers3_m_sing,
              pers3_f_sing,
              pers3_n_sing,
              pers1_plur,
              pers2_plur,
              pers3_m_plur,
              pers3_f_plur,
              pers3_n_plur;
   };

   struct conjug_name_t {
      const char *abbrev;
      const char *name;
   };

protected:

   vector<conjug_t>       conjugations;

   static conjug_name_t   names [];
   static vector<QString> userTenses;
};


#endif // grammarmanager_included

