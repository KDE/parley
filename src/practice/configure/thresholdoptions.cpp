/*
    threshold options for Parley
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "thresholdoptions.h"

#include <KEduVocDocument>
#include <KEduVocWordtype>

ThresholdOptions::ThresholdOptions(KEduVocDocument *doc, QWidget *parent)
    : QWidget(parent)
    , m_doc{doc}
{
    setupUi(this);
}

void ThresholdOptions::updateWidgets()
{
}

bool ThresholdOptions::isDefault()
{
    return true;
    ///@todo
}

void ThresholdOptions::updateSettings()
{
}

bool ThresholdOptions::hasChanged()
{
    return false;
}

#include "moc_thresholdoptions.cpp"
