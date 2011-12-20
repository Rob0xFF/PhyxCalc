#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    void setAppSettings(AppSettings *settings);
    void getAppSettings(AppSettings *settings);

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
