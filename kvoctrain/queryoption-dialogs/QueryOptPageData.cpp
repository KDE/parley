/**********************************************************************

	--- Qt Architect generated file ---

	File: QueryOptPageData.cpp
	Last generated: Thu Oct 4 17:23:21 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "QueryOptPageData.h"

#define Inherited QDialog

#include <qbuttongroup.h>
#include <kapp.h>  // i18n macro

#include <compat_2x.h> // string type macro 


QueryOptPageData::QueryOptPageData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE, 0 )
{
	QButtonGroup* qtarch_ButtonGroup_12;
	qtarch_ButtonGroup_12 = new QButtonGroup( this, "ButtonGroup_12" );
	qtarch_ButtonGroup_12->setGeometry( 10, 10, 500, 340 );
	qtarch_ButtonGroup_12->setFrameStyle( 49 );
	qtarch_ButtonGroup_12->setTitle( i18n( "Query options" ));
	qtarch_ButtonGroup_12->setAlignment( 1 );

	query_group = new QButtonGroup( this, "ButtonGroup_14" );
	query_group->setGeometry( 30, 40, 460, 120 );
	query_group->setFrameStyle( 49 );
	query_group->setTitle( i18n( "Time per query" ));
	query_group->setAlignment( 1 );

	label_mqtime = new QLabel( this, "Label_31" );
	label_mqtime->setGeometry( 280, 60, 140, 20 );
	label_mqtime->setFrameStyle( 0 );
	label_mqtime->setText( i18n( "&Max. time (s)" ));
	label_mqtime->setAlignment( 289 );

	e_mqtime = new QLineEdit( this, "LineEdit_10" );
	e_mqtime->setGeometry( 280, 80, 100, 30 );
	e_mqtime->setText( "" );
	e_mqtime->setMaxLength( 32767 );

	swap = new QCheckBox( this, "CheckBox_1" );
	swap->setGeometry( 40, 190, 200, 30 );
	swap->setText( i18n( "S&wap direction randomly" ));
	swap->setChecked( FALSE );

	r_show_to = new QRadioButton( this, "RadioButton_1" );
	r_show_to->setGeometry( 50, 90, 210, 30 );
	r_show_to->setText( i18n( "&show solution" ));
	r_show_to->setChecked( FALSE );

	r_cont_to = new QRadioButton( this, "RadioButton_2" );
	r_cont_to->setGeometry( 50, 120, 210, 30 );
	r_cont_to->setText( i18n( "&continue after timeout" ));
	r_cont_to->setChecked( FALSE );

	r_nolimit_to = new QRadioButton( this, "RadioButton_3" );
	r_nolimit_to->setGeometry( 50, 60, 210, 30 );
	r_nolimit_to->setText( i18n( "&no time limitation" ));
	r_nolimit_to->setChecked( FALSE );

	showrem = new QCheckBox( this, "CheckBox_2" );
	showrem->setGeometry( 280, 130, 200, 20 );
	showrem->setText( i18n( "S&how remaining time" ));
	showrem->setChecked( FALSE );



	resize( 520,360 );
}


//QueryOptPageData::~QueryOptPageData()
//{
//}
#include "QueryOptPageData.moc"
