/***************************************************************************

    -----------------------------------------------------------------------

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STARTPRACTICEWIDGET_H
#define STARTPRACTICEWIDGET_H

#include "ui_startpracticewidget.h"
#include <QWidget>

class KEduVocDocument;

/**
 *
	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class StartPracticeWidget
    : public QWidget, public Ui::StartPracticeWidget
{
Q_OBJECT
public:
    StartPracticeWidget(KEduVocDocument* doc, QWidget *parent);

public slots:
    void commitData();

private slots:
    void fromLanguageSelected(int identifierFromIndex);
    void grammarTestToggled(bool state);

private:
    KEduVocDocument* m_doc;
};

#endif
