/***************************************************************************

    $Id$

                             about dialog

    -----------------------------------------------------------------------

    begin                : Thu Nov 8 20:50:53 MET 1999
                                           
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
                         
    email                : kvoctrain@ewald-arnold.de                                    

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:37:45  arnold
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

/*   TAKEN FROM AND MODIFIED AS NEEDED:
 *
 *   khexedit - Versatile hex editor
 *   Copyright (C) 1999  Espen Sand, espensa@online.no
 */


#include <qclipboard.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qmultilinedit.h> 

#include <kapp.h>
#include <klocale.h>
#include <kbuttonbox.h>
#include <kiconloader.h>

#include <iostream.h>

#include <kglobalsettings.h>
#include <kmessagebox.h>
#include <khtmlview.h>
#include <khtml_part.h>

#include "aboutdialog.h"
#include <kv_resource.h>
#include <compat_2x.h>

#include "../kvoctrain.h"


#define OUTER_SEP 6
#define INNER_SEP 6

//
// There should always already be an space of INNER_SEP above the 
// action buttons. 
//
// This choice gives equal amount of space above and below the 
// buttons.
//
#define ACTION_SPACE (OUTER_SEP - INNER_SEP)

CAboutDialog::CAboutDialog( QWidget *parent, const char *name )
  : QDialog( parent, name, false, WStyle_Customize|WStyle_DialogBorder ) 
{
  setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
  QVBoxLayout *topLayout = new QVBoxLayout( this, OUTER_SEP, INNER_SEP ); 
  if( topLayout == 0 ) { return; }

  QString captionText;

  captionText = QString (i18n("About %1")).arg(PACKAGE);
  setCaption( captionText );

  QFont titleFont( KGlobalSettings::generalFont() );
  titleFont.setBold( true );
  titleFont.setPointSize( 24 );

  QFont authorFont( KGlobalSettings::generalFont() );
  authorFont.setBold( true );

  QString appName( i18n("KDE vocabulary trainer") );
  QLabel *title = new QLabel( appName.data(), this );
  title->setFont(titleFont);
  title->setFixedHeight( title->sizeHint().height() );
  title->setFixedWidth( title->sizeHint().width() );
  title->setAlignment(AlignCenter);
  topLayout->addWidget( title );

  topLayout->addSpacing( INNER_SEP );

  QHBoxLayout *hbox = new QHBoxLayout();
  if( hbox == 0 ) { return; }
  topLayout->addLayout( hbox );

  QLabel *pixmap = new QLabel( this );
  KIconLoader loader;
  pixmap->setPixmap( loader.loadIcon( "kvoctrain.xpm", KIcon::Desktop, 48 ));
  pixmap->setFixedHeight( pixmap->sizeHint().height() );
  pixmap->setMinimumWidth( pixmap->sizeHint().width() );
  hbox->addWidget( pixmap, 0, AlignCenter );
  
  QVBoxLayout *vbox = new QVBoxLayout();
  if( vbox == 0 ) { return; }
  hbox->addLayout( vbox );

  QString versionString;
  about_version = i18n("Version: %1 %2, August 13. 2001 (KDE %3)");
  versionString = QString (about_version).arg(PACKAGE).arg(VERSION).arg(KDE_VERSION_STRING);
  QLabel *version = new QLabel( versionString, this );
  version->setFont(authorFont);
  version->setFixedHeight( version->sizeHint().height() );
  version->setMinimumWidth( version->sizeHint().width() );
  vbox->addWidget( version, 0, AlignLeft );

  QString authorString = i18n("© 1999-2001 Ewald Arnold");
  QLabel *author = new QLabel( authorString, this );
  author->setFont(authorFont);
  author->setFixedHeight( author->sizeHint().height() );
  author->setMinimumWidth( author->sizeHint().width() );
  vbox->addWidget( author, 0, AlignLeft );
  hbox->addStretch(10);

  topLayout->addSpacing( INNER_SEP );

  mTabControl = new KTabCtl( this );
  if( mTabControl == 0 ) { return; }
  topLayout->addWidget( mTabControl, 10 );

  KConfig *config = KApplication::kApplication()->config();
  config->setGroup(CFG_GENERAL);
  bool first_time = config->readNumEntry(CFG_FIRST_TIME, 1);
  config->writeEntry(CFG_FIRST_TIME, (bool) 0);

  mPage[ page_welcome ] = 0;

//  if (first_time)
//    mPage[ page_welcome ] = addPage( QString(i18n("&Welcome")) );

  mPage[ page_general ] = addPage( QString(i18n("&General")) );
  mPage[ page_contributor ] = addPage( QString(i18n("&Contributors")) );
  mPage[ page_licence ] = addPage( QString(i18n("&Licence agreement")) );
//  if (!first_time)
//    mPage[ page_welcome ] = addPage( QString(i18n("&Welcome")) );

  topLayout->addSpacing( ACTION_SPACE );
  KButtonBox *buttonBox = new KButtonBox( this, KButtonBox::Horizontal, 0,
					  INNER_SEP );
  buttonBox->addStretch();
  QPushButton *closeButton = buttonBox->addButton( i18n("&OK"), false );
  closeButton->setDefault(true);
  connect( closeButton, SIGNAL(clicked()),this,SLOT(reject()) );
  buttonBox->layout();
  buttonBox->setMinimumSize( buttonBox->sizeHint() );
  topLayout->addWidget( buttonBox );

//  createWelcomePage();
  createGeneralPage();
  createContributorPage();
  createLicencePage();

  //
  // Compute the size of the tab widget. This will ensure that
  // the dialog is positioned (almost) correct the first time
  //
  QSize s( 0,0 );
  for( uint i=0; i<page_MAX; i++)
  {
    if (mPage[i] != 0)
      s = s.expandedTo( mPage[i]->minimumSize() );
  }
  s.setHeight( s.height()+20 ); // Tabs (bad practice!)
  mTabControl->setMinimumSize( s );

  topLayout->activate();
  setGeometry( x(), y(), minimumSize().width(), minimumSize().height() );
  setMinimumSize( s );

// FIXME: quick and dirty
  resize( 520+16,420+76 );
  setMinimumSize( 520+16,420+76 );

}


