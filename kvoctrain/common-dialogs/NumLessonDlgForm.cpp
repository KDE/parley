/****************************************************************************
** Form implementation generated from reading ui file 'NumLessonDlg.ui'
**
** Created: Fri Oct 12 20:29:20 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "NumLessonDlgForm.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a NumLessonDlgForm which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
NumLessonDlgForm::NumLessonDlgForm( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "LessonInputDlgForm" );
    resize( 249, 147 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 100, 140 ) );
    setCaption( tr( "LessonInputDlg" ) );
    LessonInputDlgFormLayout = new QVBoxLayout( this ); 
    LessonInputDlgFormLayout->setSpacing( 6 );
    LessonInputDlgFormLayout->setMargin( 11 );

    title_label = new QLabel( this, "title_label" );
    title_label->setText( tr( "Number of entries per lesson" ) );
    LessonInputDlgFormLayout->addWidget( title_label );

    e_title = new QLineEdit( this, "e_title" );
    e_title->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, e_title->sizePolicy().hasHeightForWidth() ) );
    e_title->setMinimumSize( QSize( 0, 0 ) );
    LessonInputDlgFormLayout->addWidget( e_title );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    LessonInputDlgFormLayout->addItem( spacer );

    Frame3 = new QFrame( this, "Frame3" );
    Frame3->setFrameShape( QFrame::NoFrame );
    Frame3->setFrameShadow( QFrame::Raised );
    Frame3Layout = new QHBoxLayout( Frame3 ); 
    Frame3Layout->setSpacing( 6 );
    Frame3Layout->setMargin( 11 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Frame3Layout->addItem( spacer_2 );

    b_ok = new QPushButton( Frame3, "b_ok" );
    b_ok->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, b_ok->sizePolicy().hasHeightForWidth() ) );
    b_ok->setMinimumSize( QSize( 100, 30 ) );
    b_ok->setText( tr( "&OK" ) );
    Frame3Layout->addWidget( b_ok );

    b_cancel = new QPushButton( Frame3, "b_cancel" );
    b_cancel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, b_cancel->sizePolicy().hasHeightForWidth() ) );
    b_cancel->setMinimumSize( QSize( 100, 30 ) );
    b_cancel->setText( tr( "&Cancel" ) );
    Frame3Layout->addWidget( b_cancel );
    LessonInputDlgFormLayout->addWidget( Frame3 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
NumLessonDlgForm::~NumLessonDlgForm()
{
    // no need to delete child widgets, Qt does it all for us
}

