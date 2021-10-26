/*
    SPDX-FileCopyrightText: 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CONFIGUREPRACTICEDIALOG_H
#define CONFIGUREPRACTICEDIALOG_H

#include <KConfigSkeleton>
#include <KConfigWidgets/KConfigDialog>
#include <KEduVocDocument>

class GeneralPracticeOptions;
class SpecificPracticeOptions;
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

    ~ConfigurePracticeDialog() override;

    void commitData();

private slots:
    /**
     * Called when the user clicks Apply or OK.
     */
    void updateSettings() override;
    /**
     * Updates dialog widgets. Here only used after loading a profile.
     * Profiles only store the settings of the last three pages in the dialog.
     */
    void updateWidgets() override;
    /**
     * Called when the user clicks Default
     */
    void updateWidgetsDefault() override;

    /**
     * Returns true if the current state of the dialog is different from the saved settings
     */
    bool hasChanged() override;

private:
    /**
     * Returns true if the current state of the dialog represents the default settings.
     */
    bool isDefault() override;

    GeneralPracticeOptions *m_generalPracticeOptions;
    BlockOptions *m_blockOptions;
    ThresholdOptions *m_thresholdOptions;
    SpecificPracticeOptions *m_specificPracticeOptions;

    KConfigSkeleton *m_config;
};

#endif // ConfigurePracticeDialog_included
