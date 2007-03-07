/***************************************************************************

                   entry dialog for table cell contents

    -----------------------------------------------------------------------

    begin         : Thu Mar 11 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2001 The KDE-EDU team
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EntryDlg_included
#define EntryDlg_included

#include <QItemSelection>
#include <QCloseEvent>

#include <kpagedialog.h>

#include "FromToEntryPage.h"
#include "CommonEntryPage.h"
#include "AuxInfoEntryPage.h"
#include "TenseEntryPage.h"
#include "AdjEntryPage.h"
#include "MCEntryPage.h"

class KVTLanguages;
class QTabWidget;
class KMainWindow;
class KEduVocDocument;

class EntryDlg : public KPageDialog
{
  Q_OBJECT

public:

  enum EditResult {EditCancel, EditApply, EditUndo, EditPageUp, EditPageDown, EditUp, EditDown, EditLeft, EditRight};

  enum EnableType {EnableAll, EnableOnlyCommon, EnableNone, EnableOnlyOriginal };

  EntryDlg(KMainWindow *main, KEduVocDocument *doc, KVTQuery &querymanager);

  ~EntryDlg ();

  void setData
   (bool           multi_sel,
    grade_t        f_grd,
    grade_t        t_grd,
    count_t        f_qcount,
    count_t        t_qcount,
    count_t        f_bcount,
    count_t        t_bcount,
    QDateTime      f_qdate,
    QDateTime      t_qdate,
    QString        f_faux_ami,
    QString        t_faux_ami,
    QString        _expr,
    int            _lesson,
    QString        remark,
    QString        _type,
    QString        pronounce,
    QString        synonym,
    QString        antonym,
    QString        example,
    QString        usagelabel,
    QString        paraphrase,
    const          KEduVocConjugation &con_prefix,
    const          KEduVocConjugation &conjugations,
    const          KEduVocComparison &comp,
    const          KEduVocMultipleChoice &mc,
    const QString &title,
    bool           active);

  QString getFromFauxAmi  () const { return from_page ? from_page->getFauxAmi() : QString(""); }
  QDateTime  getFromDate   () const { return from_page ? from_page->getDate() : QDateTime(); }
  grade_t getFromGrade  () const { return from_page ? from_page->getGrade() : KV_NORM_GRADE; }
  count_t getFromBCount () const { return from_page ? from_page->getBCount() : 0; }
  count_t getFromQCount () const { return from_page ? from_page->getQCount() : 0; }

  QString getToFauxAmi  () const { return to_page ? to_page->getFauxAmi() : QString(""); }
  QDateTime  getToDate     () const { return to_page ? to_page->getDate() : QDateTime(); }
  grade_t getToGrade    () const { return to_page ? to_page->getGrade() : KV_NORM_GRADE; }
  count_t getToBCount   () const { return to_page ? to_page->getBCount() : 0; }
  count_t getToQCount   () const { return to_page ? to_page->getQCount() : 0; }

  int     getLesson   ()  const { return comm_page->getLesson(); }
  QString getType     ()  const { return comm_page->getType(); }
  QString getExpr     ()  const { return comm_page->getExpr(); }
  QString getPronounce ()  const { return comm_page->getPronounce(); }
  QString getUsageLabel() const { return comm_page->getUsageLabel(); }
  bool    getActive()     const { return comm_page->getActive(); }

  QString getSynonym  () const { return aux_page->getSynonym(); }
  QString getAntonym  () const { return aux_page->getAntonym(); }
  QString getRemark   () const { return aux_page->getRemark(); }
  QString getExample  () const { return aux_page->getExample(); }
  QString getParaphrase () const { return aux_page->getParaphrase(); }

  KEduVocConjugation getConjugation() const { return tense_page->getConjugation(); }

  KEduVocComparison getComparison() const { return adj_page->getComparison(); }

  KEduVocMultipleChoice getMultipleChoice() const { return mc_page->getMultipleChoice(); }

  bool isModified();
  void setModified(bool mod);
  void setEnabled(int);

  void setCell(int row, int col, const QItemSelection & sel);
  void getCell(int &row, int &col, QItemSelection & sel) const;

signals:
  void sigEditChoice(int);

public slots:
  void slotDisplayModified();
  void slotApply();
  void slotUndo();
  void slotDockHorizontal();
  void slotDockVertical();

protected slots:
  void updatePages(const QString &type);
  void slotClose();

protected:
  virtual void closeEvent(QCloseEvent*e);

  FromToEntryPage  *from_page;
  FromToEntryPage  *to_page;
  CommonEntryPage  *comm_page;
  AuxInfoEntryPage *aux_page;
  TenseEntryPage   *tense_page;
  AdjEntryPage     *adj_page;
  MCEntryPage      *mc_page;
  int               edit_row;
  int               edit_col;
  QTabWidget       *tabber;
  QItemSelection    m_selection;
  KMainWindow      *mainwin;
  QSize             oldMainSize;
  QPoint            oldMainPos;
  bool              docked;
};

#endif // EntryDlg_included
