/***************************************************************************

                       edit "additional" properties

    -----------------------------------------------------------------------

    begin         : Thu Nov 25 17:29:44 1999

    copyright     : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
                    (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                    (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#ifndef ADDITIONALEDITPAGE_H
#define ADDITIONALEDITPAGE_H

#include "ui_additionaleditpage.h"
#include <keduvocdocument.h>

namespace Phonon
{
    class MediaObject;
}

class AdditionalEditPage : public QWidget, public Ui::AdditionalEditPage
{
    Q_OBJECT

public:
    explicit AdditionalEditPage(KEduVocDocument *doc, QWidget *parent = 0);


    void setData(int row, int col);
    void commitData();
    void clear();

    bool isModified();

signals:
    void sigModified();

private slots:
    void slotDataChanged();
    void slotImageChanged(const QString& url);
    void playAudio();

private:
    KEduVocDocument     *m_doc;
    int m_currentRow;
    int m_currentTranslation;

    Phonon::MediaObject* m_player;
};

#endif
