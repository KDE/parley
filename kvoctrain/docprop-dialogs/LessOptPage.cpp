/***************************************************************************

    $Id$

                   lesson properties dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 22:03:50 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
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

#include "LessOptPage.h"

#include <kapp.h>

#include <kmessagebox.h>

#include <qcombobox.h>
#include <qkeycode.h>
#include <qlistbox.h>
#include <qpushbutton.h>

#include "../common-dialogs/LessonInputDlg.h"

#include <kvoctraindoc.h>

#define LESS_TAG ". "

LessOptPage::LessOptPage
(
        QComboBox     *lessons,
        kvoctrainDoc  *_doc,
	QWidget       *parent,
	const char    *name,
        bool           modal
)
	:
	LessOptPageForm( parent, name )
{
	connect( lessonList, SIGNAL(highlighted(int)), SLOT(slotLessonChosen(int)) );
	connect( b_new, SIGNAL(clicked()), SLOT(slotNewLesson()) );
	connect( b_modify, SIGNAL(clicked()), SLOT(slotModifyLesson()) );
	connect( b_delete, SIGNAL(clicked()), SLOT(slotDeleteLesson()) );
	connect( b_cleanup, SIGNAL(clicked()), SLOT(slotCleanup()) );

        doc = _doc;
	setCaption(i18n("Options"));

        QString str;
        for (int i = 1; i < lessons->count(); i++) {
          str.setNum (i);
          if (i <= 9)
            str.insert (0, " ");
          lessonList->insertItem (str+LESS_TAG+lessons->text(i));
          lessonIndex.push_back(i-1);
        }

        act_lesson = 0;
        if (lessonList->count() != 0)
          lessonList->setCurrentItem (act_lesson);
        lessonList->setFocus();

        b_modify->setEnabled(lessonList->count() != 0);
        b_delete->setEnabled(lessonList->count() != 0);
}


void LessOptPage::initFocus() const
{
  lessonList->setFocus();
}


void LessOptPage::slotLessonChosen(int index)
{
   act_lesson = index;
}


void LessOptPage::slotNewLesson()
{
     LessonInputDlg lid ("", i18n("Input lesson description"), i18n("Lesson description"));
     if (lid.exec() == QDialog::Accepted) {
       QString str;
       int i = lessonList->count()+1;
       str.setNum (i);
       if (i <= 9)
         str.insert (0, " ");
       lessonList->insertItem (str+LESS_TAG+lid.getInput().stripWhiteSpace());
       lessonIndex.push_back(-(i-1));
       act_lesson = lessonList->count();
       lessonList->setCurrentItem (i-1);
       b_modify->setEnabled(true);
       b_delete->setEnabled(true);
     }
}


void LessOptPage::slotModifyLesson()
{
   if (lessonList->count() != 0
       && (int) lessonList->count() > act_lesson) {
     QString str = lessonList->text (act_lesson);
     int pos = str.find (LESS_TAG);
     str.remove (0, pos+strlen (LESS_TAG));
     LessonInputDlg lid (str, i18n("Input lesson description"), i18n("Lesson description"));
     if (lid.exec() == QDialog::Accepted) {
       QString str2;
       str2.setNum (act_lesson+1);
       if (act_lesson <= 9)
         str2.insert (0, " ");
       lessonList->changeItem (str2+LESS_TAG+lid.getInput().stripWhiteSpace(), act_lesson);
     }
   }
}


void LessOptPage::updateListBox(int start)
{
  QString str, str2;
  for (int i = start; i < (int) lessonList->count(); i++) {
     str = lessonList->text (i);
     int pos = str.find (LESS_TAG);
     str.remove (0, pos+strlen (LESS_TAG));
     str2.setNum (i+1);
     if (i <= 9)
       str2.insert (0, " ");
     lessonList->changeItem (str2+LESS_TAG+str, i);
  }
}


void LessOptPage::slotDeleteLesson()
{
   int act = act_lesson;
   if (lessonList->count() != 0
       && (int) lessonList->count() > act) {

     for (int ent = 0; ent < doc->numEntries(); ent++) {
       // FIXME: ProgressDlg here?
       if (doc->getEntry(ent)->getLesson() == lessonIndex[act_lesson]+1) {
         KMessageBox::information(this,
                   i18n("Can't delete this lesson\nbecause it is in use."),
                   kapp->makeStdCaption(i18n("Deleting a lesson")));
         return;
       }
     }

     lessonList->removeItem (act);
     lessonIndex.erase (&lessonIndex[act], &lessonIndex[act+1]);

     if ((int) lessonList->count() <= act)
        act = lessonList->count()-1;
     else
        updateListBox(act); // update items after current
  
     if (act >= 0)
       lessonList->setCurrentItem (act);
   }
   b_modify->setEnabled(lessonList->count() != 0);
   b_delete->setEnabled(lessonList->count() != 0);
}


void LessOptPage::getLesson (QComboBox *ret_lesson, vector<int> &ret_Index)
{
        while (ret_lesson->count() > 1) /* first entry is "no lesson" */
          ret_lesson->removeItem (1);

        QString str;
        for (int i = 0; i < (int) lessonList->count(); i++) {
          str = lessonList->text(i);
          int pos = str.find (LESS_TAG);
          str.remove (0, pos+strlen (LESS_TAG));
          ret_lesson->insertItem (str);
        }
        ret_Index = lessonIndex;
}


