/***************************************************************************

    $Id$

                    maintain a document in text format

    -----------------------------------------------------------------------

    begin                : Sat Sep 06 11:00:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
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

#include "compat_2x.h"
#include <kapp.h>

#include "kvoctraindoc.h"
#include "kvoctrain.h"
#include "kv_resource.h"

#include <qtextstream.h>
#include <qtextcodec.h>
#include <qmessagebox.h>

bool kvoctrainDoc::saveTypeNameCsv (QTextStream &os)
{
 return true;
}


bool kvoctrainDoc::loadTypeNameCsv (QTextStream &is)
{
 return true;
}


bool kvoctrainDoc::saveLessonCsv (QTextStream &os)
{
 return true;
}


bool kvoctrainDoc::loadLessonCsv (QTextStream &is)
{
 return true;
}

bool kvoctrainDoc::saveToCsv (QTextStream& os, QString title,
                              const QString separator, QStringList *lang_order)
{
  saveTypeNameCsv (os);
  saveLessonCsv (os);

  int ent_no = 0;
  int ent_percent = (int) vocabulary.size () / 100;
  float f_ent_percent = (int) vocabulary.size () / 100.0;
  emit progressChanged(this, 0);

  os << i18n("! Title:")  << separator << getTitle() << "\n";
  os << i18n("! Author:") << separator << getAuthor() << "\n";

  vector <int> csv_order = kvoctrainApp::getCsvOrder(this, lang_order);

  vector<kvoctrainExpr>::const_iterator first =  vocabulary.begin ();
  QString exp;

  bool utf8_mode = false;
  while (first != vocabulary.end ()) {

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, ent_no / f_ent_percent);

    exp = "";
    // now concat columns according to paste-order
    bool sep = false;
    for (int i = 0; i < (int) csv_order.size(); i++) {
      if (!sep)
        sep = true;
      else
        exp += separator;

      if (csv_order[i] >= 0) {
        if (csv_order[i] == 0)
          exp += (*first).getOriginal();
        else
          exp += (*first).getTranslation(csv_order[i]);
      }
    }

    if (!exp.isEmpty())
      os << exp << "\n";

    first++;
  }

  setModified (false);
  return os.device()->status() == IO_Ok;
}


bool kvoctrainDoc::loadFromCsv (QTextStream& is,
                                const QString separator, QStringList *lang_order)
{

  langs.clear();
  vocabulary.clear();
  loadTypeNameCsv (is);
  loadLessonCsv (is);

  int size = is.device()->size ();
  int ln = size / 20 / 100;  // assume each line about 20 chars
  float f_ent_percent = size / 100.0;
  emit progressChanged (this, 0);

  bool utf8_mode = false;
  int lang_num = 0;
  for (int l = 0; !is.eof(); l++) {
    QString s = is.readLine();

    // autodetect utf8
    if (!utf8_mode) {
      for (int i = 0; !utf8_mode && i < s.length(); ++i) {
        ushort trigger = s[i].unicode();
        if (trigger >= 0x80) {
          int remain = s.length() - i;
          cout << "trigger " << hex << trigger << endl;
          if ((trigger & 0xE0) == 0xC0){         // 110x xxxx
            cout << "trigger1a " << hex << s[i+1].unicode() << endl;
            if (   (remain > 1)
                && (s[i+1].unicode() & 0xC0) == 0x80)
              utf8_mode = true;
          }
          else if ((trigger & 0xF0) == 0xE0) {  // 1110 xxxx
            if (   remain > 2
                && s[i+1].unicode() & 0xC0 == 0x80
                && s[i+2].unicode() & 0xC0 == 0x80)
              utf8_mode = true;
          }
          else if ((trigger & 0xF8) == 0xF0) {   // 1111 0xxx
            if (   remain > 3
                && s[i+1].unicode() & 0xC0 == 0x80
                && s[i+2].unicode() & 0xC0 == 0x80
                && s[i+3].unicode() & 0xC0 == 0x80)
              utf8_mode = true;
          }
          else if ((trigger & 0xFC) == 0xF8) {   // 1111 10xx
            if (   remain > 4
                && s[i+1].unicode() & 0xC0 == 0x80
                && s[i+2].unicode() & 0xC0 == 0x80
                && s[i+3].unicode() & 0xC0 == 0x80
                && s[i+4].unicode() & 0xC0 == 0x80)
              utf8_mode = true;
          }
          else if ((trigger & 0xFE) == 0xFC) {   // 1111 110x
            if (   remain > 5
                && s[i+1].unicode() & 0xC0 == 0x80
                && s[i+2].unicode() & 0xC0 == 0x80
                && s[i+3].unicode() & 0xC0 == 0x80
                && s[i+4].unicode() & 0xC0 == 0x80
                && s[i+5].unicode() & 0xC0 == 0x80)
              utf8_mode = true;
          }
        }
      }

      if (utf8_mode) {
        is.setCodec(QTextCodec::codecForName(KES_UTF8));
        s = QString::fromUtf8(s.ascii());
      }

    }

    ln--;
    if (ln <= 0) {
      ln = size / 20 / 100;  // assume each line about 20 chars
      emit progressChanged(this, is.device()->at() / f_ent_percent);
    }

    // similar in kva_clip.cpp::slotEditPaste()

    if (!s.stripWhiteSpace().isEmpty()) {
      kvoctrainExpr bucket (s, separator);
      kvoctrainExpr expr;
      // now move columns according to paste-order
      for (int i = 0; i < bucket.numTranslations()+1; i++) {
        lang_num = QMAX (lang_num, bucket.numTranslations()+1);
        if (i == 0)
          expr.setOriginal(bucket.getOriginal());
        else
          expr.setTranslation(i, bucket.getTranslation(i));
      }
      appendEntry (&expr);
    }
  }

  for (int j = 0; j < lang_num; j++) {
    if (j < (int) lang_order->count() ) {
      langs.push_back((*lang_order)[j]);
    }
    else {
      if (j == 0)
        langs.push_back("org");
      else {
        QString s;
        s.setNum(j);
        s.insert(0, "trans");
        langs.push_back(s);
      }
    }
  }
  setModified (false);
  return is.device()->status() == IO_Ok;
}


void kvoctrainDoc::errorCsv (int line, const QString &text )
{
   unknown_elem = true;
   QApplication::setOverrideCursor( arrowCursor, true );
   QString s = kapp->makeStdCaption(i18n("Error in csv file"));
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