QFrame *CAboutDialog::addPage( const QString &title )
{
  QFrame *page = new QFrame( mTabControl, title.data() );
  page->setFrameStyle( QFrame::NoFrame );
  page->setEnabled( true );
  page->hide();

  mTabControl->addTab( page, title.data() );
  return( page );
}


void CAboutDialog::createGeneralPage( void )
{
  QFrame *page = mPage[ page_general ];

  QVBoxLayout *topLayout = new QVBoxLayout( page, INNER_SEP, INNER_SEP ); 
  if( topLayout == 0 ) { return; }
  
  topLayout->addStretch(5);

  QHBoxLayout *hbox = new QHBoxLayout(0);
  if( hbox == 0 ) { return; }
  topLayout->addLayout( hbox );

  QVBoxLayout *vbox = new QVBoxLayout(0);
  if( hbox == 0 ) { return; }
  hbox->addStretch(5);
  hbox->addLayout(vbox);
  hbox->addStretch(5);

  static QString reasonToMailMe = i18n(""
    "This is a work in progress. If you\n\n"
    "1. have a suggestion for improvement\n"
    "2. have found a bug\n"
    "3. want to contribute with something\n"
    "   - a vocabulary\n"
    "   - a localisation to a new language\n"
    "   - a nicer icon\n"
    "   or something else\n"
    "4. just want to tell me how nice or useful kvoctrain is\n\n"
    "then feel free to send me a mail.\n");

  QLabel *label;
  label = new QLabel( reasonToMailMe, page );
  label->setFixedHeight( label->sizeHint().height() );
  label->setFixedWidth( label->sizeHint().width() );
  vbox->addWidget( label, 0, AlignLeft );

  QGridLayout *gbox = new QGridLayout( 4, 3, INNER_SEP+5 );
  vbox->addLayout( gbox );

  label = new QLabel( i18n("Email:"), page ); 
  label->setFixedHeight( label->sizeHint().height() );
  label->setFixedWidth( label->sizeHint().width() );
  gbox->addWidget( label, 0, 1, AlignLeft );

  {
    label = new QLabel(page );
    QPixmap pix (EA_KDEDATADIR("",  "kvoctrain/sf-mail.xpm"));
    label->setFixedHeight( pix.height() );
    label->setFixedWidth( pix.width() );
    label->setPixmap (pix);
    gbox->addWidget( label, 0, 0, AlignLeft );
  }

  CURLLabel *email = new CURLLabel( page, "email" );
  email->setText("kvoctrain@ewald-arnold.de");
  email->setURL("kvoctrain@ewald-arnold.de");
  email->setFloat(true);
  email->setUnderline(true);
  email->setFixedHeight( label->sizeHint().height() );
  email->setFixedWidth( email->sizeHint().width() );
  connect(email, SIGNAL(leftClick(const QString &)), 
	  SLOT(emailClick(const QString &)));
  gbox->addWidget( email, 0, 2, AlignLeft );

//---------------------

  label = new QLabel( i18n("Kvoctrain homepage:"), page );
  label->setFixedHeight( label->sizeHint().height() );
  label->setFixedWidth( label->sizeHint().width() );
  gbox->addWidget( label, 1, 1, AlignLeft );

  CURLLabel *kv_homepage = new CURLLabel( page, "kv_homePage" );
  kv_homepage->setText("http://kvoctrain.sourceforge.net/");
  kv_homepage->setURL("http://kvoctrain.sourceforge.net/");
  kv_homepage->setFloat(true);
  kv_homepage->setUnderline(true);
  kv_homepage->setFixedHeight( label->sizeHint().height() );
  kv_homepage->setFixedWidth( kv_homepage->sizeHint().width() );
  connect(kv_homepage, SIGNAL(leftClick(const QString &)), 
	  SLOT(urlClick(const QString &)));
  gbox->addWidget( kv_homepage, 1, 2, AlignLeft );

  {
    label = new QLabel(page );
    QPixmap pix (EA_KDEDATADIR("",  "kvoctrain/sf-home.xpm"));
    label->setFixedHeight( pix.height() );
    label->setFixedWidth( pix.width() );
    label->setPixmap (pix);
    gbox->addWidget( label, 1, 0, AlignLeft );
  }

//----------------------

  label = new QLabel( i18n("Reporting bugs:"), page );
  label->setFixedHeight( label->sizeHint().height() );
  label->setFixedWidth( label->sizeHint().width() );
  gbox->addWidget( label, 2, 1, AlignLeft );

  CURLLabel *kv_bugreport = new CURLLabel( page, "kv_bugreport" );
  kv_bugreport->setText("http://sourceforge.net/bugs/?group_id=9265");
  kv_bugreport->setURL("http://sourceforge.net/bugs/?group_id=9265");
  kv_bugreport->setFloat(true);
  kv_bugreport->setUnderline(true);
  kv_bugreport->setFixedHeight( label->sizeHint().height() );
  kv_bugreport->setFixedWidth( kv_bugreport->sizeHint().width() );
  connect(kv_bugreport, SIGNAL(leftClick(const QString &)),
	  SLOT(urlClick(const QString &)));
  gbox->addWidget( kv_bugreport, 2, 2, AlignLeft );

  {
    label = new QLabel(page );
    QPixmap pix (EA_KDEDATADIR("",  "kvoctrain/sf-bugs.xpm"));
    label->setFixedHeight( pix.height() );
    label->setFixedWidth( pix.width() );
    label->setPixmap (pix);
    gbox->addWidget( label, 2, 0, AlignLeft );
  }

//-----------------------

  label = new QLabel( i18n("Authors homepage:"), page );
  label->setFixedHeight( label->sizeHint().height() );
  label->setFixedWidth( label->sizeHint().width() );
  gbox->addWidget( label, 3, 1, AlignLeft );

  CURLLabel *my_homepage = new CURLLabel( page, "my_homePage" );
  my_homepage->setText("http://www.ewald-arnold.de");
  my_homepage->setURL("http://www.ewald-arnold.de");
  my_homepage->setFloat(true);
  my_homepage->setUnderline(true);
  my_homepage->setFixedHeight( label->sizeHint().height() );
  my_homepage->setFixedWidth( my_homepage->sizeHint().width() );
  connect(my_homepage, SIGNAL(leftClick(const QString &)),
	  SLOT(urlClick(const QString &)));
  gbox->addWidget( my_homepage, 3, 2, AlignLeft );

  topLayout->addStretch(5);
  topLayout->activate();
}


