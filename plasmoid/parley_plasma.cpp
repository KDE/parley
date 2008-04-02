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
#include <KFontDialog>
#include <KColorDialog>
#include <KDebug>

ParleyPlasma::ParleyPlasma(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_theme("widgets/parley_plasma_card", this),
    m_dialog(0),
    m_font(QFont()),
    m_size(256,160)

{
    m_dialog = 0;
    m_label1 = 0;
    m_label2 = 0;
    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
    setDrawStandardBackground(false);
    setRemainSquare(true);

    m_theme.resize();
}

void ParleyPlasma::init()
{
    kDebug() << "init";

    KConfigGroup cg = config();
    m_updateInterval = cg.readEntry("updateInterval", 10000);
    m_engine = dataEngine("parley");

    m_theme.setContentType(Plasma::Svg::SingleImage);
    m_theme.size().height();

    m_label1 = new Plasma::Label(this);
    m_label2 = new Plasma::Label(this);

    m_label1->setPos( m_theme.elementRect( "translation1" ).topLeft() );
    m_label2->setPos( m_theme.elementRect( "translation2" ).topLeft() );

    m_label1->setFont(cg.readEntry("font",m_font));
    m_label2->setFont(cg.readEntry("font",m_font));

    switch (cg.readEntry("Vocabulary File Source", 0)) {
        case Parley: {
            kDebug() << "open file from parleyrc";
            KConfig parleyConfig("parleyrc");
            kDebug() << parleyConfig.groupList();
            KConfigGroup recentFilesGroup( &parleyConfig, "Recent Files" );
            // take the last file, but there are File1..n and Name1..n entries..
            m_sourceFile = recentFilesGroup.readEntry( recentFilesGroup.keyList().value(recentFilesGroup.keyList().count()/2-1), QString() );
            kDebug() << "open file: " << m_sourceFile;
            m_engine->connectSource(m_sourceFile, this, m_updateInterval);
            break;
        }
        case UserDefined: {
            QString m_sourceFile = cg.readEntry("File Name");
            m_engine->connectSource(m_sourceFile, this, m_updateInterval);
            kDebug() << "open file: " << m_sourceFile;
            break;
        }
    }
}

void ParleyPlasma::constraintsUpdated(Plasma::Constraints constraints)
{
    setDrawStandardBackground(false);
    prepareGeometryChange();
    if (constraints & Plasma::SizeConstraint) {
        m_theme.resize(contentSize().toSize());
    }
    m_label1->setPos( m_theme.elementRect( "translation1" ).topLeft() );
    m_label1->setFont( m_font );
    double scale = qMin(m_theme.elementRect( "translation1" ).width()/m_label1->boundingRect().width(), m_theme.elementRect( "translation1" ).height()/m_label1->boundingRect().height());
    m_label1->setTransform(QTransform().scale(scale, scale));

    m_label2->setPos( m_theme.elementRect( "translation2" ).topLeft() );
    m_label2->setFont( m_font );
    scale = qMin(m_theme.elementRect( "translation2" ).width()/m_label2->boundingRect().width(), m_theme.elementRect( "translation2" ).height()/m_label2->boundingRect().height());
    m_label2->setTransform(QTransform().scale(scale, scale));
}

ParleyPlasma::~ParleyPlasma()
{
    delete m_dialog;
}

void ParleyPlasma::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    kDebug() << "data updated" << source << data;
    QStringList words = data[source].toString().split(",,");


    if ( m_label1) {
        m_label1->setText(words[0]);
        double scale = qMin(m_theme.elementRect( "translation1" ).width()/m_label1->boundingRect().width(), m_theme.elementRect( "translation1" ).height()/m_label1->boundingRect().height());
        m_label1->setTransform(QTransform().scale(scale, scale));
        m_label1->setPos(m_theme.elementRect( "translation1" ).topLeft()
                + QPointF(
                    (m_theme.elementRect("translation1").width()-m_label1->boundingRect().width()*scale)/2.0,
                    (m_theme.elementRect("translation1").height()-m_label1->boundingRect().height()*scale)/2.0));

        if (words.size() > 1) {
        m_label2->setText(words[1]);
        scale = qMin(m_theme.elementRect( "translation2" ).width()/m_label2->boundingRect().width(), m_theme.elementRect( "translation2" ).height()/m_label2->boundingRect().height());
        m_label2->setTransform(QTransform().scale(scale, scale));
        m_label2->hide();
        m_label2->setPos(m_theme.elementRect( "translation2" ).topLeft()
                + QPointF(
                            (m_theme.elementRect("translation2").width()-m_label2->boundingRect().width()*scale)/2.0,
                            (m_theme.elementRect("translation2").height()-m_label2->boundingRect().height()*scale)/2.0));
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

    m_theme.resize((int)contentsRect.width(),
                         (int)contentsRect.height());
    m_theme.paint(p,
                       (int)contentsRect.left(),
                       (int)contentsRect.top());
}

void ParleyPlasma::showConfigurationInterface()
{
    if (m_dialog == 0) {
        m_dialog = new KDialog;
        m_dialog->setWindowIcon(KIcon("parley"));
        m_dialog->setCaption( i18n("ParleyPlasma Configuration") );
        ui.setupUi(m_dialog->mainWidget());
        m_dialog->mainWidget()->layout()->setMargin(0);
        ui.updateIntervalSpinBox->setValue(m_updateInterval/1000);
        KConfigGroup cg = config();

        int fileSource = cg.readEntry("Vocabulary File Source", 0);
        switch (fileSource) {
            case Parley:
                ui.sourceParleyRadioButton->setChecked(true);
                break;
            case UserDefined:
                ui.sourceCustomRadioButton->setChecked(true);
                break;
        }
        m_sourceFile = cg.readEntry("File Name");
        ui.filechooser->setPath(m_sourceFile);
        kDebug() << "set file url: " << cg.readEntry("File Name");
        m_dialog->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply );
        connect( m_dialog, SIGNAL(applyClicked()), this, SLOT(configAccepted()) );
        connect( m_dialog, SIGNAL(okClicked()), this, SLOT(configAccepted()) );
        connect( ui.fontSelectButton, SIGNAL(clicked()), this, SLOT(showFontSelectDlg()) );
    }

    m_dialog->show();
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

    if (ui.sourceParleyRadioButton->isChecked()) {
        cg.writeEntry("Vocabulary File Source", (int) Parley);
        KConfig parleyConfig("parleyrc");
        KConfigGroup recentFilesGroup( &parleyConfig, "Recent Files" );
            // take the last file, but there are File1..n and Name1..n entries..
        m_sourceFile = recentFilesGroup.readEntry( recentFilesGroup.keyList().value(recentFilesGroup.keyList().count()/2-1), QString() );
    } else {
        cg.writeEntry("Vocabulary File Source", (int) UserDefined);
        m_sourceFile = ui.filechooser->url().url();
        cg.writeEntry("File Name", m_sourceFile);

    }

    m_engine->disconnectSource(m_sourceFile, this);
    m_engine->connectSource(m_sourceFile, this, m_updateInterval);

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
    m_label2->hide();
}

#include "parley_plasma.moc"
