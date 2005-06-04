/***************************************************************************

                     read big body of a kvtml document

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
#include "UsageManager.h"

#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>

bool kvoctrainDoc::extract_T_DESCR_attr (XmlReader &xml,
                                         XmlElement &elem, int &no)
{
  no = 0;
  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {
    if ((*first).name () == KV_LESS_NO)
      no = (*first).intValue();
    else {
      if (!unknownAttribute(xml.lineNumber(), "descr", (*first).name ()))
         return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::extract_T_GROUP_attr (XmlReader &xml,
                                         XmlElement &elem)
{
  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {
/*
    if ((*first).name () == KV_SIZEHINT)
      width = (*first).intValue();
    else */ {
      if (!unknownAttribute(xml.lineNumber(), "type", (*first).name ()))
         return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::loadTypeNameKvtMl (XmlElement elem, XmlReader& xml)
{
  bool endOfGroup = false;
  QString s;
  type_descr.clear();

  if (!extract_T_GROUP_attr (xml, elem))
    return false;

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_TYPE_GRP) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(KV_TYPE_GRP));
        return false;
      }
      else
        endOfGroup = true;

    }
    else if (elem.tag () == KV_TYPE_DESC && !elem.isEndTag() ) {
      int no;
      if (!extract_T_DESCR_attr (xml, elem, no))
        return false;

      if (!xml.readElement (elem)) {
        errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
        return false;
      }

      if (elem.tag() == "#PCDATA") { // element data
        s = xml.getText();

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
        else if (elem.tag() != KV_TYPE_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_TYPE_DESC));
          return false;
        }
      }
      else {
        if (elem.tag() != KV_TYPE_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_TYPE_DESC));
          return false;
        }

        s = "";
      }
      type_descr.push_back (s);
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


bool kvoctrainDoc::loadTenseNameKvtMl (XmlElement elem, XmlReader& xml)
{
  bool endOfGroup = false;
  QString s;
  tense_descr.clear();

  if (!extract_T_GROUP_attr (xml, elem))
    return false;

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_TENSE_GRP) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(KV_TENSE_GRP));
        return false;
      }
      else
        endOfGroup = true;

    }
    else if (elem.tag () == KV_TENSE_DESC && !elem.isEndTag() ) {
      int no;
      if (!extract_T_DESCR_attr (xml, elem, no))
        return false;

      if (!xml.readElement (elem)) {
        errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
        return false;
      }

      if (elem.tag() == "#PCDATA") { // element data
        s = xml.getText();

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
        else if (elem.tag() != KV_TENSE_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_TENSE_DESC));
          return false;
        }
      }
      else {
        if (elem.tag() != KV_TENSE_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_TENSE_DESC));
          return false;
        }

        s = "";
      }
      tense_descr.push_back (s);
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


bool kvoctrainDoc::loadUsageNameKvtMl (XmlElement elem, XmlReader& xml)
{
  bool endOfGroup = false;
  QString s;
  usage_descr.clear();

  if (!extract_T_GROUP_attr (xml, elem))
    return false;

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_USAGE_GRP) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(KV_USAGE_GRP));
        return false;
      }
      else
        endOfGroup = true;

    }
    else if (elem.tag () == KV_USAGE_DESC && !elem.isEndTag() ) {
      int no;
      if (!extract_T_DESCR_attr (xml, elem, no))
        return false;

      if (!xml.readElement (elem)) {
        errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
        return false;
      }

      if (elem.tag() == "#PCDATA") { // element data
        s = xml.getText();

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
        else if (elem.tag() != KV_USAGE_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_USAGE_DESC));
          return false;
        }
      }
      else {
        if (elem.tag() != KV_USAGE_DESC || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_USAGE_DESC));
          return false;
        }

        s = "";
      }
      usage_descr.push_back (s);
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


