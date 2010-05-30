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

#include <klocale.h>
#include <kdebug.h>
#include <ktemporaryfile.h>
#include <kstandarddirs.h>
#include <kprocess.h>

#include <QLabel>
#include <QProcess>
#include <QFileInfo>

using namespace Practice;

const char* texTemplate = "\\documentclass[12pt,fleqn]{article}          \n "\
                          "\\usepackage{latexsym,amsfonts,amssymb,ulem}  \n "\
                          "\\usepackage[dvips]{graphicx}                 \n "\
                          "\\setlength\\textwidth{5in}                   \n "\
                          "\\setlength{\\parindent}{0pt}                 \n "\
                          "\\usepackage{amsmath}                         \n "\
                          "\\pagestyle{empty}                            \n "\
                          "\\begin{document}                             \n "\
                          "%1                                            \n "\
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
        tex.replace(0,2,"\\begin{eqnarray*}").remove(-2,2).append("\\end{eqnarray*}");
    } else {
        tex.remove(0,2).remove(-2,2);
    }
    kDebug()<<"rendering as latex";

    QString dir=KGlobal::dirs()->saveLocation("tmp", "parley/");

    //Check if the parley subdir exists, if not, create it
    KTemporaryFile *texFile=new KTemporaryFile();
    texFile->setPrefix( "parley/" );
    texFile->setSuffix( ".tex" );
    texFile->open();

    QString expressionTex=QString(texTemplate).arg(tex.trimmed());

    texFile->write(expressionTex.toUtf8());
    texFile->flush();

    QString fileName = texFile->fileName();
    kDebug()<<"fileName: "<<fileName;
    m_latexFilename=fileName;
    m_latexFilename.replace(".tex", ".eps");
    KProcess *p=new KProcess( this );
    p->setWorkingDirectory(dir);

    (*p)<<"latex"<<"-interaction=batchmode"<<"-halt-on-error"<<fileName;

    connect(p, SIGNAL( finished(int,  QProcess::ExitStatus) ), this, SLOT( convertToPs() ) );
    p->start();
}

bool LatexRenderer::isLatex(const QString& tex)
{
    return tex.length() > 4 && tex.mid(2, tex.length()-4).simplified().length() > 0 &&
            ((tex.startsWith(QLatin1String("$$")) && tex.endsWith(QLatin1String("$$"))) ||
            (tex.startsWith(QString::fromUtf8("§§")) && tex.endsWith(QString::fromUtf8("§§"))));
}

void LatexRenderer::convertToPs()
{
    kDebug()<<"converting to ps";
    QString dviFile=m_latexFilename;
    dviFile.replace(".eps", ".dvi");
    KProcess *p=new KProcess( this );
    kDebug()<<"running: "<<"dvips"<<"-E"<<"-o"<<m_latexFilename<<dviFile;
    (*p)<<"dvips"<<"-E"<<"-o"<<m_latexFilename<<dviFile;

    connect(p, SIGNAL( finished(int,  QProcess::ExitStatus) ), this, SLOT( convertToImage() ) );
    p->start();
}

void LatexRenderer::convertToImage()
{
    kDebug()<<"converting to ps";
    QString pngFile=m_latexFilename;
    pngFile.replace(".eps", ".png");
    KProcess *p=new KProcess( this );
    kDebug()<<"running:"<<"convert"<<m_latexFilename<<pngFile;
    (*p)<<"convert"<<"-density"<<"85"<<m_latexFilename<<pngFile;

    connect(p, SIGNAL( finished(int,  QProcess::ExitStatus) ), this, SLOT( latexRendered() ) );
    p->start();
}

void LatexRenderer::latexRendered()
{
    kDebug()<<"rendered file "<<m_latexFilename;

    QString pngFile=m_latexFilename;
    pngFile.replace(".eps", ".png");
    if(QFileInfo(pngFile).exists())
    {
        QPixmap pixmap(pngFile);
        m_label->setPixmap(pixmap);
        m_label->setMinimumSize(pixmap.size().boundedTo(QSize(600, 300)));
    } else {
        m_label->setText(i18n("LaTeX error.")); //TODO: better error handling and error messages
    }

    //cleanup the temp directory a bit...
    QString dir=KGlobal::dirs()->saveLocation("tmp", "parley/");
    QStringList extensions;
    extensions<<".log"<<".aux"<<".tex"<<".dvi"<<".eps"<<".png";
    foreach(const QString& ext, extensions)
    {
        QString s=m_latexFilename;
        s.replace(".eps", ext);
        QFile f(s);
        f.remove();
    }
}

#include "latexrenderer.moc"
