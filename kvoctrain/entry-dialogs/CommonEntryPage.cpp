/***************************************************************************

    $Id$

                       edit common properties

    -----------------------------------------------------------------------

    begin                : Mon Jun 28 21:02:16 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.17  2002/01/07 17:55:19  arnold
    fixed typo

    Revision 1.16  2002/01/07 17:22:05  arnold
    fixed i18n() issues

    Revision 1.15  2002/01/06 15:57:07  arnold
    fixed handling of usage labels

    Revision 1.14  2002/01/05 18:50:23  arnold
    fixed i18n() related isuses

    Revision 1.13  2002/01/04 21:08:27  binner
    CVS_SILENT Fixed capitalisation.

    Revision 1.12  2001/12/26 15:11:29  mueller
    CVSSILINT: fixincludes

    Revision 1.11  2001/12/13 18:39:29  arnold
    added phonetic alphabet stuff

    Revision 1.10  2001/11/23 18:16:08  mueller
    compile fixes (builddir != stddir, --enable-final)

    Revision 1.9  2001/11/17 17:58:55  arnold
    added inline editing of all columns

    Revision 1.8  2001/11/10 22:28:25  arnold
    removed compatibility for kde1

    Revision 1.7  2001/11/10 21:13:00  arnold
    removed icons and compatibilty for kde1

    Revision 1.6  2001/11/09 14:18:49  arnold
    fixed and improved some dialog pages

    Revision 1.5  2001/11/09 10:40:05  arnold
    removed ability to display a different font for each column

    Revision 1.4  2001/10/23 18:15:39  arnold
    replaced qtarch dialog files with qtdesigner

    Revision 1.3  2001/10/17 21:41:15  waba
    Cleanup & port to Qt3, QTableView -> QTable
    TODO:
    * Fix actions that work on selections
    * Fix sorting
    * Fix language-menu

    Revision 1.2  2001/10/13 11:45:29  coolo
    includemocs and other smaller cleanups. I tried to fix it, but as it's still
    qt2 I can't test :(

    Revision 1.1  2001/10/05 15:40:37  arnold
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

#include "CommonEntryPage.h"

#include <qkeycode.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qgroupbox.h>

#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include <QueryManager.h>
#include <kv_resource.h>
#include <langset.h>

#include "DocPropDlg.h"
#include "LessOptPage.h"
#include "blockall.h"
#include "PhoneticEntryPage.h"


CommonEntryPage::CommonEntryPage
(
        QDialog      *_dlgbook,
        kvoctrainDoc *_doc,
        bool          multi_sel,
        QString       expr,
        int           less,
        QComboBox    *lessbox,
        QString       lang,
        QString       act_type,
        QString       pron,
        QString       act_usage,
        QString       label,
        QueryManager &_querymanager,
        bool          active,
        const QFont&  _ipafont,
	QWidget      *parent,
	const char   *name
)
	:
	CommonEntryPageForm( parent, name ),
	pronunce(pron),
	expression(expr),
        usageCollection (act_usage),
        lesson(less),
        type (act_type),
        dlgbook(_dlgbook),
        doc(_doc),
        querymanager(_querymanager),
        entry_active(active),
        ipafont(_ipafont)
{
        phoneticDlg = 0;

	connect( b_usageDlg, SIGNAL(clicked()), SLOT(invokeUsageDlg()) );
	connect( b_LessDlg, SIGNAL(clicked()), SLOT(invokeLessDlg()) );
	connect( b_pronDlg, SIGNAL(clicked()), SLOT(invokePronDlg()) );
	connect( b_TypeDlg, SIGNAL(clicked()), SLOT(invokeTypeDlg()) );
	connect( usage_box, SIGNAL(selectionChanged()), SLOT(slotUsageChanged()) );
	connect( lesson_box, SIGNAL(highlighted(int)), SLOT(slotLessonSelected(int)) );
	connect( subtype_box, SIGNAL(highlighted(int)), SLOT(slotSubTypeSelected(int)) );
	connect( type_box, SIGNAL(highlighted(int)), SLOT(slotTypeSelected(int)) );
	connect( pronunce_line, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( expr_line, SIGNAL(returnPressed()), SLOT(accept()) );
        connect( c_active, SIGNAL(toggled(bool)), SLOT(slotActiveChanged(bool)) );

	connect( pronunce_line, SIGNAL(textChanged(const QString&)), SLOT(slotPronunceSelected(const QString&)) );
	connect( expr_line, SIGNAL(textChanged(const QString&)), SLOT(slotExprSelected(const QString&)) );

    usage_label->setTitle(i18n("usage (area) of an expression", "&Usage Labels"));
    pronunce_line->setFont(ipafont);
    setLessonBox (lessbox, less);
    lesson_label->setBuddy(lesson_box);

    setUsageBox (usageCollection);

    setTypeBox(act_type);
    subtype_label->setBuddy(subtype_box);

    int start = -1;
    int i = 0;
    while (start < 0 && i < (int) all_types.size()) {
      if (all_types [i].shortStr() == QueryManager::getMainType(act_type))
        start = i;
      i++;
    }
    int offset = -1;
    while (offset < 0 && i < (int) all_types.size()) {
      if (all_types [i].shortStr() == act_type)
        offset = i - start;
      i++;
    }
    if (offset >= 0) {
      slotSubTypeSelected(offset-1);
      subtype_box->setCurrentItem(offset);
    }

    pronunce_line->setText(pronunce);
    pronunce_label->setBuddy(pronunce_line);

    c_active->setChecked(entry_active);
    lesson_dirty = false;

    expr_label->setText( label );
    expr_label->setBuddy(expr_line);

    expr_line->setText(expression);

    lesson_box->setValidator (new BlockAllValidator() );
    type_box->setValidator (new BlockAllValidator() );
    subtype_box->setValidator (new BlockAllValidator() );
    if (multi_sel) {
      expr_line->setEnabled (false);
      pronunce_line->setEnabled (false);
      expr_line->setText ("");
      pronunce_line->setText ("");
      lesson_box->clearEdit();
      type_box->clearEdit();
      subtype_box->clearEdit();
    }
    else {
      expr_line->selectAll();
      expr_line->setFocus();
    }

    // list-win and pron-win have same size
    // buttons also have same size
    QPixmap list_pm = QPixmap(locate("data", "kvoctrain/list-win.xpm"));
    int x_add = b_LessDlg->width() - list_pm.width() +4;
    int y_add = b_LessDlg->height() - list_pm.height() +4;

    lesson_dirty = false;
    b_LessDlg->setGeometry( b_LessDlg->x()-2, b_LessDlg->y()-2,
                   list_pm.width()+x_add, list_pm.height()+y_add );
    b_LessDlg->setPixmap(list_pm);

    type_dirty = false;
    b_TypeDlg->setGeometry( b_TypeDlg->x()-2, b_TypeDlg->y()-2,
                   list_pm.width()+x_add, list_pm.height()+y_add );
    b_TypeDlg->setPixmap(list_pm);

    b_usageDlg->setGeometry( b_usageDlg->x()-2, b_usageDlg->y()-2,
                   list_pm.width()+x_add, list_pm.height()+y_add );
    b_usageDlg->setPixmap(list_pm);

    QPixmap pron_pm = QPixmap(locate("data", "kvoctrain/pron-win.xpm"));
    b_pronDlg->setGeometry( b_pronDlg->x()-2, b_pronDlg->y()-2,
                   pron_pm.width()+x_add, pron_pm.height()+y_add );
    b_pronDlg->setPixmap(pron_pm);
}


void CommonEntryPage::initFocus() const
{
  expr_line->setFocus();
}


CommonEntryPage::~CommonEntryPage()
{
  deletePronDlg();
}


void CommonEntryPage::setTypeBox(const QString &act_type)
{
    all_types = QueryManager::getRelation(false);
    all_maintypes = QueryManager::getRelation(true);

    QString s = QueryManager::getMainType(act_type)+QM_TYPE_DIV;
    int curr_type = 0;
    type_box->clear();
    type_box->insertItem (i18n("<none>"));
    for (int i = 0; i < (int) all_maintypes.size(); i++) {
      type_box->insertItem (all_maintypes[i].longStr());
      if (s == all_maintypes[i].shortStr()+QM_TYPE_DIV)
        curr_type = i+1;
    }
    type_box->setCurrentItem(curr_type);
    slotTypeSelected(curr_type);
}


void CommonEntryPage::setLessonBox(QComboBox *lessbox, int lesson)
{
    lesson_box->clear();
    for (int i = 0; i < lessbox->count(); i++)
      lesson_box->insertItem (lessbox->text(i));
    if (lesson >= lesson_box->count() )
      lesson = 0;
    lesson_box->setCurrentItem(lesson);
}


void CommonEntryPage::setUsageBox(QString act_usage)
{
    usages = UsageManager::getRelation();
    usage_box->clear();
    for (int i = 0; i < (int) usages.size(); i++) {
      usage_box->insertItem (usages[i].longStr());
      if (UsageManager::contains(QString(usages[i].identStr()), act_usage)) {
        usage_box->setSelected (i, true);
      }
    }
    slotUsageChanged();
}


void CommonEntryPage::slotUsageChanged()
{
   usageCollection = "";
   QString s;
   for (int i = 0; i < (int) usage_box->count(); i++) {
     if (usage_box->isSelected(i)) {

       if (!usageCollection.isEmpty() )
         usageCollection += UL_USAGE_DIV;
       usageCollection += usages[i].identStr();

       if (!s.isEmpty() )
         s += ", ";
       s += usages[i].shortStr();
     }
   }
   usage_line->setText (s);
}


void CommonEntryPage::slotLessonSelected (int l)
{
  lesson = l;
  lesson_dirty = true;
}


void CommonEntryPage::slotActiveChanged(bool state)
{
  entry_active = state;
  active_dirty = true;
}


void CommonEntryPage::slotExprSelected (const QString& s)
{
  expression = s;
}


void CommonEntryPage::slotPronunceSelected (const QString& s)
{
  pronunce = s;
}


void CommonEntryPage::slotSubTypeSelected(int i)
{
   if (i < (int) current_subtypes.size()) {
     type = current_subtypes[i];
     emit typeSelected(type);
     type_dirty = true;
   }
}


void CommonEntryPage::slotTypeSelected(int idx)
{
   subtype_box->clear();
   current_subtypes.clear();
   bool first = true;

   if (idx == 0) { // 0 == none !
     type = "";
     emit typeSelected(type);
   }
   else {
     type = all_maintypes[idx-1].shortStr();
     emit typeSelected(type);
     QString main_patt = all_maintypes[idx-1].shortStr()+QM_TYPE_DIV;
     int sub_idx;
     if ( idx-1 < (int) all_types.size() ){
       for (sub_idx = 0; sub_idx < (int) all_types.size(); sub_idx++) {
         if (all_types[sub_idx].shortStr().left(main_patt.length()) == main_patt) {
           if (first) {
             subtype_box->insertItem (i18n("<none>"));
             current_subtypes.push_back(all_maintypes[idx-1].shortStr());
             first = false;
           }
           QString s = all_types[sub_idx].longStr();
           subtype_box->insertItem (s.stripWhiteSpace());
           current_subtypes.push_back(all_types[sub_idx].shortStr());
         }
       }
     }
   }
   type_dirty = true;

   subtype_box->setEnabled(!first);
   subtype_label->setEnabled(!first);
}


void CommonEntryPage::deletePronDlg()
{
  delete phoneticDlg;
  phoneticDlg = 0;
}


void CommonEntryPage::phoneticSelected(wchar_t wc)
{
  pronunce += QChar(wc);
  pronunce_line->setText(pronunce);
}


void CommonEntryPage::invokePronDlg()
{
  if (phoneticDlg == 0) {
    phoneticDlg = new PhoneticEntryPage (ipafont, this);
    connect (phoneticDlg, SIGNAL(wantClose()), SLOT(deletePronDlg()) );
    connect (phoneticDlg, SIGNAL(charSelected(wchar_t)), SLOT(phoneticSelected(wchar_t)) );
    phoneticDlg->show();
  }
  else
    phoneticDlg->show();
}


void CommonEntryPage::invokeUsageDlg()
{
  vector<int> usageIndex;
  vector<QString> new_usageStr;

  int old_usages = (int) doc->getUsageDescr().size();
  QTabDialog usageOpt(0, "", true);
  usageOpt.setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
  usageOpt.setCaption (kapp->makeStdCaption(i18n("usage (area) of an expression",
                                                 "Edit User-Defined Usage Labels")));
  UsageOptPage *usageOptPage
    = new UsageOptPage (doc->getUsageDescr(), doc, this, "name");
  usageOpt.addTab( usageOptPage, _DocDlg_USAGES);

  usageOpt.setCancelButton();
  connect( &usageOpt, SIGNAL(applyButtonPressed()),  &usageOpt, SLOT(accept()) );
  connect( &usageOpt, SIGNAL(cancelButtonPressed()), &usageOpt, SLOT(reject()) );

  if (usageOpt.exec() == QDialog::Accepted) {
    usageOptPage->getUsageLabels(new_usageStr, usageIndex);
    UsageOptPage::cleanUnused(doc, usageIndex, old_usages);
    UsageManager::setUsageNames (new_usageStr);
    setUsageBox(usageCollection);
    doc->setUsageDescr (new_usageStr);
    doc->setModified();
  }
}


void CommonEntryPage::invokeLessDlg()
{
  vector<int> lessonIndex;
  vector<QString> new_lessonStr;

/*
  int act_less = lesson_box->currentItem();
*/
  int old_lessons = (int) lesson_box->count();
  QTabDialog lessOpt(0, "", true);
  lessOpt.setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));

  lessOpt.setCaption (kapp->makeStdCaption(i18n("Edit Lesson Names")));
  LessOptPage *lessOptPage = new LessOptPage (lesson_box, doc, this, "name");
  lessOpt.addTab( lessOptPage, _DocDlg_LESSONS);

  lessOpt.setCancelButton();
  connect( &lessOpt, SIGNAL(applyButtonPressed()),  &lessOpt, SLOT(accept()) );
  connect( &lessOpt, SIGNAL(cancelButtonPressed()), &lessOpt, SLOT(reject()) );

  vector<int> lessoninquery = doc->getLessonsInQuery();
  if (lessOpt.exec() == QDialog::Accepted) {
    lessOptPage->getLesson(lesson_box, lessonIndex);
    LessOptPage::cleanUnused(doc, lesson_box, lessonIndex, old_lessons, lessoninquery);
    for (int i = 1; i < lesson_box->count(); i++)
      new_lessonStr.push_back(lesson_box->text(i));
    doc->setLessonDescr (new_lessonStr);
    doc->setLessonsInQuery(lessoninquery);
    querymanager.setLessonItems(lessoninquery);
    doc->setModified();
  }
}


