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

#ifndef CONTAINERMIMEDATA_H
#define CONTAINERMIMEDATA_H

// #include <keduvoccontainer.h>

#include <QMimeData>

class KEduVocContainer;

namespace Editor {
    
class ContainerMimeData :public QMimeData {
    Q_OBJECT
public:
    void addContainer(KEduVocContainer* container);
    QList<KEduVocContainer*> containerList() const;

private:
    QList<KEduVocContainer*> m_containers;
};

}

#endif
