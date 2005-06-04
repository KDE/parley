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

#include <klocale.h>


bool kvoctrainDoc::parseBody_e (XmlElement elem, XmlReader& xml)
{
  grade_t       grade,
                r_grade;
  int           qcount,
                r_qcount;
  int           bcount,
                r_bcount;
  QString       remark;
  QString       pronunce;
  time_t        qdate,
                r_qdate;
  bool          inquery;
  bool          active;
  QString       lang;
  QString       textstr;
  QString       exprtype;
  bool          org_found = false;
  QString       q_org,
                q_trans;
  QString       query_id;
  kvoctrainExpr expr;
  int           lesson;
  int           width;
  QString       type;
  QString       faux_ami_f;
  QString       faux_ami_t;
  QString       synonym;
  QString       example;
  QString       antonym;
  QString       usage;
  QString       paraphrase;
  vector<Conjugation> conjug;
  Comparison     comparison;
  MultipleChoice mc;

  if (!extract_KVT_E_attr (xml, elem, lesson, inquery, active, exprtype))
    return false;

  if (lesson
      && lesson > (int) lesson_descr.size() ) { // description missing ?
    QString s;
    for (int i = lesson_descr.size(); i < lesson; i++) {
      s.setNum (i+1);
      s.insert (0, "#");  // invent descr according to number
      lesson_descr.push_back (s);
    }
  }

  if (! xml.readElement (elem) ) {
    errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
    return false;
  }

  if (elem.tag() == KV_EXPR && !elem.isEndTag() ) {
    errorKvtMl (xml.lineNumber(),
                i18n("disallowed occurrence of tag <%1>").arg(elem.tag()));
    return false;
  }

  unsigned int count = 0;
  org_found = false;
  while (elem.tag() != KV_EXPR) {

    // now want "original" and one or more "translations"

    // found original <o>

    if (elem.tag() == KV_ORG && !elem.isEndTag() ) {
      if (org_found) {
        errorKvtMl (xml.lineNumber(),
                    i18n("repeated occurrence of tag <%1>").arg(elem.tag()));
        return false;
      }
      org_found = true;

      type = exprtype;
      if (!extract_O_T_attr (
                        xml,
                        elem,
                        lang,
                        grade,  r_grade,
                        qcount, r_qcount,
                        qdate,  r_qdate,
                        remark,
                        bcount, r_bcount,
                        query_id,
                        pronunce,
                        width,
                        type,
                        faux_ami_t,
                        faux_ami_f,
                        synonym,
                        example,
                        antonym,
                        usage,
                        paraphrase))
        return false;


      if (vocabulary.size() == 0) {  // only accept in first entry
        if (width >= 0)
          setSizeHint (count, width);

        if (query_id == KV_O)
          q_org = lang;

        if (query_id == KV_T)
          q_trans = lang;
      }

      if (langs.size() == 0) {          // first entry
        if (lang.isEmpty())                 // no definition in first entry
          lang = "original";
        langs.push_back(lang);

      }
      else {
        if (lang != langs[0] && !lang.isEmpty()) {  // different originals ?
          errorKvtMl (xml.lineNumber(),
                      i18n("ambiguous definition of language code"));
          return false;
        }
      }
      count = 0;

//========================================
// FIXME:: make loop for this

      if (! xml.readElement (elem) ) {
        errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
        return false;
      }

      if (elem.tag () == KV_CONJUG_GRP && !elem.isEndTag() ) {
        if (!loadConjugKvtMl (conjug, (QString) KV_CON_TYPE, elem, xml))
          return false;

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
      }

      comparison.clear();
      if (elem.tag () == KV_COMPARISON_GRP && !elem.isEndTag() ) {
        if (!loadComparison (comparison, elem, xml))
          return false;

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
      }

      mc.clear();
      if (elem.tag () == KV_MULTIPLECHOICE_GRP && !elem.isEndTag() ) {
        if (!loadMultipleChoice (mc, elem, xml))
          return false;

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
      }

      if (elem.tag() == "#PCDATA") { // element data
        textstr = xml.getText();
        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }

        if (elem.tag() != KV_ORG || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_ORG));
          return false;
        }
      }
      else {
        if (elem.tag() != KV_ORG || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_ORG));
          return false;
        }
        textstr = "";
      }

