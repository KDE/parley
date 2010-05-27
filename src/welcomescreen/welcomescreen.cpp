/***************************************************************************
    Copyright 2008-2010 Daniel Laidig <d.laidig@gmx.de>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "welcomescreen.h"
#include "buttondelegate.h"
#include "parleymainwindow.h"
#include "parleydocument.h"
#include "practice/themedbackgroundrenderer.h"
#include "practice/imagewidget.h"

#include <KMimeType>

#include <QStandardItemModel>
#include <QTimer>

WelcomeScreen::WelcomeScreen(ParleyMainWindow *parent)
    :KXmlGuiWindow(parent)
    ,m_parleyApp(parent)
{
    // KXmlGui
    setXMLFile("welcomescreenui.rc");
    setObjectName("WelcomeScreen");
    
    m_widget = new Practice::ImageWidget(this);
    m_widget->setScalingEnabled(false, false);
    m_widget->setKeepAspectRatio(Qt::IgnoreAspectRatio);
    m_widget->setFadingEnabled(false);

    ui = new Ui::WelcomeScreen();
    ui->setupUi(m_widget);
    setCentralWidget(m_widget);

    QColor fgColor = palette().text().color();

    QString css = QString::fromUtf8(
       "#recentLabel {"
        "   font-weight: bold;"
        "   border-bottom: 1px solid %1;"
        "}"

        "QListView {"
        "   background-color: transparent;"
        "}"
        )
        .arg(fgColor.name());
    setStyleSheet(css);

    ui->newButton->setIcon(KIcon("document-new"));
    ui->openButton->setIcon(KIcon("document-open"));
    ui->ghnsButton->setIcon(KIcon("get-hot-new-stuff"));

    m_recentFilesModel = new QStandardItemModel(this);
    updateRecentFilesModel();
    ui->recentFiles->setModel(m_recentFilesModel);
    ui->recentFiles->setSelectionMode(QAbstractItemView::NoSelection);

    ButtonDelegate* delegate = new ButtonDelegate(ui->recentFiles, this);
    ui->recentFiles->setItemDelegate(delegate);

    ParleyDocument* doc = m_parleyApp->parleyDocument();
    connect(ui->newButton, SIGNAL(clicked()), doc, SLOT(slotFileNew()));
    connect(ui->openButton, SIGNAL(clicked()), doc, SLOT(slotFileOpen()));
    connect(ui->ghnsButton, SIGNAL(clicked()), doc, SLOT(slotGHNS()));
    if (KGlobalSettings::singleClick()) {
        connect(ui->recentFiles, SIGNAL(clicked(const QModelIndex&)), this, SLOT(slotDoubleClicked(const QModelIndex&)));
    } else {
        connect(ui->recentFiles, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(slotDoubleClicked(const QModelIndex&)));
    }

    connect(m_parleyApp, SIGNAL(recentFilesChanged()), this, SLOT(updateRecentFilesModel()));
    
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);

    m_themedBackgroundRenderer = new Practice::ThemedBackgroundRenderer(this, "startpagethemecache.bin");

    connect(Prefs::self(), SIGNAL(configChanged()), this, SLOT(setTheme()));
    setTheme();

    connect(m_themedBackgroundRenderer, SIGNAL(backgroundChanged(QPixmap)), this, SLOT(backgroundChanged(QPixmap)));
    connect(m_widget, SIGNAL(sizeChanged()), this, SLOT(updateBackground()));
}

WelcomeScreen::~WelcomeScreen()
{
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    saveMainWindowSettings(cfg);
}

void WelcomeScreen::updateRecentFilesModel()
{
    KConfig parleyConfig("parleyrc");
    KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");

    m_recentFilesModel->clear();
    for (int i = recentFilesGroup.keyList().count() / 2; i > 0 ; i--) {
        QString urlString = recentFilesGroup.readPathEntry("File"+QString::number(i), QString());
        QString nameString = recentFilesGroup.readEntry("Name"+QString::number(i), QString());

        KUrl url(urlString);

        QStandardItem* item = new QStandardItem;
        item->setEditable(false);
        item->setText(nameString+" ("+url.fileName()+')');
        item->setToolTip(nameString+" ("+url.pathOrUrl()+')');
        item->setData(QVariant(url), Qt::UserRole);

        item->setIcon(KIcon("practice-start"));

        m_recentFilesModel->appendRow(item);
    }
}

void WelcomeScreen::slotOpenUrl(const KUrl& url)
{
    m_parleyApp->parleyDocument()->open(url);
    m_parleyApp->showEditor();
}

void WelcomeScreen::slotDoubleClicked(const QModelIndex& index)
{
    KUrl url = index.data(Qt::UserRole).toUrl();
    m_openUrl = url;
    QTimer::singleShot(0, this, SLOT(slotDoubleClickOpen()));
}

void WelcomeScreen::slotDoubleClickOpen()
{
    slotPracticeUrl(m_openUrl);
}

void WelcomeScreen::slotPracticeUrl(const KUrl & url)
{
    m_parleyApp->parleyDocument()->open(url);
    m_parleyApp->showStatistics();
}

void WelcomeScreen::backgroundChanged(const QPixmap &pixmap)
{
    m_widget->setPixmap(pixmap);
}

void WelcomeScreen::updateBackground()
{
    m_themedBackgroundRenderer->clearRects();
    m_themedBackgroundRenderer->addRect("startbackground", QRect(QPoint(), m_widget->size()));
    m_themedBackgroundRenderer->addRect("recentfiles", ui->recentFiles->frameGeometry());
    QPixmap pixmap = m_themedBackgroundRenderer->getScaledBackground();
    if (!pixmap.isNull()) {
        m_widget->setPixmap(pixmap);
    }
    m_themedBackgroundRenderer->updateBackground();
}

void WelcomeScreen::setTheme()
{
    m_themedBackgroundRenderer->setTheme(Prefs::theme());
    updateBackground();
    m_widget->setContentsMargins(m_themedBackgroundRenderer->contentMargins());
}

#include "welcomescreen.moc"
