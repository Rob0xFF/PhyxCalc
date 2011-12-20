#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);

    QPushButton *button = ui->buttonBox->addButton(tr("Copy to Clipboard"), QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()),
            this, SLOT(copyToClipboard()));
    button = ui->buttonBox->addButton(tr("Open Formel Editor"), QDialogButtonBox::ActionRole);
    connect(button, SIGNAL(clicked()),
            this, SLOT(openFormelEditor()));
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::setText(QString text)
{
    ui->plainTextEdit->setPlainText(text);
}

void ExportDialog::copyToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->plainTextEdit->toPlainText());
}

void ExportDialog::openFormelEditor()
{
    system("libreoffice --math");
}
