/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
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
