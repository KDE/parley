/***************************************************************************

                    viewing options dialog page

    -----------------------------------------------------------------------

    begin                : Fri Sep 10 16:42:01 1999

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

#include <kapplication.h>
#include <kfontdialog.h>
#include <kcolorbutton.h>
#include <klocale.h>

#include <qlabel.h>
#include <qcheckbox.h>

#include "ViewOptPage.h"

#include <QueryManager.h>

#include <kstandarddirs.h>

ViewOptPage::ViewOptPage
(
        QFont        &_tablefont,
        QFont        &_ipa_font,
        GradeCols    &cols,
        QueryManager *,
	QWidget      *parent,
	const char   *name
)
	:
	ViewOptPageForm( parent, name ),
        gc(cols),
        tablefont(_tablefont),
        ipa_font(_ipa_font)
{
   connect( kcfg_gradeCol0, SIGNAL(changed(const QColor&)), SLOT(slotCol0(const QColor&)) );
   connect( kcfg_gradeCol1, SIGNAL(changed(const QColor&)), SLOT(slotCol1(const QColor&)) );
   connect( kcfg_gradeCol2, SIGNAL(changed(const QColor&)), SLOT(slotCol2(const QColor&)) );
   connect( kcfg_gradeCol3, SIGNAL(changed(const QColor&)), SLOT(slotCol3(const QColor&)) );
   connect( kcfg_gradeCol4, SIGNAL(changed(const QColor&)), SLOT(slotCol4(const QColor&)) );
   connect( kcfg_gradeCol5, SIGNAL(changed(const QColor&)), SLOT(slotCol5(const QColor&)) );
   connect( kcfg_gradeCol6, SIGNAL(changed(const QColor&)), SLOT(slotCol6(const QColor&)) );
   connect( kcfg_gradeCol7, SIGNAL(changed(const QColor&)), SLOT(slotCol7(const QColor&)) );
   connect( kcfg_useGradeCol, SIGNAL(toggled(bool)), SLOT(slotColUsed(bool)) );
   connect( b_choose, SIGNAL(clicked()), SLOT(slotChooseFont()) );
   connect( b_choose_ipa, SIGNAL(clicked()), SLOT(slotChooseIPAFont()) );

   setCaption( kapp->makeStdCaption(i18n("Document Properties" )));

   e_font->setText (QString("%1 %2pt").arg(tablefont.family()).arg(tablefont.pointSize()));
   e_ipa_font->setText (QString("%1 %2pt").arg(ipa_font.family()).arg(ipa_font.pointSize()));

   kcfg_gradeCol0->setColor (cols.col0);
   kcfg_gradeCol1->setColor (cols.col1);
   kcfg_gradeCol2->setColor (cols.col2);
   kcfg_gradeCol3->setColor (cols.col3);
   kcfg_gradeCol4->setColor (cols.col4);
   kcfg_gradeCol5->setColor (cols.col5);
   kcfg_gradeCol6->setColor (cols.col6);
   kcfg_gradeCol7->setColor (cols.col7);
   kcfg_useGradeCol->setChecked (gc.use);
   slotColUsed(gc.use);
}


void ViewOptPage::initFocus() const
{
  kcfg_useGradeCol->setFocus();
}


void ViewOptPage::slotChooseFont()
{
        KFontDialog fdlg (0L, 0L, false, true);
        fdlg.setCaption(i18n("Choose Table Font"));
        fdlg.setFont(tablefont);
        if (fdlg.exec() == QDialog::Accepted ) {
        tablefont = fdlg.font();
        tablefont.setWeight(QFont::Normal);
        tablefont.setStrikeOut(false);
        tablefont.setUnderline(false);
        e_font->setText (QString("%1 %2pt").arg(tablefont.family()).arg(tablefont.pointSize()));
       }
}


void ViewOptPage::slotChooseIPAFont()
{
   KFontDialog fdlg (0L, 0L, false, true);
   fdlg.setCaption(i18n("Choose IPA Font"));
   fdlg.setFont(ipa_font);
   if (fdlg.exec() == QDialog::Accepted ) {
     ipa_font = fdlg.font();
     ipa_font.setWeight(QFont::Normal);
     ipa_font.setStrikeOut(false);
     ipa_font.setUnderline(false);
     e_ipa_font->setText (QString("%1 %2pt").arg(ipa_font.family()).arg(ipa_font.pointSize()));
   }
}


void ViewOptPage::slotCol0(const QColor &col)
{
  gc.col0 = col;
}


void ViewOptPage::slotCol1(const QColor &col)
{
  gc.col1 = col;
}


void ViewOptPage::slotCol2(const QColor &col)
{
  gc.col2 = col;
}


void ViewOptPage::slotCol3(const QColor &col)
{
  gc.col3 = col;
}


void ViewOptPage::slotCol4(const QColor &col)
{
  gc.col4 = col;
}


void ViewOptPage::slotCol5(const QColor &col)
{
  gc.col5 = col;
}


void ViewOptPage::slotCol6(const QColor &col)
{
  gc.col6 = col;
}


void ViewOptPage::slotCol7(const QColor &col)
{
  gc.col7 = col;
}


void ViewOptPage::slotColUsed(bool used)
{
  gc.use = used;

  kcfg_gradeCol0->setEnabled(used);
  kcfg_gradeCol1->setEnabled(used);
  kcfg_gradeCol2->setEnabled(used);
  kcfg_gradeCol3->setEnabled(used);
  kcfg_gradeCol4->setEnabled(used);
  kcfg_gradeCol5->setEnabled(used);
  kcfg_gradeCol6->setEnabled(used);
  kcfg_gradeCol7->setEnabled(used);

  l_lev0->setEnabled(used);
  l_lev1->setEnabled(used);
  l_lev2->setEnabled(used);
  l_lev3->setEnabled(used);
  l_lev4->setEnabled(used);
  l_lev5->setEnabled(used);
  l_lev6->setEnabled(used);
  l_lev7->setEnabled(used);
}

void ViewOptPage::keyPressEvent( QKeyEvent *e )
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
#include "ViewOptPage.moc"
