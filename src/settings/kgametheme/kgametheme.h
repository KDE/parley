/*
    SPDX-FileCopyrightText: 2007 Mauricio Piacentini <mauricio@tabuleiro.com>
    SPDX-FileCopyrightText: 2007 Matt Williams <matt@milliams.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KGAMETHEME_H
#define KGAMETHEME_H


#include <QString>

class KGameThemePrivate;
class QPixmap;

/**
 * \class KGameTheme kgametheme.h <KGameTheme>
 *
 * @short Class for loading theme files
 *
 * Essentially just a wrapper around a .desktop theme file. Load a file with
 * load() and then access its properties.
 *
 * For more advanced features like dynamic themes or custom game rules, it
 * will likely be necessary to derive from this class
 *
 * @author Mauricio Piacentini
 **/
class KGameTheme
{
public:
    explicit KGameTheme(const QString &themeGroup = QStringLiteral("KGameTheme"));
    virtual ~KGameTheme();

    /**
     * Load the default theme file. Called "default.desktop"
     * @return true if the theme files and properties could be loaded
     */
    virtual bool loadDefault();
    /**
     * Load a specific theme file.
     * Note that although theme could be successfully loaded,
     * no check on the validity of theme's SVG file contents is done.
     * Application writers will need to perform this check manually
     * e.g. by calling KSvgRenderer::isValid()
     * @param file the name of the theme file relative to the share/apps/appname
     * directory. e.g. "themes/classic.desktop"
     * @return true if the theme files and properties could be loaded
     */
    virtual bool load(const QString &file);
    /// @return the full path of the .desktop file
    QString path() const;
    /// @return just the "*.desktop" part
    QString fileName() const;
    /// @return the full path of the svg file which is specified in "FileName" key
    virtual QString graphics() const;
    /// @return a property directly from the .desktop file
    QString property(const QString &key) const;
    /// @return a preview pixmap
    QPixmap preview() const;
    /**
     * Possible keys:
     * - Name
     * - Author
     * - Description
     * - AuthorEmail
     * @param key the key of the wanted property
     * @return the data related to 'key'
     */
    virtual QString themeProperty(const QString &key) const;

private:
    friend class KGameThemePrivate;
    KGameThemePrivate *const d;
};

#endif
