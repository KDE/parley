/***************************************************************************

    $Id$

                   usage labels dialog page

    -----------------------------------------------------------------------

    begin                : Thu Mar 30 20:38:31 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.9  2002/01/07 17:54:59  arnold
    fixed typo

    Revision 1.8  2002/01/07 17:21:34  arnold
    fixed i18n() issues

    Revision 1.7  2001/12/26 15:11:08  mueller
    CVSSILINT: fixincludes

    Revision 1.6  2001/11/11 12:52:26  arnold
    fixed some strings for i18n purposes

    Revision 1.5  2001/11/10 22:27:55  arnold
    removed compatibility for kde1

    Revision 1.4  2001/10/21 15:22:00  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

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

#include "UsageOptPage.h"

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <qkeycode.h>
#include <qlistbox.h>
#include <qgroupbox.h>
#include <qpushbutton.h>

#include <kvoctraindoc.h>
#include <QueryManager.h>

#include "../common-dialogs/LessonInputDlg.h"

#define USAGE_TAG ". "

UsageOptPage::UsageOptPage
(
        const vector<QString> &usages,
        kvoctrainDoc    *_doc,
	QWidget         *parent,
	const char      *name,
        bool             modal
)
	:
	UsageOptPageForm( parent, name ),
        doc(_doc)
{
	usage_group->setTitle(i18n("usage (area) of an expression", "Usage Labels" ));

	connect( b_cleanup, SIGNAL(clicked()), SLOT(slotCleanup()) );
	connect( b_delete, SIGNAL(clicked()), SLOT(slotDeleteUsage()) );
	connect( b_modify, SIGNAL(clicked()), SLOT(slotModifyUsage()) );
	connect( b_new, SIGNAL(clicked()), SLOT(slotNewUsage()) );
	connect( usageList, SIGNAL(highlighted(int)), SLOT(slotUsageChosen(int)) );

        QString str;
        for (int i = 0; i < (int) usages.size(); i++) {
          str.setNum (i+1);
          if (i <= 9)
            str.insert (0, " ");
          usageList->insertItem (str+USAGE_TAG+usages[i]);
          usageIndex.push_back(i);
        }

        act_usage = 0;
        if (usages.size() != 0)
          usageList->setCurrentItem (act_usage);

        b_modify->setEnabled(usageList->count() != 0);
        b_delete->setEnabled(usageList->count() != 0);

        usageList->setFocus();
}


void UsageOptPage::initFocus() const
{
  usageList->setFocus();
}


void UsageOptPage::slotUsageChosen(int index)
{
   act_usage = index;
}


void UsageOptPage::slotNewUsage()
{
     LessonInputDlg lid ("", i18n("usage (area) of an expression", "Input usage description"),
                             i18n("usage (area) of an expression", "Usage description"));
     if (lid.exec() == QDialog::Accepted) {
       QString str;
       int i = usageList->count()+1;
       str.setNum (i);
       if (i <= 9)
         str.insert (0, " ");
       usageList->insertItem (str+USAGE_TAG+lid.getInput().stripWhiteSpace());
       usageIndex.push_back(-(i-1));
       act_usage = usageList->count();
       usageList->setCurrentItem (i-1);
       b_modify->setEnabled(true);
       b_delete->setEnabled(true);
     }
}


void UsageOptPage::slotModifyUsage()
{
   if (usageList->count() != 0
       && (int) usageList->count() > act_usage) {
     QString str = usageList->text (act_usage);
     int pos = str.find (USAGE_TAG);
     str.remove (0, pos+strlen (USAGE_TAG));
     LessonInputDlg lid (str, i18n("usage (area) of an expression", "Input usage description"),
                              i18n("usage (area) of an expression", "Usage description"));
     if (lid.exec() == QDialog::Accepted) {
       QString str2;
       str2.setNum (act_usage+1);
       if (act_usage <= 9)
         str2.insert (0, " ");
       usageList->changeItem (str2+USAGE_TAG+lid.getInput().stripWhiteSpace(), act_usage);
     }
   }
}


void UsageOptPage::updateListBox(int start)
{
  QString str, str2;
  for (int i = start; i < (int) usageList->count(); i++) {
     str = usageList->text (i);
     int pos = str.find (USAGE_TAG);
     str.remove (0, pos+strlen (USAGE_TAG));
     str2.setNum (i+1);
     if (i <= 9)
       str2.insert (0, " ");
     usageList->changeItem (str2+USAGE_TAG+str, i);
  }
}


void UsageOptPage::slotDeleteUsage()
{
   int act = act_usage;
   if (usageList->count() != 0
       && (int) usageList->count() > act) {

     QString t;
     t.setNum(usageIndex[act_usage]+1);
     t.insert (0,  UL_USER_USAGE);
     t += UL_USAGE_DIV;
     for (int ent = 0; ent < doc->numEntries(); ent++) {
       // FIXME: ProgressDlg here?
       kvoctrainExpr *exp = doc->getEntry(ent);
       for (int lang = 0; lang < doc->numLangs(); lang++) {
         QString ul = exp->getUsageLabel(lang) + UL_USAGE_DIV;
         if (ul.find(t) >= 0 ) {
           KMessageBox::information(this,
                     i18n("usage (area) of an expression",
                          "This user-defined usage label could not be deleted\n"
                          "because it is in use."),
                     kapp->makeStdCaption(i18n("usage (area) of an expression",
                                               "Deleting a usage label")));
           return;
         }
       }
     }

     usageList->removeItem (act);
     usageIndex.erase (&usageIndex[act], &usageIndex[act+1]);

     if ((int) usageList->count() <= act)
       act = usageList->count()-1;
     else
       updateListBox(act); // update items after current
  
     if (act >= 0)
       usageList->setCurrentItem (act);
   }
   b_modify->setEnabled(usageList->count() != 0);
   b_delete->setEnabled(usageList->count() != 0);
}


void UsageOptPage::getUsageLabels (vector<QString> &ret_usage,
                                   vector<int> &ret_Index)
{
   QString str;    ret_usage.clear();
   for (int i = 0; i < (int) usageList->count(); i++) {
     str = usageList->text(i);
     int pos = str.find (USAGE_TAG);
     str.remove (0, pos+strlen (USAGE_TAG));
     ret_usage.push_back (str);
   }
   ret_Index = usageIndex;
}


void UsageOptPage::slotCleanup()
{

   vector<bool> used_usage;
   for (int i = 0; i <= (int) usageList->count(); i++)
     used_usage.push_back(false);

   for (int col = 0; col < doc->numLangs(); col++)
     for (int i = 0; i < (int) doc->numEntries(); i++) {
       QString t = doc->getEntry(i)->getUsageLabel(col);
       QString n;
       while (t.left(strlen(UL_USER_USAGE)) == UL_USER_USAGE) {
         t.remove (0, 1);

         int next;
         if ((next = t.find(UL_USAGE_DIV)) >= 0) {
           n = t.left(next);
           t.remove (0, next+1);
         }
         else {
           n = t;
           t = "";
         }

         int idx = n.toInt();
         if ((int) used_usage.size() < idx)
           used_usage.resize(idx);
         if (idx != 0)
           used_usage[idx-1] = true ;
       }
     }

   for (int i = used_usage.size()-1; i >= 0; i--) {
     if (!used_usage[i]) {
       for (int u = 0; u < (int) usageIndex.size() ; u++) {
         if (usageIndex[u] == i || usageIndex[u] < 0) {
           act_usage = i;
           slotDeleteUsage();
           break;
         }
       }
     }
   }

   act_usage = 0;
   usageList->setCurrentItem (act_usage);
}


void UsageOptPage::cleanUnused(kvoctrainDoc *doc,
                               const vector<int> &usageIndex,
                               int old_usages)
{
   vector<int> translate_index;
   vector<QString> new_usageStr;

   /////////////////////////////////////////////////////
   // translate_index contains new index number for each
   // old index
   for (int i = 0; i <= QMAX (old_usages, (int) usageIndex.size()); i++)
     translate_index.push_back(0);

   // now adjust lesson descriptions to new index

   for (int i = 0; i < (int) usageIndex.size(); i++) {
     if (usageIndex[i] >= 0)
       translate_index[usageIndex[i]+1] = i+1;
   }

   // only keep remaining usage indices

   // set usage index to 0 when not needed any more
   // and translate to new index

   for (int col = 0; col < doc->numLangs(); col++) {
     for (int i = 0; i < doc->numEntries(); i++) {
       QString t = doc->getEntry(i)->getUsageLabel (col);
       if (!t.isEmpty() && t.left(strlen(UL_USER_USAGE)) == UL_USER_USAGE) {
         QString tg;
         while (t.left(strlen(UL_USER_USAGE)) == UL_USER_USAGE) {
           QString n;
           t.remove (0, 1);
           int next;
           if ((next = t.find(UL_USAGE_DIV)) >= 0) {
             n = t.left(next);
             t.remove (0, next+1);
           }
           else {
             n = t;
             t = "";
           }
  
           QString newusage;
           int o = n.toInt();
           if (translate_index[o] != 0) {
             newusage.setNum (translate_index[o]);
             newusage.insert (0, UL_USER_USAGE);
           }

           if (tg.length() == 0)
             tg = newusage;
           else
             if (newusage.length() )
               tg += UL_USAGE_DIV + newusage;
         }

         if (tg.length() == 0)
           tg = t;
         else if (t.length() != 0)
           tg += UL_USAGE_DIV + t;

         doc->getEntry(i)->setUsageLabel (col, tg);
       }
     }
   }

}


void UsageOptPage::keyPressEvent( QKeyEvent *e )
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
#include "UsageOptPage.moc"
