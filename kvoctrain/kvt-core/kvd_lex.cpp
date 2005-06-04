/***************************************************************************

            maintain a *.lex document (vocabulary trainer for Win)

    -----------------------------------------------------------------------

    begin                : Sat Sep 04 11:00:53 MET 1999

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

#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "kvoctraindoc.h"


#include <vector>
using namespace std;

#define LEX_MAX_ATTR    20
#define LEX_MAX_LESSON  9

/*  sample for a lex-file

Vocabulary Trainer V5.0
LEX|Deutsch|Englisch| 259 260 1 1 1 0 0 268 0 1 1 1 1 145 6 23 1999 255 1 1 653 0 0 0 0 0 0 0 0 0 0 1 1 1 0 1 13 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 10 1 1 0 0 |
-11 700
System
Energie- und Automatisierungstechnik. V. Manuela Lutz. Danke.
1 1 der die das|he she it|
1 1 eine einer eines einen einem|has have had is be|
1 1 er sie es|his him her its|
1 1 wir ihr euch|in to into on at too|
1 1 in an auf bei|the a an|
1 1 zu um zur zum|out by with of off|
1 1 ist sind seid|something someone some|
1 1 dies dieser dieses|under below|
1 1 ||
1 1 ||
der die das ein eine einer eines einen einem|the a an|
|
|
|
|
|
|
|
|
|
|
|
|
|
|
|
|
|
|
|
|
Sachgebiete|
|
|
|
|
|
|
|
|
32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32
1 0 255 255 255 255 255 255 0 0 1 Abbildung||||mapping||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
1 0 255 255 255 255 255 255 0 0 1 Abblendkappe||||anti-dazzle device||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
...
*/


static QString extract (QString &line)
{
   QString ret;
   int lim = line.find ("|");
   if (lim < 0) {
     ret = line;
     line = "";
   }
   else {
     ret = line.left(lim);
     line.remove (0, lim+1);
   }

   return ret;
}


bool kvoctrainDoc::saveTypeNameLex (QTextStream &os)
{
  int i;
  for (i = 0; i < (int) type_descr.size() && i < LEX_MAX_ATTR; i++)
    os << getTypeName(i) << "|\n";

  while (i++ < LEX_MAX_ATTR)
    os << "|\n";

  return os.device()->status() == IO_Ok;
}


bool kvoctrainDoc::loadTypeNameLex (QTextStream &is)
{
  QString s;
  QString attr;

  type_descr.clear();

  for (int i = 0; i < LEX_MAX_ATTR; i++) {
    s = is.readLine();
    attr = extract (s);
/*
    if (attr.stripWhiteSpace().isEmpty()) {
      attr.setNum (i+1);
      attr.insert (0, "#");
    }
*/
    type_descr.push_back (attr);
  }

  return is.device()->status() == IO_Ok;
}


bool kvoctrainDoc::saveLessonLex (QTextStream &os)
{
  int i;
  for (i = 0; i < (int) lesson_descr.size() && i < LEX_MAX_LESSON; i++)
    os << lesson_descr[i] << "|\n";

  while (i++ < LEX_MAX_LESSON)
    os << "|\n";

  return os.device()->status() == IO_Ok;
}


bool kvoctrainDoc::loadLessonLex (QTextStream &is)
{
  QString s;
  QString lesson;

  lesson_descr.clear();

  for (int i = 0; i < LEX_MAX_LESSON; i++) {
    s = is.readLine();
    lesson = extract (s);
    if (lesson.stripWhiteSpace().isEmpty()) {
      lesson = "#" + QString::number(i+1);
    }
    lesson_descr.push_back (lesson);
  }

  return is.device()->status() == IO_Ok;
}


