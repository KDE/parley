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

#ifndef LEITNERVIEW_H
#define LEITNERVIEW_H

#include "leitnermodel.h"

#include <QListView>

class ParleyApp;

/**
 * View for the lesson list.
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class LeitnerView : public QListView
{
    Q_OBJECT

public:
    LeitnerView(ParleyApp *parent);

    /** Set the model for the view.
     * @param model the model */
    void setModel(LeitnerModel *model);

public slots:
    /** Append a lesson to the model and automatically set an edit up so the user can change "New lesson" into something meaningfull.*/
    void slotCreateLeitnerBox();

    /** Remove a lesson. Ask if it's not empty. */
    void slotDeleteLeitnerBox();

private:
    LeitnerModel *m_model;
};

#endif
