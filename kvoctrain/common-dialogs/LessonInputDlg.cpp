/***************************************************************************

    $Id$

                            input several descriptions

    -----------------------------------------------------------------------

    begin                : Thu Dec 9 1999
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "LessonInputDlg.h"

#define Inherited LessonInputDlgData

#include <qpixmap.h>

#include <kv_resource.h>
#include <compat_2x.h>
#include "../kvoctrain.h"

LessonInputDlg::LessonInputDlg
(
        QString  _input,
        QString  _descr,
        QString  _label,
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	connect( e_title, SIGNAL(returnPressed()), SLOT(accept()) );
	connect( b_cancel, SIGNAL(clicked()), SLOT(reject()) );
	connect( b_ok, SIGNAL(clicked()), SLOT(accept()) );
        b_ok->setDefault(true);

        if (_descr.isEmpty() )
          _descr = i18n("Input lesson description");

        title_label->setText (_label);
        setCaption (kvoctrainApp::generateCaption(_descr));

        input = _input;
        e_title->setText (input);
	connect( e_title, SIGNAL(textChanged(const QString&)), SLOT(slotInputChanged(const QString&)) );
   	e_title->setFocus();
	e_title->selectAll();
        setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
}


void LessonInputDlg::slotInputChanged(const QString& s)
{
  input = s;
}
