/**********************************************************************

	--- Qt Architect generated file ---

	File: GroupOptPageData.cpp
	Last generated: Sat Oct 13 21:49:58 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "GroupOptPageData.h"

#define Inherited QDialog

#include <qbuttongroup.h>
#include <kapp.h>  // i18n macro

#include <compat_2x.h> // string type macro 


GroupOptPageData::GroupOptPageData
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
	qtarch_ButtonGroup_12->setTitle( i18n( "Option groups" ));
	qtarch_ButtonGroup_12->setAlignment( 1 );

	l_name = new QLabel( this, "Label_32" );
	l_name->setGeometry( 40, 60, 100, 20 );
	l_name->setFrameStyle( 0 );
	l_name->setText( i18n( "N&ame" ));
	l_name->setAlignment( 289 );

	ps_name = new QComboBox( FALSE, this, "ComboBox_1" );
	ps_name->setGeometry( 40, 80, 440, 30 );
	ps_name->setSizeLimit( 10 );
	ps_name->setAutoResize( FALSE );
	ps_name->setMaxCount( 2147483647 );
	ps_name->setAutoCompletion( FALSE );

	ps_store = new QPushButton( this, "PushButton_4" );
	ps_store->setGeometry( 40, 150, 100, 30 );
	ps_store->setText( i18n( "&Retain" ));

	ps_recall = new QPushButton( this, "PushButton_5" );
	ps_recall->setGeometry( 150, 150, 100, 30 );
	ps_recall->setText( i18n( "Re&call" ));

	ps_new = new QPushButton( this, "PushButton_1" );
	ps_new->setGeometry( 270, 150, 100, 30 );
	ps_new->setText( i18n( "&New" ));

	ps_del = new QPushButton( this, "PushButton_2" );
	ps_del->setGeometry( 380, 150, 100, 30 );
	ps_del->setText( i18n( "&Delete" ));


	resize( 520,360 );
}


//GroupOptPageData::~GroupOptPageData()
//{
//}

#include "GroupOptPageData.moc"
