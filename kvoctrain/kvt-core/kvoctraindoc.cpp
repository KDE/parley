/***************************************************************************

    $Id$

                   maintain a kvoctrain document

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de


    -----------------------------------------------------------------------

    $Log$
    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

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


#include "kvoctraindoc.h"
#include <kapp.h>
#include <eadebug.h>
#include "compat_2x.h"

#include <qmessagebox.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>

#include <algo.h>
#include <function.h>
#include <vector.h>
#include <iostream.h>
#include <fstream.h>
#include <strstream.h>

#ifdef __FreeBSD__
#include <float.h>
#else
#include <values.h>
#endif

#include "kv_resource.h"

//********************************************************
//  kvoctrainDoc
//********************************************************

void kvoctrainDoc::setVersion (QString vers)
{
   doc_version = vers;
}


void kvoctrainDoc::getVersion(int &major, int &minor, int &patch)
{
}


void kvoctrainDoc::Init ()
{

  setEncoding (KE_UTF8);
  setVersion (VERSION);
  lesson_descr.clear();
  type_descr.clear();
  tense_descr.clear();
  langs.clear();
  extraSizehints.clear();
  sizehints.clear();
  vocabulary.clear();
  dirty = false;
  sort_allowed = true;
  unknown_attr = false;
  unknown_elem = false;
  sort_lesson = false;
  for (int i = 0; i < (int) langs.size(); i++)
    sort_lang.push_back(false);
  setCurrentLesson (0);
  queryorg = "";
  querytrans = "";
  mainfile = "";
  doctitle = "";
  author = "";
}


kvoctrainDoc::kvoctrainDoc(QObject *parent, QString filename,
                           const QString separator, QStringList *lang_order)
{
  Init();
  mainfile = filename;
  connect( this, SIGNAL(progressChanged(kvoctrainDoc*,int)),
           parent, SLOT(slotProgress(kvoctrainDoc*,int)) );

  FileType ft = detectFT(filename);

  if (!mainfile.isEmpty()) {

    QFile f( filename );
    QFileInfo fi (f);
    bool isfile = fi.isFile() || fi.isSymLink();
    if (!f.exists() || !isfile) {
      QString format = i18n("Could not open \"%1\"\n"
                            "\n"
                            "Probably this is no regular file or it does not exist.");
      QString msg = format.arg(mainfile);
      QMessageBox mb(kapp->makeStdCaption(i18n("I/O failure")),
          msg,
          QMessageBox::Warning,
          QMessageBox::Ok | QMessageBox::Default,
          0,
          0);
      mb.exec();
      return;
    }

    bool read = false;
    while (!read) {

      QApplication::setOverrideCursor( waitCursor );
      switch (ft) {
        case kvtml:
        {
          QFile f( filename );
          f.open( IO_ReadOnly );
          QTextStream is (&f);
          read = loadFromKvtMl (is);
        }
        break;

        case vt_lex:
        {
          QFile f( filename );
          f.open( IO_ReadOnly );
          QTextStream is (&f);
          read = loadFromLex (is);
          f.close();
        }
        break;

        case vt_vcb:
        {
          QFile f( filename );
          f.open( IO_ReadOnly );
          QTextStream is (&f);
          read = loadFromVcb (is);
          f.close();
        }
        break;

        case csv:
        {
          QFile f( filename );
          f.open( IO_ReadOnly );
          QTextStream is (&f);
          read = loadFromCsv (is, separator, lang_order);
          f.close();
        }
        break;

        default:
        {
          QFile f( mainfile );
          f.open( IO_ReadOnly );
          QTextStream is (&f);
          read = loadFromKvtMl (is);
        }
      }

      QApplication::restoreOverrideCursor();

      if (!read) {
        if (unknown_attr || unknown_elem ) {
          Init();
          return;
        }
        QString format = i18n("Could not load \"%1\"\nDo you want to repeat ?");
        QString msg = format.arg(mainfile);
        QMessageBox mb( kapp->makeStdCaption(i18n("I/O failure")),
            msg,
            QMessageBox::Warning,
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No | QMessageBox::Escape,
            0);
        if ( mb.exec() == QMessageBox::No ) {
          Init();
          return;
        }
      }
    }
  }
}


kvoctrainDoc::~kvoctrainDoc()
{
}


bool kvoctrainDoc::saveAs (QObject *parent, QString name, QString title,
                           FileType ft,
                          const QString separator, QStringList *lang_order)
{
  connect( this, SIGNAL(progressChanged(kvoctrainDoc*,int)),
           parent, SLOT(slotProgress(kvoctrainDoc*,int)) );

  QString tmp (name);
  if (tmp.isEmpty())
    tmp = mainfile;

  if (tmp.isEmpty())
    tmp = "outfile.kvtml";

  if (ft == automatic) {

    if (tmp.right(strlen("." KVTML_EXT)) == "." KVTML_EXT)
      ft = kvtml;
    else if (tmp.right(strlen("." VT5_LEX_EXT)) == "." VT5_LEX_EXT)
      ft = vt_lex;
    else if (tmp.right(strlen("." VCB_EXT)) == "." VCB_EXT)
      ft = vt_vcb;
    else if (tmp.right(strlen("." CSV_EXT)) == "." CSV_EXT)
      ft = csv;
    else
      ft = kvtml;
  }

  bool saved = false;
  while (!saved) {

    QApplication::setOverrideCursor( waitCursor );
    QFile f( tmp);
    f.open( IO_WriteOnly );                     // open file for writing
    switch (ft) {
      case kvtml: {
        QTextStream os( &f );                       // serialize using f
        saved = saveToKvtMl(os, title);
      }
      break;

      case vt_lex: {
        QTextStream os( &f );                       // serialize using f
        saved = saveToLex(os, title);
        f.close();           
      }
      break;

      case vt_vcb: {
        QTextStream os( &f );                       // serialize using f
        saved = saveToVcb(os, title);
        f.close();           
      }
      break;

      case csv: {
        QTextStream os( &f );                       // serialize using f
        saved = saveToCsv(os, title, separator, lang_order);
        f.close();           
      }
      break;

      default: {
        cerr << "kvcotrainDoc::saveAs(): unknown filetype\n";
      }
      break;
    }
    QApplication::restoreOverrideCursor();

    if (!saved) {
      QString format = i18n("Could not save \"%1\"\nDo you want to repeat ?");
      QString msg = format.arg(tmp);
      QMessageBox mb( kapp->makeStdCaption(i18n("I/O failure")),
          msg,
          QMessageBox::Warning,
          QMessageBox::Yes | QMessageBox::Default,
          QMessageBox::No | QMessageBox::Escape,
          0);
     if ( mb.exec() == QMessageBox::No ) return false;
    }
  }
  mainfile = tmp;
  dirty = false;
  emit docModified(false);
  return true;
}


kvoctrainExpr *kvoctrainDoc::getEntry(int index)
{
  if (index < 0 || index >= (int)vocabulary.size() )
    return 0;
  else
    return &vocabulary[index];
}


void kvoctrainDoc::removeEntry(int index)
{
  if (index >= 0 && index < (int)vocabulary.size() )
    vocabulary.erase (&vocabulary[index], &vocabulary[index+1]);
}


int kvoctrainDoc::findIdent (const QString lang) const
{
  vector<QString>::const_iterator first = langs.begin();
  int count = 0;
  while (first != langs.end()) {
    if ( *first == lang)
      return count;
    first++;
    count++;
  }
  return -1;
}


QString kvoctrainDoc::getIdent (int index) const
{
  if (index >= (int)langs.size() || index < 1 )
    return "";
  else
    return langs[index];
}


void kvoctrainDoc::setIdent (int idx, const QString id)
{
  if (idx < (int)langs.size() && idx >= 1 ) {
    langs[idx] = id;
  }
}


QString kvoctrainDoc::getTypeName (int index) const
{
  if (index >= (int)type_descr.size())
    return "";
  else
    return type_descr[index];
}


void kvoctrainDoc::setTypeName (int idx, const QString id)
{
  if (idx >= (int)type_descr.size())
    for (int i = (int)type_descr.size(); i <= idx; i++)
      type_descr.push_back ("");

  type_descr[idx] = id;
}


QString kvoctrainDoc::getTenseName (int index) const
{
  if (index >= (int)tense_descr.size())
    return "";
  else
    return tense_descr[index];
}


void kvoctrainDoc::setTenseName (int idx, const QString id)
{
  if (idx >= (int)tense_descr.size())
    for (int i = (int)tense_descr.size(); i <= idx; i++)
      tense_descr.push_back ("");

  tense_descr[idx] = id;
}


QString kvoctrainDoc::getUsageName (int index) const
{
  if (index >= (int)usage_descr.size())
    return "";
  else
    return usage_descr[index];
}


void kvoctrainDoc::setUsageName (int idx, const QString id)
{
  if (idx >= (int)usage_descr.size())
    for (int i = (int)usage_descr.size(); i <= idx; i++)
      usage_descr.push_back ("");

  usage_descr[idx] = id;
}


void kvoctrainDoc::setConjugation (int idx, const Conjugation &con)
{
  if ( idx < 0) return;

  // extend conjugation with empty elements
  if ((int)conjugations.size() <= idx )
    for (int i = conjugations.size(); i < idx+1; i++)
      conjugations.push_back (Conjugation());

  conjugations[idx] = con;
}


Conjugation kvoctrainDoc::getConjugation (int idx) const
{
  if (idx >= (int)conjugations.size() || idx < 0) {
    return Conjugation();
  }
  else {
    return conjugations[idx];
  }
}


void kvoctrainDoc::setArticle (int idx, const Article &art)
{
  if ( idx < 0) return;

  // extend conjugation with empty elements
  if ((int)articles.size() <= idx )
    for (int i = articles.size(); i < idx+1; i++)
      articles.push_back (Article());

  articles[idx] = art;
}


Article kvoctrainDoc::getArticle (int idx) const
{
  if (idx >= (int)articles.size() || idx < 0) {
    return Article();
  }
  else {
    return articles[idx];
  }
}


int kvoctrainDoc::getSizeHint (int idx) const
{
  if (idx < 0) {
    idx = -idx;
    if (idx >= (int)extraSizehints.size() )
      return 80; // make a good guess about column size
    else {
//      cout << "gsh " << idx << "  " << extraSizehints[idx] << endl;
      return extraSizehints[idx];
    }
  }
  else {
    if (idx >= (int)sizehints.size() )
      return 150; // make a good guess about column size
    else {
//      cout << "gsh " << idx << "  " << sizehints[idx] << endl;
      return sizehints[idx];
    }
  }
}


void kvoctrainDoc::setSizeHint (int idx, const int width)
{
//  cout << "ssh " << idx << "  " << width << endl;
  if (idx < 0) {
    idx = -idx;
    if (idx >= (int)extraSizehints.size()) {
      for (int i = (int)extraSizehints.size(); i <= idx; i++)
        extraSizehints.push_back (80);
    }
    extraSizehints[idx] = width;

  }
  else {
    if (idx >= (int)sizehints.size()) {
      for (int i = (int)sizehints.size(); i <= idx; i++)
        sizehints.push_back (150);
    }
    sizehints[idx] = width;
  }
}

#if QT_VERSION < 300
QFont::CharSet kvoctrainDoc::getCharSet (int idx) const
{
  if (idx >= (int)charsets.size() )
    return QFont::AnyCharSet;
  else
    return charsets[idx];
}


QFont::CharSet kvoctrainDoc::string2CharSet (QString &s)
{
  if (s == KES_ANY)
   return QFont::AnyCharSet;

  else if (s == KES_8859_1)
   return QFont::ISO_8859_1;

  else if (s == KES_8859_2)
    return QFont::ISO_8859_2;

  else if (s == KES_8859_3)
    return QFont::ISO_8859_3;

  else if (s == KES_8859_4)
    return QFont::ISO_8859_4;

  else if (s == KES_8859_5)
    return QFont::ISO_8859_5;

  else if (s == KES_8859_6)
    return QFont::ISO_8859_6;

  else if (s == KES_8859_7)
    return QFont::ISO_8859_7;

  else if (s == KES_8859_8)
    return QFont::ISO_8859_8;

  else if (s == KES_8859_9)
    return QFont::ISO_8859_9;

  else if (s == KES_KOI8_R)
    return QFont::KOI8R;

  else if (s == KES_8859_10)
    return QFont::ISO_8859_10;

  else if (s == KES_8859_11)
    return QFont::ISO_8859_11;

  else if (s == KES_8859_12)
    return QFont::ISO_8859_12;

  else if (s == KES_8859_13)
    return QFont::ISO_8859_13;

  else if (s == KES_8859_14)
    return QFont::ISO_8859_14;

  else if (s == KES_8859_15)
    return QFont::ISO_8859_15;

  else if (s == KES_SET_JA)
    return QFont::Set_Ja;

  else if (s == KES_SET_KO)
    return QFont::Set_Ko;

  else if (s == KES_SET_TH_TH)
    return QFont::Set_Th_TH;

  else if (s == KES_SET_ZH)
    return QFont::Set_Zh;

  else if (s == KES_SET_ZH_TW)
    return QFont::Set_Zh_TW;

  else if (s == KES_UNICODE)
    return QFont::Unicode;

  else if (s == KES_SET_BIG5)
    return QFont::Set_Big5;

  return QFont::AnyCharSet;
}


QString kvoctrainDoc::charSet2String (QFont::CharSet cs,
                                      bool show_std,
                                      bool show_countries)

////////////////////////////////////////////////////////////
// also update docprop-dialogs/LangPropPage.cpp CharSetRef
////////////////////////////////////////////////////////////
{
  QString ret;
  switch (cs) {
    case QFont::AnyCharSet:
      return show_std ? KES_8859_1 : KES_ANY;
    break;

    case QFont::ISO_8859_1:
      if (!show_countries)
        ret = KES_8859_1;
      else
        ret = (QString) KES_8859_1 + " "+ i18n("(West Europe)");
      return ret;
    break;

    case QFont::ISO_8859_2:
      if (!show_countries)
        ret = KES_8859_2;
      else
        ret = (QString) KES_8859_2+ " " + i18n("(East Europe)");
      return ret;
    break;

    case QFont::ISO_8859_3:
      if (!show_countries)
        ret = KES_8859_3;
      else
        ret = (QString) KES_8859_3+ " " + i18n("(South Europe)");
      return ret;
    break;

    case QFont::ISO_8859_4:
      if (!show_countries)
        ret = KES_8859_4;
      else
        ret = (QString) KES_8859_4+ " " + i18n("(North Europe)");
      return ret;
    break;

    case QFont::ISO_8859_5:
      if (!show_countries)
        ret = KES_8859_5;
      else
        ret = (QString) KES_8859_5+ " " + i18n("(Cyrillic)");
      return ret;
    break;

    case QFont::ISO_8859_6:
      if (!show_countries)
        ret = KES_8859_6;
      else
        ret = (QString) KES_8859_6+ " " + i18n("(Arabic)");
      return ret;
    break;

    case QFont::ISO_8859_7:
      if (!show_countries)
        ret = KES_8859_7;
      else
        ret = (QString) KES_8859_7+ " " + i18n("(Greek)");
      return ret;
    break;

    case QFont::ISO_8859_8:
      if (!show_countries)
        ret = KES_8859_8;
      else
        ret = (QString) KES_8859_8+ " " + i18n("(Hebrew)");
      return ret;
    break;

    case QFont::ISO_8859_9:
      if (!show_countries)
        ret = KES_8859_9;
      else
        ret = (QString) KES_8859_9+ " " + i18n("(Turkish)");
      return ret;
    break;

    case QFont::KOI8R:
      if (!show_countries)
        ret = KES_KOI8_R;
      else
        ret = (QString) KES_KOI8_R + " " + i18n("(Cyrillic, RFC1489)");
      return ret;
    break;

    case QFont::ISO_8859_10:
      if (!show_countries)
      return KES_8859_10;
      else
        ret = (QString) KES_8859_9+ " " + i18n("(Nordic)");
      return ret;
    break;

    case QFont::ISO_8859_11:
      return KES_8859_11;
    break;

    case QFont::ISO_8859_12:
      return KES_8859_12;
    break;

    case QFont::ISO_8859_13:
      return KES_8859_13;
    break;

    case QFont::ISO_8859_14:
      return KES_8859_14;
    break;

    case QFont::ISO_8859_15:
      return KES_8859_15;
    break;

    case QFont::Set_Ja:
      return KES_SET_JA;
    break;

    case QFont::Set_Ko:
      return KES_SET_KO;
    break;

    case QFont::Set_Th_TH:
      return   KES_SET_TH_TH;
    break;

    case QFont::Set_Zh:
      return KES_SET_ZH;
    break;

    case QFont::Set_Zh_TW:
      return KES_SET_ZH_TW;
    break;

    case QFont::Unicode:
      if (!show_countries)
        ret = KES_UNICODE;
      else
        ret = (QString) KES_UNICODE + " " + i18n("(independent)");
      return ret;
    break;

    case QFont::Set_Big5:
      return KES_SET_BIG5;
    break;

    default : return "";   // i18n(" (unknown charset)");
  }
}


/*
#define CHARSETS_COUNT 25
static const char *charsetsStr[CHARSETS_COUNT]={
    "unicode",
    "iso-8859-1",
    "iso-8859-2",
    "iso-8859-3",
    "iso-8859-4",
    "iso-8859-5",
    "iso-8859-6",
    "iso-8859-7",
    "iso-8859-8",
    "iso-8859-9",
    "iso-8859-10",
    "iso-8859-11",
    "iso-8859-12",
    "iso-8859-13",
    "iso-8859-14",
    "iso-8859-15",
    "koi8r",
    "eucjp",
    "euckr",
    "set-th-th",
    "set-zh",
    "set-zh-tw",
    "utf-8",
    "utf-16",
    "Any"
};
static const char *xNames[CHARSETS_COUNT]={
    "iso10646-1",
    "iso8859-1",
    "iso8859-2",
    "iso8859-3",
    "iso8859-4",
    "iso8859-5",
    "iso8859-6",
    "iso8859-7",
    "iso8859-8",
    "iso8859-9",
    "iso8859-10",
    "iso8859-11",
    "iso8859-12",
    "iso8859-13",
    "iso8859-14",
    "iso8859-15",
    "koi8-r",
    "jisx0208.1983-0",
    "ksc5601.1987-0",
    "unknown",
    "unknown",
    "unknown",
    "utf8",
    "utf16",
    ""  // this will always return true...
};

static const QFont::CharSet charsetsIds[CHARSETS_COUNT]={
    QFont::Unicode,
    QFont::ISO_8859_1,
    QFont::ISO_8859_2,
    QFont::ISO_8859_3,
    QFont::ISO_8859_4,
    QFont::ISO_8859_5,
    QFont::ISO_8859_6,
    QFont::ISO_8859_7,
    QFont::ISO_8859_8,
    QFont::ISO_8859_9,
    QFont::ISO_8859_10,
    QFont::ISO_8859_11,
    QFont::ISO_8859_12,
    QFont::ISO_8859_13,
    QFont::ISO_8859_14,
    QFont::ISO_8859_15,
    QFont::KOI8R,
    QFont::Set_Ja,
    QFont::Set_Ko,
    QFont::Set_Th_TH,
    QFont::Set_Zh,
    QFont::Set_Zh_TW,
    QFont::Unicode,
    QFont::Unicode,
    QFont::AnyCharSet
};
*/
QFont::CharSet kvoctrainDoc::XName2CharSet (QString &s)
{
  if (s == "*-*")
   return QFont::AnyCharSet;

  else if (s == "iso8859-1")
   return QFont::ISO_8859_1;

  else if (s == "iso8859-2")
    return QFont::ISO_8859_2;

  else if (s == "iso8859-3")
    return QFont::ISO_8859_3;

  else if (s == "iso8859-4")
    return QFont::ISO_8859_4;

  else if (s == "iso8859-5")
    return QFont::ISO_8859_5;

  else if (s == "iso8859-6")
    return QFont::ISO_8859_6;

  else if (s == "iso8859-7")
    return QFont::ISO_8859_7;

  else if (s == "iso8859-8")
    return QFont::ISO_8859_8;

  else if (s == "iso8859-9")
    return QFont::ISO_8859_9;

  else if (s == "koi8-r")
    return QFont::KOI8R;

  else if (s == "koi8-1")
    return QFont::KOI8R;

  else if (s == "koi8-*")
    return QFont::KOI8R;

  else if (s == "iso8859-10")
    return QFont::ISO_8859_10;

  else if (s == "iso8859-11")
    return QFont::ISO_8859_11;

  else if (s == "iso8859-12")
    return QFont::ISO_8859_12;

  else if (s == "iso8859-13")
    return QFont::ISO_8859_13;

  else if (s == "iso8859-14")
    return QFont::ISO_8859_14;

  else if (s == "iso8859-15")
    return QFont::ISO_8859_15;

  else if (s == "jisx0208.1983-0")
    return QFont::Set_Ja;

  else if (s == "ksc5601.1987-0")
    return QFont::Set_Ko;
/*
  else if (s == KES_SET_TH_TH)
    return QFont::Set_Th_TH;

  else if (s == KES_SET_ZH)
    return QFont::Set_Zh;

  else if (s == KES_SET_ZH_TW)
    return QFont::Set_Zh_TW;
*/
  else if (s == "iso10646-1")
    return QFont::Unicode;

  else if (s == "iso10646-*")
    return QFont::Unicode;

  else if (s == "iso10646.1991-1")
    return QFont::Unicode;
/*
  else if (s == KES_SET_BIG5)
    return QFont::Set_Big5;
*/

  return QFont::AnyCharSet;

}


