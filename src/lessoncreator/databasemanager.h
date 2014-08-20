/***************************************************************************
    Copyright 2014 Amarvir Singh <amarvir.ammu.93@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QFile>

#include "db/drivermanager.h"
#include "db/driver.h"
#include "db/connection.h"
#include "db/cursor.h"


class DatabaseManager : public QObject
{
    public:
        DatabaseManager(QObject *parent = 0);
        ~DatabaseManager();

    public:
        void createDB();
        void getAllTranslations();
        void createTables();

    private:
        KexiDB::Connection *conn;

};

#endif // DATABASEMANAGER_H