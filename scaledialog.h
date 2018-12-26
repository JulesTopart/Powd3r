#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include "mesh.h"

namespace Ui {
class ScaleDialog;
}

class ScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleDialog(QWidget *parent = nullptr);
    explicit ScaleDialog(mesh::Model *parent = nullptr);
    ~ScaleDialog();

private slots:
    void on_applyButton_clicked();

    void on_cancelButton_clicked();

    void on_widthSpin_valueChanged(double arg1);

    void on_heightSpin_valueChanged(double arg1);

    void on_depthSpin_valueChanged(double arg1);

private:
    Ui::ScaleDialog *ui;
    QVector3D initialScale = QVector3D(0,0,0);;
    mesh::Model* mdlPtr;
};

#endif // SCALEDIALOG_H
