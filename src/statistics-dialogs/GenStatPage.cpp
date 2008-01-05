/***************************************************************************

                     general statistics dialog page

    -----------------------------------------------------------------------

    begin         : Thu Sep 21 20:50:53 MET 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2006 Peter Hedlund <peter.hedlund@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "GenStatPage.h"

#include <QLabel>

#include <keduvocdocument.h>
#include <keduvoclesson.h>

GenStatPage::GenStatPage(KEduVocDocument *doc, QWidget* parent): QWidget(parent)
{
    setupUi(this);
    l_filename->setText(doc->url().path());
    l_title->setText(doc->title());
    l_author->setText(doc->author());
    QString s;
    s.setNum(doc->lesson()->entriesRecursive().count());
    kcfg_entriesPerLesson->setText(s);

//     QStringList lesson = doc->lessonNames();
//     s.setNum(lesson.size());
//     l_lessons->setText(s);
}

#include "GenStatPage.moc"
