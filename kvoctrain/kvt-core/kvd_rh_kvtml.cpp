/***************************************************************************

                   read header parts of a kvtml document

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999

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

#include "QueryManager.h"

#include <klocale.h>

#include <qtextcodec.h>

bool kvoctrainDoc::loadFromKvtMl (QTextStream& is)
{
  // important, first action
  // switch to actual encoding afterwards, otherwise blocked
  is.setCodec(QTextCodec::codecForName("UTF-8"));
  is.setEncoding(QTextStream::UnicodeUTF8);

  langs.clear();
  vocabulary.clear();
  XmlReader xml (is);

  if (! xml.validHeader ()) {
    errorKvtMl (xml.lineNumber(), i18n("invalid xml file header") );
    return false;
  }

  generator = "";
  cols = 0;
  lines = 0;
  doctitle = "";
  author = "";
  license = "";
  doc_remark = "";
  XmlElement elem;
  if (! xml.readElement (elem) ) {
    errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
    return false;
  }

  if (elem.tag () != KV_DOCTYPE) {
    errorKvtMl (xml.lineNumber(),
                i18n("Tag <%1> was expected "
                     "but tag <%2> was read." ).arg(KV_DOCTYPE).arg(elem.tag()));
    return false;
  }

  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  doctitle = i18n("Untitled");
  while (first != elem.attributes ().end ()) {

    if ((*first).name () == KV_ENCODING) {
  
      if ((*first).stringValue().upper() == (QString)"UTF-8" ) {
        is.setCodec(QTextCodec::codecForName("UTF-8"));
        is.setEncoding(QTextStream::UnicodeUTF8);
      }
      else if ((*first).stringValue().upper() == (QString)"8BIT" ) {  // old compatibility mode
        is.setCodec(QTextCodec::codecForName("ISO 8859-1"));
        is.setEncoding(QTextStream::Latin1);
      }
      else {
        is.setCodec(QTextCodec::codecForName("ISO 8859-1"));
        is.setEncoding(QTextStream::Latin1);
        QString format = i18n("Unknown document encoding \"%1\" was encountered.\n\n"
				"It will be ignored. Encoding is now \"%2\".");
        QString msg =format.arg((*first).stringValue ().upper()).arg("ISO 8859-1");
        warningKvtMl (xml.lineNumber(), msg);
      }
  
    }
    else if ((*first).name () == KV_TITLE) {
        doctitle = (*first).stringValue ();
    }
    else if ((*first).name () == KV_AUTHOR) {
      author = (*first).stringValue ();
    }
    else if ((*first).name () == KV_LICENSE) {
      license = (*first).stringValue ();
    }
    else if ((*first).name () == KV_DOC_REM) {
      doc_remark = (*first).stringValue ();
    }
    else if ((*first).name () == KV_GENERATOR) {
      generator = (*first).stringValue ();
      int pos = generator.findRev (KVD_VERS_PREFIX);
      if (pos >= 0) {
        doc_version = generator;
        doc_version.remove (0, pos+2);
      }
    }
    else if ((*first).name () == KV_COLS) {
      cols = (*first).intValue ();
    }
    else if ((*first).name () == KV_LINES) {
      lines = (*first).intValue ();
    }
    else {
      if (!unknownAttribute(xml.lineNumber(), "kvtml", (*first).name ()))
        return false;
    }

    first++;
  }

  bool result = parseBody_kvtml (elem, xml);  // read vocabulary

  setModified (false);
  return result;
}


bool kvoctrainDoc::extract_L_DESCR_attr (XmlReader &xml,
                                         XmlElement &elem, int &no,
                                         bool &isCurr, bool &inQuery)
{
  no = 0;
  isCurr = false;
  inQuery = false;
  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {
    if ((*first).name () == KV_LESS_NO)
      no = (*first).intValue();
    else if ((*first).name () == KV_LESS_CURR)
      isCurr = (*first).intValue() != 0;
    else if ((*first).name () == KV_LESS_QUERY)
      inQuery = (*first).intValue() != 0;
    else {
      if (!unknownAttribute(xml.lineNumber(), "descr", (*first).name ()))
         return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::check_Empty_attr (QString tag_name,
                                     XmlReader &xml,
                                     XmlElement &elem)
{
  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  if (first != elem.attributes ().end ()) {
    if (!unknownAttribute(xml.lineNumber(), tag_name, (*first).name ()))
      return false;
  }
  return true;
}


bool kvoctrainDoc::extract_simple_tag (QString tag_name,
                                XmlReader& xml,
                                XmlElement &elem,
                                QString &data,
                                bool check_it)
{
  if (check_it)
    if (!check_Empty_attr (tag_name, xml, elem))
      return false;

  if (! xml.readElement (elem) ) {
    errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
    return false;
  }

  if (elem.tag() == "#PCDATA") { // element data
    data = xml.getText();
    if (! xml.readElement (elem) ) {
      errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
      return false;
    }
    else if (elem.tag() != tag_name || !elem.isEndTag() ) {
      errorKvtMl (xml.lineNumber(), i18n("expected ending tag <%1>").arg(tag_name));
      return false;
    }
  }
  else {
    if (elem.tag() != tag_name || !elem.isEndTag() ) {
      errorKvtMl (xml.lineNumber(), i18n("expected ending tag <%1>").arg(tag_name));
      return false;
    }
    data = "";
  }

  return true;
}


bool kvoctrainDoc::extract_CON_E_attr (XmlReader& xml,
                                       XmlElement &elem,
                                       QString &lang)
{
  lang = "";

  list<XmlAttribute>::const_iterator first = elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {

    if ((*first).name () == KV_LANG)
      lang = (*first).stringValue();

    else {
      if (!unknownAttribute(xml.lineNumber(), "e", (*first).name ()))
        return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::loadArticleKvtMl (XmlElement elem, XmlReader& xml)

/*
 <article>
  <e l="de">        lang determines also lang order in entries !!
   <fi>eine</fi>    which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/

{
  bool endOfGroup = false;
  bool inEntry = false;
  QString s;
  articles.clear();

  if (!check_Empty_attr (KV_ARTICLE_GRP, xml, elem))
    return false;

  QString fem_def,
          mal_def,
          nat_def,
          fem_indef,
          mal_indef,
          nat_indef;
  QString lang;
  int count = 0;

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_ARTICLE_GRP) {
      if (inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_ARTICLE_GRP));
        return false;
      }
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(KV_ARTICLE_GRP));
        return false;
      }
      else
        endOfGroup = true;
    }
    else if (elem.tag () == KV_ART_ENTRY) {
      if (! elem.isEndTag ()) {
        if (inEntry) {
          errorKvtMl (xml.lineNumber(),
                      i18n("disallowed occurrence of tag <%1>").arg(KV_ART_ENTRY));
          return false;
        }
        inEntry = true;

        if (!extract_CON_E_attr (xml, elem, lang) )
          return false;

        if ((int)langs.size() <= count) {      // first entry
          if (lang.isEmpty())                      // no definition in first entry
            lang = "original";
          langs.push_back(lang);
        }
        else {
          if (lang != langs[count] && !lang.isEmpty()) {  // different originals ?
            errorKvtMl (xml.lineNumber(),
                        i18n("ambiguous definition of language code"));
            return false;
          }
        }
        fem_def = "";
        mal_def = "";
        nat_def = "";
        fem_indef = "";
        mal_indef = "";
        nat_indef = "";
      }
      else {
        count++;
        inEntry = false;
        articles.push_back (Article(fem_def, fem_indef,
                                    mal_def, mal_indef,
                                    nat_def, nat_indef));
      }
    }
    else if (elem.tag () == KV_ART_FD && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_ART_FD));
        return false;
      }
      if (!extract_simple_tag (KV_ART_FD, xml, elem, fem_def))
        return false;
    }
    else if (elem.tag () == KV_ART_FI && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_ART_FI));
        return false;
      }
      if (!extract_simple_tag (KV_ART_FI, xml, elem, fem_indef))
        return false;
    }
    else if (elem.tag () == KV_ART_MD && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_ART_MD));
        return false;
      }
      if (!extract_simple_tag (KV_ART_MD, xml, elem, mal_def))
        return false;
    }
    else if (elem.tag () == KV_ART_MI && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_ART_MI));
        return false;
      }
      if (!extract_simple_tag (KV_ART_MI, xml, elem, mal_indef))
        return false;
    }
    else if (elem.tag () == KV_ART_ND && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_ART_ND));
        return false;
      }
      if (!extract_simple_tag (KV_ART_ND, xml, elem, nat_def))
        return false;
    }
    else if (elem.tag () == KV_ART_NI && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_ART_NI));
        return false;
      }
      if (!extract_simple_tag (KV_ART_NI, xml, elem, nat_indef))
        return false;
    }
    else {
      if (elem.isEndTag() ) {
        errorKvtMl (xml.lineNumber(),
                    i18n("unexpected ending tag <%1>" ).arg(elem.tag()));
        return false;
      }
      else {
        unknownElement (xml.lineNumber(), elem.tag() );
        return false;
      }
    }
  } while (! endOfGroup);

  return true;
}


bool kvoctrainDoc::extract_T_attr (XmlReader &xml,
                                   XmlElement &elem,
                                   QString &name)
{
  name = "";
  list<XmlAttribute>::const_iterator first = elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {

    if ((*first).name () == KV_CON_NAME)
      name = (*first).stringValue();

    else {
      if (!unknownAttribute(xml.lineNumber(), "e", (*first).name ()))
        return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::extract_BOOL_attr (
                               XmlReader &xml,
                               XmlElement &elem,
                               const QString &tag_name,
                               const QString &attr_name,
                               bool &flag)
{
  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {
    if ((*first).name () == attr_name)
      flag = (bool) (*first).intValue();
    else {
      if (!unknownAttribute(xml.lineNumber(), tag_name, (*first).name ()))
         return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::loadConjugKvtMl (vector<Conjugation> &curr_conjug,
                                    const QString &entry_tag,
                                    XmlElement elem, XmlReader& xml)

/*
 <conjugation>        used in header for definiton of "prefix"
  <e l="de">          lang determines also lang order in entries !!
   <s1>I</s1>         which must NOT differ
   <s2>you<2>
   <s3f>he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f>they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

 <conjugation>        and in entry for definition of tenses of (irreg.) verbs
  <t n="sipa">
   <s1>go</s1>
   <s2>go</s2>
   <s3f>goes</s3f>
   <s3m>goes</s3m>
   <s3n>goes</s3n>
   <p1>go</p1>
   <p2>go</p2>
   <p3f>go</p3f>
   <p3m>go</p3m>
   <p3n>go</p3n>
  </t>
 </conjugation>
*/

