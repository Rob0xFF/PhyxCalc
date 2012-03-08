#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
#ifdef MOBILE_VERSION
    this->setWindowState(Qt::WindowMaximized);
#endif

    this->setWindowIcon(QIcon::fromTheme("help-contents",QIcon(":/icons/help-contents")));
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