bool kvoctrainDoc::extract_O_T_attr (
                       XmlReader &xml,
                       XmlElement &elem,
                       QString &lang,
                       grade_t &grade, grade_t &rev_grade,
                       int &count, int &rev_count,
                       time_t &date, time_t &rev_date,
                       QString &remark,
                       int &bcount, int &rev_bcount,
                       QString &query_id,
                       QString &pronunce,
                       int &width,
                       QString &type,
                       QString &faux_ami_f,
                       QString &faux_ami_t,
                       QString &synonym,
                       QString &example,
                       QString &antonym,
                       QString &usage,
                       QString &paraphrase)
{
  grade = KV_NORM_GRADE;
  rev_grade = KV_NORM_GRADE;
  count = 0;
  rev_count = 0;
  bcount = 0;
  rev_bcount = 0;
  date = 0;
  rev_date = 0;
  remark = "";
  lang = "";
  query_id = "";
  pronunce = "";
  faux_ami_t = "";
  faux_ami_f = "";
  synonym = "";
  example = "";
  usage = "";
  paraphrase = "";
  antonym = "";
  width = -1;
//type = exprtype
  list<XmlAttribute>::const_iterator first = elem.attributes ().begin ();
  int pos;
  while (first != elem.attributes ().end ()) {

    if ((*first).name () == KV_LANG)
      lang = (*first).stringValue();

    else if ((*first).name () == KV_SIZEHINT)
      width = (*first).intValue();

    else if ((*first).name () == KV_CHARSET) {
    }

    else if ((*first).name () == KV_GRADE) {
      QString s = (*first).stringValue();

      if ((pos = s.find(';')) >= 1) {
        grade = s.left(pos).toInt();
        rev_grade = s.mid(pos+1, s.length()).toInt();
      }
      else
        grade = s.toInt();
    }

    else if ((*first).name () == KV_COUNT) {
      QString s = (*first).stringValue();
      if ((pos = s.find(';')) >= 1) {
        count = s.left(pos).toInt();
        rev_count = s.mid(pos+1, s.length()).toInt();
      }
      else
        count = s.toInt();
    }

    else if ((*first).name () == KV_BAD) {
      QString s = (*first).stringValue();
      if ((pos = s.find(';')) >= 1) {
        bcount = s.left(pos).toInt();
        rev_bcount = s.mid(pos+1, s.length()).toInt();
      } else
        bcount = s.toInt();
    }

    else if ((*first).name () == KV_DATE) {
      QString s = (*first).stringValue();
      if ((pos = s.find(';')) >= 1) {
      date = s.left(pos).toInt();
      rev_date = s.mid(pos+1, s.length()).toInt();
      }
      else {
        date = s.toInt();
      }
    }

    else if ((*first).name () == KV_DATE2) {
      QString s = (*first).stringValue();
      if ((pos = s.find(';')) >= 1) {
        date = decompressDate (s.left(pos));
        rev_date = decompressDate (s.mid(pos+1, s.length()));
      }
      else {
        date = decompressDate (s);
      }
    }

    else if ((*first).name () == KV_REMARK) {
      remark = (*first).stringValue();
    }

    else if ((*first).name () == KV_FAUX_AMI_F) {
      faux_ami_f = (*first).stringValue();
    }

    else if ((*first).name () == KV_FAUX_AMI_T) {
      faux_ami_t = (*first).stringValue();
    }

    else if ((*first).name () == KV_SYNONYM) {
      synonym = (*first).stringValue();
    }

    else if ((*first).name () == KV_EXAMPLE) {
      example = (*first).stringValue();
    }

    else if ((*first).name () == KV_USAGE) {
      usage = (*first).stringValue();

      if (usage.length() != 0 && usage.left(1) == UL_USER_USAGE) {
        int num = QMIN(usage.mid (1, 40).toInt(), 1000); // paranioa check
        if( num > (int) usage_descr.size() ) { // description missing ?
          QString s;
          for (int i = usage_descr.size(); i < num; i++) {
            s.setNum (i+1);
            s.insert (0, "#");  // invent descr according to number
            usage_descr.push_back (s);
          }
        }
      }
    }

    else if ((*first).name () == KV_PARAPHRASE) {
      paraphrase = (*first).stringValue();
    }

    else if ((*first).name () == KV_ANTONYM) {
      antonym = (*first).stringValue();
    }

    else if ((*first).name () == KV_EXPRTYPE) {
      type = (*first).stringValue();
      if (type == "1")
        type = QM_VERB;
      else if (type == "2")  // convert from pre-0.5 versions
        type = QM_NOUN;
      else if (type == "3")
        type = QM_NAME;

      if (type.length() != 0 && type.left(1) == QM_USER_TYPE) {
        int num = QMIN(type.mid (1, 40).toInt(), 1000); // paranoia check
        if( num > (int) type_descr.size() ) { // description missing ?
          QString s;
          for (int i = type_descr.size(); i < num; i++) {
            s.setNum (i+1);
            s.insert (0, "#");  // invent descr according to number
            type_descr.push_back (s);
          }
        }
      }
    }

    else if ((*first).name () == KV_PRONUNCE) {
      pronunce = (*first).stringValue();
    }

    else if ((*first).name () == KV_QUERY) {
      query_id = (*first).stringValue();
    }

    else {
      if (!unknownAttribute(xml.lineNumber(), "o> or <t", (*first).name ()))
        return false;
    }
    first++;
  }

  return true;
}


