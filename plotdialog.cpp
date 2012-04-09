#include "plotdialog.h"
#include "ui_plotdialog.h"

PlotDialog::PlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotDialog)
{
    ui->setupUi(this);

    connect(ui->variableEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateOutput()));
    connect(ui->expressionEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateOutput()));
    connect(ui->startEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateOutput()));
    connect(ui->stopEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateOutput()));
    connect(ui->stepEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateOutput()));
    connect(ui->linearRadio, SIGNAL(clicked()),
            this, SLOT(updateOutput()));
    connect(ui->logRadio, SIGNAL(clicked()),
            this, SLOT(updateOutput()));

    updateOutput();
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

void PlotDialog::setExpression(const QString expression)
{
    ui->expressionEdit->setText(expression);
}

void PlotDialog::updateOutput()
{
    QString output;

    if (ui->logRadio->isChecked())
        output.append("datalog");
    else
        output.append("data");
    output.append(QString("([%1],%2,%3,%4").arg(ui->expressionEdit->text())
                  .arg(ui->variableEdit->text())
                  .arg(ui->startEdit->text())
                  .arg(ui->stopEdit->text()));
    if (!ui->stepEdit->text().isEmpty())
        output.append(QString(",%1").arg(ui->stepEdit->text()));
    output.append(")");

    ui->outputLabel->setText(output);
    m_output = output;
}

void PlotDialog::on_linearRadio_clicked()
{
    ui->stepLabel->setText(tr("Step size:"));
}

void PlotDialog::on_logRadio_clicked()
{
    ui->stepLabel->setText(tr("Steps:"));
}
