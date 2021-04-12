/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONTAINERMIMEDATA_H
#define CONTAINERMIMEDATA_H

#include <QMimeData>

class KEduVocContainer;


class ContainerMimeData : public QMimeData
{
    Q_OBJECT
public:
    void addContainer(KEduVocContainer* container);
    QList<KEduVocContainer*> containerList() const;

private:
    QList<KEduVocContainer*> m_containers;
};


#endif
