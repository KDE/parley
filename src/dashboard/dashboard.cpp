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

#include "dashboard.h"
#include "buttondelegate.h"
#include "parleymainwindow.h"
#include "parleydocument.h"
#include "practice/themedbackgroundrenderer.h"
#include "practice/imagewidget.h"
#include "statistics/statisticsmainwindow.h"
#include "statistics/statisticsmodel.h"
#include <QSignalMapper>
#include <KLocalizedString>

#include <QGraphicsDropShadowEffect>
#include <QStandardItemModel>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QDebug>

#include <QtGui>
#include <Qt>

#include "collectionwidget.h"
#include "barwidget.h"
#include "gradereferencewidget.h"

// ----------------------------------------------------------------

static int randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

// ================================================================
//                         class Dashboard


int ROWSIZE = 4;      // Number of collection widgets (+ 1 initial spacerItem) per row

Dashboard::Dashboard(ParleyMainWindow *parent)
    : KXmlGuiWindow(parent)
    , m_mainWindow(parent)
{
    // KXmlGui
    setXMLFile("dashboardui.rc");
    setObjectName("Dashboard");

    m_widget = new Practice::ImageWidget(this);

    m_widget->setScalingEnabled(false, false);
    m_widget->setKeepAspectRatio(Qt::IgnoreAspectRatio);
    m_widget->setFadingEnabled(false);

    ui = new Ui::Dashboard();
    ui->setupUi(m_widget);
    setCentralWidget(m_widget);
    practiceSignalMapper = new QSignalMapper(this);
    removeSignalMapper = new QSignalMapper(this);

    gradeColor[0] = QColor(25,38,41);
    gradeColor[1] = QColor(25,38,41,64);// Need 8 colors, so find a suitable color for this grade, currently gray.
    gradeColor[2] = QColor(237,21,21);
    gradeColor[3] = QColor(246,116,0);
    gradeColor[4] = QColor(201,206,59);
    gradeColor[5] = QColor(28,220,154);
    gradeColor[6] = QColor(17,209,22);
    gradeColor[7] = QColor(61,174,253);
    gradeColor[8] = QColor(255,221,217);//These two are placeholders for the wordcloud background color.
    gradeColor[9] = QColor(238,232,213);//

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QFont font = ui->recentLabel->font();
    font.setBold(true);
    ui->recentLabel->setFont(font);
    font = ui->completedLabel->font();
    font.setBold(true);
    ui->completedLabel->setFont(font);

    ui->newButton->setIcon(QIcon::fromTheme("document-new"));
    ui->openButton->setIcon(QIcon::fromTheme("document-open"));
    ui->ghnsButton->setIcon(QIcon::fromTheme("get-hot-new-stuff"));
    GradeReferenceWidget *gradeReferenceWidget = new GradeReferenceWidget();
    gradeReferenceWidget->setMinimumSize(m_widget->width(), 50);
    ui->gridLayout->addWidget(gradeReferenceWidget, 2, 0, 1, ROWSIZE, Qt::AlignCenter);

    m_subGridLayout = new QGridLayout();
    m_subGridLayout->setHorizontalSpacing(50);
    m_subGridLayout->setVerticalSpacing(30);
    ui->gridLayout->addLayout(m_subGridLayout, 5, 0, 1, 4);

    m_completedGridLayout = new QGridLayout();
    m_completedGridLayout->setHorizontalSpacing(50);
    m_completedGridLayout->setVerticalSpacing(30);
    ui->gridLayout->addLayout(m_completedGridLayout, 9, 0, 1, 4);

    populateMap();
    populateGrid();

    // Signals from the main buttons.
    ParleyDocument* doc = m_mainWindow->parleyDocument();
    connect(ui->newButton,  SIGNAL(clicked()), m_mainWindow, SLOT(slotFileNew()));
    connect(ui->openButton, SIGNAL(clicked()), doc, SLOT(slotFileOpen()));
    connect(ui->ghnsButton, SIGNAL(clicked()), doc, SLOT(slotGHNS()));

    // Signals FROM the signal mappers.  The ones TO the signal mappers are
    // handled below.
    connect(practiceSignalMapper, SIGNAL(mapped(const QString &)),
       this,                 SLOT(slotPracticeButtonClicked(const QString &)));
    connect(removeSignalMapper,   SIGNAL(mapped(const QString &)),
       this,                 SLOT(slotRemoveButtonClicked(const QString &)));

    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
    applyMainWindowSettings(cfg);

    m_themedBackgroundRenderer = new Practice::ThemedBackgroundRenderer(this, "startpagethemecache.bin");

    connect(Prefs::self(), SIGNAL(configChanged()), this, SLOT(setTheme()));
    setTheme();

    connect(m_themedBackgroundRenderer, SIGNAL(backgroundChanged(QPixmap)), this, SLOT(backgroundChanged(QPixmap)));
    connect(m_widget, SIGNAL(sizeChanged()), this, SLOT(updateBackground()));

    /*statisticsWidget = new StatisticsMainWindow(m_mainWindow->parleyDocument()->document(), m_mainWindow);
    for (int i = 0; i < m_count; i++)
        statisticsHandler(urlArray[i]); */ //Used to get the statistics for each Collection. TODO find a better way.
}