QString kvoctrainDoc::charSet2XName(QFont::CharSet charSet)
// from kcharset.cpp xCharsetName(QFont::CharSet charSet)
{
    switch( charSet )
    {
    case QFont::ISO_8859_1:
	return "iso8859-1";

    case QFont::ISO_8859_2:
	return "iso8859-2";

    case QFont::ISO_8859_3:
	return "iso8859-3";

    case QFont::ISO_8859_4:
	return "iso8859-4";

    case QFont::ISO_8859_5:
	return "iso8859-5";

    case QFont::ISO_8859_6:
	return "iso8859-6";

    case QFont::ISO_8859_7:
	return "iso8859-7";

    case QFont::ISO_8859_8:
	return "iso8859-8";

    case QFont::ISO_8859_9:
	return "iso8859-9";

    case QFont::KOI8R:
	return "koi8-*";

    case QFont::ISO_8859_10:
	return "iso8859-10";

    case QFont::ISO_8859_11:
	return "iso8859-11";

    case QFont::ISO_8859_12:
	return "iso8859-12";

    case QFont::ISO_8859_13:
	return "iso8859-13";

    case QFont::ISO_8859_14:
	return "iso8859-14";

    case QFont::ISO_8859_15:
	return "iso8859-15";

    case QFont::Set_Ko:
	return "ksc5601.1987-0";

    case QFont::Set_Ja:
        return "jisx0208.1983-0";

    case QFont::Unicode:
	return "iso10646-1";

    case QFont::Set_Th_TH:
    case QFont::Set_Zh:
    case QFont::Set_Zh_TW:

    case QFont::AnyCharSet:
    default:
	break;
    }
    return "*-*";
}


