/*
    SPDX-FileCopyrightText: 2014 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "dashboard.h"

#include <QTimer>
#include <QAction>
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>

//#include <KMimeType>
#include <KActionCollection>

#include "../utils.h"
#include "buttondelegate.h"
#include "parleymainwindow.h"
#include "parleydocument.h"
#include "practice/themedbackgroundrenderer.h"
#include "practice/imagewidget.h"
#include "statistics/statisticsmodel.h"

#include "collection.h"
#include "collectionwidget.h"
#include "gradereferencewidget.h"


// ================================================================
//                         class Dashboard


int ROWSIZE = 4;      // Number of collection widgets (+ 1 initial spacerItem) per row

Dashboard::Dashboard(ParleyMainWindow *parent)
    : KXmlGuiWindow(parent)
    , m_mainWindow(parent)
{
    // KXmlGui
    setXMLFile(QStringLiteral("dashboardui.rc"));
    setObjectName(QStringLiteral("Dashboard"));

    m_widget = new Practice::ImageWidget(this);

    m_widget->setScalingEnabled(false, false);
    m_widget->setKeepAspectRatio(Qt::IgnoreAspectRatio);
    m_widget->setFadingEnabled(false);

    m_ui.setupUi(m_widget);
    setCentralWidget(m_widget);

    QFont font = m_ui.recentLabel->font();
    font.setBold(true);
    m_ui.recentLabel->setFont(font);
    font = m_ui.completedLabel->font();
    font.setBold(true);
    m_ui.completedLabel->setFont(font);

    m_ui.newButton->setIcon(QIcon::fromTheme(QStringLiteral("document-new")));
    m_ui.openButton->setIcon(QIcon::fromTheme(QStringLiteral("document-open")));
    m_ui.ghnsButton->setIcon(QIcon::fromTheme(QStringLiteral("get-hot-new-stuff")));

    GradeReferenceWidget *gradeReferenceWidget = new GradeReferenceWidget();
    gradeReferenceWidget->setMinimumSize(m_widget->width(), 50);
    m_ui.gridLayout->addWidget(gradeReferenceWidget, 1, 0, 1, ROWSIZE, Qt::AlignCenter);

    m_subGridLayout = new QGridLayout();
    m_subGridLayout->setHorizontalSpacing(50);
    m_subGridLayout->setVerticalSpacing(30);
    m_ui.gridLayout_2->addLayout(m_subGridLayout, 2, 0, 1, 1);


    m_completedGridLayout = new QGridLayout();
    m_completedGridLayout->setHorizontalSpacing(50);
    m_completedGridLayout->setVerticalSpacing(30);
    m_ui.gridLayout_2->addLayout(m_completedGridLayout, 6, 0, 1, 1);

    populateMap();
    populateGrid();

    // Signals from the main buttons.
    ParleyDocument* doc = m_mainWindow->parleyDocument();
    connect(m_ui.newButton,  &QAbstractButton::clicked, m_mainWindow, &ParleyMainWindow::slotFileNew);
    connect(m_ui.openButton, &QAbstractButton::clicked, doc, &ParleyDocument::slotFileOpen);
    connect(m_ui.ghnsButton, &QAbstractButton::clicked, doc, &ParleyDocument::slotGHNS);

    KConfigGroup cfg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), objectName());
    applyMainWindowSettings(cfg);

    m_themedBackgroundRenderer = new Practice::ThemedBackgroundRenderer(this, QStringLiteral("startpagethemecache.bin"));

    // Set theme and prepare for future theme changes.
    connect(Prefs::self(), &KCoreConfigSkeleton::configChanged, this, &Dashboard::setTheme);
    setTheme();

    connect(m_themedBackgroundRenderer, &Practice::ThemedBackgroundRenderer::backgroundChanged,
            this,                       &Dashboard::backgroundChanged);
    connect(m_widget, &Practice::ImageWidget::sizeChanged, this, &Dashboard::updateBackground);

    QAction *updateAction = new QAction(this);
    updateAction->connect(updateAction, &QAction::triggered, this, &Dashboard::updateWidgets);
    actionCollection()->addAction(QStringLiteral("update_dashboard"), updateAction);
    actionCollection()->setDefaultShortcut(updateAction, QKeySequence(Qt::Key_F5));
}

Dashboard::~Dashboard()
{
    KConfigGroup cfg(KSharedConfig::openConfig(QStringLiteral("parleyrc")), objectName());
    saveMainWindowSettings(cfg);
}


void Dashboard::clearGrid()
{
   remove(m_subGridLayout,
          m_subGridLayout->rowCount() - 1, m_subGridLayout->columnCount() - 1,
          true);
   remove(m_completedGridLayout,
          m_completedGridLayout->rowCount() - 1, m_completedGridLayout->columnCount() - 1,
          true);
}

/**
 * Helper function. Removes all layout items within the given @a layout
 * which either span the given @a row or @a column. If @a deleteWidgets
 * is true, all concerned child widgets become not only removed from the
 * layout, but also deleted.
 */
