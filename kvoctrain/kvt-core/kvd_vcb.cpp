/***************************************************************************

    $Id$

                   maintain a document in vocabbox format

    -----------------------------------------------------------------------

    begin                : Thr Mar 23 21:00:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.4  2001/11/25 11:11:23  arnold
    switch for inline edit, splitted kv_resource.h

    Revision 1.3  2001/11/10 22:28:46  arnold
    removed compatibility for kde1

    Revision 1.2  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

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

#include <klocale.h>
#include <kapplication.h>

#include "kvoctraindoc.h"
#include "kvoctraincore.h"

#include <qtextstream.h>
#include <qmessagebox.h>


bool kvoctrainDoc::saveTypeNameVcb (QTextStream &os)
{
 return true;
}


bool kvoctrainDoc::loadTypeNameVcb (QTextStream &is)
{
 return true;
}


bool kvoctrainDoc::saveLessonVcb (QTextStream &os)
{
 return true;
}


bool kvoctrainDoc::loadLessonVcb (QTextStream &is)
{
 return true;
}


bool kvoctrainDoc::saveToVcb (QTextStream& os, QString title)
{
  saveTypeNameVcb (os);
  saveLessonVcb (os);

  int ent_no = 0;
  int ent_percent = (int) vocabulary.size () / 100;
  float f_ent_percent = (int) vocabulary.size () / 100.0;
  emit progressChanged(this, 0);

  vector<kvoctrainExpr>::const_iterator first =  vocabulary.begin ();
  while (first != vocabulary.end ()) {

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, ent_no / f_ent_percent);

    QString s, exp;

    exp = (*first).getOriginal()+VCB_SEPARATOR;
    exp += (*first).getTranslation(1)+VCB_SEPARATOR;

    exp += (*first).gradeStr (1, false)+VCB_SEPARATOR;
    exp += (*first).gradeStr (1, true)+VCB_SEPARATOR;
    s.setNum ((*first).getQueryDate (1, false));
    exp += s+VCB_SEPARATOR;
    s.setNum ((*first).getQueryDate (1, true));
    exp += s;

    os << exp << "\n";

    first++;
  }

  setModified (false);
  return os.device()->status() == IO_Ok;
}


bool kvoctrainDoc::loadFromVcb (QTextStream& is)
{

  langs.clear();
  vocabulary.clear();
  loadTypeNameVcb (is);
  loadLessonVcb (is);

  int size = is.device()->size ();
  int ln = size / 40 / 100;  // assume each line about 40 chars
  float f_ent_percent = size / 100.0;
  emit progressChanged (this, 0);

  for (int l = 0; !is.eof(); l++) {
    QString s = is.readLine();

    ln--;
    if (ln <= 0) {
      ln = size / 40 / 100;  // assume each line about 40 chars
      emit progressChanged(this, is.device()->at() / f_ent_percent);
    }

    if (!s.stripWhiteSpace().isEmpty()) {
      QString o, t;
      grade_t ograde = KV_LEV1_GRADE,
              tgrade = KV_LEV1_GRADE;
      size_t  odate = time(0),
              tdate = time(0);
 
      int pos = s.find (VCB_SEPARATOR);
      if (pos >= 0) {
        o = s.left(pos);
        s.remove (0, pos+QString(VCB_SEPARATOR).length());
      }

      pos = s.find (VCB_SEPARATOR);
      if (pos >= 0) {
        t = s.left(pos);
        s.remove (0, pos+QString(VCB_SEPARATOR).length());
      }
      else {
        t = s;
      }
 
      pos = s.find (VCB_SEPARATOR);
      if (pos >= 0) {
        ograde = s.left(pos).toInt();
        s.remove (0, pos+QString(VCB_SEPARATOR).length());
      }
 
      pos = s.find (VCB_SEPARATOR);
      if (pos >= 0) {
        tgrade = s.left(pos).toInt();
        s.remove (0, pos+QString(VCB_SEPARATOR).length());
      }
 
      pos = s.find (VCB_SEPARATOR);
      if (pos >= 0) {
        odate = s.left(pos).toInt();
        s.remove (0, pos+QString(VCB_SEPARATOR).length());
      }
 
      if (s.stripWhiteSpace().length() >= 0) {
        tdate = s.toInt();
      }
 
      kvoctrainExpr bucket;
      bucket.setOriginal(o);
      bucket.addTranslation(t);

      bucket.setGrade(1, ograde, false);
      if (ograde != 0)
        bucket.setQueryCount(1, 1, false);

      bucket.setGrade(1, tgrade, true);
      if (tgrade != 0) 
        bucket.setQueryCount(1, 1, true);

      bucket.setQueryDate(1, odate, false);
      bucket.setQueryDate(1, tdate, true);
 
      appendEntry (&bucket);
    }
  }

  langs.push_back("en");
  langs.push_back("en");

  setModified (false);
  return is.device()->status() == IO_Ok;
}


void kvoctrainDoc::errorVcb (int line, const QString &text )
{
   unknown_elem = true;
   QApplication::setOverrideCursor( arrowCursor, true );
   QString s = kapp->makeStdCaption(i18n("Error in vocabbox file"));
   QString msg = text;
   QMessageBox mb( s,
       msg,
       QMessageBox::Critical,
       QMessageBox::Abort | QMessageBox::Default,
       0,
       0);
   mb.exec();
   QApplication::restoreOverrideCursor();
}
