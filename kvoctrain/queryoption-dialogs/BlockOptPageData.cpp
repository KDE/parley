/**********************************************************************

	--- Qt Architect generated file ---

	File: BlockOptPageData.cpp
	Last generated: Thu Oct 4 17:23:25 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "BlockOptPageData.h"

#define Inherited QDialog

#include <qbuttongroup.h>
#include <kapp.h>  // i18n macro

#include <compat_2x.h> // string type macro 


BlockOptPageData::BlockOptPageData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE, 0 )
{
	QButtonGroup* qtarch_ButtonGroup_1;
	qtarch_ButtonGroup_1 = new QButtonGroup( this, "ButtonGroup_1" );
	qtarch_ButtonGroup_1->setGeometry( 10, 10, 500, 340 );
	qtarch_ButtonGroup_1->setFrameStyle( 49 );
	qtarch_ButtonGroup_1->setTitle( i18n( "Query blocking and expiring times" ));
	qtarch_ButtonGroup_1->setAlignment( 1 );

	c_blocking = new QCheckBox( this, "CheckBox_2" );
	c_blocking->setGeometry( 150, 30, 140, 30 );
	c_blocking->setText( i18n( "Bl&ocking" ));
	c_blocking->setChecked( FALSE );

	c_expire = new QCheckBox( this, "CheckBox_3" );
	c_expire->setGeometry( 310, 30, 140, 30 );
	c_expire->setText( i18n( "E&xpiring" ));
	c_expire->setChecked( FALSE );

	l_lev7 = new QLabel( this, "Label_14" );
	l_lev7->setGeometry( 30, 60, 90, 20 );
	l_lev7->setFrameStyle( 0 );
	l_lev7->setText( i18n( "Level &7:" ));
	l_lev7->setAlignment( 289 );

	block7 = new QComboBox( FALSE, this, "ComboBox_24" );
	block7->setGeometry( 150, 60, 130, 30 );
	block7->setSizeLimit( 10 );
	block7->setAutoResize( FALSE );
	block7->setMaxCount( 2147483647 );
	block7->setAutoCompletion( FALSE );

	expire7 = new QComboBox( FALSE, this, "ComboBox_32" );
	expire7->setGeometry( 310, 60, 130, 30 );
	expire7->setSizeLimit( 10 );
	expire7->setAutoResize( FALSE );
	expire7->setMaxCount( 2147483647 );
	expire7->setAutoCompletion( FALSE );

	l_lev6 = new QLabel( this, "Label_15" );
	l_lev6->setGeometry( 30, 100, 90, 20 );
	l_lev6->setFrameStyle( 0 );
	l_lev6->setText( i18n( "Level &6:" ));
	l_lev6->setAlignment( 289 );

	block6 = new QComboBox( FALSE, this, "ComboBox_26" );
	block6->setGeometry( 150, 100, 130, 30 );
	block6->setSizeLimit( 10 );
	block6->setAutoResize( FALSE );
	block6->setMaxCount( 2147483647 );
	block6->setAutoCompletion( FALSE );

	expire6 = new QComboBox( FALSE, this, "ComboBox_33" );
	expire6->setGeometry( 310, 100, 130, 30 );
	expire6->setSizeLimit( 10 );
	expire6->setAutoResize( FALSE );
	expire6->setMaxCount( 2147483647 );
	expire6->setAutoCompletion( FALSE );

	l_lev5 = new QLabel( this, "Label_16" );
	l_lev5->setGeometry( 30, 140, 90, 20 );
	l_lev5->setFrameStyle( 0 );
	l_lev5->setText( i18n( "Level &5:" ));
	l_lev5->setAlignment( 289 );

	block5 = new QComboBox( FALSE, this, "ComboBox_27" );
	block5->setGeometry( 150, 140, 130, 30 );
	block5->setSizeLimit( 10 );
	block5->setAutoResize( FALSE );
	block5->setMaxCount( 2147483647 );
	block5->setAutoCompletion( FALSE );

	expire5 = new QComboBox( FALSE, this, "ComboBox_34" );
	expire5->setGeometry( 310, 140, 130, 30 );
	expire5->setSizeLimit( 10 );
	expire5->setAutoResize( FALSE );
	expire5->setMaxCount( 2147483647 );
	expire5->setAutoCompletion( FALSE );

	l_lev4 = new QLabel( this, "Label_17" );
	l_lev4->setGeometry( 30, 180, 90, 20 );
	l_lev4->setFrameStyle( 0 );
	l_lev4->setText( i18n( "Level &4:" ));
	l_lev4->setAlignment( 289 );

	block4 = new QComboBox( FALSE, this, "ComboBox_28" );
	block4->setGeometry( 150, 180, 130, 30 );
	block4->setSizeLimit( 10 );
	block4->setAutoResize( FALSE );
	block4->setMaxCount( 2147483647 );
	block4->setAutoCompletion( FALSE );

	expire4 = new QComboBox( FALSE, this, "ComboBox_35" );
	expire4->setGeometry( 310, 180, 130, 30 );
	expire4->setSizeLimit( 10 );
	expire4->setAutoResize( FALSE );
	expire4->setMaxCount( 2147483647 );
	expire4->setAutoCompletion( FALSE );

	l_lev3 = new QLabel( this, "Label_18" );
	l_lev3->setGeometry( 30, 220, 90, 20 );
	l_lev3->setFrameStyle( 0 );
	l_lev3->setText( i18n( "Level &3:" ));
	l_lev3->setAlignment( 289 );

	block3 = new QComboBox( FALSE, this, "ComboBox_29" );
	block3->setGeometry( 150, 220, 130, 30 );
	block3->setSizeLimit( 10 );
	block3->setAutoResize( FALSE );
	block3->setMaxCount( 2147483647 );
	block3->setAutoCompletion( FALSE );

	expire3 = new QComboBox( FALSE, this, "ComboBox_36" );
	expire3->setGeometry( 310, 220, 130, 30 );
	expire3->setSizeLimit( 10 );
	expire3->setAutoResize( FALSE );
	expire3->setMaxCount( 2147483647 );
	expire3->setAutoCompletion( FALSE );

	l_lev2 = new QLabel( this, "Label_19" );
	l_lev2->setGeometry( 30, 260, 90, 20 );
	l_lev2->setFrameStyle( 0 );
	l_lev2->setText( i18n( "Level &2:" ));
	l_lev2->setAlignment( 289 );

	block2 = new QComboBox( FALSE, this, "ComboBox_30" );
	block2->setGeometry( 150, 260, 130, 30 );
	block2->setSizeLimit( 10 );
	block2->setAutoResize( FALSE );
	block2->setMaxCount( 2147483647 );
	block2->setAutoCompletion( FALSE );

	expire2 = new QComboBox( FALSE, this, "ComboBox_37" );
	expire2->setGeometry( 310, 260, 130, 30 );
	expire2->setSizeLimit( 10 );
	expire2->setAutoResize( FALSE );
	expire2->setMaxCount( 2147483647 );
	expire2->setAutoCompletion( FALSE );

	l_lev1 = new QLabel( this, "Label_20" );
	l_lev1->setGeometry( 30, 300, 90, 20 );
	l_lev1->setFrameStyle( 0 );
	l_lev1->setText( i18n( "Level &1:" ));
	l_lev1->setAlignment( 289 );

	block1 = new QComboBox( FALSE, this, "ComboBox_31" );
	block1->setGeometry( 150, 300, 130, 30 );
	block1->setSizeLimit( 10 );
	block1->setAutoResize( FALSE );
	block1->setMaxCount( 2147483647 );
	block1->setAutoCompletion( FALSE );

	expire1 = new QComboBox( FALSE, this, "ComboBox_38" );
	expire1->setGeometry( 310, 300, 130, 30 );
	expire1->setSizeLimit( 10 );
	expire1->setAutoResize( FALSE );
	expire1->setMaxCount( 2147483647 );
	expire1->setAutoCompletion( FALSE );


	resize( 520,360 );
}


//BlockOptPageData::~BlockOptPageData()
//{
//}