QString kvoctrainDoc::getCharSetString (int idx) const
{
  QFont::CharSet cs;
  if (idx >= (int)charsets.size() )
    cs = QFont::AnyCharSet;
  else
    cs = charsets[idx];

  return charSet2String (cs);
}


void kvoctrainDoc::setCharSet (int idx, const QFont::CharSet cs)
{
    if (idx >= (int)charsets.size()) {
      for (int i = (int)charsets.size(); i <= idx; i++)
        charsets.push_back (QFont::AnyCharSet);
    }
    charsets[idx] = cs;
}
#endif

class eraseTrans : public unary_function<kvoctrainExpr, void>
{

public:

  eraseTrans (int idx)
    : index (idx) {}

  void operator() (kvoctrainExpr& x) const
    {
      x.removeTranslation(index);
    }

 private:
    int index;
};


void kvoctrainDoc::removeIdent (int index)
{
  if (index < (int)langs.size() && index >= 1 ) {
    langs.erase(&langs[index], &langs[index+1]);
    for_each (vocabulary.begin(), vocabulary.end(), eraseTrans(index));
  }
}


QString kvoctrainDoc::getOriginalIdent () const
{
  if (langs.size() > 0)
    return langs[0];
  else
    return "";
}


void kvoctrainDoc::setOriginalIdent (const QString id)
{
  if (langs.size() > 0) {
    langs[0] = id;
  }
}


