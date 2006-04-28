/***************************************************************************

                    maintain a document in text format

    -----------------------------------------------------------------------

    begin          : Sat Sep 06 11:00:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2001 The KDE-EDU team
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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

#include <vector>

#include <QTextCodec>
#include <QTextStream>
#include <QApplication>

#include <klocale.h>
#include "kvoctraindoc.h"
#include <kmessagebox.h>

using namespace std;

bool kvoctrainDoc::saveTypeNameCsv (QTextStream &)
{
 return true;
}


bool kvoctrainDoc::loadTypeNameCsv (QTextStream &)
{
 return true;
}


bool kvoctrainDoc::saveLessonCsv (QTextStream &)
{
 return true;
}


bool kvoctrainDoc::loadLessonCsv (QTextStream &)
{
 return true;
}

bool kvoctrainDoc::saveToCsv (QTextStream& os, QString &)
{
  QString separator = Prefs::separator();

  saveTypeNameCsv (os);
  saveLessonCsv (os);

  int ent_no = 0;
  int ent_percent = (int) vocabulary.size () / 100;
  float f_ent_percent = (int) vocabulary.size () / 100.0;
  emit progressChanged(this, 0);

  os << i18n("! Title:")  << separator << getTitle() << "\n";
  os << i18n("! Author:") << separator << getAuthor() << "\n";

  vector <int> csv_order = kvoctrainApp::getCsvOrder(this);

  vector<kvoctrainExpr>::const_iterator first =  vocabulary.begin ();
  QString exp;

  os.setCodec(QTextCodec::codecForName("UTF-8"));
  while (first != vocabulary.end ()) {

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, ent_no / (int) f_ent_percent);

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


bool kvoctrainDoc::loadFromCsv (QTextStream& is)
{
  QString separator = Prefs::separator();
  QStringList lang_order = Prefs::pasteOrder();

  langs.clear();
  vocabulary.clear();
  loadTypeNameCsv (is);
  loadLessonCsv (is);

  // autodetect utf8
  // FIXME: provide choice for Latinx?
  is.setCodec(QTextCodec::codecForName("ISO 8859-1"));
  is.setEncoding(QTextStream::Latin1);

  int size = is.device()->size ();
  int ln = size / 20 / 100;  // assume each line about 20 chars
  float f_ent_percent = size / 100.0;
  emit progressChanged (this, 0);

  bool utf8_mode = false;
  int lang_num = 0;
  for (int l = 0; !is.atEnd(); l++) {
    QString s = is.readLine();

    // autodetect utf8
    if (!utf8_mode) {
      for (int i = 0; !utf8_mode && i < (int) s.length(); ++i) {
        ushort trigger = s[i].unicode();
        if (trigger >= 0x80) {
          int remain = s.length() - i;
          if ((trigger & 0xE0) == 0xC0){         // 110x xxxx
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
        is.setCodec(QTextCodec::codecForName("UTF-8"));
        is.setEncoding(QTextStream::UnicodeUTF8);
        s = QString::fromUtf8(s.ascii());
      }

    }

    ln--;
    if (ln <= 0) {
      ln = size / 20 / 100;  // assume each line about 20 chars
      emit progressChanged(this, is.device()->at() / (int) f_ent_percent);
    }

    // similar in kva_clip.cpp::slotEditPaste()

    if (!s.simplified().isEmpty()) {
      kvoctrainExpr bucket (s, separator);
      kvoctrainExpr expr;
      // now move columns according to paste-order
      for (int i = 0; i < bucket.numTranslations()+1; i++) {
        lang_num = qMax (lang_num, bucket.numTranslations()+1);
        if (i == 0)
          expr.setOriginal(bucket.getOriginal());
        else {
          QString ts = bucket.getTranslation(i);
          expr.setTranslation(i, ts);
        }
      }
      appendEntry (&expr);
    }
  }

  for (int j = 0; j < lang_num; j++) {
    if (j < (int) lang_order.count() ) {
      langs.push_back((lang_order)[j]);
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


void kvoctrainDoc::errorCsv (int /*line*/, const QString &text )
{
   unknown_elem = true;
   QApplication::setOverrideCursor( Qt::ArrowCursor, true );
   QString s = KInstance::makeStdCaption(i18n("Error in csv file"));
   QString msg = text;
   KMessageBox::error( 0, msg, s);
   QApplication::restoreOverrideCursor();
}
