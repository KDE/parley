/***************************************************************************

    $Id$

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

#include <qkeycode.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "ViewOptPage.h"
#include "LangOptPage.h"

#include <QueryManager.h>
#include <kv_resource.h>

#include <kstandarddirs.h>
#include  <kconfig.h>

ViewOptPage::ViewOptPage
(
        QFont        &_tablefont,
        QFont        &_ipa_font,
        GradeCols    &cols,
        QueryManager *manager,
	QWidget      *parent,
	const char   *name
)
	:
	ViewOptPageForm( parent, name ),
        gc(cols),
        tablefont(_tablefont),
        ipa_font(_ipa_font)
{
   connect( b_col0, SIGNAL(changed(const QColor&)), SLOT(slotCol0(const QColor&)) );
   connect( b_col1, SIGNAL(changed(const QColor&)), SLOT(slotCol1(const QColor&)) );
   connect( b_col2, SIGNAL(changed(const QColor&)), SLOT(slotCol2(const QColor&)) );
   connect( b_col3, SIGNAL(changed(const QColor&)), SLOT(slotCol3(const QColor&)) );
   connect( b_col4, SIGNAL(changed(const QColor&)), SLOT(slotCol4(const QColor&)) );
   connect( b_col5, SIGNAL(changed(const QColor&)), SLOT(slotCol5(const QColor&)) );
   connect( b_col6, SIGNAL(changed(const QColor&)), SLOT(slotCol6(const QColor&)) );
   connect( b_col7, SIGNAL(changed(const QColor&)), SLOT(slotCol7(const QColor&)) );
   connect( c_use, SIGNAL(toggled(bool)), SLOT(slotColUsed(bool)) );
   connect( b_choose, SIGNAL(clicked()), SLOT(slotChooseFont()) );
   connect( b_choose_ipa, SIGNAL(clicked()), SLOT(slotChooseIPAFont()) );

   setCaption( kapp->makeStdCaption(i18n("Document Properties" )));

   e_font->setText (QString("%1 %2pt").arg(tablefont.family()).arg(tablefont.pointSize()));
   e_ipa_font->setText (QString("%1 %2pt").arg(ipa_font.family()).arg(ipa_font.pointSize()));

   b_col0->setColor (cols.col0);
   b_col1->setColor (cols.col1);
   b_col2->setColor (cols.col2);
   b_col3->setColor (cols.col3);
   b_col4->setColor (cols.col4);
   b_col5->setColor (cols.col5);
   b_col6->setColor (cols.col6);
   b_col7->setColor (cols.col7);
   c_use->setChecked (gc.use);
   slotColUsed(gc.use);
}


void ViewOptPage::initFocus() const
{
  c_use->setFocus();
}


void ViewOptPage::slotChooseFont()
{
   KFontDialog fdlg (0L, 0L, false, true);
   fdlg.setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
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
   fdlg.setIcon (QPixmap (locate("data",  "kvoctrain/mini-kvoctrain.xpm" )));
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

  b_col0->setEnabled(used);
  b_col1->setEnabled(used);
  b_col2->setEnabled(used);
  b_col3->setEnabled(used);
  b_col4->setEnabled(used);
  b_col5->setEnabled(used);
  b_col6->setEnabled(used);
  b_col7->setEnabled(used);

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
