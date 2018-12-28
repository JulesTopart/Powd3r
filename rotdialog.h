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
    explicit RotDialog(Mesh *ptr = nullptr);
    ~RotDialog();

private slots:
    void on_applyButton_clicked();

    void on_cancelButton_clicked();

    void on_ZSpinBox_valueChanged(double arg1);

    void on_YSpinBox_valueChanged(double arg1);

    void on_XSpinBox_valueChanged(double arg1);

    void on_RotDialog_destroyed();

private:
    Mesh* mdlPtr;
    QVector3D initialValue = QVector3D(0,0,0);
    Ui::RotDialog *ui;
};

#endif // ROTDIALOG_H
