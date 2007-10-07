/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GRAMMARDIALOG_H
#define GRAMMARDIALOG_H

#include <KPageDialog>
#include <QList>

class KEduVocDocument;
class LangPropPage;
class UsageOptPage;
class WordTypeOptionPage;
class TenseOptPage;

/**
  * This is the KPageDialog for all the language option pages.
  * It contains the pages for Types, Tenses, Usage and Articles/Personal Pronouns.
  */
class GrammarDialog : public KPageDialog
{
    Q_OBJECT
public:
    GrammarDialog(KEduVocDocument *doc, QWidget *parent);
    ~GrammarDialog();
    void accept();

private:
    UsageOptPage          *useOptPage;
    WordTypeOptionPage    *typeOptPage;
    TenseOptPage          *tenseOptPage;
    QList<LangPropPage *> langPages;
};

#endif
