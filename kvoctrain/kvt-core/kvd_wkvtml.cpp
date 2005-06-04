/***************************************************************************

                         write a kvtml document

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

#include "kvoctraincore.h"
#include "QueryManager.h"

#include <kdebug.h>

#include <qtextcodec.h>

bool kvoctrainDoc::saveTypeNameKvtMl (XmlWriter &xml)
{
  if (type_descr.size() == 0)
    return true;

  xml.writeText(" ");
  xml.startTag (KV_TYPE_GRP, true, false, true);

  for (int lfn = 0; lfn < (int) type_descr.size(); lfn++) {
    if (!type_descr[lfn].isNull() ) {
      xml.writeText("  ");
      xml.startTag (KV_TYPE_DESC, false);
      xml.addAttribute (KV_TYPE_NO, lfn+1 );
      xml.closeTag();
      xml.writeText (type_descr[lfn]);
      xml.endTag (KV_TYPE_DESC, true);
    }
  }

  xml.writeText(" ");
  xml.endTag (KV_TYPE_GRP, true);
  xml.writeText("\n");
  return true;
}


bool kvoctrainDoc::saveTenseNameKvtMl (XmlWriter &xml)
{
  if (tense_descr.size() == 0)
    return true;

  xml.writeText(" ");
  xml.startTag (KV_TENSE_GRP, true, false, true);

  for (int lfn = 0; lfn < (int) tense_descr.size(); lfn++) {
    if (!tense_descr[lfn].isNull() ) {
      xml.writeText("  ");
      xml.startTag (KV_TENSE_DESC, false);
      xml.addAttribute (KV_TENSE_NO, lfn+1 );
      xml.closeTag();
      xml.writeText (tense_descr[lfn]);
      xml.endTag (KV_TENSE_DESC, true);
    }
  }

  xml.writeText(" ");
  xml.endTag (KV_TENSE_GRP, true);
  xml.writeText("\n");
  return true;
}


bool kvoctrainDoc::saveUsageNameKvtMl (XmlWriter &xml)
{
  if (usage_descr.size() == 0)
    return true;

  xml.writeText(" ");
  xml.startTag (KV_USAGE_GRP, true, false, true);

  for (int lfn = 0; lfn < (int) usage_descr.size(); lfn++) {
    if (!usage_descr[lfn].isNull() ) {
      xml.writeText("  ");
      xml.startTag (KV_USAGE_DESC, false);
      xml.addAttribute (KV_USAGE_NO, lfn+1 );
      xml.closeTag();
      xml.writeText (usage_descr[lfn]);
      xml.endTag (KV_USAGE_DESC, true);
    }
  }

  xml.writeText(" ");
  xml.endTag (KV_USAGE_GRP, true);
  xml.writeText("\n");
  return true;
}


bool kvoctrainDoc::saveLessonKvtMl (XmlWriter &xml)
{
  if (lesson_descr.size() == 0)
    return true;

  xml.writeText(" ");            // indent entry a bit
  xml.startTag (KV_LESS_GRP, false);
  xml.addAttribute (KV_SIZEHINT, getSizeHint (-1));
  xml.closeTag(false, true);

  for (int lfn = 0; lfn < (int) lesson_descr.size(); lfn++) {
    if (!lesson_descr[lfn].isNull() ) {
      xml.writeText("  ");
      xml.startTag (KV_LESS_DESC, false);
      xml.addAttribute (KV_LESS_NO, lfn+1 );
      if (getCurrentLesson() == lfn+1)
        xml.addAttribute (KV_LESS_CURR, (QString) "1" );
      if (lfn < (int) lessons_in_query.size() && lessons_in_query[lfn])
        xml.addAttribute (KV_LESS_QUERY, (QString) "1" );
      xml.closeTag();
      xml.writeText (lesson_descr[lfn]);
      xml.endTag (KV_LESS_DESC, true);
    }
  }

  xml.writeText(" ");
  xml.endTag (KV_LESS_GRP, true);
  xml.writeText("\n");
  return true;
}


bool kvoctrainDoc::saveConjug (const Conjugation &curr_conjug, QString type,
                               XmlWriter &xml, QString identstr)
{
  bool linefeed = type == CONJ_PREFIX;
  if (linefeed)
    identstr+"  ";
  else
    identstr = "";

  if (!curr_conjug.pers1Singular(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P1S, true, false, false);
    xml.writeText (curr_conjug.pers1Singular(type));
    xml.endTag (KV_CON_P1S, linefeed);
  }

  if (!curr_conjug.pers2Singular(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P2S, true, false, false);
    xml.writeText (curr_conjug.pers2Singular(type));
    xml.endTag (KV_CON_P2S, linefeed);
  }

  bool common = curr_conjug.pers3SingularCommon(type);
  if (!curr_conjug.pers3FemaleSingular(type).isEmpty()
      || common) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P3SF, false, false, false);
    if (common)
      xml.addAttribute (KV_CONJ_COMMON, common);
    xml.closeTag(false, false);
    xml.writeText (curr_conjug.pers3FemaleSingular(type));
    xml.endTag (KV_CON_P3SF, linefeed);
  }

  if (!curr_conjug.pers3MaleSingular(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P3SM, true, false, false);
    xml.writeText (curr_conjug.pers3MaleSingular(type));
    xml.endTag (KV_CON_P3SM, linefeed);
  }

  if (!curr_conjug.pers3NaturalSingular(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P3SN, true, false, false);
    xml.writeText (curr_conjug.pers3NaturalSingular(type));
    xml.endTag (KV_CON_P3SN, linefeed);
  }

  if (!curr_conjug.pers1Plural(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P1P, true, false, false);
    xml.writeText (curr_conjug.pers1Plural(type));
    xml.endTag (KV_CON_P1P, linefeed);
  }

  if (!curr_conjug.pers2Plural(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P2P, true, false, false);
    xml.writeText (curr_conjug.pers2Plural(type));
    xml.endTag (KV_CON_P2P, linefeed);
  }

  common = curr_conjug.pers3PluralCommon(type);
  if (!curr_conjug.pers3FemalePlural(type).isEmpty()
      || common) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P3PF, false, false, false);
    if (common)
      xml.addAttribute (KV_CONJ_COMMON, common);
    xml.closeTag(false, false);
    xml.writeText (curr_conjug.pers3FemalePlural(type));
    xml.endTag (KV_CON_P3PF, linefeed);
  }

  if (!curr_conjug.pers3MalePlural(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P3PM, true, false, false);
    xml.writeText (curr_conjug.pers3MalePlural(type));
    xml.endTag (KV_CON_P3PM, linefeed);
  }

  if (!curr_conjug.pers3NaturalPlural(type).isEmpty() ) {
    xml.writeText(identstr);
    xml.startTag (KV_CON_P3PN, true, false, false);
    xml.writeText (curr_conjug.pers3NaturalPlural(type));
    xml.endTag (KV_CON_P3PN, linefeed);
  }

  return true;

}

bool kvoctrainDoc::saveConjugHeader(vector<Conjugation> &curr_conjug,
                                    XmlWriter &xml)
{
/*
 <conjugation>        used in header for definiton of "prefix"
  <e l="de">          lang determines also lang order in entries !!
   <s1>I</s1>         which must NOT differ
   <s2>you<2>
   <s3f common="0">he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f common="1">they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

*/
  if (curr_conjug.size() == 0)
    return true;

  xml.writeText(" ");            // indent entry a bit
  xml.startTag (KV_CONJUG_GRP, false);
  xml.closeTag(false, true);

  QString def, indef, s;
  for (int ent = 0; ent < QMIN((int) curr_conjug.size(), numLangs()); ent++) {
    xml.writeText("  ");            // indent entry a bit
    xml.startTag (KV_CON_ENTRY, false);

    if (ent == 0) {
      s = getOriginalIdent().stripWhiteSpace();
      if (s.isEmpty() )
         s = "original";
    }
    else {
      s = getIdent(ent).stripWhiteSpace();
      if (s.isEmpty() ) {
         s.setNum (ent);
         s.insert (0, "translation ");
      }
    }
    xml.addAttribute (KV_LANG, s);
    xml.closeTag(false, true);

    if (!saveConjug (curr_conjug[ent], CONJ_PREFIX, xml, "   ") )
      return false;

    xml.writeText("  ");
    xml.endTag (KV_CON_ENTRY, true);
  }

  xml.writeText(" ");
  xml.endTag (KV_CONJUG_GRP, true);
  xml.writeText("\n");
  return true;
}