Dashboard::~Dashboard()
{
    KConfigGroup cfg(KSharedConfig::openConfig("parleyrc"), objectName());
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
    KConfig parleyConfig("parleyrc");
    KConfigGroup recentFilesGroup(&parleyConfig, "Recent Files");
    for (int i = recentFilesGroup.keyList().count() / 2; i > 0 ; i--) {
        QString urlString = recentFilesGroup.readPathEntry("File" + QString::number(i), QString());
        QString nameString = recentFilesGroup.readEntry("Name" + QString::number(i), QString());
        recentFilesMap.insert(urlString,nameString);
    }
}

void Dashboard::populateGrid()
{
    int j = 0, k = 0, jc = 0, kc = 0;

    QMapIterator<QString, QString> it(recentFilesMap);
    while (it.hasNext()) {
        it.next();
        QString urlString  = it.key();
        QString nameString = it.value();

   // Automatically initialized.
   // FIXME: Will be initialized by the KEduVocDocument later.
   DueWords due;

        QUrl  url( QUrl::fromLocalFile(urlString) );
        urlArray[k] = url;
        if (due.percentageCompleted != 100) {
            if (j % ROWSIZE == 0) {
                m_subGridLayout->addItem(new QSpacerItem(50,1), j / ROWSIZE, 0);
                j++;
            }
        }
        else {
            if (jc % ROWSIZE == 0) {
                m_completedGridLayout->addItem(new QSpacerItem(50,1), jc / ROWSIZE, 0);
                jc++;
            }
        }

        QWidget* backWidget = new CollectionWidget(nameString, &due);
        if (due.percentageCompleted != 100) {
                backWidget->setFixedSize(COLLWIDTH, COLLHEIGHT1);
                m_subGridLayout->addWidget(backWidget, j / ROWSIZE, j % ROWSIZE);
        }
        else {
                backWidget->setFixedSize(COLLWIDTH, COLLHEIGHT2);
                m_completedGridLayout->addWidget(backWidget, jc / ROWSIZE, jc % ROWSIZE);
        }

        practiceSignalMapper->setMapping(backWidget, urlString);
        connect(backWidget, SIGNAL(practiceButtonClicked()), practiceSignalMapper, SLOT(map()));
        removeSignalMapper->setMapping(backWidget, urlString);
        connect(backWidget, SIGNAL(removeButtonClicked()), removeSignalMapper, SLOT(map()));

        if (due.percentageCompleted != 100) {
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
        ui->recentLabel->setText(i18n("Active Collections"));
    } else {
        ui->recentLabel->clear();
    }
    if (kc) {
        ui->completedLabel->setText(i18n("Completed Collections"));
    } else {
        ui->completedLabel->clear();
    }
}

void Dashboard::statisticsHandler(QUrl url)
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
    qDebug() << urlString;
    QUrl url( QUrl::fromLocalFile(urlString) );
    m_openUrl = url;
    QTimer::singleShot(0, this, SLOT(slotDoubleClickOpen()));
}

void Dashboard::slotRemoveButtonClicked(const QString& urlString)
{
    qDebug() << urlString;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, i18n("Remove"),
              i18n("Are you sure you want to remove this collection?"),
              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        recentFilesMap.remove(urlString);
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
    m_mainWindow->showPracticeConfiguration();
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

void Dashboard::updateFontColors()
{
    QPalette p(QApplication::palette());
    QColor c = m_themedBackgroundRenderer->fontColor("Start", p.color(QPalette::Active, QPalette::WindowText));
    p.setColor(QPalette::Base, Qt::transparent);
    p.setColor(QPalette::Text, c);
    p.setColor(QPalette::WindowText, c);
    m_widget->setPalette(p);
}

void Dashboard::updateBackground()
{
    m_themedBackgroundRenderer->clearRects();
    m_themedBackgroundRenderer->addRect("startbackground", QRect(QPoint(), m_widget->size()));
    QRect headerRect = ui->headingLabel->frameGeometry();
    headerRect.setBottom(ui->recentFiles->frameGeometry().top() - 1);
    m_themedBackgroundRenderer->addRect("startheader", headerRect);
    m_themedBackgroundRenderer->addRect("recentfiles", ui->recentFiles->frameGeometry());
    QPixmap pixmap = m_themedBackgroundRenderer->getScaledBackground();
    if (!pixmap.isNull()) {
        m_widget->setPixmap(pixmap);
    }
    m_themedBackgroundRenderer->updateBackground();
}
