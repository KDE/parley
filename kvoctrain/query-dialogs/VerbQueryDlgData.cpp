/**********************************************************************

	--- Qt Architect generated file ---

	File: VerbQueryDlgData.cpp
	Last generated: Thu Oct 4 17:21:47 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "VerbQueryDlgData.h"

#define Inherited QueryDlgBase

#include <qlabel.h>
#include <kseparator.h>
#include <qbuttongroup.h>
#include <kapp.h>  // i18n macro

#include <compat_2x.h> // string type macro 


VerbQueryDlgData::VerbQueryDlgData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, 0 )
{
	QButtonGroup* qtarch_ButtonGroup_1;
	qtarch_ButtonGroup_1 = new QButtonGroup( this, "ButtonGroup_1" );
	qtarch_ButtonGroup_1->setGeometry( 10, 10, 600, 390 );
	qtarch_ButtonGroup_1->setFrameStyle( 49 );
	qtarch_ButtonGroup_1->setTitle( "" );
	qtarch_ButtonGroup_1->setAlignment( 1 );

	p1sLabel = new QLabel( this, "Label_19" );
	p1sLabel->setGeometry( 40, 90, 30, 30 );
	p1sLabel->setFrameStyle( 0 );
	p1sLabel->setText( i18n( "-" ));
	p1sLabel->setAlignment( 289 );

	p1sField = new QLineEdit( this, "LineEdit_1" );
	p1sField->setGeometry( 80, 90, 130, 30 );
	p1sField->setText( "" );
	p1sField->setMaxLength( 32767 );

	p1pLabel = new QLabel( this, "Label_33" );
	p1pLabel->setGeometry( 240, 90, 50, 30 );
	p1pLabel->setFrameStyle( 0 );
	p1pLabel->setText( i18n( "-" ));
	p1pLabel->setAlignment( 289 );

	p2sLabel = new QLabel( this, "Label_21" );
	p2sLabel->setGeometry( 40, 130, 30, 30 );
	p2sLabel->setFrameStyle( 0 );
	p2sLabel->setText( i18n( "-" ));
	p2sLabel->setAlignment( 289 );

	p2sField = new QLineEdit( this, "LineEdit_6" );
	p2sField->setGeometry( 80, 130, 130, 30 );
	p2sField->setText( "" );
	p2sField->setMaxLength( 32767 );

	p3sfLabel = new QLabel( this, "Label_22" );
	p3sfLabel->setGeometry( 40, 170, 30, 30 );
	p3sfLabel->setFrameStyle( 0 );
	p3sfLabel->setText( i18n( "-" ));
	p3sfLabel->setAlignment( 289 );

	p3sfField = new QLineEdit( this, "LineEdit_7" );
	p3sfField->setGeometry( 80, 170, 130, 30 );
	p3sfField->setText( "" );
	p3sfField->setMaxLength( 32767 );

	p3pfLabel = new QLabel( this, "Label_31" );
	p3pfLabel->setGeometry( 240, 170, 50, 30 );
	p3pfLabel->setFrameStyle( 0 );
	p3pfLabel->setText( i18n( "-" ));
	p3pfLabel->setAlignment( 289 );

	p3smLabel = new QLabel( this, "Label_23" );
	p3smLabel->setGeometry( 40, 210, 30, 30 );
	p3smLabel->setFrameStyle( 0 );
	p3smLabel->setText( i18n( "-" ));
	p3smLabel->setAlignment( 289 );

	p3smField = new QLineEdit( this, "LineEdit_8" );
	p3smField->setGeometry( 80, 210, 130, 30 );
	p3smField->setText( "" );
	p3smField->setMaxLength( 32767 );

	p3pmLabel = new QLabel( this, "Label_30" );
	p3pmLabel->setGeometry( 240, 210, 50, 30 );
	p3pmLabel->setFrameStyle( 0 );
	p3pmLabel->setText( i18n( "-" ));
	p3pmLabel->setAlignment( 289 );

	p3snLabel = new QLabel( this, "Label_24" );
	p3snLabel->setGeometry( 40, 250, 30, 30 );
	p3snLabel->setFrameStyle( 0 );
	p3snLabel->setText( i18n( "-" ));
	p3snLabel->setAlignment( 289 );

	p3snField = new QLineEdit( this, "LineEdit_9" );
	p3snField->setGeometry( 80, 250, 130, 30 );
	p3snField->setText( "" );
	p3snField->setMaxLength( 32767 );

	p3pnLabel = new QLabel( this, "Label_29" );
	p3pnLabel->setGeometry( 240, 250, 50, 30 );
	p3pnLabel->setFrameStyle( 0 );
	p3pnLabel->setText( i18n( "-" ));
	p3pnLabel->setAlignment( 289 );

	p1pField = new QLineEdit( this, "LineEdit_5" );
	p1pField->setGeometry( 300, 90, 130, 30 );
	p1pField->setText( "" );
	p1pField->setMaxLength( 32767 );

	p2pField = new QLineEdit( this, "LineEdit_10" );
	p2pField->setGeometry( 300, 130, 130, 30 );
	p2pField->setText( "" );
	p2pField->setMaxLength( 32767 );

	p3pfField = new QLineEdit( this, "LineEdit_11" );
	p3pfField->setGeometry( 300, 170, 130, 30 );
	p3pfField->setText( "" );
	p3pfField->setMaxLength( 32767 );

	p3pmField = new QLineEdit( this, "LineEdit_12" );
	p3pmField->setGeometry( 300, 210, 130, 30 );
	p3pmField->setText( "" );
	p3pmField->setMaxLength( 32767 );

	p3pnField = new QLineEdit( this, "LineEdit_13" );
	p3pnField->setGeometry( 300, 250, 130, 30 );
	p3pnField->setText( "" );
	p3pnField->setMaxLength( 32767 );

	show_all = new QPushButton( this, "PushButton_2" );
	show_all->setGeometry( 60, 310, 110, 30 );
	show_all->setText( i18n( "Show &all" ));

	verify = new QPushButton( this, "PushButton_3" );
	verify->setGeometry( 60, 350, 110, 30 );
	verify->setText( i18n( "&Verify" ));

	know_it = new QPushButton( this, "PushButton_5" );
	know_it->setGeometry( 260, 310, 110, 30 );
	know_it->setText( i18n( "I &know it" ));

	dont_know = new QPushButton( this, "PushButton_4" );
	dont_know->setGeometry( 260, 350, 110, 30 );
	dont_know->setText( i18n( "&Don't know" ));

	b_edit = new QPushButton( this, "PushButton_6" );
	b_edit->setGeometry( 480, 230, 100, 30 );
	b_edit->setText( i18n( "&Edit" ));

	stop_it = new QPushButton( this, "PushButton_7" );
	stop_it->setGeometry( 480, 310, 100, 30 );
	stop_it->setText( i18n( "&Stop query" ));

	options = new QPushButton( this, "PushButton_8" );
	options->setGeometry( 480, 350, 100, 30 );
	options->setText( i18n( "&Options" ));

	p2pLabel = new QLabel( this, "Label_32" );
	p2pLabel->setGeometry( 240, 130, 50, 30 );
	p2pLabel->setFrameStyle( 0 );
	p2pLabel->setText( i18n( "-" ));
	p2pLabel->setAlignment( 289 );

	KSeparator* qtarch_KSeparator_1;
	qtarch_KSeparator_1 = new KSeparator( this, "KSeparator_1" );
	qtarch_KSeparator_1->setGeometry( 220, 80, 10, 210 );
	qtarch_KSeparator_1->setFrameStyle( 53 );

	QLabel* qtarch_Label_34;
	qtarch_Label_34 = new QLabel( this, "Label_34" );
	qtarch_Label_34->setGeometry( 80, 70, 100, 20 );
	qtarch_Label_34->setFrameStyle( 0 );
	qtarch_Label_34->setText( i18n( "singular" ));
	qtarch_Label_34->setAlignment( 289 );

	QLabel* qtarch_Label_35;
	qtarch_Label_35 = new QLabel( this, "Label_35" );
	qtarch_Label_35->setGeometry( 300, 70, 100, 20 );
	qtarch_Label_35->setFrameStyle( 0 );
	qtarch_Label_35->setText( i18n( "plural" ));
	qtarch_Label_35->setAlignment( 289 );

	groupTitle = new QLabel( this, "Label_36" );
	groupTitle->setGeometry( 30, 30, 210, 30 );
	{
		QFont font( "helvetica", 12, 75, 0 );
		font.setStyleHint( (QFont::StyleHint)0 );
		font.setCharSet( (QFont::CharSet)0 );
		groupTitle->setFont( font );
	}
	groupTitle->setFrameStyle( 0 );
	groupTitle->setText( i18n( "-" ));
	groupTitle->setAlignment( 289 );

	QButtonGroup* qtarch_ButtonGroup_5;
	qtarch_ButtonGroup_5 = new QButtonGroup( this, "ButtonGroup_5" );
	qtarch_ButtonGroup_5->setGeometry( 460, 20, 130, 170 );
	qtarch_ButtonGroup_5->setFrameStyle( 49 );
	qtarch_ButtonGroup_5->setTitle( i18n( "Progress" ));
	qtarch_ButtonGroup_5->setAlignment( 1 );

	countlabel = new QLabel( this, "Label_13" );
	countlabel->setGeometry( 470, 40, 100, 20 );
	countlabel->setFrameStyle( 0 );
	countlabel->setText( i18n( "Count" ));
	countlabel->setAlignment( 289 );

	countbar = new MyProgressBar( this, "User_1" );
	countbar->setGeometry( 470, 60, 110, 30 );

	timelabel = new QLabel( this, "Label_14" );
	timelabel->setGeometry( 470, 100, 100, 20 );
	timelabel->setFrameStyle( 0 );
	timelabel->setText( i18n( "Time" ));
	timelabel->setAlignment( 289 );

	timebar = new MyProgressBar( this, "User_2" );
	timebar->setGeometry( 470, 120, 110, 30 );

	QLabel* qtarch_Label_9;
	qtarch_Label_9 = new QLabel( this, "Label_9" );
	qtarch_Label_9->setGeometry( 470, 160, 60, 20 );
	qtarch_Label_9->setFrameStyle( 0 );
	qtarch_Label_9->setText( i18n( "Cycle:" ));
	qtarch_Label_9->setAlignment( 289 );

	progCount = new QLabel( this, "Label_10" );
	progCount->setGeometry( 550, 160, 20, 20 );
	progCount->setFrameStyle( 0 );
	progCount->setText( i18n( "1" ));
	progCount->setAlignment( 289 );

	p1sAccel = new QLabel( this, "Label_49" );
	p1sAccel->setGeometry( 20, 90, 20, 30 );
	p1sAccel->setFrameStyle( 0 );
	p1sAccel->setText( i18n( "&1:" ));
	p1sAccel->setAlignment( 289 );

	p2sAccel = new QLabel( this, "Label_50" );
	p2sAccel->setGeometry( 20, 130, 20, 30 );
	p2sAccel->setFrameStyle( 0 );
	p2sAccel->setText( i18n( "&2:" ));
	p2sAccel->setAlignment( 289 );

	p3sfAccel = new QLabel( this, "Label_51" );
	p3sfAccel->setGeometry( 20, 170, 20, 30 );
	p3sfAccel->setFrameStyle( 0 );
	p3sfAccel->setText( i18n( "&3:" ));
	p3sfAccel->setAlignment( 289 );

	p3smAccel = new QLabel( this, "Label_52" );
	p3smAccel->setGeometry( 20, 210, 20, 30 );
	p3smAccel->setFrameStyle( 0 );
	p3smAccel->setText( i18n( "&4:" ));
	p3smAccel->setAlignment( 289 );

	p3snAccel = new QLabel( this, "Label_53" );
	p3snAccel->setGeometry( 20, 250, 20, 30 );
	p3snAccel->setFrameStyle( 0 );
	p3snAccel->setText( i18n( "&5:" ));
	p3snAccel->setAlignment( 289 );

	groupVerb = new QLabel( this, "Label_54" );
	groupVerb->setGeometry( 250, 30, 180, 30 );
	groupVerb->setFrameStyle( 51 );
	groupVerb->setText( i18n( "-" ));
	groupVerb->setAlignment( 289 );



	resize( 620,410 );
}


//VerbQueryDlgData::~VerbQueryDlgData()
//{
//}
#include "VerbQueryDlgData.moc"
