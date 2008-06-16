//
// C++ Interface: translationscript
//
// Description: 
//
//
// Author: Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
class TranslationScript : public QObject {
    Kross::Action* action;
public:
    TranslationScript(QString filename);
    void getLanguagePairs();
    QStringList translateWord(QString word, QString from, QString to);

    ~TranslationScript();

};

#endif