void CAboutDialog::createContributorPage( void )
{
  QFrame *page = mPage[ page_contributor ];
  QVBoxLayout *topLayout = new QVBoxLayout( page, INNER_SEP, INNER_SEP ); 
  if( topLayout == 0 ) { return; }
  
//FIXME: use about::credits()
    /**
     * @return credit information (list of persons).
    const QValueList<KAboutPerson> credits() const;

    QString name() const;
    QString task() const;
     */

  static QString contrbutorText = i18n("\n"
    "This program uses modified code and techniques from other KDE programs,\n"
    "specifically klpq, killustrator and khexedit.\n"
    "Credit goes to their authors and maintainers.\n"
    "\n"
    "My personal thank goes to the following people:\n"
    "  Localisators of user interface:\n"
    "    Italian:\tAndrea Marconi\n"
    "    French:\tHans Kottman\n"
    "    Polish:\tGrzegorz Ilczuk\n"
    "\n");

  QMultiLineEdit *contrbutorTextArea = new QMultiLineEdit(page);
  contrbutorTextArea->setReadOnly(true);
  contrbutorTextArea->setText( contrbutorText.data() );

  //
  // Add 20 pixels to (hopefully) avoid the vertical scrollbar
  //
  contrbutorTextArea->setMinimumWidth(contrbutorTextArea->maxLineWidth()+20);
  topLayout->addWidget( contrbutorTextArea );
  topLayout->activate();
}


