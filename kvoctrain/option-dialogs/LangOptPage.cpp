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
    Revision 1.11  2001/11/24 11:46:21  arnold
    fix for kde3, better sorting of regions

    Revision 1.10  2001/11/21 18:30:17  arnold
    quick fix for change in KLocale class

    Revision 1.9  2001/11/10 22:29:11  arnold
    removed compatibility for kde1

    Revision 1.8  2001/11/10 17:35:03  arnold
    fixed language property dialog page

    Revision 1.7  2001/11/09 10:40:46  arnold
    removed ability to display a different font for each column

    Revision 1.6  2001/11/02 04:31:42  waba
    Add include to make it work with KDE CVS.

    Revision 1.5  2001/10/25 17:34:19  arnold
    replaced qtarch dialog files by qtdesigner

    Revision 1.4  2001/10/21 15:29:50  arnold
    removed all the 'charset' stuff

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
#include <qlabel.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qpopupmenu.h>

#include <kfiledialog.h>
#include <kapp.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstddirs.h>

#include "LangOptPage.h"

#include <langset.h>
#include <kv_resource.h>
#include <kvoctraincore.h>
#include <kvoctraindoc.h>

#include <map.h>
#include <algo.h>

LangOptPage::LangOptPage
(
        QString    _deflang,
        LangSet    &_langset,
        QString    &lastPix,
	QWidget* parent,
	const char* name
)
	:
	LangOptPageForm( parent, name ),
        langset (_langset),
        lastPixName(lastPix)
{
  langset_popup = 0;
  deflang = _deflang;

  connect( b_langDel, SIGNAL(clicked()), SLOT(slotDeleteClicked()) );
  connect( b_langNew, SIGNAL(clicked()), SLOT(slotNewClicked()) );
  connect( b_langPixmap, SIGNAL(clicked()), SLOT(slotPixmapClicked()) );

  connect( e_newName, SIGNAL(textChanged(const QString&)), SLOT(slotNewNameChanged(const QString&)) );
  connect( d_shortName, SIGNAL(activated(const QString&)), SLOT(slotShortActivated(const QString&)) );
  connect( e_langLong, SIGNAL(textChanged(const QString&)), SLOT(slotLangChanged(const QString&)) );
  connect( e_shortName2, SIGNAL(textChanged(const QString&)), SLOT(slotShort2Changed(const QString&)) );

  loadCountryData();
  b_lang_kde->setPopup(langset_popup);
  b_langNew->setEnabled(false); // activate after data is entered

  setCaption(i18n("Options" ));

  for (int i = 0; i < (int) langset.size() && i < MAX_LANGSET; i++)
    d_shortName->insertItem (langset.shortId(i));

  enableLangWidgets();

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
        b_langPixmap->setText (i18n("Picture is invalid"));
    }
    else
      b_langPixmap->setText  (i18n("No picture selected"));
  }
  else {
    b_langPixmap->setText  (i18n("No picture selected"));
    b_langPixmap->setEnabled(false);
  }

//  label_newName->setBuddy(e_newName);
  label_shortName2->setBuddy(e_shortName2);
  label_shortName->setBuddy(d_shortName);
  label_langLong->setBuddy(e_langLong);
  label_langPixmap->setBuddy(b_langPixmap);
}


LangOptPage::~LangOptPage ()
{
 delete langset_popup;
}


void LangOptPage::initFocus() const
{
  d_shortName->setFocus();
}


void LangOptPage::slotDeleteClicked()
{
  if (d_shortName->count() != 0) {
    langset.erase (d_shortName->currentItem());
    d_shortName->removeItem (d_shortName->currentItem());
    if (d_shortName->count() != 0)
      d_shortName->setCurrentItem (0);
  }

  if (d_shortName->count() != 0) {
    setPixmap(langset.PixMapFile(d_shortName->currentItem()));
    e_langLong->setText(langset.longId(d_shortName->currentItem()));
    e_shortName2->setText(langset.shortId2(d_shortName->currentItem()));
  }
  else {
    b_langPixmap->setText (i18n("No picture selected"));
    e_langLong->setText("");
    e_shortName2->setText("");
    b_langPixmap->setEnabled(false);
  }
  enableLangWidgets();
}


void LangOptPage::enableLangWidgets()
{
  bool enabled = d_shortName->count() != 0;
  b_langDel->setEnabled(enabled);
  b_langPixmap->setEnabled(enabled);
  d_shortName->setEnabled(enabled);
  e_langLong->setEnabled(enabled);
  e_shortName2->setEnabled(enabled);
}


void LangOptPage::slotNewNameChanged(const QString& _s)
{
   QString s = _s;
   b_langNew->setEnabled(s.stripWhiteSpace().length() >= 2);
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
   int i = 0;
   QString id = _id.stripWhiteSpace();
   if (d_shortName->count() > (int) langset.size() ) {
     // avoid doublettes with language code
     for (i = 0; i < d_shortName->count(); i++)
       if (d_shortName->text(i).isNull() ) {
         d_shortName->removeItem(d_shortName->currentItem());
         d_shortName->setCurrentItem(0);
       }
  
     for (i = 0; i < d_shortName->count()-1; i++)  // omit last
       if (id == d_shortName->text(i)) {
         d_shortName->removeItem(d_shortName->currentItem());
         d_shortName->setCurrentItem(i);
         return;
       }
  
     if (d_shortName->count() > (int) langset.size()
         && langset.size() < MAX_LANGSET ) {
       langset.addSet (id, "", "");
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
         b_langPixmap->setText (i18n("Picture is invalid"));
     }
     else
       b_langPixmap->setText (i18n("No picture selected"));
   }
}


void LangOptPage::slotLangChanged(const QString& s)
{
  if (   d_shortName->count() != 0
      && d_shortName->currentItem() < (int) langset.size() )
    langset.setLongId(s, d_shortName->currentItem());
}


