/*
    threshold options for Parley
    SPDX-FileCopyrightText: 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef THRESHOLDOPTIONS_H
#define THRESHOLDOPTIONS_H

#include "prefs.h"
#include "ui_thresholdoptions.h"

class KEduVocDocument;

class ThresholdOptions : public QWidget, public Ui::ThresholdOptions
{
    Q_OBJECT
public:
    ThresholdOptions(KEduVocDocument *doc, QWidget *parent);

    void updateWidgets();
    bool hasChanged();
    bool isDefault();
    void updateSettings();

private:
    KEduVocDocument *m_doc{nullptr};
    KConfigSkeleton *m_config{nullptr};
};

#endif