void CommonEntryPage::invokeTypeDlg()
{
  vector<int> typeIndex;
  vector<QString> new_typeStr;

  int old_types = (int) doc->getTypeDescr().size();
  QTabDialog typeOpt(0, "", true);
  typeOpt.setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
  typeOpt.setCaption (kapp->makeStdCaption(i18n("Edit User Defined Types")));
  TypeOptPage *typeOptPage
    = new TypeOptPage (doc->getTypeDescr(), doc, this, "name");
  typeOpt.addTab( typeOptPage, _DocDlg_TYPES);

  typeOpt.setCancelButton();
  connect( &typeOpt, SIGNAL(applyButtonPressed()),  &typeOpt, SLOT(accept()) );
  connect( &typeOpt, SIGNAL(cancelButtonPressed()), &typeOpt, SLOT(reject()) );

  if (typeOpt.exec() == QDialog::Accepted) {
    typeOptPage->getTypeNames(new_typeStr, typeIndex);
    TypeOptPage::cleanUnused(doc, typeIndex, old_types);
    QueryManager::setTypeNames (new_typeStr);
    setTypeBox(type);
    doc->setTypeDescr (new_typeStr);
    doc->setModified();
  }
}


void CommonEntryPage::keyPressEvent( QKeyEvent *e )
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
#include "CommonEntryPage.moc"
