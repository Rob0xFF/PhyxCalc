#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>

namespace Ui {
class PlotDialog;
}

class PlotDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString output READ output)
    
public:
    explicit PlotDialog(QWidget *parent = 0);
    ~PlotDialog();

    void setExpression(QString const expression);
    
    QString output() const
    {
        return m_output;
    }

private slots:
    void updateOutput();

    void on_linearRadio_clicked();

    void on_logRadio_clicked();

private:
    Ui::PlotDialog *ui;
    QString m_output;
};

#endif // PLOTDIALOG_H
