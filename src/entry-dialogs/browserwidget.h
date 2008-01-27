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

#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H


#include "ui_browserwidget.h"

class KEduVocExpression;
class KHTMLPart;

class BrowserWidget : public QWidget, public Ui::BrowserWidget
{
    Q_OBJECT
public:
    explicit BrowserWidget(QWidget* parent = 0);

public slots:
    /**
     * Update the word
     * @param entry 
     * @param translation 
     */
    void setTranslation(KEduVocExpression* entry, int translation);


private slots:
    void showCurrentTranslation();
    void openUrl(const KUrl& targetUrl);
private:
    /// Column in the document - corresponds to the language (-KV_COL_TRANS)
    int               m_currentTranslation;
    /// Selection in the doc - if more than one row is selected behavior is different
    KEduVocExpression* m_entry;

    KHTMLPart *m_htmlPart;
};
#endif