class sortByOrg : public binary_function<kvoctrainExpr, kvoctrainExpr, bool>
{

public:

  sortByOrg (bool _dir)
    : dir (_dir) {}

  bool operator() (const kvoctrainExpr& x, const kvoctrainExpr& y) const
    {
      return
        !dir
        ? (EA_STRICMP (x.getOriginal(),
                       y.getOriginal() ) < 0)
        : (EA_STRICMP (x.getOriginal(),
                       y.getOriginal() ) > 0);
    }

 private:
  bool          dir;
};


class sortByLessonAndOrg_alpha
  : public binary_function<kvoctrainExpr, kvoctrainExpr, bool>
{

public:

  sortByLessonAndOrg_alpha (bool _dir, kvoctrainDoc &_doc)
    : dir (_dir), doc(_doc) {}

  bool operator() (const kvoctrainExpr& x, const kvoctrainExpr& y) const
    {
      if (x.getLesson() != y.getLesson() )
        return
          !dir
          ? (EA_STRICMP (doc.getLessonDescr(x.getLesson()),
                         doc.getLessonDescr(y.getLesson()) ) < 0)
          : (EA_STRICMP (doc.getLessonDescr(x.getLesson()),
                         doc.getLessonDescr(y.getLesson()) ) > 0);
      else
        return
          !dir
          ? (EA_STRICMP (x.getOriginal(),
                         y.getOriginal() ) < 0)
          : (EA_STRICMP (x.getOriginal(),
                         y.getOriginal() ) > 0);
    }

 private:
  bool          dir;
  kvoctrainDoc &doc;
};