bool kvoctrainDoc::saveComparison (const Comparison &comp,
                                   XmlWriter &xml, int ident)

/*
 <comparison>
   <l1>good</l1>
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/

{
  if (comp.isEmpty())
    return true;

  xml.writeText("\n");
  QString identstr;
  identstr.fill (' ', ident+1);

  xml.writeText(identstr);
  xml.startTag (KV_COMPARISON_GRP, false);
  xml.closeTag(false, true);
  xml.writeText(identstr+" ");

  if (!comp.l1().isEmpty() ) {
    xml.startTag (KV_COMP_L1, true, false, false);
    xml.writeText (comp.l1());
    xml.endTag (KV_COMP_L1, false);
  }

  if (!comp.l2().isEmpty() ) {
    xml.startTag (KV_COMP_L2, true, false, false);
    xml.writeText (comp.l2());
    xml.endTag (KV_COMP_L2, false);
  }

  if (!comp.l3().isEmpty() ) {
    xml.startTag (KV_COMP_L3, true, false, false);
    xml.writeText (comp.l3());
    xml.endTag (KV_COMP_L3, false);
  }

  xml.writeText("\n"+identstr);
  xml.endTag (KV_COMPARISON_GRP, true);
  xml.writeText(identstr);   // indent next element
  return true;
}


bool kvoctrainDoc::saveMultipleChoice (const MultipleChoice &mc,
                                       XmlWriter &xml, int ident)

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
  if (mc.isEmpty())
    return true;

  xml.writeText("\n");
  QString identstr;
  identstr.fill (' ', ident+1);

  xml.writeText(identstr);
  xml.startTag (KV_MULTIPLECHOICE_GRP, false);
  xml.closeTag(false, true);
  xml.writeText(identstr+" ");

  if (!mc.mc1().isEmpty() ) {
    xml.startTag (KV_MC_1, true, false, false);
    xml.writeText (mc.mc1());
    xml.endTag (KV_MC_1, false);
  }

  if (!mc.mc2().isEmpty() ) {
    xml.startTag (KV_MC_2, true, false, false);
    xml.writeText (mc.mc2());
    xml.endTag (KV_MC_2, false);
  }

  if (!mc.mc3().isEmpty() ) {
    xml.startTag (KV_MC_3, true, false, false);
    xml.writeText (mc.mc3());
    xml.endTag (KV_MC_3, false);
  }

  if (!mc.mc4().isEmpty() ) {
    xml.startTag (KV_MC_4, true, false, false);
    xml.writeText (mc.mc4());
    xml.endTag (KV_MC_4, false);
  }

  if (!mc.mc5().isEmpty() ) {
    xml.startTag (KV_MC_5, true, false, false);
    xml.writeText (mc.mc5());
    xml.endTag (KV_MC_5, false);
  }

  xml.writeText("\n"+identstr);
  xml.endTag (KV_MULTIPLECHOICE_GRP, true);
  xml.writeText(identstr);   // indent next element
  return true;
}


bool kvoctrainDoc::saveConjugEntry (Conjugation &curr_conjug,
                                    XmlWriter &xml,
                                    int ident)

/*
 <conjugation>        in entry for definition of tenses of (irreg.) verbs
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
  curr_conjug.cleanUp();
  if (curr_conjug.numEntries() == 0 )
    return true;

  QString identstr;
  identstr.fill (' ', ident+1);
  xml.writeText("\n");

  xml.writeText(identstr);                  // indent entry a bit
  xml.startTag (KV_CONJUG_GRP, false);
  xml.closeTag(false, true);

  QString def, indef, s;
  QString type;
  for (int lfn = 0; lfn < (int) curr_conjug.numEntries(); lfn++) {
    xml.writeText(identstr+" ");            // indent entry a bit
    xml.startTag (KV_CON_TYPE, false);
    type = curr_conjug.getType(lfn);
    xml.addAttribute (KV_CON_NAME, type);
    xml.closeTag(false, false);

    if (!saveConjug (curr_conjug, curr_conjug.getType(lfn), xml, identstr) )
      return false;

    xml.endTag (KV_CON_TYPE, true);
  }

  xml.writeText(identstr);
  xml.endTag (KV_CONJUG_GRP, true);
  xml.writeText(identstr); // indent next element
  return true;
}


bool kvoctrainDoc::saveArticleKvtMl (XmlWriter &xml)

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
  if (articles.size() == 0)
    return true;

  xml.writeText(" ");            // indent entry a bit
  xml.startTag (KV_ARTICLE_GRP, false);
  xml.closeTag(false, true);

  QString def, indef, s;
  for (int lfn = 0; lfn < QMIN((int) articles.size(), numLangs()); lfn++) {
    xml.writeText("  ");            // indent entry a bit
    xml.startTag (KV_ART_ENTRY, false);
    if (lfn == 0) {
      s = getOriginalIdent().stripWhiteSpace();
      if (s.isEmpty() )
         s = "original";
    }
    else {
      s = getIdent(lfn).stripWhiteSpace();
      if (s.isEmpty() ) {
         s.setNum (lfn);
         s.insert (0, "translation ");
      }
    }
    xml.addAttribute (KV_LANG, s);
    xml.closeTag(false, true);

    articles[lfn].female(def, indef);
    if (!def.isEmpty() ) {
      xml.writeText("   ");
      xml.startTag (KV_ART_FD, true, false, false);
      xml.writeText (def);
      xml.endTag (KV_ART_FD, true);
    }
    if (!indef.isEmpty() ) {
      xml.writeText("   ");
      xml.startTag (KV_ART_FI, true, false, false);
      xml.writeText (indef);
      xml.endTag (KV_ART_FI, true);
    }

    articles[lfn].male(def, indef);
    if (!def.isEmpty() ) {
      xml.writeText("   ");
      xml.startTag (KV_ART_MD, true, false, false);
      xml.writeText (def);
      xml.endTag (KV_ART_MD, true);
    }
    if (!indef.isEmpty() ) {
      xml.writeText("   ");
      xml.startTag (KV_ART_MI, true, false, false);
      xml.writeText (indef);
      xml.endTag (KV_ART_MI, true);
    }

    articles[lfn].natural(def, indef);
    if (!def.isEmpty() ) {
      xml.writeText("   ");
      xml.startTag (KV_ART_ND, true, false, false);
      xml.writeText (def);
      xml.endTag (KV_ART_ND, true);
    }
    if (!indef.isEmpty() ) {
      xml.writeText("   ");
      xml.startTag (KV_ART_NI, true, false, false);
      xml.writeText (indef);
      xml.endTag (KV_ART_NI, true);
    }

    xml.writeText("  ");
    xml.endTag (KV_ART_ENTRY, true);
    xml.writeText("");
  }

  xml.writeText(" ");
  xml.endTag (KV_ARTICLE_GRP, true);
  xml.writeText("\n");
  return true;
}


bool kvoctrainDoc::saveOptionsKvtMl (XmlWriter &xml)
{
  xml.writeText(" ");            // indent entry a bit
  xml.startTag (KV_OPTION_GRP, false);
  xml.closeTag(false, true);

  xml.writeText("  ");
  xml.startTag (KV_OPT_SORT, false);
  xml.addAttribute (KV_BOOL_FLAG, sort_allowed );
  xml.closeTag (true, true);

  xml.writeText(" ");
  xml.endTag (KV_OPTION_GRP, true);
  xml.writeText("\n");
  return true;
}


bool kvoctrainDoc::saveToKvtMl (QTextStream& os, QString &title) {
  bool first_expr = true;

  os.setCodec(QTextCodec::codecForName("UTF-8"));

  XmlWriter xml (os);
  xml.setAutoEndl (false);

  xml.startTag ("!DOCTYPE", false);
  xml.writeText (" kvtml SYSTEM \"kvoctrain.dtd\"");
  xml.closeTag (false, true);

  xml.startTag ("!--", false, true, true);
  xml.writeText ("\nThis is a machine generated file.\n"
                 "Be careful when editing here.\n"
                 "\n"
                 "Short definition:\n"
                 "\n"
                 "lesson       lesson group\n"
                 " desc        name\n"
                 "   %no       its index\n"
                 "   %query    is in query selection\n"
                 "   %current  is current lesson\n"
                 "type         type group\n"
                 " desc        name\n"
                 "   %no       its index\n"
                 "e            entry of dictionary\n"
                 "  %s         is selected\n"
                 "  %m         lesson member\n"
                 "  %t         common expression type\n"
                 " o           original\n"
                 "   %q        in query (\"o\" is given, \"t\" is wanted)\n"
                 "   %l        language code\n"
                 "   %r        remark\n"
                 "   %p        pronunciation\n"
                 "   %width    column width\n"
                 "   %t        expression type (see QueryManager.h)\n"
                 "   %tf       false friend from\n"
                 "   %ff       false friend to\n"
                 "   %a        antonym\n"
                 "   %y        synonym\n"
                 "   %x        example\n"
                 "   %u        usage label\n"
                 "   %h        paraphrase\n"
                 " t           translation ..\n"
                 "   %q        in query (\"t\" is given, \"o\" is wanted)\n"
                 "   %l        language code\n"
                 "   %r        remark\n"
                 "   %p        pronunciation\n"
                 "   %width    column width\n"
                 "   %t        expression type\n"
                 "   %tf       false friend from\n"
                 "   %ff       false friend to\n"
                 "   %a        antonym\n"
                 "   %y        synonym\n"
                 "   %x        example\n"
                 "   %u        usage label\n"
                 "   %h        paraphrase\n"
                 "\n"
                 "   %d        last query date (from;to)\n"
                 "   %w        dito, compressed and deprecated\n"
                 "   %g        grade (from;to)\n"
                 "   %c        count (from;to)\n"
                 "   %b        bad count (from;to)\n"
                 "\n"
                 "\nValid xml means:\n"
                 " - Close all tags\n"
                 " - Keep proper hierarchy\n"
                 " - All attributes are quoted\n"
                 "--");
  xml.closeTag (false, true);

  xml.writeText ("\n ");
  xml.startTag (KV_DOCTYPE, false);
  xml.writeText ("\n ");
  xml.addAttribute (KV_ENCODING, (QString)"UTF-8");
  xml.writeText ("\n ");

  xml.addAttribute (KV_GENERATOR, (QString) "kvoctrain" KVD_VERS_PREFIX KVOCTRAIN_VERSION_STRING);
  xml.writeText ("\n ");
  xml.addAttribute (KV_COLS,  numLangs() );
  xml.writeText ("\n ");
  xml.addAttribute (KV_LINES, numEntries() );
  if (!title.isEmpty()) {
    xml.writeText ("\n ");
    xml.addAttribute (KV_TITLE, getTitle() );
    doctitle = title;
  }
  else if (!doctitle.isEmpty()) {
    xml.writeText ("\n ");
    xml.addAttribute (KV_TITLE, doctitle);
  }

  if (!author.isEmpty()) {
    xml.writeText ("\n ");
    xml.addAttribute (KV_AUTHOR, getAuthor() );
  }

  if (!license.isEmpty()) {
    xml.writeText ("\n ");
    xml.addAttribute (KV_LICENSE, getLicense() );
  }

  if (!doc_remark.isEmpty()) {
    xml.writeText ("\n ");
    xml.addAttribute (KV_DOC_REM, getDocRemark() );
  }

  xml.closeTag (false, true);
  xml.writeText ("\n");

  if (!saveLessonKvtMl(xml))
    return false;

  if (!saveArticleKvtMl(xml))
    return false;

  if (!saveConjugHeader(conjugations, xml))
    return false;

  if (!saveOptionsKvtMl(xml))
    return false;

  if (!saveTypeNameKvtMl(xml))
    return false;

  if (!saveTenseNameKvtMl(xml))
    return false;

  if (!saveUsageNameKvtMl(xml))
    return false;

  QString q_org, q_trans;
  vector<kvoctrainExpr>::const_iterator first =  vocabulary.begin ();
  getQueryLang (q_org, q_trans);

  int ent_no = 0;
  int ent_percent = (int) vocabulary.size () / 100;
  float f_ent_percent = (int) vocabulary.size () / 100.0;
  emit progressChanged(this, 0);

  while (first != vocabulary.end ()) {
    xml.writeText(" ");                // indent entry a bit
    xml.startTag (KV_EXPR, false);

    ent_no++;
    if (ent_percent != 0 && (ent_no % ent_percent) == 0 )
      emit progressChanged(this, ent_no / (int) f_ent_percent);

    if ((*first).getLesson() != 0) {   // entry belongs to lesson x
      QString ls;
      int lm = (*first).getLesson();
      if (lm > (int) lesson_descr.size() ) { // should not be
        kdError() << "index of lesson member too high: " << lm << endl;
        lm = 0;
      }
      ls.setNum (lm);
      xml.addAttribute (KV_LESS_MEMBER, ls);
    }

    if ((*first).isInQuery()) {   // entry was selected for query
      xml.addAttribute (KV_SELECTED, (QString) "1");
    }

    if (!(*first).isActive()) {   // entry was inactive
      xml.addAttribute (KV_INACTIVE, (QString) "1");
    }

    if ((*first).uniqueType() && !(*first).getType(0).isEmpty()) {
      xml.addAttribute (KV_EXPRTYPE, (*first).getType(0));
    }

    xml.closeTag ();
    xml.startTag (KV_ORG, false);
    if (first_expr) { // save space, only tell language in first entry
      QString s;
      s.setNum (getSizeHint (0));
      xml.addAttribute (KV_SIZEHINT, s);

      s = getOriginalIdent().stripWhiteSpace();
      if (s.isEmpty() )
         s = "original";
      xml.addAttribute (KV_LANG, s);
      if (s == q_org)
        xml.addAttribute (KV_QUERY, (QString) KV_O);
      else if (s == q_trans)
        xml.addAttribute (KV_QUERY, (QString) KV_T);

    }

    if (!(*first).getRemark(0).isEmpty() ) {
      xml.addAttribute (KV_REMARK, (*first).getRemark(0));
    }

    if (!(*first).getSynonym(0).isEmpty() ) {
      xml.addAttribute (KV_SYNONYM, (*first).getSynonym(0));
    }

    if (!(*first).getExample(0).isEmpty() ) {
      xml.addAttribute (KV_EXAMPLE, (*first).getExample(0));
    }

    if (!(*first).getUsageLabel(0).isEmpty() ) {
      xml.addAttribute (KV_USAGE, (*first).getUsageLabel(0));
    }

    if (!(*first).getParaphrase(0).isEmpty() ) {
      xml.addAttribute (KV_PARAPHRASE, (*first).getParaphrase(0));
    }

    if (!(*first).getAntonym(0).isEmpty() ) {
      xml.addAttribute (KV_ANTONYM, (*first).getAntonym(0));
    }

    if (!(*first).getPronunce(0).isEmpty() ) {
      xml.addAttribute (KV_PRONUNCE, (*first).getPronunce(0));
    }

    if (!(*first).uniqueType() && !(*first).getType(0).isEmpty()) {
      xml.addAttribute (KV_EXPRTYPE, (*first).getType(0));
    }

    xml.closeTag ();

    if (!saveMultipleChoice ((*first).getMultipleChoice(0), xml, 1))
        return false;

    QString s;
    QString entype = s = (*first).getType(0);
    int pos = s.find (QM_TYPE_DIV);
    if (pos >= 0)
      entype = s.left (pos);
    else
      entype = s;

    if (   entype == QM_VERB
        && (*first).getConjugation(0).numEntries() > 0) {
      Conjugation conj = (*first).getConjugation(0);
      if (!saveConjugEntry(conj, xml, 1))
        return false;
    }
    else if (entype == QM_ADJ
        && !(*first).getComparison(0).isEmpty()) {
      Comparison comp = (*first).getComparison(0);
      if (!saveComparison (comp, xml, 1))
        return false;
    }

    xml.writeText( (*first).getOriginal());
    xml.endTag (KV_ORG);

    int trans = 1;
    while (trans < (int)langs.size()) {
      xml.startTag (KV_TRANS, false);
      if (first_expr) {   // save space, only tell language in first entry
        QString s;
        s.setNum (getSizeHint (trans));
        xml.addAttribute (KV_SIZEHINT, s);

        s = getIdent(trans).stripWhiteSpace();
        if (s.isEmpty() ) {
           s.setNum (trans);
           s.insert (0, "translation ");
        }
        xml.addAttribute (KV_LANG, s);
        if (s == q_org)
          xml.addAttribute (KV_QUERY, (QString) KV_O);
        else if (s == q_trans)
          xml.addAttribute (KV_QUERY, (QString) KV_T);
      }

      QString s1, s2;

      if ((*first).getGrade(trans, false) != 0
          ||(*first).getGrade(trans, true) != 0) {
        xml.addAttribute (KV_GRADE, (*first).gradeStr(trans, false)
                          +';'
                          +(*first).gradeStr(trans, true));
      }

      if ((*first).getQueryCount(trans, false) != 0
          ||(*first).getQueryCount(trans, true) != 0) {
        s1.setNum((*first).getQueryCount(trans, false));
        s2.setNum((*first).getQueryCount(trans, true));
        xml.addAttribute (KV_COUNT, s1 +';' +s2);
      }

      if ((*first).getBadCount(trans, false) != 0
          ||(*first).getBadCount(trans, true) != 0) {
        s1.setNum((*first).getBadCount(trans, false));
        s2.setNum((*first).getBadCount(trans, true));
        xml.addAttribute (KV_BAD, s1 +';' +s2);
      }

      if ((*first).getQueryDate(trans, false) != 0
          ||(*first).getQueryDate(trans, true) != 0) {
        s1.setNum((*first).getQueryDate(trans, false));
        s2.setNum((*first).getQueryDate(trans, true));
        xml.addAttribute (KV_DATE, s1 +';' +s2);
      }

      if (!(*first).getRemark(trans).isEmpty() ) {
        xml.addAttribute (KV_REMARK, (*first).getRemark(trans));
      }

      if (!(*first).getFauxAmi(trans, false).isEmpty() ) {
        xml.addAttribute (KV_FAUX_AMI_F, (*first).getFauxAmi(trans, false));
      }

      if (!(*first).getFauxAmi(trans, true).isEmpty() ) {
        xml.addAttribute (KV_FAUX_AMI_T, (*first).getFauxAmi(trans, true));
      }

      if (!(*first).getSynonym(trans).isEmpty() ) {
        xml.addAttribute (KV_SYNONYM, (*first).getSynonym(trans));
      }

      if (!(*first).getExample(trans).isEmpty() ) {
        xml.addAttribute (KV_EXAMPLE, (*first).getExample(trans));
      }

      if (!(*first).getUsageLabel(trans).isEmpty() ) {
        xml.addAttribute (KV_USAGE, (*first).getUsageLabel(trans));
      }

      if (!(*first).getParaphrase(trans).isEmpty() ) {
        xml.addAttribute (KV_PARAPHRASE, (*first).getParaphrase(trans));
      }

      if (!(*first).getAntonym(trans).isEmpty() ) {
        xml.addAttribute (KV_ANTONYM, (*first).getAntonym(trans));
      }

      if (!(*first).getPronunce(trans).isEmpty() ) {
        xml.addAttribute (KV_PRONUNCE, (*first).getPronunce(trans));
      }

      if (!(*first).uniqueType() && !(*first).getType(trans).isEmpty()) {
        xml.addAttribute (KV_EXPRTYPE, (*first).getType(trans));
      }

      xml.closeTag ();

      // only save conjugations when type == verb

      if (!saveMultipleChoice ((*first).getMultipleChoice(trans), xml, 1))
          return false;

      QString s;
      QString entype = s = (*first).getType(0);
      int pos = s.find (QM_TYPE_DIV);
      if (pos >= 0)
        entype = s.left (pos);
      else
        entype = s;

      if (   entype == QM_VERB
          && (*first).getConjugation(trans).numEntries() > 0) {
        Conjugation conj = (*first).getConjugation(trans);
        if (!saveConjugEntry(conj, xml, 1+trans))
          return false;
      }

      if (   entype == QM_ADJ
          && !(*first).getComparison(trans).isEmpty()) {
        Comparison comp = (*first).getComparison(trans);
        if (!saveComparison (comp, xml, 1+trans))
          return false;
      }

      xml.writeText((*first).getTranslation(trans));
      xml.endTag (KV_TRANS);
      trans++;
    }
    xml.endTag (KV_EXPR, true);
    first++;
    first_expr = false;
  }
  xml.endTag (KV_DOCTYPE, true);

  setModified (false);
  return os.device()->status() == IO_Ok;
}
