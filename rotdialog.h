#ifndef ROTDIALOG_H
#define ROTDIALOG_H

#include <QDialog>
#include "mesh.h"

namespace Ui {
class RotDialog;
}

class RotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RotDialog(QWidget *parent = nullptr);
    explicit RotDialog(mesh::Model *ptr = nullptr);
    ~RotDialog();

private slots:
    void on_applyButton_clicked();

    void on_cancelButton_clicked();

    void on_ZSpinBox_valueChanged(double arg1);

    void on_YSpinBox_valueChanged(double arg1);

    void on_XSpinBox_valueChanged(double arg1);

private:
    mesh::Model* rotPtr;
    Ui::RotDialog *ui;
};

#endif // ROTDIALOG_H