class sortByLessonAndOrg_index
  : public binary_function<kvoctrainExpr, kvoctrainExpr, bool>
{

public:

  sortByLessonAndOrg_index (bool _dir, kvoctrainDoc &_doc)
    : dir (_dir), doc(_doc) {}

  bool operator() (const kvoctrainExpr& x, const kvoctrainExpr& y) const
    {
      if (x.getLesson() != y.getLesson() )
        return
          !dir
          ? x.getLesson() < y.getLesson()
          : y.getLesson() < x.getLesson();
      else
        return
          !dir
          ? (EA_STRICMP (x.getOriginal(),
                         y.getOriginal() ) < 0)
          : (EA_STRICMP (x.getOriginal(),
                         y.getOriginal() ) > 0);
    }

 private:
  bool          dir;
  kvoctrainDoc &doc;
};


class sortByTrans : public binary_function<kvoctrainExpr, kvoctrainExpr, bool>
{

public:

  sortByTrans (int i, bool _dir)
    : index(i), dir (_dir) {}

  bool operator() (const kvoctrainExpr& x, const kvoctrainExpr& y) const
    {
      return
        !dir
        ? (EA_STRICMP (x.getTranslation(index),
                       y.getTranslation(index) ) < 0)
        : (EA_STRICMP (x.getTranslation(index),
                       y.getTranslation(index) ) > 0);
    }