bool kvoctrainDoc::extract_KVT_E_attr (
                         XmlReader& xml,
                         XmlElement   &elem,
                         int          &lesson,
                         bool         &sel,
                         bool         &active,
                         QString      &type)
{
  lesson = 0;
  sel = false;
  active = true;
  type = "";
  list<XmlAttribute>::const_iterator first =  elem.attributes ().begin ();
  while (first != elem.attributes ().end ()) {
    if ((*first).name () == KV_LESS_MEMBER)
      lesson = (*first).intValue();
    else if ((*first).name () == KV_SELECTED)
      sel = (*first).intValue() != 0;
    else if ((*first).name () == KV_INACTIVE)
      active = !(*first).intValue() != 0;
    else if ((*first).name () == KV_EXPRTYPE) {
      type = (*first).stringValue();
      if (type == "1")
        type = QM_VERB;
      else if (type == "2")  // convert from pre-0.5 versions
        type = QM_NOUN;
      else if (type == "3")
        type = QM_NAME;

      if (type.length() != 0 && type.left(1) == QM_USER_TYPE) {
        int num = QMIN(type.mid (1, 40).toInt(), 1000); // paranoia check
        if( num > (int) type_descr.size() ) { // description missing ?
          QString s;
          for (int i = type_descr.size(); i < num; i++) {
            s.setNum (i+1);
            s.insert (0, "#");  // invent descr according to number
            type_descr.push_back (s);
          }
        }
      }
    }
    else {
      if (!unknownAttribute(xml.lineNumber(), "e", (*first).name ()))
        return false;
    }
    first++;
  }
  return true;
}


bool kvoctrainDoc::loadComparison (Comparison &comp, XmlElement elem,
                                   XmlReader &xml)
