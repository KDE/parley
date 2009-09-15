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

#ifndef SYNONYMWIDGET_H
#define SYNONYMWIDGET_H

#include "ui_synonymwidget.h"

#include <QModelIndex>

class KEduVocExpression;
class KEduVocTranslation;
class QStringListModel;

namespace Editor {
    
class SynonymWidget : public QWidget, public Ui::SynonymWidget
{
    Q_OBJECT
public:
    enum SynonymWidgetType {
        Synonym,
        Antonym,
        FalseFriend
    };

    explicit SynonymWidget(SynonymWidgetType type, QWidget *parent = 0);

public slots:
    void setTranslation(KEduVocExpression* entry, int translation);

private slots:
    void togglePair();
    void updateList();

private:
    // the one that was selected before
    KEduVocTranslation* m_lastTranslation;
    // the one the user clicked on latest
    KEduVocTranslation* m_currentTranslation;
    QStringListModel * m_listModel;
    SynonymWidgetType m_type;
};
}
#endif