 private:
  int  index;
  bool dir;
};


void kvoctrainDoc::sort (int index)
{
  if (index >= numLangs())
    return;

  for (int i = 0; i < (int) langs.size(); i++)
    sort_lang.push_back(false);

  if (index == 0)
    ::sort (vocabulary.begin(), vocabulary.end(), sortByOrg(sort_lang[0]));
  else
    ::sort (vocabulary.begin(), vocabulary.end(), sortByTrans(index, sort_lang[index]));
  sort_lang[index] = !sort_lang[index];
}


bool kvoctrainDoc::sortByLesson_alpha ()
{
  if (!sort_allowed)
    return false;

  ::sort (vocabulary.begin(), vocabulary.end(), sortByLessonAndOrg_alpha(sort_lesson, *this ));
  sort_lesson = !sort_lesson;

  return true;
}


bool kvoctrainDoc::sortByLesson_index ()
{
  if (!sort_allowed)
    return false;

  ::sort (vocabulary.begin(), vocabulary.end(), sortByLessonAndOrg_index(sort_lesson, *this ));
  sort_lesson = !sort_lesson;

  return true;
}


class resetAll : public unary_function<kvoctrainExpr, void>
{

public:

  resetAll (int less)
    : lesson(less) {}

  void operator() (kvoctrainExpr& x)
    {
       for (int i = 0; i <= x.numTranslations(); i++) {
         if (lesson == 0 || lesson == x.getLesson() ) {
            x.setGrade(i, KV_NORM_GRADE, false);
            x.setGrade(i, KV_NORM_GRADE, true);
            x.setQueryCount (i, 0, true);
            x.setQueryCount (i, 0, false);
            x.setBadCount (i, 0, true);
            x.setBadCount (i, 0, false);
            x.setQueryDate (i, 0, true);
            x.setQueryDate (i, 0, false);
         }
       }
    }
 private:
  int lesson;
};


