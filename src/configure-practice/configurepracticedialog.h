/***************************************************************************

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

    -----------------------------------------------------------------------

 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONFIGUREPRACTICEDIALOG_H
#define CONFIGUREPRACTICEDIALOG_H

#include <keduvocdocument.h>
#include <KConfigDialog>

class GeneralPracticeOptions;
class ConfigurePracticeWidget;
class AdvancedPracticeOptions;
class ThresholdOptions;
class BlockOptions;

/**
  *
  */
class ConfigurePracticeDialog : public KConfigDialog
{
    Q_OBJECT

public:
    ConfigurePracticeDialog(KEduVocDocument *doc, QWidget *parent, const QString &name, KConfigSkeleton *config);

    ~ConfigurePracticeDialog();

    void commitData();

private slots:
    /**
     * Called when the user clicks Apply or OK.
     */
    void updateSettings();
    /**
     * Updates dialog widgets. Here only used after loading a profile.
     * Profiles only store the settings of the last three pages in the dialog.
     */
    void updateWidgets();
    /**
     * Called when the user clicks Default
     */
    void updateWidgetsDefault();

    /**
     * Returns true if the current state of the dialog is different from the saved settings
     */
    bool hasChanged();

private:

    /**
     * Returns true if the current state of the dialog represents the default settings.
     */
    bool isDefault();

    GeneralPracticeOptions  *m_generalPracticeOptions;
    BlockOptions            *m_blockOptions;
    ThresholdOptions        *m_thresholdOptions;
    AdvancedPracticeOptions *m_advancedPracticeOptions;

    KConfigSkeleton *m_config;
};

#endif // ConfigurePracticeDialog_included

