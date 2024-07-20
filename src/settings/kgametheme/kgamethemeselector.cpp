/*
    SPDX-FileCopyrightText: 2007 Mauricio Piacentini <mauricio@tabuleiro.com>
    SPDX-FileCopyrightText: 2007 Matt Williams <matt@milliams.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kgamethemeselector.h"

#include <KConfigSkeleton>
#include <KLocalizedString>
#include <QDirIterator>

#include "kgametheme.h"
#include "ui_kgamethemeselector.h"

class KGameThemeSelectorPrivate
{
    KGameThemeSelector *q_ptr;
    Q_DECLARE_PUBLIC(KGameThemeSelector)

public:
    KGameThemeSelectorPrivate(KGameThemeSelector *parent)
        : q_ptr(parent)
    {
    }
    ~KGameThemeSelectorPrivate()
    {
        qDeleteAll(themeMap);
    }

    QMap<QString, KGameTheme *> themeMap;
    Ui::KGameThemeSelectorBase ui;
    QString lookupDirectory;
    QString groupName;

    void setupData(KConfigSkeleton *config, KGameThemeSelector::NewStuffState knsflags);
    void findThemes(const QString &initialSelection);

    // private slots
    void _k_updatePreview();
    void _k_updateThemeList(const QString &strTheme);
};

KGameThemeSelector::KGameThemeSelector(QWidget *parent,
                                       KConfigSkeleton *aconfig,
                                       KGameThemeSelector::NewStuffState knsflags,
                                       const QString &groupName,
                                       const QString &directory)
    : QWidget(parent)
    , d_ptr(new KGameThemeSelectorPrivate(this))
{
    d_func()->lookupDirectory = directory;
    d_func()->groupName = groupName;
    d_func()->setupData(aconfig, knsflags);
}

KGameThemeSelector::~KGameThemeSelector()
{
    delete d_ptr;
}

void KGameThemeSelectorPrivate::setupData(KConfigSkeleton *aconfig, KGameThemeSelector::NewStuffState knsflags)
{
    ui.setupUi(q_ptr);
    ui.getNewButton->setConfigFile(QStringLiteral("parley-themes.knsrc"));

    // The lineEdit widget holds our theme path for automatic connection via KConfigXT.
    // But the user should not manipulate it directly, so we hide it.
    ui.kcfg_Theme->hide();
    q_func()->connect(ui.kcfg_Theme, SIGNAL(textChanged(QString)), q_ptr, SLOT(_k_updateThemeList(QString)));

    // Disable KNS button?
    if (knsflags == KGameThemeSelector::NewStuffDisableDownload) {
        ui.getNewButton->hide();
    }

    // Get the last used theme path from the KConfigSkeleton
    KConfigSkeletonItem *configItem = aconfig->findItem(QStringLiteral("Theme"));
    QString lastUsedTheme = configItem->property().toString();

    // Now get our themes into the list widget
    findThemes(lastUsedTheme);

    q_func()->connect(ui.getNewButton, &KNSWidgets::Button::dialogFinished, q_ptr, [&](const QList<KNSCore::Entry> &changedEntries) {
        if (!changedEntries.isEmpty()) {
            findThemes(ui.kcfg_Theme->text());
        }
    });
}

void KGameThemeSelectorPrivate::findThemes(const QString &initialSelection)
{
    qDeleteAll(themeMap);
    themeMap.clear();

    // Disconnect the themeList as we are going to clear it and do not want previews generated
    ui.themeList->disconnect();
    ui.themeList->clear();
    ui.themeList->setSortingEnabled(true);

    QStringList themesAvailable;
    QStringList themePaths = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation,
                                                       QCoreApplication::applicationName() + '/' + lookupDirectory + '/',
                                                       QStandardPaths::LocateDirectory);
    for (const QString &dir : std::as_const(themePaths)) {
        QDirIterator it(dir, QStringList() << QStringLiteral("*.desktop"), QDir::NoFilter, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            themesAvailable.append(it.fileName());
        }
    }

    bool initialFound = false;
    for (const QString &file : std::as_const(themesAvailable)) {
        QString themePath = lookupDirectory + '/' + file;
        KGameTheme *atheme = new KGameTheme(groupName);

        if (atheme->load(themePath)) {
            QString themeName = atheme->themeProperty(QStringLiteral("Name"));
            // Add underscores to avoid duplicate names.
            while (themeMap.contains(themeName))
                themeName += '_';
            themeMap.insert(themeName, atheme);
            QListWidgetItem *item = new QListWidgetItem(themeName, ui.themeList);

            // Find if this is our currently configured theme
            if (themePath == initialSelection) {
                initialFound = true;
                ui.themeList->setCurrentItem(item);
                _k_updatePreview();
            }
        } else {
            delete atheme;
        }
    }

    if (!initialFound) {
        // TODO change this if we ever change KGameTheme::loadDefault
        QString defaultPath = QStringLiteral("themes/default.desktop");
        for (KGameTheme *theme : std::as_const(themeMap)) {
            if (theme->path().endsWith(defaultPath)) {
                const QList<QListWidgetItem *> itemList = ui.themeList->findItems(theme->themeProperty(QStringLiteral("Name")), Qt::MatchExactly);
                // never can be != 1 but better safe than sorry
                if (itemList.count() == 1) {
                    ui.themeList->setCurrentItem(itemList.first());
                    _k_updatePreview();
                }
            }
        }
    }

    // Reconnect the themeList
    q_func()->connect(ui.themeList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), q_ptr, SLOT(_k_updatePreview()));
}

void KGameThemeSelectorPrivate::_k_updatePreview()
{
    KGameTheme *seltheme = themeMap.value(ui.themeList->currentItem()->text());
    // Sanity checkings. Should not happen.
    if (!seltheme)
        return;
    if (seltheme->path() == ui.kcfg_Theme->text()) {
        return;
    }
    ui.kcfg_Theme->setText(seltheme->fileName());

    QString authstr(QStringLiteral("Author"));
    QString contactstr(QStringLiteral("AuthorEmail"));
    QString descstr(QStringLiteral("Description"));
    QString emailstr;
    if (!seltheme->themeProperty(contactstr).isEmpty()) {
        emailstr = QStringLiteral("<a href=\"mailto:%1\">%1</a>").arg(seltheme->themeProperty(contactstr));
    }

    ui.themeAuthor->setText(seltheme->themeProperty(authstr));
    ui.themeContact->setText(emailstr);
    ui.themeDescription->setText(seltheme->themeProperty(descstr));

    // Draw the preview
    QPixmap pix(seltheme->preview());
    ui.themePreview->setPixmap(pix.scaled(ui.themePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void KGameThemeSelectorPrivate::_k_updateThemeList(const QString &strTheme)
{
    // find theme and set selection to the current theme; happens when pressing "Default"
    QListWidgetItem *currentItem = ui.themeList->currentItem();

    QString currentGameThemeFileName;
    if (currentItem) {
        KGameTheme *currentGameTheme = themeMap.value(currentItem->text());
        if (currentGameTheme) {
            currentGameThemeFileName = currentGameTheme->fileName();
        }
    }

    if (!currentItem || (currentGameThemeFileName != strTheme)) {
        for (int i = 0; i < ui.themeList->count(); i++) {
            KGameTheme *listItemGameTheme = themeMap.value(ui.themeList->item(i)->text());
            if (listItemGameTheme && (listItemGameTheme->fileName() == strTheme)) {
                ui.themeList->setCurrentItem(ui.themeList->item(i));
                break;
            }
        }
    }
}

#include "moc_kgamethemeselector.cpp"