class resetOne : public unary_function<kvoctrainExpr, void>
{

public:

  resetOne (int idx, int less)
    : index (idx), lesson(less) {}

  void operator() (kvoctrainExpr& x)
    {
       if (lesson == 0 || lesson == x.getLesson() ) {
         x.setGrade(index, KV_NORM_GRADE, false);
         x.setGrade(index, KV_NORM_GRADE, true);
         x.setQueryCount (index, 0, true);
         x.setQueryCount (index, 0, false);
         x.setBadCount (index, 0, true);
         x.setBadCount (index, 0, false);
         x.setQueryDate (index, 0, true);
         x.setQueryDate (index, 0, false);
       }
    }

 private:
  int index;
  int lesson;
};


void kvoctrainDoc::resetEntry (int index, int lesson)
{
  if (index < 0)
    for_each (vocabulary.begin(), vocabulary.end(), resetAll(lesson) );
  else
    for_each (vocabulary.begin(), vocabulary.end(), resetOne(index, lesson) );
}


QString kvoctrainDoc::getLessonDescr(int idx) const
{
  if (idx <= 0 || idx > (int) lesson_descr.size() )
    return "";

  return lesson_descr[idx-1];
}


vector<int> kvoctrainDoc::getLessonsInQuery() const
{
  vector<int> iqvec;
  for (unsigned i = 0; i < lessons_in_query.size(); i++)
    if (lessons_in_query[i]) {
      iqvec.push_back(i+1);   // Offset <no lesson>
//      cout << "getliq: " << i+1 << endl;
    }
  return iqvec;
}


void kvoctrainDoc::setLessonsInQuery(vector<int> lesson_iq)
{
  lessons_in_query.clear();
  for (unsigned i = 0; i < lesson_descr.size(); i++)
    lessons_in_query.push_back(false);

  for (unsigned i = 0; i < lesson_iq.size(); i++)
    if (lesson_iq[i] <= (int) lessons_in_query.size() ) {
      lessons_in_query[lesson_iq[i]-1] = true;    // Offset <no lesson>
//      cout << "setliq: " << lesson_iq[i] << " " << i << endl;
    }
}


QString kvoctrainDoc::getTitle() const
{
  return doctitle;
}


QString kvoctrainDoc::getAuthor() const
{
  return author;
}


QString kvoctrainDoc::getLicense() const
{
  return license;
}


QString kvoctrainDoc::getDocRemark() const
{
  return doc_remark;
}


void kvoctrainDoc::setTitle(QString title)
{
  doctitle = title.stripWhiteSpace();
}


void kvoctrainDoc::setAuthor(QString s)
{
  author = s.stripWhiteSpace();
}


void kvoctrainDoc::setLicense(QString s)
{
  license = s.stripWhiteSpace();
}


void kvoctrainDoc::setDocRemark(QString s)
{
  doc_remark = s.stripWhiteSpace();
}


int kvoctrainDoc::search(QString substr, int id,
                         int first, int last,
                         bool word_start,
                         bool tolerant)
{
   if (last >= numEntries()
       || last < 0 )
     last = numEntries();

   if (first < 0)
     first = 0;

   if (id >= numLangs()
      || last < first
      )
     return -1;

   if (id == 0) {
     for (int i = first; i < last; i++) {
       if (word_start) {
         if (getEntry(i)->getOriginal().find (substr, 0, false) == 0)  // case insensitive
           return i;
       }
       else {
         if (getEntry(i)->getOriginal().find (substr, 0, false) > -1)  // case insensitive
           return i;
       }
     }
   }
   else {
     for (int i = first; i < last; i++) {
       if (word_start) {
         if (getEntry(i)->getTranslation(id).find (substr, 0, false) == 0) // case insensitive
           return i;
       }
       else {
         if (getEntry(i)->getTranslation(id).find (substr, 0, false) > -1) // case insensitive
           return i;
       }
     }
   }
   return -1;
}

