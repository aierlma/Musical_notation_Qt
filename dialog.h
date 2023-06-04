#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTextEdit>

class DocumentationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentationDialog(QWidget *parent = nullptr);
    void setDocumentText(const QString &text);

private:
    QTextEdit *textEdit;
};

#endif // DIALOG_H
