/****************************************************************************
** Form implementation generated from reading ui file 'CharSetDlg.ui'
**
** Created: Mon Oct 8 21:04:52 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "CharSetDlgForm.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a CharSetDlgForm which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
CharSetDlgForm::CharSetDlgForm( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "CharSetDlgForm" );
    resize( 580, 607 ); 
    setMinimumSize( QSize( 0, 0 ) );
    setBaseSize( QSize( 470, 410 ) );
    setCaption( tr( "CharSetDlg" ) );
    CharSetDlgFormLayout = new QVBoxLayout( this ); 
    CharSetDlgFormLayout->setSpacing( 6 );
    CharSetDlgFormLayout->setMargin( 11 );

    Frame4 = new QFrame( this, "Frame4" );
    Frame4->setFrameShape( QFrame::NoFrame );
    Frame4->setFrameShadow( QFrame::Raised );
    Frame4Layout = new QGridLayout( Frame4 ); 
    Frame4Layout->setSpacing( 6 );
    Frame4Layout->setMargin( 11 );

    l_text = new QLabel( Frame4, "l_text" );
    l_text->setMinimumSize( QSize( 350, 100 ) );
    l_text->setMaximumSize( QSize( 350, 100 ) );
    l_text->setText( tr( "TextLabel2" ) );

    Frame4Layout->addWidget( l_text, 1, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Frame4Layout->addItem( spacer, 0, 1 );

    l_pixmap = new QLabel( Frame4, "l_pixmap" );
    l_pixmap->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, l_pixmap->sizePolicy().hasHeightForWidth() ) );
    l_pixmap->setMinimumSize( QSize( 44, 44 ) );
    l_pixmap->setMaximumSize( QSize( 44, 44 ) );
    l_pixmap->setText( tr( "TextLabel1" ) );

    Frame4Layout->addWidget( l_pixmap, 0, 0 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Frame4Layout->addItem( spacer_2, 1, 0 );
    CharSetDlgFormLayout->addWidget( Frame4 );

    Frame3 = new QFrame( this, "Frame3" );
    Frame3->setFrameShape( QFrame::NoFrame );
    Frame3->setFrameShadow( QFrame::Raised );
    Frame3Layout = new QVBoxLayout( Frame3 ); 
    Frame3Layout->setSpacing( 6 );
    Frame3Layout->setMargin( 11 );

    l_charset = new QLabel( Frame3, "l_charset" );
    l_charset->setText( tr( "Available &fonts" ) );
    Frame3Layout->addWidget( l_charset );

    listbox = new QListBox( Frame3, "listbox" );
    listbox->insertItem( tr( "New Item" ) );
    Frame3Layout->addWidget( listbox );
    CharSetDlgFormLayout->addWidget( Frame3 );

    Frame5 = new QFrame( this, "Frame5" );
    Frame5->setFrameShape( QFrame::NoFrame );
    Frame5->setFrameShadow( QFrame::Raised );
    Frame5Layout = new QHBoxLayout( Frame5 ); 
    Frame5Layout->setSpacing( 6 );
    Frame5Layout->setMargin( 11 );

    l_sample = new QLabel( Frame5, "l_sample" );
    l_sample->setText( tr( "&Sample text:" ) );
    Frame5Layout->addWidget( l_sample );

    e_sample = new QLineEdit( Frame5, "e_sample" );
    Frame5Layout->addWidget( e_sample );
    CharSetDlgFormLayout->addWidget( Frame5 );

    Frame9 = new QFrame( this, "Frame9" );
    Frame9->setFrameShape( QFrame::NoFrame );
    Frame9->setFrameShadow( QFrame::Raised );
    Frame9Layout = new QHBoxLayout( Frame9 ); 
    Frame9Layout->setSpacing( 6 );
    Frame9Layout->setMargin( 11 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Frame9Layout->addItem( spacer_3 );

    b_ok = new QPushButton( Frame9, "b_ok" );
    b_ok->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, b_ok->sizePolicy().hasHeightForWidth() ) );
    b_ok->setText( tr( "&OK" ) );
    Frame9Layout->addWidget( b_ok );

    b_cancel = new QPushButton( Frame9, "b_cancel" );
    b_cancel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, b_cancel->sizePolicy().hasHeightForWidth() ) );
    b_cancel->setText( tr( "&Cancel" ) );
    Frame9Layout->addWidget( b_cancel );
    CharSetDlgFormLayout->addWidget( Frame9 );

    // tab order
    setTabOrder( listbox, e_sample );
    setTabOrder( e_sample, b_ok );
    setTabOrder( b_ok, b_cancel );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
CharSetDlgForm::~CharSetDlgForm()
{
    // no need to delete child widgets, Qt does it all for us
}

