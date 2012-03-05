#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QFile>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HelpDialog(QWidget *parent = 0);
    void setHelpFile(QString fileName);
    ~HelpDialog();
    
private:
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
