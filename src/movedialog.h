#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>
#include "mesh.h"

namespace Ui {
class moveDialog;
}

class moveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit moveDialog(QWidget *parent = nullptr);
    explicit moveDialog(Mesh *ptr = nullptr);
    ~moveDialog();

private slots:
    void on_validateButton_clicked();
    void on_cancelButton_clicked();

    void on_XSpinBox_valueChanged(double);
    void on_YSpinBox_valueChanged(double);
    void on_ZSpinBox_valueChanged(double);

private:
    Ui::moveDialog *ui;
    Mesh* mdlPtr;
    Vec3 initialPos;
};

#endif // MOVEDIALOG_H
