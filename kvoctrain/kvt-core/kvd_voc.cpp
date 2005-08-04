/***************************************************************************

                   maintain a document in Vokabletrainer format

    -----------------------------------------------------------------------

    begin          : Wed Jun 15 19:32:00 PDT 2005

    copyright      : (C) 2005 Peter Hedlund <peter@peterandlinda.com>

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
#include <kapplication.h>
#include <kmessagebox.h>
#include <kdebug.h>

#include "kvoctraindoc.h"
#include "kvoctraincore.h"
//Added by qt3to4:
#include <QTextStream>


bool kvoctrainDoc::loadFromVoc(QTextStream& is)
{
  author = "http://www.vokabeln.de"; //DO NOT translate

  langs.clear();
  vocabulary.clear();
  lesson_descr.clear();

  QString title,
          lang1,
          lang2,
          expression,
          original,
          translation,
          lessonDescr,
          temp;

  int     i,
          wordCount,
          lesson;

  int lines = 10000;

  QStringList titles,
              languages,
              words;

  bool keepGoing = true;

  while (keepGoing)
  {
    temp = is.readLine();
    keepGoing = !(temp.find("\",") > 0);
    title.append(temp);
    if (keepGoing)
      title.append(" ");
  }

  titles = QStringList::split("\",", title);
  doctitle = titles[0].mid(1);
  wordCount = titles[1].section(',', 0, 0).toInt();

  is.readLine();
  is.readLine();

  lang1 = is.readLine();
  languages = QStringList::split("\",", lang1);

  langs.push_back(languages[0].mid(1));
  langs.push_back(languages[1].mid(1));

  keepGoing = true;
  while (keepGoing)
    keepGoing = !(is.readLine().find("8. Lernhilfe") > 0); //DO NOT translate

  for (i = 0; i <= 14; i++)
    is.readLine();

  for (i = 0; i < wordCount - 1; i++)
  {
    int c = 0;
    expression.setLength(0);

    while (c < 2)
    {
      temp = is.readLine();
      c+= temp.contains("\",");
      expression.append(temp);
      if (c < 2)
        expression.append(" ");
    }

    words = QStringList::split("\",", expression);
    original = words[0].mid(1);
    translation = words[1].mid(1);
    lesson = words[2].toInt();

    kvoctrainExpr kve;
    kve.setOriginal(original);
    kve.setGrade(1, 0, false);
    kve.setGrade(1, 0, true);
    kve.setLesson(lesson);
    kve.addTranslation(translation);

    vocabulary.push_back(kve);

    is.readLine();
    is.readLine();
  }

  is.readLine();
  is.readLine();
  is.readLine();

  for (int i = 0; !is.atEnd() && i < lines; i++)
  {
    lessonDescr = is.readLine();
    lessonDescr = lessonDescr.mid(1, lessonDescr.length() - 2);
    if (!lessonDescr.isEmpty())
      lesson_descr.push_back(lessonDescr);
    else
      break;
    is.readLine();
  }

  setModified(false);
  doc_url.setFileName(i18n("Untitled"));
  return is.device()->status() == IO_Ok;
}
