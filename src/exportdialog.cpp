/*
    SPDX-FileCopyrightText: 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportdialog.h"

#include "parleydocument.h"
#include <KEduVocDocument>
#include <KEduVocKVTML2Writer>

#include <KLocalizedString>
#include <KMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QUrl>

#include <QDialogButtonBox>

#include <libxml/HTMLtree.h>
#include <libxml/catalog.h>
#include <libxml/debugXML.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>
#include <libxml/xmlmemory.h>
#include <libxslt/transform.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <string.h>

using namespace Qt::Literals::StringLiterals;

ExportDialog::ExportDialog(ParleyDocument *doc, QWidget *parent)
    : QDialog(parent)
    , m_doc{doc}
{
    ui = new Ui::ExportOptions();
    QWidget *widget = new QWidget();
    ui->setupUi(widget);

    QDialogButtonBox *button_dialog = new QDialogButtonBox;
    button_dialog->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(widget);
    layout->addWidget(button_dialog);

    setLayout(layout);

    connect(button_dialog, &QDialogButtonBox::accepted, this, &ExportDialog::accept);
    connect(button_dialog, &QDialogButtonBox::rejected, this, &ExportDialog::reject);

    setWindowTitle(i18nc("@title:window", "Export"));
}

void ExportDialog::accept()
{
    QDialog::accept();

    if (ui->csvRadio->isChecked()) {
        /// Find the CSV filter in the standard filter list
        //!@todo: good and clean solution
        QStringList defaultFilters = KEduVocDocument::pattern(KEduVocDocument::Writing).split(QStringLiteral(";;"));
        QString filter = defaultFilters.filter(QStringLiteral("csv")).join(QStringLiteral(";;"));
        QUrl filename = getFileName(filter);
        if (filename != QUrl()) {
            m_doc->saveAs(filename);
        }
        return;
    }

    QString xslFile;
    if (ui->flashCardRadio->isChecked()) {
        xslFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("parley/xslt/flashcards.xsl"));
    } else {
        xslFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("parley/xslt/table.xsl"));
    }

    QString filter = i18n("HTML document") + u" (*.html)"_s;
    QUrl filename = getFileName(filter);
    if (filename.isEmpty()) {
        return;
    }

    qDebug() << "XSLT starting";

    xsltStylesheetPtr cur = nullptr;
    xmlDocPtr doc, res;

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;
    cur = xsltParseStylesheetFile((const xmlChar *)xslFile.toLatin1().constData());

    doc = xmlParseDoc((const xmlChar *)m_doc->document()->toByteArray(m_doc->document()->generator()).constData());

    res = xsltApplyStylesheet(cur, doc, nullptr);
    FILE *result = fopen(QFile::encodeName(filename.toLocalFile()).constData(), "w");
    if (result != nullptr) {
        xsltSaveResultToFile(result, res, cur);
        fclose(result);
    } else {
        KMessageBox::error(this, i18n("Could not write to file \"%1\"", filename.toLocalFile()));
    }

    xsltFreeStylesheet(cur);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);

    xsltCleanupGlobals();
    xmlCleanupParser();

    qDebug() << "XSLT finished";
}

QUrl ExportDialog::getFileName(const QString &filter)
{
    return QUrl::fromLocalFile(
        QFileDialog::getSaveFileName(parentWidget(),
                                     i18n("Export As"),
                                     (m_doc->document()->url().fileName() == i18n("Untitled")) ? QLatin1String("") : m_doc->document()->url().toLocalFile(),
                                     filter));
}

#include "moc_exportdialog.cpp"