{
  bool endOfGroup = false;
  bool inEntry = false;
  QString s;

  curr_conjug.clear();

  if (!check_Empty_attr (KV_CONJUG_GRP, xml, elem))
    return false;

  bool    p3_common,
          s3_common;
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

  QString lang;
  QString type;
  int count = 0;
  curr_conjug.push_back (Conjugation());

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_CONJUG_GRP) {
      if (inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CONJUG_GRP));
        return false;
      }
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(elem.tag()));
        return false;
      }
      else
        endOfGroup = true;
    }
    else if (elem.tag () == entry_tag) {
      if (! elem.isEndTag ()) {
        if (inEntry) {
          errorKvtMl (xml.lineNumber(),
                      i18n("disallowed occurrence of tag <%1>").arg(entry_tag));
          return false;
        }
        inEntry = true;

        if (entry_tag == KV_CON_ENTRY) {
          type = CONJ_PREFIX;
          if (!extract_CON_E_attr (xml, elem, lang) )
            return false;

          if ((int)langs.size() <= count) { // first entry
            if (lang.isEmpty())                 // no definition in first entry
              lang = "original";
            langs.push_back(lang);
          }
          else {
            if (lang != langs[count] && !lang.isEmpty()) {  // different originals ?
              errorKvtMl (xml.lineNumber(),
                          i18n("ambiguous definition of language code"));
              return false;
            }
          }
        }
        else if (entry_tag == KV_CON_TYPE) {
          if (!extract_T_attr (xml, elem, type))
            return false;
          if (type.length() != 0 && type.left(1) == UL_USER_TENSE) {
            int num = QMIN(type.mid (1, 40).toInt(), 1000); // paranoia check
            if( num > (int) tense_descr.size() ) { // description missing ?
              QString s;
              for (int i = tense_descr.size(); i < num; i++) {
                s.setNum (i+1);
                s.insert (0, "#");  // invent descr according to number
                tense_descr.push_back (s);
              }
            }
          }
        }

        pers1_sing = "";
        pers2_sing = "";
        pers3_m_sing = "";
        pers3_f_sing = "";
        pers3_n_sing = "";
        pers1_plur = "";
        pers2_plur = "";
        pers3_m_plur = "";
        pers3_f_plur = "";
        pers3_n_plur = "";
        p3_common = false;
        s3_common = false;
      }
      else {
        inEntry = false;
        if (entry_tag == KV_CON_ENTRY)
          while (count+1 > (int) curr_conjug.size() )
            curr_conjug.push_back (Conjugation());

        curr_conjug[count].setPers3SingularCommon(type, s3_common);
        curr_conjug[count].setPers3PluralCommon(type, p3_common);
        curr_conjug[count].setPers1Singular(type, pers1_sing);
        curr_conjug[count].setPers2Singular(type, pers2_sing);
        curr_conjug[count].setPers3FemaleSingular(type, pers3_f_sing);
        curr_conjug[count].setPers3MaleSingular(type, pers3_m_sing);
        curr_conjug[count].setPers3NaturalSingular(type, pers3_n_sing);
        curr_conjug[count].setPers1Plural(type, pers1_plur);
        curr_conjug[count].setPers2Plural(type, pers2_plur);
        curr_conjug[count].setPers3FemalePlural(type, pers3_f_plur);
        curr_conjug[count].setPers3MalePlural(type, pers3_m_plur);
        curr_conjug[count].setPers3NaturalPlural(type, pers3_n_plur);

        if (entry_tag == KV_CON_ENTRY)
          count++;
      }
    }
    else if (elem.tag () == KV_CON_P1S && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P1S));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P1S, xml, elem, pers1_sing) )
        return false;
    }
    else if (elem.tag () == KV_CON_P2S && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P2S));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P2S, xml, elem, pers2_sing))
        return false;
    }
    else if (elem.tag () == KV_CON_P3SF && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P3SF));
        return false;
      }

      if (!extract_BOOL_attr (xml, elem,KV_CON_P3SF, KV_CONJ_COMMON,
                              s3_common))
        return false;

      if (!extract_simple_tag (KV_CON_P3SF, xml, elem, pers3_f_sing, false))
        return false;
    }
    else if (elem.tag () == KV_CON_P3SM && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P3SM));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P3SM, xml, elem, pers3_m_sing))
        return false;
    }
    else if (elem.tag () == KV_CON_P3SN && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P3SN));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P3SN, xml, elem, pers3_n_sing))
        return false;
    }
    else if (elem.tag () == KV_CON_P1P && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P1P));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P1P, xml, elem, pers1_plur) )
        return false;
    }
    else if (elem.tag () == KV_CON_P2P && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P2P));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P2P, xml, elem, pers2_plur))
        return false;
    }
    else if (elem.tag () == KV_CON_P3PF && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P3PF));
        return false;
      }

      if (!extract_BOOL_attr (xml, elem, KV_CON_P3PF, KV_CONJ_COMMON,
                              p3_common))
        return false;

      if (!extract_simple_tag (KV_CON_P3PF, xml, elem, pers3_f_plur, false))
        return false;
    }
    else if (elem.tag () == KV_CON_P3PM && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P3PM));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P3PM, xml, elem, pers3_m_plur))
        return false;
    }
    else if (elem.tag () == KV_CON_P3PN && !elem.isEndTag() ) {
      if (!inEntry) {
        errorKvtMl (xml.lineNumber(),
                    i18n("disallowed occurrence of tag <%1>").arg(KV_CON_P3PN));
        return false;
      }
      if (!extract_simple_tag (KV_CON_P3PN, xml, elem, pers3_n_plur))
        return false;
    }
    else {
      if (elem.isEndTag() ) {
        errorKvtMl (xml.lineNumber(),
                    i18n("unexpected ending tag <%1>" ).arg(elem.tag()));
        return false;
      }
      else {
        unknownElement (xml.lineNumber(), elem.tag() );
        return false;
      }
    }
  } while (! endOfGroup);

  return true;
}


