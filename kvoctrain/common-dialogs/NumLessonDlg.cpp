/***************************************************************************

    $Id$

                     input number of entries per lesson

    -----------------------------------------------------------------------

    begin                : Sat May 27 19:37:59 2000
    copyright            : (C) 1999-2001 Ewald Arnold
                           (C) 2001 The KDE-EDU team
    email                : kvoctrain@ewald-arnold.de

    -----------------------------------------------------------------------

    $Log$
    Revision 1.1  2001/10/05 15:37:45  arnold
    import of version 0.7.0pre8 to kde-edu


 ***************************************************************************/

#include "NumLessonDlg.h"

#include <kapp.h>

#include <kv_resource.h>
#include <compat_2x.h>
#include "../kvoctrain.h"

#include <qlineedit.h>

NumLessonDlg::NumLessonDlg
(
  int         num,
	QWidget    *parent,
	const char *name
)
	:
	NumLessonDlgForm( parent, name )
{
      input = num;
      connect( e_title, SIGNAL(textChanged(const QString&)), SLOT(slotInputChanged(const QString&)) );
      connect( e_title, SIGNAL(returnPressed()), SLOT(accept()) );
      connect( b_cancel, SIGNAL(clicked()), SLOT(reject()) );
      connect( b_ok, SIGNAL(clicked()), SLOT(accept()) );
      b_ok->setDefault(true);

      QString s;
      s.setNum (input);
      e_title->setText(s);
      setIcon (QPixmap (EA_KDEDATADIR("",  "kvoctrain/mini-kvoctrain.xpm" )));
      setCaption (kvoctrainApp::generateCaption(i18n("Assign lessons"), true));
}


//NumLessonDlg::~NumLessonDlg()
//{
//}


void NumLessonDlg::slotInputChanged(const QString& s)
{
  input = QString(s).toInt();
}
