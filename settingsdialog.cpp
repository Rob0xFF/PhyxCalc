#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setAppSettings(AppSettings *settings)
{
    //read output options
    switch (settings->output.numbers.format)
    {
    case 'e':ui->formatRadioe->setChecked(true);
             break;
    case 'E':ui->formatRadioE->setChecked(true);
             break;
    case 'f':ui->formatRadiof->setChecked(true);
             break;
    case 'g':ui->formatRadiog->setChecked(true);
             break;
    case 'G':ui->formatRadioG->setChecked(true);
             break;
    }
    ui->decimalPrecisionSpin->setValue(settings->output.numbers.decimalPrecision);

    //read line parser options
    ui->expressionOutputNumbersCheck->setChecked(settings->lineParser.expression.outputWithNumbers);
    ui->expressionOutputResultCheck->setChecked(settings->lineParser.expression.outputResult);
}

void SettingsDialog::getAppSettings(AppSettings *settings)
{
    //write output options
    if (ui->formatRadioe->isChecked())
        settings->output.numbers.format = 'e';
    else if (ui->formatRadioE->isChecked())
        settings->output.numbers.format = 'E';
    else if (ui->formatRadiof->isChecked())
        settings->output.numbers.format = 'f';
    else if (ui->formatRadiog->isChecked())
        settings->output.numbers.format = 'g';
    else if (ui->formatRadioG->isChecked())
        settings->output.numbers.format = 'G';

    settings->output.numbers.decimalPrecision = ui->decimalPrecisionSpin->value();

    //write line parser options
    settings->lineParser.expression.outputWithNumbers = ui->expressionOutputNumbersCheck->isChecked();
    settings->lineParser.expression.outputResult = ui->expressionOutputResultCheck->isChecked();
}

void SettingsDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}
