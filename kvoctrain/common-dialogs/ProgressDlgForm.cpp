/****************************************************************************
** Form implementation generated from reading ui file 'ProgressDlg.ui'
**
** Created: Fri Oct 12 20:07:23 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ProgressDlgForm.h"

#include <qframe.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ProgressDlgForm which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProgressDlgForm::ProgressDlgForm( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Form1" );
    resize( 400, 135 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 400, 120 ) );
    setCaption( tr( "Form1" ) );
    Form1Layout = new QVBoxLayout( this ); 
    Form1Layout->setSpacing( 6 );
    Form1Layout->setMargin( 11 );

    Frame3 = new QFrame( this, "Frame3" );
    Frame3->setFrameShape( QFrame::NoFrame );
    Frame3->setFrameShadow( QFrame::Raised );
    Frame3Layout = new QGridLayout( Frame3 ); 
    Frame3Layout->setSpacing( 6 );
    Frame3Layout->setMargin( 11 );

    TextLabel1 = new QLabel( Frame3, "TextLabel1" );
    QFont TextLabel1_font(  TextLabel1->font() );
    TextLabel1_font.setBold( TRUE );
    TextLabel1->setFont( TextLabel1_font ); 
    TextLabel1->setText( tr( "Filename:" ) );

    Frame3Layout->addWidget( TextLabel1, 0, 0 );

    TextLabel2 = new QLabel( Frame3, "TextLabel2" );
    QFont TextLabel2_font(  TextLabel2->font() );
    TextLabel2_font.setBold( TRUE );
    TextLabel2->setFont( TextLabel2_font ); 
    TextLabel2->setText( tr( "Title:" ) );

    Frame3Layout->addWidget( TextLabel2, 1, 0 );

    l_file = new QLabel( Frame3, "l_file" );
    l_file->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)1, l_file->sizePolicy().hasHeightForWidth() ) );
    l_file->setText( tr( "file.kvtml" ) );

    Frame3Layout->addWidget( l_file, 0, 1 );

    l_title = new QLabel( Frame3, "l_title" );
    l_title->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)1, l_title->sizePolicy().hasHeightForWidth() ) );
    l_title->setText( tr( "title" ) );

    Frame3Layout->addWidget( l_title, 1, 1 );
    Form1Layout->addWidget( Frame3 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Form1Layout->addItem( spacer );

    progress = new QProgressBar( this, "progress" );
    progress->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, progress->sizePolicy().hasHeightForWidth() ) );
    progress->setMinimumSize( QSize( 340, 40 ) );
    progress->setFrameShape( QProgressBar::WinPanel );
    progress->setFrameShadow( QProgressBar::Raised );
    Form1Layout->addWidget( progress );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProgressDlgForm::~ProgressDlgForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*  
 *  Main event handler. Reimplemented to handle application
 *  font changes
 */
bool ProgressDlgForm::event( QEvent* ev )
{
    bool ret = QDialog::event( ev ); 
    if ( ev->type() == QEvent::ApplicationFontChange ) {
	QFont TextLabel1_font(  TextLabel1->font() );
	TextLabel1_font.setBold( TRUE );
	TextLabel1->setFont( TextLabel1_font ); 
	QFont TextLabel2_font(  TextLabel2->font() );
	TextLabel2_font.setBold( TRUE );
	TextLabel2->setFont( TextLabel2_font ); 
    }
    return ret;
}

