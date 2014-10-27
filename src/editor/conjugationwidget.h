/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONJUGATIONWIDGET_H
#define CONJUGATIONWIDGET_H

#include "ui_conjugationwidget.h"

#include <keduvocwordflags.h>

class KEduVocExpression;
class KEduVocDocument;

namespace Editor
{

class ConjugationWidget : public QWidget, public Ui::ConjugationWidget
{
    Q_OBJECT

public:
    explicit ConjugationWidget(QWidget *parent = 0);

public slots:
    void setDocument(KEduVocDocument* doc);
    void setTranslation(KEduVocExpression* entry, int translation);

signals:
    void sigModified();

private slots:
    void textChanged(const QString&);
    void slotTenseSelected(int);
    void slotNextTense();
    void tenseEditingFinished();

private:
    void updateVisiblePersons();

    void showWidgets(bool tenses, bool singular, bool dual, bool plural, bool maleVisible, bool femaleVisible, bool neuterVisible);

    /**
     * Fill the line edits
     */
    void updateEntries();

    QString m_lastTenseSelection;
    int m_identifier;
    KEduVocExpression* m_entry;
    KEduVocDocument* m_doc;

    /**
     * All line edits and labels, index corresponding to KEduVocWordFlag::indexOf
     */
    QMap< KEduVocWordFlags, QLineEdit* > m_conjugationLineEdits;
};

}

#endif