/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
  bool endOfGroup = false;
  QString s;
  comp.clear();

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_COMPARISON_GRP) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(elem.tag()));
        return false;
      }
      else
        endOfGroup = true;

    }

    else if (elem.tag () == KV_COMP_L1 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_COMP_L1, xml, elem, s))
        return false;
      comp.setL1(s);
    }

    else if (elem.tag () == KV_COMP_L2 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_COMP_L2, xml, elem, s))
        return false;
      comp.setL2(s);
    }

    else if (elem.tag () == KV_COMP_L3 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_COMP_L3, xml, elem, s))
        return false;
      comp.setL3(s);
    }

    else {
      if (elem.isEndTag() ) {
        errorKvtMl (xml.lineNumber(),
                    i18n("unexpected ending tag <%1>").arg(elem.tag()));
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


bool kvoctrainDoc::loadMultipleChoice (MultipleChoice &mc, XmlElement elem,
                                       XmlReader &xml)
/*
 <multiplechoice>
   <mc1>good</mc1>
   <mc2>better</mc2>
   <mc3>best</mc3>
   <mc4>best 2</mc4>
   <mc5>best 3</mc5>
 </multiplechoice>
*/

{
  bool endOfGroup = false;
  QString s;
  mc.clear();

  do {
    if (! xml.readElement (elem))
      break;

    if (elem.tag () == KV_MULTIPLECHOICE_GRP) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(elem.tag()));
        return false;
      }
      else
        endOfGroup = true;

    }

    else if (elem.tag () == KV_MC_1 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_MC_1, xml, elem, s))
        return false;
      mc.setMC1(s);
    }

    else if (elem.tag () == KV_MC_2 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_MC_2, xml, elem, s))
        return false;
      mc.setMC2(s);
    }

    else if (elem.tag () == KV_MC_3 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_MC_3, xml, elem, s))
        return false;
      mc.setMC3(s);
    }

    else if (elem.tag () == KV_MC_4 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_MC_4, xml, elem, s))
        return false;
      mc.setMC4(s);
    }

    else if (elem.tag () == KV_MC_5 && !elem.isEndTag() ) {
      if (!extract_simple_tag (KV_MC_5, xml, elem, s))
        return false;
      mc.setMC5(s);
    }

    else {
      if (elem.isEndTag() ) {
        errorKvtMl (xml.lineNumber(),
                    i18n("unexpected ending tag <%1>").arg(elem.tag()));
        return false;
      }
      else {
        unknownElement (xml.lineNumber(), elem.tag() );
        return false;
      }
    }
  } while (! endOfGroup);

  mc.normalize();
  return true;
}