void LessOptPage::slotCleanup()
{
   vector<bool> used_lesson;
   for (int i = 0; i < (int) lessonList->count(); i++)
     used_lesson.push_back(false);

   for (int i = 0; i < (int) doc->numEntries(); i++) {
     int idx = doc->getEntry(i)->getLesson();
     if ((int) used_lesson.size() < idx)
       used_lesson.resize(idx);
     if (idx != 0)
       used_lesson[idx-1] = true ;
   }

   for (int i = used_lesson.size()-1; i >= 0; i--)
     if (!used_lesson[i]) {
       for (int u = 0; u < (int) lessonIndex.size() ; u++) {
         if (lessonIndex[u] == i || lessonIndex[u] < 0) {
           act_lesson = i;
           slotDeleteLesson();
           break;
         }
       }
     }

   act_lesson = 0;
   lessonList->setCurrentItem (act_lesson);
}


void LessOptPage::keyPressEvent( QKeyEvent *e )
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


void LessOptPage::cleanUnused (kvoctrainDoc *doc,
                               const QComboBox *lessons,
                               const vector<int> &lessonIndex,
                               int old_lessons,
                               vector<int> &lessons_in_query)
{
   vector<int> translate_index;

   /////////////////////////////////////////////////////
   // translate_index contains new index number for each
   // old index
   for (int i = 0; i <= QMAX (old_lessons, (int) lessonIndex.size()); i++)
     translate_index.push_back(0);
 
   // now adjust lesson descriptions to new index
   for (int i = 0; i < (int) lessonIndex.size(); i++) {
     if (lessonIndex[i] >= 0)
       translate_index[lessonIndex[i]+1] = i+1;
   }
 
   // now adjust for query selected lessons to new index
   for (int i = (int) lessons_in_query.size()-1; i >= 0; i--) {
     if (translate_index[lessons_in_query[i]] > 0)
       lessons_in_query[i] = translate_index[lessons_in_query[i]];
     else
       lessons_in_query.erase(&lessons_in_query[i]);
   }

   // only keep remaining lesson member indices
 
   // set lesson index to 0 when not needed any more
   // and translate to new index
   for (int i = 0; i < doc->numEntries(); i++) {
     if (doc->getEntry(i)->getLesson () != 0)
       doc->getEntry(i)->setLesson (translate_index[doc->getEntry(i)->getLesson ()]);
   }
 
}
#include "LessOptPage.moc"