bool kvoctrainDoc::saveToLex (QTextStream& os, QString & /*title*/)
{
  os << LEX_IDENT_50 "\n";
  os << "LEX|" << getOriginalIdent() << "|" << getIdent(1) << "|"
     << "259 260 1 1 1 0 0 268 0 1 1 1 1 ";
  os << (int) vocabulary.size();
  os << " 9 5 1999 255 1 1 653 0 0 0 0 0 0 0 0 0 0 1 1 1 0 1 13 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 10 1 1 0 0 |#3"  << "\n";
  os << "-11 700 0 0 0 0 0 0 0 0 0 0" << "\n";
  os << "System\n";
  os << getTitle() << "\n";
  os << "1 1 der die das|he she it|" << "\n";
  os << "1 1 eine einer eines einen einem|has have had is be|" << "\n";
  os << "1 1 er sie es|his him her its|" << "\n";
  os << "1 1 wir ihr euch|in to into on at too|" << "\n";
  os << "1 1 in an auf bei|the a an|" << "\n";
  os << "1 1 zu um zur zum|out by with of off|" << "\n";
  os << "1 1 ist sind seid|something someone some|" << "\n";
  os << "1 1 dies dieser dieses|under below|" << "\n";
  os << "1 1 ||" << "\n";
  os << "1 1 ||" << "\n";
  os << "der die das ein eine einer eines einen einem|the a an|" << "\n";

  if (!saveTypeNameLex (os))
    return false;

  if (!saveLessonLex (os))
    return false;

  os << "32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32 32" << "\n";

  int ent_no = 0;
  int ent_percent = (int) vocabulary.size () / 100;
  float f_ent_percent = (int) vocabulary.size () / 100.0;
  emit progressChanged(this, 0);

  vector<kvoctrainExpr>::const_iterator first =  vocabulary.begin ();
  while (first != vocabulary.end ()) {

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, int(ent_no / f_ent_percent));

    os << "0" // type
       <<  " " << "255 255 255 255 255 255 255"
       <<  " " << QMIN (2, (*first).getGrade(false))
       <<  " " << QMIN (2, (*first).getGrade(true))
       <<  " " << (*first).getLesson()
       <<  " " << (*first).getOriginal()
       <<  " " << "||||"  // synonyms
       << (*first).getTranslation(1)
       << "||||"  // synonyms
       << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
       << "\n";

    first++;
  }

  setModified (false);
  return os.device()->status() == IO_Ok;
}


bool kvoctrainDoc::loadFromLex (QTextStream& is)
{

  langs.clear();
  vocabulary.clear();
  QString version,
          info1,
          s_dummy,
          title,
          f_keys,
          ident,
          orgID,
          transID;

  int     i_dummy,
          type,
          f_grade,
          t_grade,
          lesson;

  int lines = 10000000;

  version = is.readLine();
  info1 = is.readLine();
  ident = extract (info1);

  orgID = extract (info1);
  if (orgID.isEmpty())
    orgID = "original";

  transID = extract (info1);
  if (transID.isEmpty())
    transID = "translation";

  if (ident != "LEX" || version != LEX_IDENT_50) {
    errorLex (1, "invalid file format");
    return false;
  }

  // first two chars usually give language code:
  // english  ==> en
  // Deutsch  ==> de
  // italano  ==> it

  langs.push_back (orgID.left(2).lower());
  langs.push_back (transID.left(2).lower());

  QTextStream params (info1, IO_ReadOnly);
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> i_dummy;
  params >> lines;

  s_dummy = is.readLine();  // Font
  s_dummy = is.readLine();  // Font
  title = is.readLine();
  setTitle (title);
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();
  s_dummy = is.readLine();

  if (!loadTypeNameLex (is))
    return false;

  if (!loadLessonLex (is))
    return false;

  f_keys = is.readLine();

  int ent_percent = (int) lines / 100;
  float f_ent_percent = (int) lines / 100.0;
  emit progressChanged(this, 0);

  for (int i = 0; !is.eof() && i < lines; i++) {
    if (ent_percent != 0 && (i % ent_percent) == 0 ) {
      emit progressChanged(this, int(i / f_ent_percent));
    }

    is >> type;
    is >> i_dummy;
    is >> i_dummy;
    is >> i_dummy;
    is >> i_dummy;
    is >> i_dummy;
    is >> i_dummy;
    is >> i_dummy;
    is >> t_grade;
    is >> f_grade;
    is >> lesson;

    QString line = is.readLine();
    QString s;

    QString original = extract (line);
    for (int i = 0; i < 3; i++) {
      s = extract (line);
      if (!s.isEmpty())
        original += ", " +s;
    }

    QString translation = extract (line);
    for (int i = 0; i < 3; i++) {
      s = extract (line);
      if (!s.isEmpty())
        translation += ", " +s;
    }

/*  ignore rest
       >> "||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
       >> "\n";
*/
    kvoctrainExpr kve (original);
    kve.setGrade (1, t_grade, false);
    kve.setGrade (1, f_grade, true);
    kve.setLesson (lesson);
    kve.addTranslation (translation);

// FIXME: convert attribute to word type ?

    vocabulary.push_back(kve);

  }

  setModified (false);
  return is.device()->status() == IO_Ok;
}


void kvoctrainDoc::errorLex (int /*line*/, const QString &text )
{
   unknown_elem = true;
   QApplication::setOverrideCursor( arrowCursor, true );
   QString s = kapp->makeStdCaption(i18n("Error in lex file"));
   QString msg = text;
   KMessageBox::error(0, msg, s);
   QApplication::restoreOverrideCursor();
}