bool kvoctrainDoc::extract_L_GROUP_attr (XmlReader &xml,
                                         XmlElement &elem,
                                         int &width)
{
  width = 0;
  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {
    if ((*first).name () == KV_SIZEHINT)
      width = (*first).intValue();
    else {
      if (!unknownAttribute(xml.lineNumber(), "lesson", (*first).name ()))
         return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::loadLessonKvtMl (XmlElement elem, XmlReader& xml)
{
  bool endOfGroup = false;
  QString s;
  bool isCurr;
  bool inQuery = false;
  lesson_descr.clear();
  int width;

  if (!extract_L_GROUP_attr (xml, elem, width))
    return false;
  setSizeHint (-1, width);

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_LESS_GRP) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(elem.tag()));
        return false;
      }
      else
        endOfGroup = true;

    }
    else if (elem.tag () == KV_LESS_DESC && !elem.isEndTag() ) {
      int no;
      if (!extract_L_DESCR_attr (xml, elem, no, isCurr, inQuery))
        return false;

      lessons_in_query.push_back(inQuery);

      if (isCurr && no != 0)
        setCurrentLesson (no);

      if (! xml.readElement (elem) ) {
        errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
        return false;
      }

      if (elem.tag() == "#PCDATA") { // element data
        s = xml.getText();

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }

        if (elem.tag() != KV_LESS_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_LESS_DESC));
          return false;
        }
      }
      else {
        if (elem.tag() != KV_LESS_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_LESS_DESC));
          return false;
        }
        s = "";
      }
      lesson_descr.push_back (s);
    }
    else {
      if (elem.isEndTag() ) {
        errorKvtMl (xml.lineNumber(),
                    i18n("unexpected ending tag <%1>" ).arg(elem.tag()));
        return false;
      }
      else {
        unknownElement (xml.lineNumber(), elem.tag() );
        return false;
      }
    }
  } while (! endOfGroup);

  return true;
}


bool kvoctrainDoc::loadOptionsKvtMl (XmlElement elem, XmlReader& xml)
{
  bool endOfGroup = false;
  QString s;

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_OPTION_GRP) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(elem.tag()));
        return false;
      }
      else
        endOfGroup = true;

    }
    else if (elem.tag () == KV_OPT_SORT) {
      sort_allowed = true;

      if (!extract_BOOL_attr (xml, elem, KV_OPTION_GRP, KV_BOOL_FLAG,
                              sort_allowed))
        return false;

    }
    else {
      if (elem.isEndTag() ) {
        errorKvtMl (xml.lineNumber(),
                    i18n("unexpected ending tag <%1>" ).arg(elem.tag()));
        return false;
      }
      else {
        unknownElement (xml.lineNumber(), elem.tag() );
        return false;
      }
    }
  } while (! endOfGroup);

  return true;
}

