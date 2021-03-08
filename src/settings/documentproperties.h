/*
    enter document title and author
    SPDX-FileCopyrightText: 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    SPDX-FileCopyrightText: 2005 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DOCUMENTPROPERTIES_H
#define DOCUMENTPROPERTIES_H

#include "ui_documentproperties.h"

class KEduVocDocument;

class DocumentProperties : public QWidget, public Ui::DocumentProperties
{
    Q_OBJECT

public:
    DocumentProperties(KEduVocDocument * doc, bool languageSetup, QWidget *parent);

    /**
     * Return a [key, value] map, where key = Native language name (Native country name), value = Locale name
     * @return A QMap of [ Native language name (Native country name), Locale name ] pairs
     */
    static QMap<QString, QString> localeLangsMap();


public slots:
    void accept();

private:
    void prepareLanguageSelection();
    void acceptLanguageConfiguration();

    KEduVocDocument* m_doc;
    // also allow the user to setup two languages - used when first creating a document
    bool m_showLanguages;
};

#endif
