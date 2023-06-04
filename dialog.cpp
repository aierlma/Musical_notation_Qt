#include "dialog.h"
#include "ui_dialog.h"
#include <QVBoxLayout>

DocumentationDialog::DocumentationDialog(QWidget *parent)
    : QDialog(parent), textEdit(new QTextEdit(this))
{
    textEdit->setReadOnly(true);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);
}

void DocumentationDialog::setDocumentText(const QString &text)
{
    textEdit->setPlainText(text);
}