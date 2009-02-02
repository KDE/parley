/***************************************************************************

    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "parley_plasma.h"

#include <KDialog>
#include <KConfigGroup>
#include <KConfigDialog>
#include <KFontDialog>
#include <KColorDialog>
#include <KDebug>

ParleyPlasma::ParleyPlasma(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_label1(0),
    m_label2(0),
    m_font(QFont())
{
    KGlobal::locale()->insertCatalog("parley");
    resize(256,160);
    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setAspectRatioMode(Plasma::KeepAspectRatio);
    setBackgroundHints(NoBackground);

    m_theme = new Plasma::Svg(this);
    m_theme->setImagePath("widgets/parley_plasma_card");
    m_theme->setContainsMultipleImages(false);
    m_theme->resize(size());
}

void ParleyPlasma::init()
{
    m_theme->size().height();

    KConfigGroup cg = config();
    m_updateInterval = cg.readEntry("updateInterval", 10000);
    m_solutionType = cg.readEntry("Solution", 0);

    m_lang1 = cg.readEntry("Top Language", 0);
    m_lang2 = cg.readEntry("Bottom Language", 1);

    m_engine = dataEngine("parley");

    m_label1 = new QGraphicsTextItem(this);
    m_label2 = new QGraphicsTextItem(this);
    m_label1->setPos( m_theme->elementRect( "translation1" ).topLeft() );
    m_label2->setPos( m_theme->elementRect( "translation2" ).topLeft() );
    m_label1->setFont(cg.readEntry("font",m_font));
    m_label2->setFont(cg.readEntry("font",m_font));

    m_sourceFile.setUrl(cg.readEntry("File Name"));
    if (m_sourceFile.isEmpty()) {
        kDebug() << "open file from parleyrc";
        KConfig parleyConfig("parleyrc");
        kDebug() << parleyConfig.groupList();
        KConfigGroup recentFilesGroup( &parleyConfig, "Recent Files" );
        // take the last file, but there are File1..n and Name1..n entries..
        m_sourceFile = recentFilesGroup.readEntry( recentFilesGroup.keyList().value(recentFilesGroup.keyList().count()/2-1), QString() );
        kDebug() << "open file: " << m_sourceFile;
    }
    m_engine->connectSource(m_sourceFile.url(), this, m_updateInterval);
}

void ParleyPlasma::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & Plasma::FormFactorConstraint) {
        setBackgroundHints(NoBackground);
    }

    if (constraints & Plasma::SizeConstraint) {
        double aspect = 256.0/160.0; // original aspect ratio
        if (formFactor() == Plasma::Horizontal) {
            // We have a fixed height, set some sensible width
            setMinimumWidth(contentsRect().height() * aspect);
        } else if (formFactor() == Plasma::Vertical) {
            // We have a fixed width, set some sensible height
            setMinimumHeight((int)contentsRect().width() / aspect);
        }

        m_label1->setPos( m_theme->elementRect( "translation1" ).topLeft() );
        m_label1->setFont( m_font );
        double scale = qMin(m_theme->elementRect( "translation1" ).width()/m_label1->boundingRect().width(), m_theme->elementRect( "translation1" ).height()/m_label1->boundingRect().height());
        m_label1->setTransform(QTransform().scale(scale, scale));

        m_label2->setPos( m_theme->elementRect( "translation2" ).topLeft() );
        m_label2->setFont( m_font );
        scale = qMin(m_theme->elementRect( "translation2" ).width()/m_label2->boundingRect().width(), m_theme->elementRect( "translation2" ).height()/m_label2->boundingRect().height());
        m_label2->setTransform(QTransform().scale(scale, scale));
    }
}

ParleyPlasma::~ParleyPlasma()
{
}

void ParleyPlasma::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    kDebug() << "data updated" << source << data;

    m_languages = data.keys();

    if ( m_label1) {
        m_label1->setPlainText(data[m_languages.value(m_lang1)].toString());
        double scale = qMin(m_theme->elementRect( "translation1" ).width()/m_label1->boundingRect().width(), m_theme->elementRect( "translation1" ).height()/m_label1->boundingRect().height());
        m_label1->setTransform(QTransform().scale(scale, scale));
        m_label1->setPos(m_theme->elementRect( "translation1" ).topLeft()
                + QPointF(
                    (m_theme->elementRect("translation1").width()-m_label1->boundingRect().width()*scale)/2.0,
                    (m_theme->elementRect("translation1").height()-m_label1->boundingRect().height()*scale)/2.0));

        if (m_languages.size() > 1) {
        m_label2->setPlainText(data[m_languages.value(m_lang2)].toString());
        scale = qMin(m_theme->elementRect( "translation2" ).width()/m_label2->boundingRect().width(), m_theme->elementRect( "translation2" ).height()/m_label2->boundingRect().height());
        m_label2->setTransform(QTransform().scale(scale, scale));
        if (m_solutionType == Hover) {
            m_label2->hide();
        }
        m_label2->setPos(m_theme->elementRect( "translation2" ).topLeft()
                + QPointF(
                            (m_theme->elementRect("translation2").width()-m_label2->boundingRect().width()*scale)/2.0,
                            (m_theme->elementRect("translation2").height()-m_label2->boundingRect().height()*scale)/2.0));
        }
    }
}

void ParleyPlasma::setContentSize(const QSizeF& size)
{
    m_size = size;
}

QSizeF ParleyPlasma::contentSizeHint() const
{
    return m_size;
}

void ParleyPlasma::paintInterface(QPainter *p,
                           const QStyleOptionGraphicsItem *option,
                           const QRect &contentsRect)
{
    Q_UNUSED(option);

    QSizeF boundSize = geometry().size();
    m_theme->resize(boundSize);
    m_theme->paint(p, contentsRect, "Card");
}

void ParleyPlasma::createConfigurationInterface(KConfigDialog * parent)
{
    QWidget *widget = new QWidget();
    ui.setupUi(widget);
    parent->addPage(widget, parent->windowTitle(), icon());
    parent->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply );
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

    connect( ui.fontSelectButton, SIGNAL(clicked()), this, SLOT(showFontSelectDlg()) );
    connect( ui.updateIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSpinBoxSuffix()) );

    ui.updateIntervalSpinBox->setValue(m_updateInterval/1000);
    emit updateSpinBoxSuffix();
    KConfigGroup cg = config();

    ui.filechooser->setUrl(m_sourceFile);
    ui.filechooser->setFilter(i18n("*.kvtml|Vocabulary Collections"));
    connect(ui.filechooser, SIGNAL(urlSelected (const KUrl &)), this, SLOT(urlSelected (const KUrl &)));
    ui.language1->addItems(m_languages);
    ui.language2->addItems(m_languages);
    ui.language1->setCurrentIndex(m_lang1);
    ui.language2->setCurrentIndex(m_lang2);

    switch (m_solutionType) {
    case Hover:
        ui.solutionOnHover->setChecked(true);
        break;
    case Always:
        ui.solutionAlways->setChecked(true);
        break;
    }
}

void ParleyPlasma::updateSpinBoxSuffix()
{
    ui.updateIntervalSpinBox->setSuffix(QString(" ") + i18np("second", "seconds", ui.updateIntervalSpinBox->value()));
}

void ParleyPlasma::urlSelected (const KUrl &file)
{
    // turn off old engine
    m_engine->disconnectSource(m_sourceFile.url(), this);
    // connect to new file
    m_sourceFile = ui.filechooser->url();
    m_engine->connectSource(m_sourceFile.url(), this, m_updateInterval);
    // get language data
    Plasma::DataEngine::Data data = m_engine->query(m_sourceFile.url());
    m_languages = data.keys();
    // update language selection
    ui.language1->clear();
    ui.language2->clear();
    ui.language1->addItems(m_languages);
    ui.language2->addItems(m_languages);
    // just select the first languages - better than nothing ;)
    ui.language1->setCurrentIndex(0);
    ui.language2->setCurrentIndex(1);
}

void ParleyPlasma::showFontSelectDlg()
{
    KFontDialog::getFont(m_font);
}

void ParleyPlasma::configAccepted()
{
    prepareGeometryChange();

    KConfigGroup cg = config();
    cg.writeEntry("font", m_font);
    m_label1->setFont(m_font);
    m_label2->setFont(m_font);
    m_updateInterval = ui.updateIntervalSpinBox->value()*1000;
    cg.writeEntry("updateInterval", m_updateInterval);

    m_engine->disconnectSource(m_sourceFile.url(), this);
    m_sourceFile = ui.filechooser->url();
    cg.writeEntry("File Name", m_sourceFile);

    m_solutionType = Hover;
    if (ui.solutionAlways->isChecked()) {
        m_solutionType = Always;
        m_label2->show();
    }
    cg.writeEntry("Solution", m_solutionType);

    m_lang1 = ui.language1->currentIndex();
    m_lang2 = ui.language2->currentIndex();

    cg.writeEntry("Top Language", m_lang1);
    cg.writeEntry("Bottom Language", m_lang2);


    m_engine->connectSource(m_sourceFile.url(), this, m_updateInterval);

    kDebug() << "open:" << m_sourceFile;

    emit configNeedsSaving();
}

void ParleyPlasma::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    Plasma::Applet::hoverEnterEvent(event);
    m_label2->show();
}

void ParleyPlasma::hoverLeaveEvent(QGraphicsSceneHoverEvent  * event)
{
    Plasma::Applet::hoverLeaveEvent(event);
}

#include "parley_plasma.moc"
