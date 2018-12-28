#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include "mesh.h"
#include "mainwindow.h"

namespace Ui {
class ScaleDialog;
}

class ScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleDialog(QWidget *parent = nullptr);
    explicit ScaleDialog(Mesh *mdlPtr = nullptr, MainWindow* mainWindow = nullptr );
    ~ScaleDialog();

private slots:
    void on_applyButton_clicked();

    void on_cancelButton_clicked();

    void on_widthSpin_valueChanged(double arg1);

    void on_heightSpin_valueChanged(double arg1);

    void on_depthSpin_valueChanged(double arg1);

    void on_fromBBCheckBox_stateChanged(int arg1);

    void on_ScaleDialog_destroyed();

private:
    Ui::ScaleDialog *ui;
    Vec3 initialScale = QVector3D(1,1,1);
    Vec3 intialDim;
    Mesh* mdlPtr;
    MainWindow* mw;
    bool running = false;
};

#endif // SCALEDIALOG_H
