/**********************************************************************

	--- Qt Architect generated file ---

	File: TypeOptPageData.cpp
	Last generated: Sun Sep 30 11:33:49 2001

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include <qpixmap.h>
#include <qlayout.h>
#include "TypeOptPageData.h"

#define Inherited QDialog

#include <kapp.h>  // i18n macro

#include <compat_2x.h> // string type macro 


TypeOptPageData::TypeOptPageData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, TRUE, 0 )
{
	type_group = new QButtonGroup( this, "ButtonGroup_10" );
	type_group->setGeometry( 10, 10, 580, 350 );
	type_group->setFrameStyle( 49 );
	type_group->setTitle( i18n( "Type Descriptions" ));
	type_group->setAlignment( 1 );

	typeList = new QListBox( this, "ListBox_1" );
	typeList->setGeometry( 20, 30, 560, 280 );
	typeList->setFrameStyle( 51 );
	typeList->setDragSelect( TRUE );
	typeList->setAutoScroll( TRUE );
	typeList->setScrollBar( FALSE );
	typeList->setAutoScrollBar( TRUE );
	typeList->setBottomScrollBar( FALSE );
	typeList->setAutoBottomScrollBar( TRUE );
	typeList->setSmoothScrolling( TRUE );
	typeList->setMultiSelection( FALSE );
	typeList->setAutoUpdate( TRUE );

	b_new = new QPushButton( this, "PushButton_22" );
	b_new->setGeometry( 20, 320, 80, 30 );
	b_new->setText( i18n( "&New" ));

	b_modify = new QPushButton( this, "PushButton_23" );
	b_modify->setGeometry( 110, 320, 80, 30 );
	b_modify->setText( i18n( "&Modify" ));

	b_delete = new QPushButton( this, "PushButton_24" );
	b_delete->setGeometry( 200, 320, 80, 30 );
	b_delete->setText( i18n( "&Delete" ));

	b_cleanup = new QPushButton( this, "PushButton_25" );
	b_cleanup->setGeometry( 500, 320, 80, 30 );
	b_cleanup->setText( i18n( "&Clean up" ));


	resize( 600,370 );
}


//TypeOptPageData::~TypeOptPageData()
//{
//}
