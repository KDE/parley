/***************************************************************************

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#ifndef STARTPRACTICEDIALOG_H
#define STARTPRACTICEDIALOG_H

#include <keduvocdocument.h>
#include <KConfigDialog>

class StartPracticeWidget;

/**
  *
  */
class StartPracticeDialog : public KConfigDialog
{
    Q_OBJECT
public:
    StartPracticeDialog(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config);

    ~StartPracticeDialog();

    void commitData();

public slots:
    void accept();

private:
    StartPracticeWidget* m_startPracticeWidget;
};

#endif // StartPracticeDialog_included

