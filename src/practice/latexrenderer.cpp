/***************************************************************************
    Copyright 2010 Daniel Laidig <laidig@kde.org>
    Copyright 2009 Alexander Rieder <alexanderrieder@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "latexrenderer.h"

#include <KLocalizedString>
#include <kprocess.h>

#include <QLabel>
#include <QProcess>
#include <QFileInfo>
#include <QColor>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QCoreApplication>
#include <QDebug>
#include <complex>

using namespace Practice;

const char* texTemplate = "\\documentclass[12pt,fleqn]{article}          \n "\
                          "\\usepackage{latexsym,amsfonts,amssymb,ulem}  \n "\
                          "\\usepackage[dvips]{graphicx}                 \n "\
                          "\\setlength\\textwidth{5in}                   \n "\
                          "\\setlength{\\parindent}{0pt}                 \n "\
                          "\\usepackage{amsmath}                         \n "\
                          "\\usepackage{color}                           \n "\
                          "\\pagestyle{empty}                            \n "\
                          "\\begin{document}                             \n "\
                          "{\\definecolor{mycolor}{rgb}{%1}              \n "\
                          "{\\color{mycolor}                             \n "\
                          "%2 }                                          \n "\
                          "\\end{document}\n";

LatexRenderer::LatexRenderer(QObject* parent)
    : QObject(parent), m_label(0)
{

}

void LatexRenderer::renderLatex(QString tex)
{
    if (m_label) {
        m_label->setText(i18n("Rendering..."));
        m_label->setToolTip(tex);
    }

    if (tex.startsWith(QLatin1String("$$"))) {
        tex.replace(0, 2, "\\begin{eqnarray*}").remove(-2, 2).append("\\end{eqnarray*}");
    } else {
        tex.remove(0, 2).remove(-2, 2);
    }
    qDebug() << "rendering as latex";

    //Check if the parley subdir exists, if not, create it
    QString dir( QDir::tempPath() + QLatin1Char('/') + QCoreApplication::applicationName() );
    QTemporaryFile *texFile = new QTemporaryFile(dir + QLatin1Char('/') + QLatin1String("XXXXXX") + ".tex");
    if ( ! texFile->open() ) {
        return;
    }

    QColor color = m_label->palette().color(QPalette::WindowText);
    QString colorString = QString::number(color.redF()) + ','
                          + QString::number(color.greenF()) + ','
                          + QString::number(color.blueF());
    QString expressionTex = QString(texTemplate).arg(colorString, tex.trimmed());

    texFile->write(expressionTex.toUtf8());
    texFile->flush();

    QString fileName = texFile->fileName();
    qDebug() << "fileName: " << fileName;
    m_latexFilename = fileName;
    m_latexFilename.replace(".tex", ".eps");
    KProcess *p = new KProcess(this);
    p->setWorkingDirectory(dir);

    (*p) << "latex" << "-interaction=batchmode" << "-halt-on-error" << fileName;

    connect(p, SIGNAL(finished(int,  QProcess::ExitStatus)), this, SLOT(convertToPs()));
    connect(p, SIGNAL(error(QProcess::ProcessError)), this, SLOT(latexRendered()));
    p->start();
}

bool LatexRenderer::isLatex(const QString& tex)
{
    return tex.length() > 4 && tex.mid(2, tex.length() - 4).simplified().length() > 0 &&
           ((tex.startsWith(QLatin1String("$$")) && tex.endsWith(QLatin1String("$$"))) ||
            (tex.startsWith(QString::fromUtf8("§§")) && tex.endsWith(QString::fromUtf8("§§"))));
}

void LatexRenderer::convertToPs()
{
    qDebug() << "converting to ps";
    QString dviFile = m_latexFilename;
    dviFile.replace(".eps", ".dvi");
    KProcess *p = new KProcess(this);
    qDebug() << "running: " << "dvips" << "-E" << "-o" << m_latexFilename << dviFile;
    (*p) << "dvips" << "-E" << "-o" << m_latexFilename << dviFile;

    connect(p, SIGNAL(finished(int,  QProcess::ExitStatus)), this, SLOT(convertToImage()));
    connect(p, SIGNAL(error(QProcess::ProcessError)), this, SLOT(latexRendered()));
    p->start();
}

void LatexRenderer::convertToImage()
{
    qDebug() << "converting to ps";
    QString pngFile = m_latexFilename;
    pngFile.replace(".eps", ".png");
    KProcess *p = new KProcess(this);
    qDebug() << "running:" << "convert" << m_latexFilename << pngFile;
    (*p) << "convert" << "-density" << "85" << m_latexFilename << pngFile;

    connect(p, SIGNAL(finished(int,  QProcess::ExitStatus)), this, SLOT(latexRendered()));
    connect(p, SIGNAL(error(QProcess::ProcessError)), this, SLOT(latexRendered()));
    p->start();
}

void LatexRenderer::latexRendered()
{
    qDebug() << "rendered file " << m_latexFilename;

    QString pngFile = m_latexFilename;
    pngFile.replace(".eps", ".png");
    if (QFileInfo(pngFile).exists()) {
        QPixmap pixmap(pngFile);
        m_label->setPixmap(pixmap);
        m_label->setMinimumSize(pixmap.size().boundedTo(QSize(600, 300)));
    } else {
        m_label->setText(i18n("LaTeX error.")); //TODO: better error handling and error messages
    }

    //cleanup the temp directory a bit...
    QString dir( QDir::tempPath() + QLatin1Char('/') + QCoreApplication::applicationName() );

    QStringList extensions;
    extensions << ".log" << ".aux" << ".tex" << ".dvi" << ".eps" << ".png";
    foreach(const QString & ext, extensions) {
        QString s = m_latexFilename;
        s.replace(".eps", ext);
        QFile f(s);
        f.remove();
    }
}
