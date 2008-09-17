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

#include "export.h"

#include "xsldialog.h"
#include "parleydocument.h"

#include <KStandardDirs>
#include <KUrl>
#include <KDebug>
#include <KFileDialog>
#include <KLocalizedString>


void ParleyExport::exportDocument(ParleyDocument* document, QWidget * parent)
{
    // first get the new file name and type
    QString filter = "*.html|" + i18n("HTML document") + '\n' + KEduVocDocument::pattern(KEduVocDocument::Writing);

    KFileDialog dlg( (document->document()->url().fileName() == i18n("Untitled")) ? "": document->document()->url().path(), filter, parent );
    dlg.setOperationMode( KFileDialog::Saving );
    dlg.setMode( KFile::File );
    dlg.setWindowTitle(i18n("Export As"));
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    QString filename = dlg.selectedFile();

    QString documentType = dlg.currentFilter();
    kDebug() << documentType;

    // if it's html, use xslt
    if (documentType == "*.html") {
        XslDialog dlg(filename, document->document(), parent);
        dlg.exec();
    } else {
        document->saveAs(filename);
    }


//     ui = new Ui::ExportOptions();
//     ui->setupUi(mainWidget());
// 
//     m_sourceFile = file;
//     KUrl fileUrl = file;
//     QString fileName = file.fileName().left(file.fileName().indexOf('.'));
//     fileName.append(".html");
//     fileUrl.setFileName(fileName);
// 
//     ui->fileName->setUrl(fileUrl);


    // else use libkdeedu
    ///@todo


}

