/**********************************************************************

	--- Qt Architect generated file ---

	File: SimpleQueryDlgData.cpp
	Last generated: Thu Oct 4 17:22:08 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "SimpleQueryDlgData.h"

#define Inherited QueryDlgBase

#include <qlabel.h>
#include <qbuttongroup.h>
#include <kapp.h>  // i18n macro

#include <compat_2x.h> // string type macro 


SimpleQueryDlgData::SimpleQueryDlgData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, 0 )
{
	QButtonGroup* qtarch_ButtonGroup_1;
	qtarch_ButtonGroup_1 = new QButtonGroup( this, "ButtonGroup_1" );
	qtarch_ButtonGroup_1->setGeometry( 10, 10, 580, 380 );
	qtarch_ButtonGroup_1->setFrameStyle( 49 );
	qtarch_ButtonGroup_1->setTitle( "" );
	qtarch_ButtonGroup_1->setAlignment( 1 );

	QButtonGroup* qtarch_ButtonGroup_5;
	qtarch_ButtonGroup_5 = new QButtonGroup( this, "ButtonGroup_5" );
	qtarch_ButtonGroup_5->setGeometry( 440, 20, 130, 170 );
	qtarch_ButtonGroup_5->setFrameStyle( 49 );
	qtarch_ButtonGroup_5->setTitle( i18n( "Progress" ));
	qtarch_ButtonGroup_5->setAlignment( 1 );

	groupName = new QButtonGroup( this, "ButtonGroup_2" );
	groupName->setGeometry( 30, 20, 400, 360 );
	groupName->setFrameStyle( 49 );
	groupName->setTitle( i18n( "Translation" ));
	groupName->setAlignment( 1 );

	show_more = new QPushButton( this, "PushButton_1" );
	show_more->setGeometry( 40, 280, 110, 30 );
	show_more->setText( i18n( "Show &more" ));

	show_all = new QPushButton( this, "PushButton_2" );
	show_all->setGeometry( 40, 330, 110, 30 );
	show_all->setText( i18n( "Show &all" ));

	verify = new QPushButton( this, "PushButton_3" );
	verify->setGeometry( 170, 280, 120, 30 );
	verify->setText( i18n( "&Verify" ));

	know_it = new QPushButton( this, "PushButton_5" );
	know_it->setGeometry( 310, 280, 110, 30 );
	know_it->setText( i18n( "I &know it" ));

	dont_know = new QPushButton( this, "PushButton_4" );
	dont_know->setGeometry( 310, 330, 110, 30 );
	dont_know->setText( i18n( "&Don't know" ));

	stop_it = new QPushButton( this, "PushButton_7" );
	stop_it->setGeometry( 460, 280, 100, 30 );
	stop_it->setText( i18n( "&Stop query" ));

	options = new QPushButton( this, "PushButton_8" );
	options->setGeometry( 460, 330, 100, 30 );
	options->setText( i18n( "&Options" ));

	QLabel* qtarch_Label_9;
	qtarch_Label_9 = new QLabel( this, "Label_9" );
	qtarch_Label_9->setGeometry( 450, 160, 60, 20 );
	qtarch_Label_9->setFrameStyle( 0 );
	qtarch_Label_9->setText( i18n( "Cycle:" ));
	qtarch_Label_9->setAlignment( 289 );

	progCount = new QLabel( this, "Label_10" );
	progCount->setGeometry( 530, 160, 20, 20 );
	progCount->setFrameStyle( 0 );
	progCount->setText( i18n( "1" ));
	progCount->setAlignment( 289 );

	b_edit = new QPushButton( this, "PushButton_14" );
	b_edit->setGeometry( 310, 220, 110, 30 );
	b_edit->setText( i18n( "E&dit" ));

	queryLabel = new QLabel( this, "Label_11" );
	queryLabel->setGeometry( 40, 60, 100, 20 );
	queryLabel->setFrameStyle( 0 );
	queryLabel->setText( i18n( "???" ));
	queryLabel->setAlignment( 289 );

	answerLabel = new QLabel( this, "Label_12" );
	answerLabel->setGeometry( 40, 130, 100, 20 );
	answerLabel->setFrameStyle( 0 );
	answerLabel->setText( i18n( "???" ));
	answerLabel->setAlignment( 289 );

	countlabel = new QLabel( this, "Label_13" );
	countlabel->setGeometry( 450, 40, 100, 20 );
	countlabel->setFrameStyle( 0 );
	countlabel->setText( i18n( "Count" ));
	countlabel->setAlignment( 289 );

	timelabel = new QLabel( this, "Label_14" );
	timelabel->setGeometry( 450, 100, 100, 20 );
	timelabel->setFrameStyle( 0 );
	timelabel->setText( i18n( "Time" ));
	timelabel->setAlignment( 289 );

	countbar = new MyProgressBar( this, "User_1" );
	countbar->setGeometry( 450, 60, 110, 30 );

	timebar = new MyProgressBar( this, "User_2" );
	timebar->setGeometry( 450, 120, 110, 30 );

	queryField = new QLabel( this, "Label_15" );
	queryField->setGeometry( 40, 80, 380, 30 );
	queryField->setFrameStyle( 51 );
	queryField->setText( "" );
	queryField->setAlignment( 289 );

	answerField = new QMultiLineEdit( this, "MultiLineEdit_1" );
	answerField->setGeometry( 40, 150, 380, 50 );
	answerField->setFrameStyle( 51 );
	answerField->insertLine( "" );
	answerField->setReadOnly( FALSE );
	answerField->setOverwriteMode( FALSE );
	answerField->setAutoUpdate( TRUE );




	resize( 600,410 );
}


//SimpleQueryDlgData::~SimpleQueryDlgData()
//{
//}
#include "SimpleQueryDlgData.moc"