void LangOptPage::slotShort2Changed(const QString& s)
{
  if (   d_shortName->count() != 0
      && d_shortName->currentItem() < (int) langset.size() )
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
    if (lastPixName.isNull() || QPixmap(lastPixName).isNull()) {
      QString s;
      if (!langset.shortId(d_shortName->currentItem()).isNull() ) {
        s = langset.shortId(d_shortName->currentItem());
        lastPixName = locate ("locale", "l10n/"+s+"/flag.png");
        if (lastPixName.isNull()) {
          lastPixName = locate ("locale", "l10n/");
        }
      }
      else {
        lastPixName = locate ("locale", "l10n/");
      }
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
        b_langPixmap->setText (i18n("Picture is invalid"));
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


LangSet LangOptPage::getLangSet () const
{
  return langset;
}


class sortByRegion : public binary_function<LangOptPage::LangRef,
                                            LangOptPage::LangRef, bool>
{
 public:

  sortByRegion () {}

  bool operator() (const LangOptPage::LangRef &x, const LangOptPage::LangRef &y) const {
      return (QString::compare(x.region.upper(), y.region.upper() ) < 0);
  }
};


void LangOptPage::loadCountryData()
{

  // temperary use of our locale as the global locale
  KLocale *lsave = KGlobal::_locale;
  QString curr_lang = lsave->language();

//  KLocale locale("kvoctrain");
  KLocale locale(QString::null);
  locale.setLanguage(curr_lang);
  KGlobal::_locale = &locale;

  globalLangs.clear();
  QString sub = QString::fromLatin1("l10n/");
  regionlist = KGlobal::dirs()->findAllResources("locale",
                                 sub + QString::fromLatin1("*.desktop"));

  typedef pair<QString, int> regionpair;
  typedef map<QString, int> regionmap_t;

  regionmap_t regionmap;
  for ( QStringList::ConstIterator it = regionlist.begin();
    it != regionlist.end();
    ++it )
  {
    QString tag = *it;
    int index;

    index = tag.findRev('/');
    if (index != -1) tag = tag.mid(index + 1);

    index = tag.findRev('.');
    if (index != -1) tag.truncate(index);

    KSimpleConfig entry(*it);
    entry.setGroup(QString::fromLatin1("KCM Locale"));
    QString name = entry.readEntry(QString::fromLatin1("Name"), i18n("without name"));

    regionmap.insert(regionpair(tag, globalLangs.size()));
    globalLangs.push_back(LangRef(name, LangSet()));
  }

  // add all languages to the list
  countrylist = KGlobal::dirs()->findAllResources("locale",
                               sub + QString::fromLatin1("*/entry.desktop"));
  countrylist.sort();

  for ( QStringList::ConstIterator it = countrylist.begin();
    it != countrylist.end(); ++it )
  {
    KSimpleConfig entry(*it);
    entry.setGroup(QString::fromLatin1("KCM Locale"));
    QString name = entry.readEntry(QString::fromLatin1("Name"),
                       i18n("without name"));
    QString submenu = entry.readEntry(QString::fromLatin1("Region"));

    QString tag = *it;
    int index = tag.findRev('/');
    tag.truncate(index);
    index = tag.findRev('/');
    tag = tag.mid(index+1);

    QString all_langs = entry.readEntry(QString::fromLatin1("Languages"));
    if (tag == all_langs)
      all_langs = "";

    QString pixmap = *it;
    index = pixmap.findRev('/');
    pixmap.truncate(index);
    pixmap += "/flag.png";

    regionmap_t::const_iterator it = regionmap.find(submenu);
    if (it != regionmap.end())
      globalLangs[(*it).second].langs.addSet (tag, all_langs, name, pixmap);
  }

  std::sort (globalLangs.begin(), globalLangs.end(), sortByRegion() );

  int idx = 0;
  global_langset.clear();
  delete langset_popup;
  langset_popup = new QPopupMenu();
  connect(langset_popup, SIGNAL(activated(int)), this, SLOT(slotLangFromGlobalActivated(int)));
  for ( unsigned i = 0; i < globalLangs.size(); ++i) {
    QPopupMenu *regpop = new QPopupMenu();
    connect(regpop, SIGNAL(activated(int)), this, SLOT(slotLangFromGlobalActivated(int)));
    langset_popup->insertItem(globalLangs[i].region, regpop);
    for (unsigned j = 0; j < globalLangs[i].langs.size(); ++j) {
      regpop->insertItem(QPixmap(globalLangs[i].langs.PixMapFile(j)),
                                 globalLangs[i].langs.longId(j),
                                 idx++);
      global_langset.addSet(globalLangs[i].langs.shortId(j),
                            globalLangs[i].langs.shortId2(j),
                            globalLangs[i].langs.longId(j),
                            globalLangs[i].langs.PixMapFile(j));
    }
  }

  // restore the old global locale
  KGlobal::_locale = lsave;
}


void LangOptPage::slotLangFromGlobalActivated(int i)
{
  if (i < (int)global_langset.size()) {
    QString s = global_langset.shortId(i);
    d_shortName->insertItem(s.stripWhiteSpace());
    d_shortName->setCurrentItem(d_shortName->count()-1);
    slotShortActivated(s);
    enableLangWidgets();

    e_shortName2->setText(global_langset.shortId2(i));
    slotShort2Changed(global_langset.shortId2(i));

    e_langLong->setText(global_langset.longId(i));
    slotLangChanged(global_langset.longId(i));

    setPixmap(global_langset.PixMapFile(i));
    e_newName->setText("");
    e_langLong->setFocus();
    e_langLong->selectAll();
  }
}


#include "LangOptPage.moc"
