/***************************************************************************

    $Id$

                   maintain a kvoctrain expression

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    


    -----------------------------------------------------------------------

    $Log$
    Revision 1.6  2001/11/09 10:40:25  arnold
    removed ability to display a different font for each column

    Revision 1.5  2001/11/02 10:18:31  arnold
    fixed colum resizing and diplaying of grade colors

    Revision 1.4  2001/10/30 14:10:53  arnold
    added property 'multiple choice'

    Revision 1.3  2001/10/22 06:39:21  waba
    Show a cute little marker to indicate marked entries.
    (Instead of using a bold type-face)

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

#include "kvoctraindoc.h"
#include <qpainter.h>
#include "qdatastream.h"
#include <vector.h>

#include "kv_resource.h"
#include <klocale.h>
#include <kstddirs.h>

QPixmap * kvoctrainExpr::s_pm_mark = 0;

// static
void kvoctrainExpr::setPixmap(const QPixmap &pm)
{
  delete s_pm_mark;
  s_pm_mark = new QPixmap(pm);
}


void kvoctrainExpr::Init()
{
  grades.push_back(KV_NORM_GRADE);
  rev_grades.push_back(KV_NORM_GRADE);
  selected = false;
  qcounts.push_back(0);
  rev_qcounts.push_back(0);
  bcounts.push_back(0);
  rev_bcounts.push_back(0);
  qdates.push_back(0);
  rev_qdates.push_back(0);
  lesson = 0;
}


kvoctrainExpr::kvoctrainExpr (QString expr, int _lesson)
{
  Init();
  setOriginal(expr.stripWhiteSpace() );
  lesson = _lesson;
}


kvoctrainExpr::kvoctrainExpr ()
{
  Init();
}


kvoctrainExpr::kvoctrainExpr (QString s, QString separator, int _lesson)
{
  Init();
  QString se;
  lesson = _lesson;

  if (separator.length() ) {
    int pos = s.find(separator);
  
    if (pos == -1) {
      setOriginal(s.stripWhiteSpace());
    }
    else {
      se = s.left(pos).stripWhiteSpace();
      setOriginal(se);
      s.remove (0, pos+separator.length() );
//      s.stripWhiteSpace();
  
      // gather all translations
      while ((pos = s.find(separator)) != -1) {
        se = s.left(pos).stripWhiteSpace();
        addTranslation(se, KV_NORM_GRADE, KV_NORM_GRADE);
        s.remove (0, pos+separator.length() );
//        s.stripWhiteSpace();
      }
      addTranslation(s.stripWhiteSpace(), KV_NORM_GRADE, KV_NORM_GRADE);
    }
  }
}


kvoctrainExpr::~kvoctrainExpr()
{
}


int kvoctrainExpr::numTranslations() const
{
  return translations.size();
}


QString kvoctrainExpr::getRemark (int idx) const
{
  if (idx >= (int)remarks.size() || idx < 0) {
    return "";
  }
  else {
    return remarks[idx];
  }
}


void kvoctrainExpr::setRemark (int idx, QString expr)
{
  if ( idx < 0) return;

  // extend remarks with empty strings if necessary
  if ((int)remarks.size() <= idx )
    for (int i = remarks.size(); i < idx+1; i++)
      remarks.push_back ("");

  remarks[idx] = expr.stripWhiteSpace();
}


void kvoctrainExpr::setFauxAmi (int idx, QString expr, bool rev_ami)
{
  if (idx < 1) return;

  if (rev_ami) {
    // extend friend with empty strings if necessary
    if ((int)rev_fauxAmi.size() <= idx )
      for (int i = rev_fauxAmi.size(); i < idx+1; i++)
        rev_fauxAmi.push_back ("");
  
    rev_fauxAmi[idx] = expr.stripWhiteSpace();

  }
  else {
    // extend friend with empty strings if necessary
    if ((int)fauxAmi.size() <= idx )
      for (int i = fauxAmi.size(); i < idx+1; i++)
        fauxAmi.push_back ("");
  
    fauxAmi[idx] = expr.stripWhiteSpace();
  }
}


QString kvoctrainExpr::getFauxAmi (int idx, bool rev_ami) const
{
  if (rev_ami) {
    if (idx >= (int)rev_fauxAmi.size() || idx < 1 ) {
      return "";
    }
  
    return rev_fauxAmi[idx];
  }

  if (idx >= (int)fauxAmi.size() || idx < 1 ) {
    return "";
  }

  return fauxAmi[idx];
}


void kvoctrainExpr::setSynonym (int idx, QString expr)
{
  if ( idx < 0) return;

  // extend synonym with empty strings if necessary
  if ((int)synonym.size() <= idx )
    for (int i = synonym.size(); i < idx+1; i++)
      synonym.push_back ("-");

  synonym[idx] = expr.stripWhiteSpace();
}


QString kvoctrainExpr::getSynonym (int idx) const
{
  if (idx >= (int)synonym.size() || idx < 0) {
    return "";
  }
  else {
    return synonym[idx];
  }
}


void kvoctrainExpr::setExample (int idx, QString expr)
{
  if ( idx < 0) return;

  // extend exampls with empty strings if necessary
  if ((int)example.size() <= idx )
    for (int i = example.size(); i < idx+1; i++)
      example.push_back ("");

  example[idx] = expr.stripWhiteSpace();
}


QString kvoctrainExpr::getExample (int idx) const
{
  if (idx >= (int)example.size() || idx < 0) {
    return "";
  }
  else {
    return example[idx];
  }
}


void kvoctrainExpr::setUsageLabel (int idx, QString expr)
{
  if ( idx < 0) return;

  // extend labels with empty strings if necessary
  if ((int)usageLabels.size() <= idx )
    for (int i = usageLabels.size(); i < idx+1; i++)
      usageLabels.push_back ("");

  usageLabels[idx] = expr.stripWhiteSpace();
}


QString kvoctrainExpr::getUsageLabel (int idx) const
{
  if (idx >= (int)usageLabels.size() || idx < 0) {
    return "";
  }
  else {
    return usageLabels[idx];
  }
}


void kvoctrainExpr::setParaphrase (int idx, QString expr)
{
  if ( idx < 0) return;

  // extend phrase with empty strings if necessary
  if ((int) paraphrases.size() <= idx )
    for (int i = paraphrases.size(); i < idx+1; i++)
      paraphrases.push_back ("");

  paraphrases[idx] = expr.stripWhiteSpace();
}


QString kvoctrainExpr::getParaphrase (int idx) const
{
  if (idx >= (int)paraphrases.size() || idx < 0) {
    return "";
  }
  else {
    return paraphrases[idx];
  }
}


void kvoctrainExpr::setAntonym (int idx, QString expr)
{
  if ( idx < 0) return;

  // extend antonym with empty strings if necessary
  if ((int)antonym.size() <= idx )
    for (int i = antonym.size(); i < idx+1; i++)
      antonym.push_back ("");

  antonym[idx] = expr.stripWhiteSpace();
}


QString kvoctrainExpr::getAntonym (int idx) const
{
  if (idx >= (int)antonym.size() || idx < 0) {
    return "";
  }
  else {
    return antonym[idx];
  }
}


void kvoctrainExpr::setConjugation (int idx, const Conjugation &con)
{
  if ( idx < 0) return;

  // extend conjugation with empty elements
  if ((int)conjugations.size() <= idx )
    for (int i = conjugations.size(); i < idx+1; i++)
      conjugations.push_back (Conjugation());

  conjugations[idx] = con;
}


Conjugation kvoctrainExpr::getConjugation (int idx) const
{
  if (idx >= (int)conjugations.size() || idx < 0) {
    return Conjugation();
  }
  else {
    return conjugations[idx];
  }
}


void kvoctrainExpr::setComparison (int idx, const Comparison &con)
{
  if ( idx < 0) return;

  // extend comparison with empty elements
  if ((int)comparisons.size() <= idx )
    for (int i = comparisons.size(); i < idx+1; i++)
      comparisons.push_back (Comparison());

  comparisons[idx] = con;
}


Comparison kvoctrainExpr::getComparison (int idx) const
{
  if (idx >= (int)comparisons.size() || idx < 0) {
    return Comparison();
  }
  else {
    return comparisons[idx];
  }
}


void kvoctrainExpr::setMultipleChoice (int idx, const MultipleChoice &mc)
{
  if ( idx < 0) return;

  // extend comparison with empty elements
  if ((int)mcs.size() <= idx )
    for (int i = mcs.size(); i < idx+1; i++)
      mcs.push_back (MultipleChoice());

  mcs[idx] = mc;
}


MultipleChoice kvoctrainExpr::getMultipleChoice (int idx) const
{
  if (idx >= (int)mcs.size() || idx < 0) {
    return MultipleChoice();
  }
  else {
    return mcs[idx];
  }
}


QString kvoctrainExpr::getPronunce (int idx) const
{
  if (idx >= (int)pronunces.size() || idx < 0) {
    return "";
  }
  else {
    return pronunces[idx];
  }
}


void kvoctrainExpr::setPronunce (int idx, QString expr)
{
  if ( idx < 0) return;

  // extend with empty strings if necessary
  if ((int)pronunces.size() <= idx )
    for (int i = pronunces.size(); i < idx+1; i++)
      pronunces.push_back ("");

  pronunces[idx] = expr.stripWhiteSpace();
}


void kvoctrainExpr::addTranslation (QString expr,
                                    grade_t grade, grade_t rev_grade)
{
  if (grade > KV_MAX_GRADE)
    grade = KV_MAX_GRADE;

  if (rev_grade > KV_MAX_GRADE)
    rev_grade = KV_MAX_GRADE;

  grades.push_back (grade);
  rev_grades.push_back (rev_grade);
  translations.push_back (expr.stripWhiteSpace());
}


QString kvoctrainExpr::getTranslation (int idx) const
{
  if (idx > (int)translations.size() || idx < 1)
    return "";
  else
    return translations[idx-1];
}


void kvoctrainExpr::removeTranslation (int idx)
{
   if (idx <= 0)
     return;

   if (idx <= numTranslations())
     translations.erase (&translations[idx-1], &translations[idx]);

   if (idx < (int)remarks.size() )
     remarks.erase (&remarks[idx], &remarks[idx+1]);

   if (idx < (int)conjugations.size() )
     conjugations.erase (&conjugations[idx], &conjugations[idx+1]);

   if (idx < (int)comparisons.size() )
     comparisons.erase (&comparisons[idx], &comparisons[idx+1]);

   if (idx < (int)fauxAmi.size() )
     fauxAmi.erase (&fauxAmi[idx], &fauxAmi[idx+1]);

   if (idx < (int)rev_fauxAmi.size() )
     rev_fauxAmi.erase (&rev_fauxAmi[idx], &rev_fauxAmi[idx+1]);

   if (idx < (int)synonym.size() )
     synonym.erase (&synonym[idx], &synonym[idx+1]);

   if (idx < (int)example.size() )
     example.erase (&example[idx], &example[idx+1]);

   if (idx < (int)usageLabels.size() )
     usageLabels.erase (&usageLabels[idx], &usageLabels[idx+1]);

   if (idx < (int)paraphrases.size() )
     paraphrases.erase (&paraphrases[idx], &paraphrases[idx+1]);

   if (idx < (int)antonym.size() )
     antonym.erase (&antonym[idx], &antonym[idx+1]);

   if (idx < (int)exprtypes.size() )
     exprtypes.erase (&exprtypes[idx], &exprtypes[idx+1]);

   if (idx < (int)pronunces.size() )
     pronunces.erase (&pronunces[idx], &pronunces[idx+1]);

   if (idx < (int)grades.size() )
     grades.erase (&grades[idx], &grades[idx+1]);

   if (idx < (int)rev_grades.size() )
     rev_grades.erase (&rev_grades[idx], &rev_grades[idx+1]);

   if (idx < (int)qcounts.size() )
     qcounts.erase (&qcounts[idx], &qcounts[idx+1]);

   if (idx < (int)rev_qcounts.size() )
     rev_qcounts.erase (&rev_qcounts[idx], &rev_qcounts[idx+1]);

   if (idx < (int)bcounts.size() )
     bcounts.erase (&bcounts[idx], &bcounts[idx+1]);

   if (idx < (int)rev_bcounts.size() )
     rev_bcounts.erase (&rev_bcounts[idx], &rev_bcounts[idx+1]);

   if (idx < (int)qdates.size() )
     qdates.erase (&qdates[idx], &qdates[idx+1]);

   if (idx < (int)rev_qdates.size() )
     rev_qdates.erase (&rev_qdates[idx], &rev_qdates[idx+1]);
}


void kvoctrainExpr::setTranslation (int idx, QString expr)
{
  if ( idx <= 0) return;

  // extend translations with empty strings if necessary
  if ((int)translations.size() < idx )
    for (int i = translations.size(); i < idx; i++)
      translations.push_back ("");

//  if (idx <= (int)translations.size())
  translations[idx-1] = expr.stripWhiteSpace();
}


QString kvoctrainExpr::gradeStr (int idx, bool rev_grade) const
{
  QString s;
  s.setNum(getGrade(idx, rev_grade));
  return s;
}


grade_t kvoctrainExpr::getGrade (int idx, bool rev_grade) const
{
  if (rev_grade) {
    if (idx >= (int)rev_grades.size() || idx < 1 ) {
      return KV_NORM_GRADE;
    }
    else if (rev_grades[idx] > KV_MAX_GRADE) {
      return KV_MAX_GRADE;
    }
  
    return rev_grades[idx];

  }
  else {
    if (idx >= (int)grades.size() || idx < 1 ) {
      return KV_NORM_GRADE;
    }
    else if (grades[idx] > KV_MAX_GRADE) {
      return KV_MAX_GRADE;
    }
  
    return grades[idx];
  }
}


void kvoctrainExpr::setGrade (int idx, grade_t grade, bool rev_grade)
{
  if (idx < 1) return;

  if (grade > KV_MAX_GRADE)
    grade = KV_MAX_GRADE;
  if (grade < KV_MIN_GRADE)
    grade = KV_MIN_GRADE;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if ((int)rev_grades.size() <= idx )
      for (int i = rev_grades.size(); i <= idx; i++) {
        rev_grades.push_back (KV_NORM_GRADE);
    }
    rev_grades[idx] = grade;
  }
  else {
    // extend grades with standard grade if necessary
    if ((int)grades.size() <= idx )
      for (int i = grades.size(); i <= idx; i++) {
        grades.push_back (KV_NORM_GRADE);
      }
    grades[idx] = grade;
  }
}


void kvoctrainExpr::incGrade (int idx, bool rev_grade)
{
  if (idx < 1) return;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if ((int)rev_grades.size() <= idx )
      for (int i = rev_grades.size(); i <= idx; i++) {
        rev_grades.push_back (KV_NORM_GRADE);
    }
    if (rev_grades[idx] < KV_MAX_GRADE)
      rev_grades[idx]++;
  }
  else {
    // extend grades with standard grade if necessary
    if ((int)grades.size() <= idx )
      for (int i = grades.size(); i <= idx; i++) {
        grades.push_back (KV_NORM_GRADE);
      }
    if (grades[idx] < KV_MAX_GRADE)
      grades[idx]++;
  }
}


void kvoctrainExpr::decGrade (int idx, bool rev_grade)
{
  if (idx < 1) return;

  if (rev_grade) {
    // extend rev grades with standard grade if necessary
    if ((int)rev_grades.size() <= idx )
      for (int i = rev_grades.size(); i <= idx; i++) {
        rev_grades.push_back (KV_NORM_GRADE);
    }
    if (rev_grades[idx] > KV_MIN_GRADE)
      rev_grades[idx]--;
  }
  else {
    // extend grades with standard grade if necessary
    if ((int)grades.size() <= idx )
      for (int i = grades.size(); i <= idx; i++) {
        grades.push_back (KV_NORM_GRADE);
      }
    if (grades[idx] > KV_MIN_GRADE)
      grades[idx]--;
  }
}


count_t kvoctrainExpr::getQueryCount (int idx, bool rev_count)  const
{
  if (rev_count) {
    if (idx >= (int)rev_qcounts.size() || idx < 1 ) {
      return 0;
    }
  
    return rev_qcounts[idx];
  }

  if (idx >= (int)qcounts.size() || idx < 1 ) {
    return 0;
  }

  return qcounts[idx];
}


void kvoctrainExpr::setQueryCount (int idx, count_t count, bool rev_count)
{
  if (idx < 1) return;

  if (rev_count) {
    // extend rev counts with 0 if necessary
    if ((int)rev_qcounts.size() <= idx )
      for (int i = rev_qcounts.size(); i <= idx; i++) {
        rev_qcounts.push_back (0);
    }

    rev_qcounts[idx] = count;

  }
  else {
    // extend counts with 0 if necessary
    if ((int)qcounts.size() <= idx )
      for (int i = qcounts.size(); i <= idx; i++) {
        qcounts.push_back (0);
      }
    qcounts[idx] = count;
  }
}


count_t kvoctrainExpr::getBadCount (int idx, bool rev_count) const
{
  if (rev_count) {
    if (idx >= (int)rev_bcounts.size() || idx < 1 ) {
      return 0;
    }
  
    return rev_bcounts[idx];
  }

  if (idx >= (int)bcounts.size() || idx < 1 ) {
    return 0;
  }

  return bcounts[idx];
}


void kvoctrainExpr::setBadCount (int idx, count_t count, bool rev_count)
{
  if (idx < 1) return;

  if (rev_count) {
    // extend rev counts with 0 if necessary
    if ((int)rev_bcounts.size() <= idx )
      for (int i = rev_bcounts.size(); i <= idx; i++) {
        rev_bcounts.push_back (0);
    }

    rev_bcounts[idx] = count;

  }
  else {
    // extend counts with 0 if necessary
    if ((int)bcounts.size() <= idx )
      for (int i = bcounts.size(); i <= idx; i++) {
        bcounts.push_back (0);
      }
    bcounts[idx] = count;
  }
}


time_t kvoctrainExpr::getQueryDate (int idx, bool rev_date) const
{
  if (rev_date) {
    if (idx >= (int)rev_qdates.size() || idx < 1 ) {
      return 0;
    }
  
    return rev_qdates[idx];
  }

  if (idx >= (int)qdates.size() || idx < 1 ) {
    return 0;
  }

  return qdates[idx];
}


void kvoctrainExpr::setQueryDate (int idx, time_t date, bool rev_date)
{
  if (idx < 1) return;

  if (rev_date) {
    // extend rev dates with 0 if necessary
    if ((int)rev_qdates.size() <= idx )
      for (int i = rev_qdates.size(); i <= idx; i++) {
        rev_qdates.push_back (0);
    }

    rev_qdates[idx] = date;

  }
  else {
    // extend dates with 0 if necessary
    if ((int)qdates.size() <= idx )
      for (int i = qdates.size(); i <= idx; i++) {
        qdates.push_back (0);
      }
    qdates[idx] = date;
  }
}


void kvoctrainExpr::paint(QPainter *p, int col, int width, bool cell_selected,
                          kvoctrainDoc *voc_doc, int current_col,
                          const GradeCols *gc)
{
  QColor color = KV_NORM_COLOR;

  if (gc != 0 && gc->use) {
    if (col > KV_COL_ORG) {
      color = gc->col0;
      if (getQueryCount(col-KV_EXTRA_COLS, false) != 0) {
        switch (getGrade(col-KV_EXTRA_COLS, false)) {
          case KV_NORM_GRADE:   color = gc->col0;    break;
          case KV_LEV1_GRADE:   color = gc->col1;    break;
          case KV_LEV2_GRADE:   color = gc->col2;    break;
          case KV_LEV3_GRADE:   color = gc->col3;    break;
          case KV_LEV4_GRADE:   color = gc->col4;    break;
          case KV_LEV5_GRADE:   color = gc->col5;    break;
          case KV_LEV6_GRADE:   color = gc->col6;    break;
          case KV_LEV7_GRADE:   color = gc->col7;    break;
          default             : color = gc->col1;
        }
      }
    }
    else if (   col == KV_COL_ORG ) {
      color = gc->col0;
      if (  numTranslations() != 0
          && current_col > KV_COL_ORG ) {
        if (getQueryCount(current_col-KV_EXTRA_COLS, true) != 0 ) {
          switch (getGrade(current_col-KV_EXTRA_COLS, true)) {
            case KV_LEV1_GRADE:   color = gc->col1;    break;
            case KV_LEV2_GRADE:   color = gc->col2;    break;
            case KV_LEV3_GRADE:   color = gc->col3;    break;
            case KV_LEV4_GRADE:   color = gc->col4;    break;
            case KV_LEV5_GRADE:   color = gc->col5;    break;
            case KV_LEV6_GRADE:   color = gc->col6;    break;
            case KV_LEV7_GRADE:   color = gc->col7;    break;
            default             : color = gc->col1;
          }
        }
      }
    }
  }

  if (cell_selected)
    p->setPen (Qt::white);
  else
    p->setPen (color);

  int fontpos = ( p->fontMetrics().lineSpacing() - p->fontMetrics().lineSpacing())/2;

  switch( col )
    {
    case KV_COL_LESS: // lesson
      {
        QString less_str;
        if (voc_doc != 0 && getLesson() != 0)
          less_str = voc_doc->getLessonDescr(getLesson() );
	p->drawText( 3, fontpos, width, p->fontMetrics().lineSpacing(),
		     Qt::AlignLeft,
		     less_str);
      }
    break;

    case KV_COL_MARK: // mark 
      {
        if (isSelected() )
        {
	  p->drawPixmap((width - s_pm_mark->width()) / 2, 
                        (p->fontMetrics().lineSpacing() - s_pm_mark->height())/2, *s_pm_mark);
        }
      }
    break;

    case KV_COL_ORG: // original
      {
	p->drawText( 3, fontpos, width, p->fontMetrics().lineSpacing(),
		     Qt::AlignLeft,
		     getOriginal() );
	break;
      }
    break;

    default: // translation x
      p->drawText( 3, fontpos, width, p->fontMetrics().lineSpacing(),
                   Qt::AlignLeft,
		   getTranslation(col-KV_COL_ORG) );
      break;
  }
}


bool kvoctrainExpr::uniqueType () const
{
  bool unique = true;
  QString type0 = getType(0);
  for (int i = 1; i < numTranslations(); i++)
    if (type0 != getType(i) )
      unique = false;
  return unique;
}


QString kvoctrainExpr::getType (int idx) const
{
  if (idx >= (int)exprtypes.size() || idx < 0) {
    return "";
  }
  else {
    return exprtypes[idx];
  }
}


int kvoctrainExpr::getLesson () const
{
  return lesson;
}

void kvoctrainExpr::incQueryCount (int index, bool rev_count)
{
  setQueryCount (index, getQueryCount(index, rev_count)+1, rev_count);
}


void kvoctrainExpr::incBadCount (int index, bool rev_count)
{
  setBadCount (index, getBadCount(index, rev_count)+1, rev_count);
}


bool kvoctrainExpr::isSelected() const
{
  return selected;
}

void kvoctrainExpr::setSelected(bool flag)
{
  selected = flag;
}


void kvoctrainExpr::setOriginal (QString expr)
{
  origin = expr;
}


QString kvoctrainExpr::getOriginal () const
{
  return origin;
}


void kvoctrainExpr::setLesson (int l)
{
  lesson = l;
}


void kvoctrainExpr::setType (int idx, QString type)
{
  if ( idx < 0) return;

  // extend types with empty strings if necessary
  if ((int)exprtypes.size() <= idx )
    for (int i = exprtypes.size(); i < idx+1; i++)
      exprtypes.push_back ("");

  exprtypes[idx] = type.stripWhiteSpace();
}

