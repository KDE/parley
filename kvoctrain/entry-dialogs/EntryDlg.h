/***************************************************************************

    $Id$

                   entry dialog for table cell contents

    -----------------------------------------------------------------------

    begin                : Thu Mar 11 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.7  2001/12/30 12:12:57  arnold
    fixed smart appending and editing

    Revision 1.6  2001/12/13 18:39:29  arnold
    added phonetic alphabet stuff

    Revision 1.5  2001/11/17 17:58:55  arnold
    added inline editing of all columns

    Revision 1.4  2001/11/09 14:18:49  arnold
    fixed and improved some dialog pages

    Revision 1.3  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.2  2001/10/30 14:10:32  arnold
    added property 'multiple choice'

    Revision 1.1  2001/10/05 15:40:37  arnold
    import of version 0.7.0pre8 to kde-edu

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef _EntryDlg_CONJUGATION
#define _EntryDlg_CONJUGATION i18n("Con&jugation")
#endif

#ifndef _EntryDlg_ADJECTIVE
#define _EntryDlg_ADJECTIVE i18n("Compar&ison")
#endif

#ifndef _EntryDlg_MULTIPLECHOICE
#define _EntryDlg_MULTIPLECHOICE i18n("&Multiple choice")
#endif

#ifdef __ONLY_TO_BE_SEEN_BY_XGETTEXT
  _EntryDlg_CONJUGATION
  _EntryDlg_ADJECTIVE
  _EntryDlg_MULTIPLECHOICE
#else

#ifndef EntryDlg_included
#define EntryDlg_included

#include "EntryDlgForm.h"
#include <qtable.h>

#include "FromToEntryPage.h"
#include "CommonEntryPage.h"
#include "AuxInfoEntryPage.h"
#include "TenseEntryPage.h"
#include "AdjEntryPage.h"
#include "MCEntryPage.h"


class LangSet;
class QTabWidget;
class KMainWindow;
class kvoctrainDoc;

class EntryDlg : public EntryDlgForm
{
    Q_OBJECT

public:

    enum EditResult {EditCancel, EditApply, EditUndo,
                     EditPageUp, EditPageDown,
                     EditUp, EditDown, EditLeft, EditRight};

    enum EnableType {EnableAll, EnableOnlyCommon, EnableNone, EnableOnlyOriginal };

    EntryDlg
    (   KMainWindow   *main,
        kvoctrainDoc  *doc,
        bool           multi_sel,
        bool           origin,
        grade_t        f_grd,
        grade_t        t_grd,
        count_t        f_qcount,
        count_t        t_qcount,
        count_t        f_bcount,
        count_t        t_bcount,
        time_t         f_qdate,
        time_t         t_qdate,
        QString        f_faux_ami,
        QString        t_faux_ami,
        QString        _expr,
        int            _lesson,
        QComboBox     *_lessonBox,
        QString        lang,
        LangSet       &langset,
        QString        remark,
        QString        _type,
        QString        pronunce,
        QString        synonym,
        QString        antonym,
        QString        example,
        QString        usagelabel,
        QString        paraphrase,
        const          Conjugation &con_prefix,
        const          Conjugation &conjugations,
        const          Article &article,
        const          Comparison &comp,
        const          MultipleChoice &mc,
        QueryManager  &querymanager,
        const QString &title,
        bool           active,
        const QFont&   ipafont,
        QWidget       *parent = 0,
        const char    *name = 0
    );

    ~EntryDlg ();

    void setData
       (kvoctrainDoc  *doc,
        bool           multi_sel,
        bool           origin,
        grade_t        f_grd,
        grade_t        t_grd,
        count_t        f_qcount,
        count_t        t_qcount,
        count_t        f_bcount,
        count_t        t_bcount,
        time_t         f_qdate,
        time_t         t_qdate,
        QString        f_faux_ami,
        QString        t_faux_ami,
        QString        _expr,
        int            _lesson,
        QComboBox     *_lessonBox,
        QString        lang,
        LangSet       &langset,
        QString        remark,
        QString        _type,
        QString        pronunce,
        QString        synonym,
        QString        antonym,
        QString        example,
        QString        usagelabel,
        QString        paraphrase,
        const          Conjugation &con_prefix,
        const          Conjugation &conjugations,
        const          Article &article,
        const          Comparison &comp,
        const          MultipleChoice &mc,
        QueryManager  &querymanager,
	const QString &title,
        bool           active);

    bool  fromDateDirty   () const { return from_page ? from_page->dateDirty() : false; }
    bool  fromGradeDirty  () const { return from_page ? from_page->gradeDirty() : false; }
    bool  fromBCountDirty () const { return from_page ? from_page->bCountDirty() : false; }
    bool  fromQCountDirty () const { return from_page ? from_page->qCountDirty() : false; }
    QString getFromFauxAmi  () const { return from_page ? from_page->getFauxAmi() : QString(""); }

    time_t  getFromDate   () const { return from_page ? from_page->getDate() : 0; }
    grade_t getFromGrade  () const { return from_page ? from_page->getGrade() : KV_NORM_GRADE; }
    count_t getFromBCount () const { return from_page ? from_page->getBCount() : 0; }
    count_t getFromQCount () const { return from_page ? from_page->getQCount() : 0; }

    bool  toDateDirty     () const { return to_page ? to_page->dateDirty() : false; }
    bool  toGradeDirty    () const { return to_page ? to_page->gradeDirty() : false; }
    bool  toBCountDirty   () const { return to_page ? to_page->bCountDirty() : false; }
    bool  toQCountDirty   () const { return to_page ? to_page->qCountDirty() : false; }
    QString getToFauxAmi  () const { return to_page ? to_page->getFauxAmi() : QString(""); }

    time_t  getToDate     () const { return to_page ? to_page->getDate() : 0; }
    grade_t getToGrade    () const { return to_page ? to_page->getGrade() : KV_NORM_GRADE; }
    count_t getToBCount   () const { return to_page ? to_page->getBCount() : 0; }
    count_t getToQCount   () const { return to_page ? to_page->getQCount() : 0; }

    bool    lessonDirty  () const { return comm_page->lessonDirty(); }
    bool    usageDirty   () const { return comm_page->usageDirty(); }
    bool    typeDirty    () const { return comm_page->typeDirty  (); }
    bool    activeDirty  () const { return comm_page->activeDirty  (); }

    int     getLesson   ()  const { return comm_page->getLesson(); }
    QString getType     ()  const { return comm_page->getType(); }
    QString getExpr     ()  const { return comm_page->getExpr(); }
    QString getPronunce ()  const { return comm_page->getPronunce(); }
    QString getUsageLabel() const { return comm_page->getUsageLabel(); }
    bool    getActive()     const { return comm_page->getActive(); }

    QString getSynonym  () const { return aux_page->getSynonym(); }
    QString getAntonym  () const { return aux_page->getAntonym(); }
    QString getRemark   () const { return aux_page->getRemark(); }
    QString getExample  () const { return aux_page->getExample(); }
    QString getParaphrase () const { return aux_page->getParaphrase(); }

    Conjugation getConjugation() const { return tense_page->getConjugation(); }

    Comparison getComparison() const { return adj_page->getComparison(); }

    MultipleChoice getMultipleChoice() const { return mc_page->getMultipleChoice(); }

    bool autoApply () const { return autoapply; }
    void setAutoApply (bool apply);

    bool isModified();
    void setModified(bool mod);
    void setEnabled(int);

    void setCell(int row, int col, const vector<QTableSelection>& sel);
    void getCell(int &row, int &col, vector<QTableSelection>& sel) const;

signals:
    void sigEditChoice(int);

public slots:
    void slotDisplayModified();
    void initFocus();
    void slotApply();
    void slotCancel();
    void slotUndo();
    void slotDockHorizontal();
    void slotDockVertical();

protected slots:
    void updatePages(const QString &type);
    void slotAutoApplyChecked(bool);

protected:

    FromToEntryPage  *from_page,
                     *to_page;
    CommonEntryPage  *comm_page;
    AuxInfoEntryPage *aux_page;
    TenseEntryPage   *tense_page;
    AdjEntryPage     *adj_page;
    MCEntryPage      *mc_page;
    int               edit_row, edit_col;
    QTabWidget       *tabber;

    vector<QTableSelection>& selections;
    KMainWindow     *mainwin;
    QSize            oldMainSize;
    QPoint           oldMainPos;
    bool             docked;
    bool             autoapply;
};


#endif // EntryDlg_included

#endif // __ONLY_TO_BE_SEEN_BY_XGETTEXT