#define _OFFSET     0x40
#define _BITMASK    0x3F
#define _BITUSED    6

QString kvoctrainDoc::compressDate(unsigned long l) const
{
   if (l == 0)
     return "";

   QString res;
   if (l <= KVD_ZERO_TIME)
     l = 1;
   else
     l -= KVD_ZERO_TIME;
   while (l != 0) {
     char c = _OFFSET + (l & _BITMASK);
     res.insert (0, c);
     l >>= _BITUSED;
   }
   return res;
}


unsigned long kvoctrainDoc::decompressDate(QString s) const
{
   if (s.isEmpty())
     return 0;

   long res = 0;
   unsigned incr = 0;
   for (int i = s.length()-1; i >= 0; i--) {
     char c = s.local8Bit()[i];
     res += ((c - _OFFSET) & _BITMASK) << incr ;
     incr += _BITUSED;
   }
   return res > 48 ? res+KVD_ZERO_TIME : 0;  // early bug with "0"
}


kvoctrainDoc::FileType kvoctrainDoc::detectFT(const QString &filename)
{
   QFile f( filename );
   f.open( IO_ReadOnly );
   QDataStream is( &f );

   Q_INT8 c1, c2, c3, c4, c5;
   is >> c1
      >> c2
      >> c3
      >> c4
      >> c5;  // guess filetype by first x bytes

   QTextStream ts (&f);
   QString line;
   line = ts.readLine();
   line.insert (0, c5);
   line.insert (0, c4);
   line.insert (0, c3);
   line.insert (0, c2);
   line.insert (0, c1);
   f.close();

   bool stat = is.device()->status();
   if (stat != IO_Ok)
     return kvd_none;
   if (c1 == '<' && c2 == '?' && c3 == 'x' && c4 == 'm' && c5 == 'l')
     return kvtml;

   if (line.find (VCB_SEPARATOR) >= 0)
     return vt_vcb;

   if (line == LEX_IDENT_50)
     return vt_lex;

   return csv;
}


class expRef {

public:

  expRef (kvoctrainExpr *_exp, int _idx)
   {
      idx    = _idx;
      exp    = _exp;
   }

  bool operator< (const expRef& y) const
    {
      QString s1 = exp->getOriginal();
      QString s2 = y.exp->getOriginal();
      int cmp = EA_STRICMP (s1, s2);
      if (cmp != 0)
        return cmp < 0;

      for (int i = 1; i < (int) exp->numTranslations(); i++) {

        s1 = exp->getTranslation(i);
        s2 = y.exp->getTranslation(i);
        cmp = EA_STRICMP (s1, s2 );
        if (cmp != 0)
          return cmp < 0;
      }
      return cmp < 0;
    }

  int            idx;
  kvoctrainExpr *exp;
};


int kvoctrainDoc::cleanUp()
{
  int count = 0;
  kvoctrainExpr *kve1, *kve2;
  vector<expRef> shadow;
  vector<int> to_delete;

  for (int i = 0; i < (int) vocabulary.size(); i++)
    shadow.push_back (expRef (getEntry(i), i));
  ::sort(shadow.begin(), shadow.end());

#ifdef CLEAN_BUG
  ofstream sso ("shadow.out");
  for (int i = shadow.size()-1; i > 0; i--) {
    kve1 = shadow[i].exp;
    sso << kve1->getOriginal() << "  ";
    for (int l = 1; l < (int) numLangs(); l++ )
      sso << kve1->getTranslation(l)  << "  ";
    sso << endl;
  }
#endif

  int ent_no = 0;
  int ent_percent = vocabulary.size () / 100;
  float f_ent_percent = vocabulary.size () / 100.0;
  emit progressChanged(this, 0);

  for (int i = shadow.size()-1; i > 0; i--) {
    kve1 = shadow[i].exp;
    kve2 = shadow[i-1].exp;

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, (ent_no / f_ent_percent) /2.0);

    bool equal = true;
    if (kve1->getOriginal() == kve2->getOriginal() ) {
      for (int l = 1; equal && l < (int) numLangs(); l++ )
        if (kve1->getTranslation(l) != kve2->getTranslation(l))
          equal = false;

      if (equal) {
        to_delete.push_back(shadow[i-1].idx);
        count++;
      }
    }
  }

  // removing might take very long
  ent_no = 0;
  ent_percent = to_delete.size () / 100;
  f_ent_percent = to_delete.size () / 100.0;
  emit progressChanged(this, 0);

  ::sort (to_delete.begin(), to_delete.end() );
  for (int i = (int) to_delete.size()-1; i >= 0; i--) {
    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, 50 + ent_no / f_ent_percent / 2.0);
#ifdef CLEAN_BUG
    sso << getEntry(to_delete[i])->getOriginal() << endl;
#endif
    removeEntry (to_delete[i]);
    setModified();
  }

  return count;
}
#include "kvoctraindoc.moc"