void CAboutDialog::createWelcomePage( void )
{
  QFrame *page = mPage[ page_welcome ];
  QVBoxLayout *topLayout = new QVBoxLayout( page, INNER_SEP, INNER_SEP ); 
  if( topLayout == 0 ) { return; }
  
  static QString welcomeText = i18n("<HTML><BODY>"
    "<H1>Welcome to kvoctrain</H1>"
    "</BODY></HTML>");

  KHTMLPart *welcomeTextArea = new KHTMLPart(page);
  welcomeTextArea->begin();
  welcomeTextArea->write(welcomeText);
  welcomeTextArea->end();

  //
  // Add 20 pixels to (hopefully) avoid the vertical scrollbar
  //
  topLayout->addWidget( welcomeTextArea->widget() );
  topLayout->activate();
}


void CAboutDialog::createLicencePage( void )
{
  QFrame *page = mPage[ page_licence ];
  QVBoxLayout *topLayout = new QVBoxLayout( page, INNER_SEP, INNER_SEP ); 
  if( topLayout == 0 ) { return; }

  static QString licenseText = i18n("\n"
    "This program is free software; you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation; either version 2 of the License, or\n"
    "(at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program; if not, write to the Free Software\n"
    "Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n");


  QMultiLineEdit *licenseTextArea = new QMultiLineEdit(page);
  licenseTextArea->setReadOnly(true);
  licenseTextArea->setText( licenseText.data() );

  //
  // Add 20 pixels to (hopefully) avoid the vertical scrollbar
  //
  licenseTextArea->setMinimumWidth(licenseTextArea->maxLineWidth()+20);
  topLayout->addWidget( licenseTextArea );
  topLayout->activate();
}


void CAboutDialog::emailClick( const QString &e )
{
  QString versionString;
  versionString = QString(about_version).arg(PACKAGE).arg(VERSION).arg(KDE_VERSION_STRING);

  kapp->invokeMailer( e, versionString );
}


void CAboutDialog::urlClick( const QString &url ) 
{

  kapp->invokeBrowser( url );
/*
  QString application("kfmclient");
 
  QString exec = QString("%1 exec %2").arg(application).arg(url);
  QStringList list;
  if( KRun::run( exec, list ) == false )
  {
    QApplication::clipboard()->setText( url );

    static QString msg = i18n(""
      "Unable to execute request\n\n"
      "The url address has been transfered to the clipboard.\n"
      "You can paste it into the address field of your web browser.");
    KMessageBox::sorry( this, msg, i18n("Operation failed"), i18n("&OK"));
  }
*/
}


CURLLabel::CURLLabel( QWidget *parent, const char *name )
  :KURLLabel( parent, name )
{
  installEventFilter( this );
}


bool CURLLabel::eventFilter( QObject *object, QEvent *e )
{
  if( e->type() == QEvent::MouseButtonPress )
  {
    QMouseEvent &m = *(QMouseEvent*)e;
    if( m.button() == LeftButton )
    {
      emit leftClick( url() );
    }
  }

  return( false );
}
#include "aboutdialog.moc"
