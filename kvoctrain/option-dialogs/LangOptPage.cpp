/***************************************************************************

    $Id$

                  language options dialog page

    -----------------------------------------------------------------------

    begin                : Thu Jun 3 18:09:06 1999
                                           
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

    Revision 1.1  2001/10/05 15:44:04  arnold
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


#include <qpixmap.h>
#include <qkeycode.h>
#include <qfileinfo.h>

#include <kfiledialog.h>
#include <kapp.h>
#include <kfontdialog.h>

#include <kmessagebox.h>
#include <kstddirs.h>

#include "LangOptPage.h"

#include <langset.h>
#include <kv_resource.h>
#include <kvoctraindoc.h>

#define Inherited LangOptPageData


LangOptPage::LangOptPage
(
        QString    _deflang,
        LangSet    &_langset,
        QString    &lastPix,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name ),
        langset (_langset),
        lastPixName(lastPix)
{
  lang_group->insert(r_stand);
  lang_group->insert(r_spec);

  connect( r_spec, SIGNAL(clicked()), SLOT(slotSpecFont()) );
  connect( r_stand, SIGNAL(clicked()), SLOT(slotStdFont()) );
  connect( b_chooseFont, SIGNAL(clicked()), SLOT(slotChooseFont()) );
  connect( b_langDel, SIGNAL(clicked()), SLOT(slotDeleteClicked()) );
  connect( b_langNew, SIGNAL(clicked()), SLOT(slotNewClicked()) );
  connect( b_langPixmap, SIGNAL(clicked()), SLOT(slotPixmapClicked()) );

  connect( e_newName, SIGNAL(textChanged(const QString&)), SLOT(slotNewNameChanged(const QString&)) );
  connect( d_shortName, SIGNAL(activated(const QString&)), SLOT(slotShortActivated(const QString&)) );
  connect( e_langLong, SIGNAL(textChanged(const QString&)), SLOT(slotLangChanged(const QString&)) );
  connect( e_shortName2, SIGNAL(textChanged(const QString&)), SLOT(slotShort2Changed(const QString&)) );

  deflang = _deflang;

  setCaption(i18n("Options" ));

  for (int i = 0; i < (int) langset.size() && i < MAX_LANGSET; i++)
    d_shortName->insertItem (langset.shortId(i));

  b_langNew->setEnabled(false);
  enableLangWidgets();

  r_stand->setChecked(true);
  r_spec->setChecked(false);

  if (d_shortName->count() ) {
    d_shortName->setCurrentItem(0);

    e_langLong->setText(langset.longId(0));
    e_shortName2->setText(langset.shortId2(0));
    if (!langset.PixMapFile(0).isEmpty() ) {
      QPixmap pix(langset.PixMapFile(0));
      if (!pix.isNull() ) {
        lastPixName = langset.PixMapFile(0);
        b_langPixmap->setPixmap (pix);
      }
      else
        b_langPixmap->setText (i18n("invalid"));
    }

    QFont font;
    bool specfont;
    langset.Font(0, font, specfont);
    if (specfont) {
      e_font->setText (fontName (font));
      r_stand->setChecked(false);
      r_spec->setChecked(true);
      slotSpecFont();
    }
    else {
      r_stand->setChecked(true);
      r_spec->setChecked(false);
      slotStdFont();
    }
  }
  else {
    b_langPixmap->setText (i18n("not selected"));
    b_langPixmap->setEnabled(false);
  }

  label_newName->setBuddy(e_newName);
  label_shortName2->setBuddy(e_shortName2);
  label_shortName->setBuddy(d_shortName);
  label_langLong->setBuddy(e_langLong);
  label_langPixmap->setBuddy(b_langPixmap);
}


void LangOptPage::initFocus() const
{
  d_shortName->setFocus();
}


void LangOptPage::slotDeleteClicked()
{
  if (d_shortName->count() ) {
    langset.erase (d_shortName->currentItem());
    d_shortName->removeItem (d_shortName->currentItem());
    if (d_shortName->count() != 0)
      d_shortName->setCurrentItem (0);
  }

  if (d_shortName->count() ) {
    setPixmap(langset.PixMapFile(d_shortName->currentItem()));
    e_langLong->setText(langset.longId(d_shortName->currentItem()));
    e_shortName2->setText(langset.shortId2(d_shortName->currentItem()));

    QFont font;
    bool specfont;
    langset.Font(d_shortName->currentItem(), font, specfont);
    if (specfont) {
      r_stand->setChecked(false);
      r_spec->setChecked(true);
      slotSpecFont();
    }
    else {
      r_stand->setChecked(true);
      r_spec->setChecked(false);
      slotStdFont();
    }
  }
  else {
    b_langPixmap->setText (i18n("not selected"));
    e_langLong->setText("");
    e_shortName2->setText("");
    b_langPixmap->setEnabled(false);
    r_stand->setChecked(true);
    r_spec->setChecked(false);
    slotStdFont();
  }
  enableLangWidgets();
}


void LangOptPage::enableLangWidgets()
{
  bool enabled = d_shortName->count() != 0;
  b_langDel->setEnabled(enabled);
  r_spec->setEnabled(enabled);
  r_stand->setEnabled(enabled);
  b_chooseFont->setEnabled(enabled);
  b_langPixmap->setEnabled(enabled);
  d_shortName->setEnabled(enabled);
  e_langLong->setEnabled(enabled);
  e_shortName2->setEnabled(enabled);
}


void LangOptPage::slotNewNameChanged(const QString& _s)
{
   QString s = _s;
   b_langNew->setEnabled(s.stripWhiteSpace().length() != 0);
}


void LangOptPage::slotNewClicked()
{
   QString s = e_newName->text();
   d_shortName->insertItem(s.stripWhiteSpace());
   d_shortName->setCurrentItem(d_shortName->count()-1);
   enableLangWidgets();
   slotShortActivated(s);
   e_newName->setText("");
   e_langLong->setFocus();
}


void LangOptPage::slotShortActivated(const QString& _id)
{
   QString id = _id;
   int i = 0;
   id = id.stripWhiteSpace();
   if (d_shortName->count() > (int) langset.size() ) {
     // avoid doublettes with language code
     for (i = 0; i < d_shortName->count(); i++)
       if (QString(d_shortName->text(i)) == "") {
         d_shortName->removeItem(d_shortName->currentItem());
         d_shortName->setCurrentItem(0);
       }
  
     for (i = 0; i < d_shortName->count()-1; i++)  // omit last
       if (id == QString(d_shortName->text(i))) {
         d_shortName->removeItem(d_shortName->currentItem());
         d_shortName->setCurrentItem(i);
         return;
       }
  
     if (d_shortName->count() > (int) langset.size()
         && langset.size() < MAX_LANGSET ) {
       langset.addSet (id, "", "", QFont(), false);
     }
   }

   if (d_shortName->count() != 0) {
     deflang = id;
     b_langPixmap->setEnabled(true);
     e_langLong->setText (langset.longId(d_shortName->currentItem()));
     e_shortName2->setText (langset.shortId2(d_shortName->currentItem()));

     if (!langset.PixMapFile(d_shortName->currentItem()).isEmpty() ) {
       QPixmap pix (langset.PixMapFile(d_shortName->currentItem()));
       if (!pix.isNull() )
         b_langPixmap->setPixmap (pix);
       else
         b_langPixmap->setText (i18n("invalid"));
     }
     else
       b_langPixmap->setText (i18n("not selected"));

     QFont font;
     bool specfont;
     langset.Font(d_shortName->currentItem(), font, specfont);
     if (specfont) {
       r_stand->setChecked(false);
       r_spec->setChecked(true);
       slotSpecFont();
     }
     else {
       r_stand->setChecked(true);
       r_spec->setChecked(false);
       slotStdFont();
     }
   }
}


void LangOptPage::slotLangChanged(const QString& s)
{
  if (d_shortName->currentItem() < (int) langset.size() )
    langset.setLongId(s, d_shortName->currentItem());
}


void LangOptPage::slotShort2Changed(const QString& s)
{
  if (d_shortName->currentItem() < (int) langset.size() )
    langset.setShortId2(s, d_shortName->currentItem());
}


bool LangOptPage::setPixmap(QString pm)
{
  if (d_shortName->count() ) {
    QPixmap pix (pm);
    if (!pix.isNull() ) {
      langset.setPixMapFile(pm, d_shortName->currentItem());
      b_langPixmap->setPixmap (pix);
      return true;
    }
  }
  return false;

}


void LangOptPage::slotPixmapClicked()
{
  if (langset.size() > 0 ) {
    if (lastPixName.isNull() ) {
      QString s;
      if (!langset.shortId(d_shortName->currentItem()).isNull() ) {
        s = langset.shortId(d_shortName->currentItem());
        lastPixName = EA_KDEDATADIR ("", "share/locale/l10n/"+s+"/flag.png");
      }
      else
        lastPixName = EA_KDEDATADIR ("", "share/locale/l10n/");
    }
    else {
      QFileInfo fi (lastPixName);
      lastPixName = fi.dirPath()+"/flag.png";
    }
    QString s = KFileDialog::getOpenFileName (lastPixName, "*.png *.xpm *.gif *.xbm");
    if (!s.isEmpty() ) {
      if (setPixmap (s) )
        lastPixName = s;
      else {
        b_langPixmap->setText (i18n("invalid"));
        KMessageBox::sorry(this,
           i18n("File does not contain a valid graphics format\n"),
           kapp->makeStdCaption(QString::null),
           false);
      }
    }
  }
}

void LangOptPage::keyPressEvent( QKeyEvent *e )
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


QString LangOptPage::fontName(const QFont &font)
{
   return QString("%1 %2pt").arg(font.family()).arg(font.pointSize());
}


void LangOptPage::slotChooseFont()
{
   KFontDialog fdlg (0L, 0L, false, true);
   int act_index = d_shortName->currentItem();
   QFont font;
   bool specfont;
   langset.Font(act_index, font, specfont);
   fdlg.setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
   fdlg.setCaption(i18n("Choose font for language"));
   fdlg.setFont(font);
   if (fdlg.exec() == QDialog::Accepted ) {
     font = fdlg.font();
     font.setWeight(QFont::Normal);
     font.setStrikeOut(false);
     font.setUnderline(false);

     langset.setFont (font, true, act_index);
     e_font->setText (fontName (font));
   }
}


LangSet LangOptPage::getLangSet () const
{
  return langset;
}


void LangOptPage::slotStdFont()
{
   int act_index = d_shortName->currentItem();
   QFont font;
   bool specfont;
   langset.Font(act_index, font, specfont);
   langset.setFont (font, false, act_index);

   b_chooseFont->setEnabled(false);
   e_font->setEnabled(false);
   e_font->setText (""); 
}


void LangOptPage::slotSpecFont()
{
   int act_index = d_shortName->currentItem();
   QFont font;
   bool specfont;
   langset.Font(act_index, font, specfont);
   langset.setFont (font, true, act_index);

   e_font->setEnabled(true);
   e_font->setText (fontName(font));
   b_chooseFont->setEnabled(true);
}
#include "LangOptPage.moc"
