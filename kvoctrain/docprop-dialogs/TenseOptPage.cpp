/***************************************************************************

    $Id$

                   user tense options dialog page

    -----------------------------------------------------------------------

    begin                : Sun May 28 12:14:31 2000
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.10  2002/04/12 10:09:55  coolo
    replacing tons of these (for gcc 3):
    -      queryList.erase(&queryList[i], &queryList[i+1]);
    +      queryList.erase(queryList.begin() + i);

    Revision 1.9  2002/01/27 07:17:47  binner
    CVS_SILENT Fixed capitalisation.

    Revision 1.8  2001/12/26 15:11:08  mueller
    CVSSILINT: fixincludes

    Revision 1.7  2001/11/11 12:52:26  arnold
    fixed some strings for i18n purposes

    Revision 1.6  2001/11/10 22:27:55  arnold
    removed compatibility for kde1

    Revision 1.5  2001/10/21 15:22:00  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.4  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.3  2001/10/13 21:14:27  arnold
    tested and fixed changes from previous cvs updatecd

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:38:38  arnold
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

#include "TenseOptPage.h"

#include <qkeycode.h>
#include <qlistbox.h>
#include <qpushbutton.h>

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <kvoctraindoc.h>
#include <QueryManager.h>

#include "../common-dialogs/LessonInputDlg.h"

#define TENSE_TAG ". "

TenseOptPage::TenseOptPage
(
        const vector<QString> &tenses,
        kvoctrainDoc    *_doc,
        QWidget         *parent,
        const char      *name,
        bool             modal
)
	:
	TenseOptPageForm( parent, name )
{
	setCaption(i18n("Tenses" ));

	connect( tenseList, SIGNAL(highlighted(int)), SLOT(slotTenseChosen(int)) );
	connect( b_cleanup, SIGNAL(clicked()), SLOT(slotCleanup()) );
	connect( b_delete, SIGNAL(clicked()), SLOT(slotDeleteTense()) );
	connect( b_modify, SIGNAL(clicked()), SLOT(slotModifyTense()) );
	connect( b_new, SIGNAL(clicked()), SLOT(slotNewTense()) );

	setCaption(i18n("Options"));

        QString str;
        for (int i = 0; i < (int) tenses.size(); i++) {
          str.setNum (i+1);
          if (i <= 9)
            str.insert (0, " ");
          tenseList->insertItem (str+TENSE_TAG+tenses[i]);
          tenseIndex.push_back(i);
        }

        act_tense = 0;
        if (tenses.size() != 0)
          tenseList->setCurrentItem (act_tense);

        b_modify->setEnabled(tenseList->count() != 0);
        b_delete->setEnabled(tenseList->count() != 0);

        tenseList->setFocus();
        doc = _doc;
}


void TenseOptPage::initFocus() const
{
  tenseList->setFocus();
}


void TenseOptPage::slotTenseChosen(int index)
{
   act_tense = index;
}


void TenseOptPage::slotNewTense()
{
     LessonInputDlg lid ("", i18n("Input Tense Description"), i18n("Tense description"));
     if (lid.exec() == QDialog::Accepted) {
       QString str;
       int i = tenseList->count()+1;
       str.setNum (i);
       if (i <= 9)
         str.insert (0, " ");
       tenseList->insertItem (str+TENSE_TAG+lid.getInput().stripWhiteSpace());
       tenseIndex.push_back(-(i-1));
       act_tense = tenseList->count();
       tenseList->setCurrentItem (i-1);
       b_modify->setEnabled(true);
       b_delete->setEnabled(true);
     }
}


void TenseOptPage::slotModifyTense()
{
   if (tenseList->count() != 0
       && (int) tenseList->count() > act_tense) {
     QString str = tenseList->text (act_tense);
     int pos = str.find (TENSE_TAG);
     str.remove (0, pos+strlen (TENSE_TAG));
     LessonInputDlg lid (str, i18n("Input Tense Description"), i18n("Tense description"));
     if (lid.exec() == QDialog::Accepted) {
       QString str2;
       str2.setNum (act_tense+1);
       if (act_tense <= 9)
         str2.insert (0, " ");
       tenseList->changeItem (str2+TENSE_TAG+lid.getInput().stripWhiteSpace(), act_tense);
     }
   }
}


void TenseOptPage::updateListBox(int start)
{
  QString str, str2;
  for (int i = start; i < (int) tenseList->count(); i++) {
     str = tenseList->text (i);
     int pos = str.find (TENSE_TAG);
     str.remove (0, pos+strlen (TENSE_TAG));
     str2.setNum (i+1);
     if (i <= 9)
       str2.insert (0, " ");
     tenseList->changeItem (str2+TENSE_TAG+str, i);
  }
}


void TenseOptPage::slotDeleteTense()
{
   int act = act_tense;
   if (tenseList->count() != 0
       && (int) tenseList->count() > act) {

     QString t;
     t.setNum(tenseIndex[act_tense]+1);
     t.insert (0, QM_USER_TYPE);
     for (int ent = 0; ent < doc->numEntries(); ent++) {
       // FIXME: ProgressDlg here?
       kvoctrainExpr *exp = doc->getEntry(ent);
       for (int lang = 0; lang < doc->numLangs(); lang++) {
         Conjugation conj = exp->getConjugation(lang);
         for (int con = 0; con < conj.numEntries(); con++ ) {
           if (conj.getType(con) == t) {
             KMessageBox::information(this,
                       i18n("This user defined tense could not be deleted\nbecause it is in use."),
                       kapp->makeStdCaption(i18n("Deleting a Tense Description")));
             return;
           }
         }
       }
     }

     tenseList->removeItem (act);
     tenseIndex.erase (tenseIndex.begin() + act);

     if ((int) tenseList->count() <= act)
        act = tenseList->count()-1;
     else
        updateListBox(act); // update items after current
  
     if (act >= 0)
       tenseList->setCurrentItem (act);
   }
   b_modify->setEnabled(tenseList->count() != 0);
   b_delete->setEnabled(tenseList->count() != 0);
}


void TenseOptPage::getTenseNames (vector<QString> &ret_tense,
                                vector<int> &ret_Index)
{
   QString str;    ret_tense.clear();
   for (int i = 0; i < (int) tenseList->count(); i++) {
     str = tenseList->text(i);
     int pos = str.find (TENSE_TAG);
     str.remove (0, pos+strlen (TENSE_TAG));
     ret_tense.push_back (str);
   }
   ret_Index = tenseIndex;
}


void TenseOptPage::slotCleanup()
{
   vector<bool> used_tense;
   for (int i = 0; i <= (int) tenseList->count(); i++)
     used_tense.push_back(false);

   for (int col = 0; col < doc->numLangs(); col++)
     for (int i = 0; i < (int) doc->numEntries(); i++) {
       Conjugation conj = doc->getEntry(i)->getConjugation(col);
       for (int ci = 0; ci < conj.numEntries(); ci++) {
         QString t = conj.getType(ci);
         if (t.left(strlen(UL_USER_TENSE)) == UL_USER_TENSE) {
           t.remove (0, strlen(UL_USER_TENSE));
           int idx = t.toInt();
           if ((int) used_tense.size() < idx)
             used_tense.resize(idx);
           if (idx != 0)
             used_tense[idx-1] = true;
         }
       }
     }

   for (int i = used_tense.size()-1; i >= 0; i--)
     if (!used_tense[i]) {
       for (int u = 0; u < (int) tenseIndex.size() ; u++) {
         if (tenseIndex[u] == i || tenseIndex[u] < 0) {
           act_tense = i;
           slotDeleteTense();
           break;
         }
       }
     }

   act_tense = 0;
   tenseList->setCurrentItem (act_tense);
}


void TenseOptPage::cleanUnused(kvoctrainDoc *doc,
                              const vector<int> &tenseIndex,
                              int old_tenses)
{
   vector<int> translate_index;
   vector<QString> new_tenseStr;

   /////////////////////////////////////////////////////
   // translate_index contains new index number for each
   // old index
   for (int i = 0; i <= QMAX (old_tenses, (int) tenseIndex.size()); i++)
     translate_index.push_back(0);

   // now adjust lesson descriptions to new index

   for (int i = 0; i < (int) tenseIndex.size(); i++) {
     if (tenseIndex[i] >= 0)
       translate_index[tenseIndex[i]+1] = i+1;
   }

   // only keep remaining tense indices

   // set tense index to 0 when not needed any more
   // and translate to new index

   for (int col = 0; col < doc->numLangs(); col++) {
     for (int i = 0; i < doc->numEntries(); i++) {
       Conjugation conj = doc->getEntry(i)->getConjugation (col);
       bool dirty = false;
       for (int ci = 0; ci < conj.numEntries(); ci++) {
         QString old = conj.getType(ci);
         if (!old.isEmpty() && old.left(strlen(QM_USER_TYPE)) == QM_USER_TYPE) {
           old.remove (0, 1);
           int o = old.toInt();

           dirty = true;
           QString newtense;
           if (translate_index[o] != 0) {
             newtense.setNum (translate_index[o]);
             newtense.insert (0, QM_USER_TYPE);
             conj.setType(ci, newtense);
           }
           else
             conj.setType(ci, "");
         }
       }
       if (dirty)
         doc->getEntry(i)->setConjugation (col, conj);
     }
   }
}


void TenseOptPage::keyPressEvent( QKeyEvent *e )
{
   if (e->state() & AltButton & ControlButton & ShiftButton == 0) {
     if (  e->key() == Key_Escape )
       emit reject();
     else if (  e->key() == Key_Enter
              ||e->key() == Key_Return)
       emit accept();
     else
       e->ignore();
   }
   else
     e->ignore();
}


//TenseOptPage::~TenseOptPage()
//{
//}

#include "TenseOptPage.moc"