bool kvoctrainDoc::parseBody_kvtml (XmlElement elem, XmlReader& xml)
{
  bool endOfBody = false;
  bool lessgroup = false;
  bool optgroup = false;
  bool attrgroup = false;
  bool tensegroup = false;
  bool usagegroup = false;
  bool articlegroup = false;
  bool conjuggroup = false;

  int ent_no = 0;
  int ent_percent = (int) lines / 100;
  float f_ent_percent = (int) lines / 100.0;
  if (lines != 0)
    emit progressChanged(this, 0);

  do {
    if (! xml.readElement (elem) ) {
      errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
      return false;
    }

    if (elem.tag () == KV_DOCTYPE) {
      if (! elem.isEndTag ()) {
        errorKvtMl (xml.lineNumber(),
                    i18n("expected ending tag <%1>").arg(elem.tag()));
        return false;
      }
      else
        endOfBody = true;
    }

    else if (elem.tag () == KV_LESS_GRP && !elem.isEndTag() ) {
      if (lessgroup) {
        errorKvtMl (xml.lineNumber(),
                    i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
        return false;
      }
      lessgroup = true;
      if (!loadLessonKvtMl (elem, xml))
        return false;
    }

    else if (elem.tag () == KV_ARTICLE_GRP && !elem.isEndTag() ) {
      if (articlegroup) {
        errorKvtMl (xml.lineNumber(),
                    i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
        return false;
      }
      articlegroup = true;
      if (!loadArticleKvtMl (elem, xml))
        return false;
    }

    else if (elem.tag () == KV_CONJUG_GRP && !elem.isEndTag() ) {
      if (conjuggroup) {
         errorKvtMl (xml.lineNumber(),
                     i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
         return false;
      }
      conjuggroup = true;
      if (!loadConjugKvtMl (conjugations, KV_CON_ENTRY, elem, xml))
        return false;
    }

    else if (elem.tag () == KV_OPTION_GRP && !elem.isEndTag() ) {
      if (optgroup)  {
        errorKvtMl (xml.lineNumber(),
                    i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
        return false;
      }
      optgroup = true;
      if (!loadOptionsKvtMl (elem, xml))
        return false;
    }

    else if (elem.tag () == KV_TYPE_GRP && !elem.isEndTag() ) {
      if (attrgroup) {
        errorKvtMl (xml.lineNumber(),
                    i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
        return false;
      }
      attrgroup = true;
      if (!loadTypeNameKvtMl (elem, xml))
        return false;
    }

    else if (elem.tag () == KV_TENSE_GRP && !elem.isEndTag() ) {
      if (tensegroup) {
        errorKvtMl (xml.lineNumber(),
                    i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
        return false;
      }
      tensegroup = true;
      if (!loadTenseNameKvtMl (elem, xml))
        return false;
//    Conjugation::setTenseNames(tense_descr);
    }

    else if (elem.tag () == KV_USAGE_GRP && !elem.isEndTag() ) {
      if (usagegroup) {
        errorKvtMl (xml.lineNumber(),
                    i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
        return false;
      }
      usagegroup = true;
      if (!loadUsageNameKvtMl (elem, xml))
        return false;
    }

    else if (elem.tag () == KV_EXPR && !elem.isEndTag() ) {
      // found expression <e>

      if (lines != 0) {
        ent_no++;
        if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
          emit progressChanged(this, int(ent_no / f_ent_percent));
      }
      if (!parseBody_e (elem, xml)) return false;
    }

    else {
      if (elem.isEndTag() ) {
        errorKvtMl (xml.lineNumber(),
                    i18n("unexpected ending tag <%1>" ).arg(elem.tag()));
        return false;
      }
      else {
        unknownElement (xml.lineNumber(), elem.tag());
        return false;
      }
    }

  } while (! endOfBody);

  return true;
}


bool kvoctrainDoc::unknownAttribute (int line, const QString &name,
                                     const QString &attr )
{
   if (unknown_attr) // show dialog only once
     return true;    // proceed

   unknown_attr = true;

   QString ln = i18n("File:\t%1\nLine:\t%2\n").arg(URL().path()).arg(line);

   QString format = i18n(
      "Your document contains an unknown attribute <%1> "   // keep trailing space
       "in tag <%2>.\n"
      "Maybe your version of KVocTrain is too old, "
      "or the document is damaged.\n"
      "If you proceed and save afterwards you are likely to lose data;\n"
      "do you want to proceed anyway?\n"
     );
   QString msg = format.arg(attr).arg(name);

   QApplication::setOverrideCursor( arrowCursor, true );
   QString s = kapp->makeStdCaption(i18n("Unknown attribute"));
   bool result = (KMessageBox::warningContinueCancel(0, ln+msg, s) == KMessageBox::Continue);
   QApplication::restoreOverrideCursor();
   return result;
}


void kvoctrainDoc::unknownElement (int line, const QString &elem )
{
   unknown_elem = true;

   QString ln = i18n("File:\t%1\nLine:\t%2\n").arg(URL().path()).arg(line);

   QString format = i18n(
      "Your document contains an unknown tag <%1>.  "  // keep trailing space
      "Maybe your version of KVocTrain is too old, "
      "or the document is damaged.\n"
      "Loading is aborted because KVocTrain cannot "
      "read documents with unknown elements.\n"
     );
   QString msg = format.arg(elem);
   QApplication::setOverrideCursor( arrowCursor, true );
   QString s = kapp->makeStdCaption(i18n("Unknown element"));
   KMessageBox::sorry(0, ln+msg, s);
   QApplication::restoreOverrideCursor();
}


void kvoctrainDoc::errorKvtMl (int line, const QString &text )
{
   QApplication::setOverrideCursor( arrowCursor, true );
   QString s = kapp->makeStdCaption(i18n("Error"));
   QString ln = i18n("File:\t%1\nLine:\t%2\n").arg(URL().path()).arg(line);

   QString msg = text;
   KMessageBox::error(0, ln+msg, s);
   QApplication::restoreOverrideCursor();
}


void kvoctrainDoc::warningKvtMl (int line, const QString &text )
{
   QApplication::setOverrideCursor( arrowCursor, true );
   QString s = kapp->makeStdCaption(i18n("Warning"));
   QString ln = i18n("File:\t%1\nLine:\t%2\n").arg(URL().path()).arg(line);
   QString msg = text;
   KMessageBox::information(0, ln+msg, s);
   QApplication::restoreOverrideCursor();
}
