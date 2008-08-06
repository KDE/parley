//
// C++ Interface: mwpcomparison
//
// Description:
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef MWPCOMPARISON_H
#define MWPCOMPARISON_H

#include "mwplogic.h"


class MWPComparison : public MWPLogic
{

    public:
        MWPComparison() {};

        void setup(const QStringList& elementids, const QList<QLineEdit*> kids, const QMap<QString, QLineEdit*> map);
        void clear();
        void setTexts(const QStringList& texts);
        void setEntry(const PracticeEntry* entry) {Q_UNUSED(entry);} // ignore
        QStringList answer();

    private:
        QStringList m_elementIds;
        QList<QLineEdit*> m_kids;
        QMap<QString, QLineEdit*> m_map;
};

#endif