//========================================

      expr = kvoctrainExpr (textstr);
      expr.setLesson (lesson);
      expr.setInQuery(inquery);
      expr.setActive(active);

      if (conjug.size() > 0) {
        expr.setConjugation(0, conjug[0]);
        conjug.clear();
      }
      if (!comparison.isEmpty()) {
        expr.setComparison(0, comparison);
        comparison.clear();
      }
      if (!mc.isEmpty()) {
        expr.setMultipleChoice(0, mc);
        mc.clear();
      }
      if (!remark.isEmpty() )
        expr.setRemark (0, remark);
      if (!pronunce.isEmpty() )
        expr.setPronunce (0, pronunce);
      if (!type.isEmpty() )
        expr.setType(0, type);
      if (!synonym.isEmpty() )
        expr.setSynonym(0, synonym);
      if (!example.isEmpty() )
        expr.setExample(0, example);
      if (!usage.isEmpty() )
        expr.setUsageLabel(0, usage);
      if (!paraphrase.isEmpty() )
        expr.setParaphrase(0, paraphrase);
      if (!antonym.isEmpty() )
        expr.setAntonym(0, antonym);
    }

    // found translation <t>

    else if (elem.tag() == KV_TRANS && !elem.isEndTag() ) {
      if (!org_found) {   // must be preceded by "original"
        errorKvtMl (xml.lineNumber(),
                    i18n("starting tag <%1> is missing").arg(KV_ORG));
        return false;
      }

      count++;
      type = exprtype;
      if (!extract_O_T_attr (
                        xml,
                        elem,
                        lang,
                        grade,  r_grade,
                        qcount, r_qcount,
                        qdate,  r_qdate,
                        remark,
                        bcount, r_bcount,
                        query_id,
                        pronunce,
                        width,
                        type,
                        faux_ami_f,
                        faux_ami_t,
                        synonym,
                        example,
                        antonym,
                        usage,
                        paraphrase))
       return false;

      if (vocabulary.size() == 0) {  // only accept in first entry
        if (width >= 0)
          setSizeHint (count, width);

        if (query_id == KV_O)
          q_org = lang;

        if (query_id == KV_T)
          q_trans = lang;

      }

      if (langs.size() <= count) {      // new translation
        if (lang.isEmpty()) {               // no definition in first entry ?
          lang.setNum (langs.size() );
          lang.insert (0, "translation ");
        }
        langs.push_back(lang);

      }
      else {
        if (lang != langs[count] && !lang.isEmpty()) { // different language ?
          errorKvtMl (xml.lineNumber(),
                      i18n("ambiguous definition of language code"));
          return false;
        }
      }

//========================================
// FIXME:: make loop for this

      if (! xml.readElement (elem) ) {
        errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
        return false;
      }

      if (elem.tag () == KV_CONJUG_GRP && !elem.isEndTag() ) {
        if (!loadConjugKvtMl (conjug, (QString) KV_CON_TYPE, elem, xml))
          return false;

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
      }

      comparison.clear();
      if (elem.tag () == KV_COMPARISON_GRP && !elem.isEndTag() ) {
        if (!loadComparison (comparison, elem, xml))
          return false;

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
      }

      mc.clear();
      if (elem.tag () == KV_MULTIPLECHOICE_GRP && !elem.isEndTag() ) {
        if (!loadMultipleChoice (mc, elem, xml))
          return false;

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }
      }

      textstr = "";
      if (elem.tag() == "#PCDATA") { // element data
        textstr = xml.getText();

        if (! xml.readElement (elem) ) {
          errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
          return false;
        }

        if (elem.tag() != KV_TRANS || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_TRANS));
          return false;
        }
        // expr ...
      }
      else {
        if (elem.tag() != KV_TRANS || !elem.isEndTag() ) {
          errorKvtMl (xml.lineNumber(),
                      i18n("expected ending tag <%1>").arg(KV_TRANS));
          return false;
        }
        textstr = "";
      }

//========================================

      if (!org_found) {
        errorKvtMl (xml.lineNumber(),
                    i18n("starting tag <%1> is missing").arg(KV_ORG));
        return false;
      }
/*
      if (qcount == 0) {
        grade = KV_NORM_GRADE;
      }

      if (r_qcount == 0) {
        r_grade = KV_NORM_GRADE;
      }
*/
      expr.addTranslation (textstr, grade, r_grade);
      expr.setQueryCount  (count, qcount, false);
      expr.setQueryCount  (count, r_qcount, true);
      expr.setBadCount    (count, bcount, false);
      expr.setBadCount    (count, r_bcount, true);
      expr.setQueryDate   (count, qdate, false);
      expr.setQueryDate   (count, r_qdate, true);

      if (conjug.size() > 0) {
        expr.setConjugation(count, conjug[0]);
        conjug.clear();
      }
      if (!comparison.isEmpty()) {
        expr.setComparison(count, comparison);
        comparison.clear();
      }
      if (!mc.isEmpty()) {
        expr.setMultipleChoice(count, mc);
        mc.clear();
      }
      if (!type.isEmpty() )
        expr.setType (count, type);
      if (!remark.isEmpty() )
        expr.setRemark (count, remark);
      if (!pronunce.isEmpty() )
        expr.setPronunce (count, pronunce);
      if (!faux_ami_f.isEmpty() )
        expr.setFauxAmi (count, faux_ami_f, false);
      if (!faux_ami_t.isEmpty() )
        expr.setFauxAmi (count, faux_ami_t, true);
      if (!synonym.isEmpty() )
        expr.setSynonym (count, synonym);
      if (!example.isEmpty() )
        expr.setExample (count, example);
      if (!usage.isEmpty() )
        expr.setUsageLabel (count, usage);
      if (!paraphrase.isEmpty() )
        expr.setParaphrase (count, paraphrase);
      if (!antonym.isEmpty() )
        expr.setAntonym (count, antonym);
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

    if (! xml.readElement (elem) ) {
      errorKvtMl (xml.lineNumber(), i18n("I/O failure") );
      return false;
    }
  }
  if (numEntries() == 0)
    setQueryLang (q_org, q_trans);
  vocabulary.push_back (expr);

  return true;
}


