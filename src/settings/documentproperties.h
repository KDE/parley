/***************************************************************************

                   enter document title and author

    -----------------------------------------------------------------------

    begin          : Thu Mar 11 20:50:53 MET 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
