/***************************************************************************

    Copyright 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TRANSLATIONSCRIPT_H
#define TRANSLATIONSCRIPT_H

#include <QString>
#include <QObject>

#include <kross/core/manager.h>
#include <kross/core/action.h>

/**
Class used to load and use a translation script

    @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class TranslationScript : public QObject
{
    Kross::Action* action;
public:
    explicit TranslationScript(QString filename);
    void getLanguagePairs();
    QStringList translateWord(QString word, QString from, QString to);

    ~TranslationScript();

};

#endif
