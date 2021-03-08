/*
    SPDX-FileCopyrightText: 2008 Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "script.h"

#include "editor/editor.h"

#include <QStringList>


/**
 * This class finds the scripts installed in the application directory and manages loading and unloading of the scripts. For each script an instance of Script class is created.
 *
 * @author Avgoustinos Kadis <avgoustinos.kadis@kdemail.net>
*/
class ScriptManager : public QObject
{
public:
    explicit ScriptManager(Editor::EditorWindow * editor);

    ~ScriptManager();

    /**
     * Finds all the available desktop files in {PARLEY_DATA_FOLDER}/plugins
     *
     * @return The list of desktop filenames available for parley
     */
    static QStringList getDesktopFiles();

    /**
     * Returns a QMap (from from categories codenames to categories display label)
     * to be used in KPluginSelector (ScriptDialog) for displaying the various
     * categories
     *
     * @note this function is not used later on (categories are disabled)
     *
     * @return the QMap described above
     */
    static QMap<QString, QString> categories();

    /**
     * Parses the desktop @p desktopFile given and returns the value of "Script" entry.
     *
     * @param desktopFile The .desktop file that will get the value from
     * @return The value of "Script" entry. Empty string of no "Script" entry is found
     */
    static QString getScriptEntry(const QString &desktopFile);

    /**
     * Returns the full path to the script name given in the @p desktopFile.
     *
     * @param desktopFile The desktop file for the parley plugin
     * @return The full-path to the script
     */
    QString getScriptFileName(const QString &desktopFile);

    /**
     * Returns a list of filenames (full path) of enabled scripts
     */
    QStringList enabledScripts();

    /**
     * Modify the parleyrc configuration so it disables the @p dektopFile plugin.
     * This function is to be used when the plugin is invalid (wrong script name,
     * incorrect desktop file etc)
     *
     * @param desktopFile
     */
    void disablePlugin(const QString &desktopFile);

    /**
     * Loads (activates) all the available scripts and notifies the user if any
     * script was not activated (due to errors in the script)
     */
    void loadScripts();

    /**
     * Adds a QObject as a module for the script
     * @param obj The QObject to be added to the script
     * @param name The name of the object as it will appear in the script
     */
    void addObject(QObject * obj, const QString & name);

    /**
     * Reloads all the scripts
     */
    void reloadScripts();

    /**
     * Add a QAction to the Scripts menu
     * @param name The action name
     * @param action QAction to be added
     */
    void addScriptAction(const QString & name, QAction * action);

    /** returns the Translator object the Scripting::Parley */
    Translator * translator() {
        return m_scriptingParley->translator();
    }

private:
    Editor::EditorWindow * m_editor;
    QList<Script*> m_scripts;
    QMap<QString, QObject*> m_scriptObjects;
    QList<QAction*> m_scriptActions;

    ///script objects (objects that will be used from inside the scripts)
    Scripting::Parley* m_scriptingParley;
};

#endif
