#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_ANDROID) || defined(Q_WS_SIMULATOR)
    this->setWindowState(Qt::WindowMaximized);
#endif
}

void HelpDialog::setHelpFile(QString fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString helpText = QString::fromUtf8(file.readAll());
        ui->textBrowser->setHtml(helpText);

        file.close();
    }
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
