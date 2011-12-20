#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QClipboard>

namespace Ui {
    class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

    void setText(QString text);

private:
    Ui::ExportDialog *ui;

private slots:
    void copyToClipboard();
    void openFormelEditor();
};

#endif // EXPORTDIALOG_H