void Dashboard::remove(QGridLayout *layout, int row, int column, bool deleteWidgets)
{
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r + rs - 1 <= row) || (c + cs - 1 <= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets) {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}

/**
 * Helper function. Deletes all child widgets of the given layout @a item.
 */
void Dashboard::deleteChildWidgets(QLayoutItem *item)
{
    if (item->layout()) {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}

void Dashboard::populateMap()
{
    KConfig parleyConfig(QStringLiteral("parleyrc"));
    KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");
    for (int i = recentFilesGroup.keyList().count() / 2; i > 0 ; i--) {
        QString urlString = recentFilesGroup.readPathEntry("File" + QString::number(i), QString());
        QString nameString = recentFilesGroup.readEntry("Name" + QString::number(i), QString());
        m_recentFilesMap.insert(urlString, nameString);
    }
}

void Dashboard::populateGrid()
{
    int j = 0, k = 0, jc = 0, kc = 0;

    QMapIterator<QString, QString> it(m_recentFilesMap);
    while (it.hasNext()) {
        it.next();
        QString urlString   = it.key();
        QString titleString = it.value();

        QUrl  url(QUrl::fromLocalFile(urlString));
        Collection *collection = new Collection(&url, this);

        WordCount dueWords;
	int percentageCompleted = dueWords.percentageCompleted();

        m_urlArray[k] = url;
        if (percentageCompleted != 100) {
            if (j % ROWSIZE == 0) {
                m_subGridLayout->addItem(new QSpacerItem(50, 1), j / ROWSIZE, 0);
                j++;
            }
        }
        else {
            if (jc % ROWSIZE == 0) {
                m_completedGridLayout->addItem(new QSpacerItem(50,1), jc / ROWSIZE, 0);
                jc++;
            }
        }

        CollectionWidget* backWidget = new CollectionWidget(collection, &dueWords, this);
        m_collectionWidgets.append(backWidget);
        if (percentageCompleted != 100) {
                backWidget->setFixedSize(COLLWIDTH, COLLHEIGHT1);
                backWidget->setMinimumSize(COLLWIDTH, COLLHEIGHT1);
                m_subGridLayout->addWidget(backWidget, j / ROWSIZE, j % ROWSIZE);
        }
        else {
                backWidget->setFixedSize(COLLWIDTH, COLLHEIGHT2);
                backWidget->setMinimumSize(COLLWIDTH, COLLHEIGHT2);
                m_completedGridLayout->addWidget(backWidget, jc / ROWSIZE, jc % ROWSIZE);
        }

        connect(backWidget, &CollectionWidget::practiceButtonClicked,
                this, [=] {slotPracticeButtonClicked(urlString);});
        connect(backWidget, &CollectionWidget::removeButtonClicked,
                this, [=] {slotRemoveButtonClicked(urlString);});

        if (percentageCompleted != 100) {
            j++;
        }
        else {
            jc++;
            kc++;
        }

        k++;
    }

    m_count = k;
    m_completedGridLayout->addItem(new QSpacerItem(50, 1,
                                                   QSizePolicy::Expanding, QSizePolicy::Fixed),
                                   m_completedGridLayout->rowCount() - 1,
                                   m_completedGridLayout->columnCount());
    m_subGridLayout->addItem(new QSpacerItem(50,1,QSizePolicy::Expanding, QSizePolicy::Fixed),
                             m_subGridLayout->rowCount() - 1, m_subGridLayout->columnCount());
    if (k - kc) {
        m_ui.recentLabel->setText(i18n("Active Collections"));
    } else {
        m_ui.recentLabel->clear();
    }
    if (kc) {
        m_ui.completedLabel->setText(i18n("Completed Collections"));
    } else {
        m_ui.completedLabel->clear();
    }
}

void Dashboard::statisticsHandler(const QUrl &url)
{
#if 1
    Q_UNUSED(url);
#else
    if (!m_mainWindow->parleyDocument()->open(url)) {
        return;
    }

    // Find due words. TODO find a better way.
    m_mainWindow->m_sessionManager.setDocument(m_mainWindow->parleyDocument()->document());

    qDebug()<<"Session Manager, allEntryCount="<<m_mainWindow->m_sessionManager.allDueEntryCount();
    statisticsWidget->setDocument(m_mainWindow->parleyDocument()->document());

    // Find the percentage completion, to categorize as active or completed collection.
    QModelIndex index = statisticsWidget->statisticsModel()->index(0, 2, QModelIndex());
    qDebug() << "Percentage:" << index.data(StatisticsModel::TotalPercent).toInt();
#endif
}

void Dashboard::slotOpenUrl(const QUrl& url)
{
    if (!m_mainWindow->parleyDocument()->open(url)) {
        return;
    }
    m_mainWindow->showEditor();
}

void Dashboard::slotPracticeButtonClicked(const QString& urlString)
{
    //qDebug() << urlString;
    QUrl url( QUrl::fromLocalFile(urlString) );
    m_openUrl = url;
    QTimer::singleShot(0, this, &Dashboard::slotDoubleClickOpen);
}

void Dashboard::slotRemoveButtonClicked(const QString& urlString)
{
    qDebug() << urlString;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, i18n("Remove"),
                                  i18n("Are you sure you want to remove this collection?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_recentFilesMap.remove(urlString);
        m_mainWindow->removeRecentFile(QUrl::fromLocalFile(urlString ));
        clearGrid();
        populateGrid();
    }
}

void Dashboard::slotDoubleClickOpen()
{
    slotPracticeUrl(m_openUrl);
}

void Dashboard::slotPracticeUrl(const QUrl & url)
{
    if (!m_mainWindow->parleyDocument()->open(url)) {
        return;
    }

    // This used to go to the practice configuration but both I and
    //some users wanted to go directly to the practice so I'm testing
    //out this for a while.
    m_mainWindow->showPracticeConfiguration();
    //m_mainWindow->showPractice();
}

void Dashboard::backgroundChanged(const QPixmap &pixmap)
{
    m_widget->setPixmap(pixmap);
}

void Dashboard::setTheme()
{
    m_themedBackgroundRenderer->setTheme(Prefs::theme());
    updateFontColors();
    updateBackground();
    m_widget->setContentsMargins(m_themedBackgroundRenderer->contentMargins());
}

void Dashboard::updateWidgets()
{
    foreach (CollectionWidget *cw, m_collectionWidgets) {
        cw->updateDue();
    }
}
void Dashboard::updateFontColors()
{
    QPalette p(QApplication::palette());
    QColor c = m_themedBackgroundRenderer->fontColor(QStringLiteral("Start"), p.color(QPalette::Active, QPalette::WindowText));
    p.setColor(QPalette::Base, Qt::transparent);
    p.setColor(QPalette::Text, c);
    p.setColor(QPalette::WindowText, c);
    m_widget->setPalette(p);
}

void Dashboard::updateBackground()
{
    m_themedBackgroundRenderer->clearRects();
    m_themedBackgroundRenderer->addRect(QStringLiteral("startbackground"), QRect(QPoint(), m_widget->size()));
    QPixmap pixmap = m_themedBackgroundRenderer->getScaledBackground();
    if (!pixmap.isNull()) {
        m_widget->setPixmap(pixmap);
    }
    m_themedBackgroundRenderer->updateBackground();
}
