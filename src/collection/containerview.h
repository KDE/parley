/***************************************************************************

    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONTAINERVIEW_H
#define CONTAINERVIEW_H

#include <QTreeView>

class KEduVocContainer;
class KEduVocExpression;

class ContainerModel;


/**
 * View for containers (KEduVocContainer).
 * @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 */
class ContainerView : public QTreeView
{
    Q_OBJECT
public:
    /** Set up the lesson list widget.
     * Also creates the menu to manipulate the lessons.
     * @param parent parent widget */
    explicit ContainerView(QWidget *parent = 0);
    /** Set the model for the view.
     * @param model the model */
    virtual void setModel(ContainerModel *model);

public slots:
    /** let the user type a new name for the container */
    void slotRename();

protected:
    /** ContainerModel for this view. */
    ContainerModel *m_model;

private:
    // intentionally hide this to make sure a proper model is passed
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE {
        Q_UNUSED(model)
    }
};


#endif
