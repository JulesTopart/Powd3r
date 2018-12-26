#include "scaledialog.h"
#include "ui_scaledialog.h"

ScaleDialog::ScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleDialog)
{
    ui->setupUi(this);
}

ScaleDialog::ScaleDialog(mesh::Model *mdlPtr_) :
    QDialog(nullptr),
    ui(new Ui::ScaleDialog)
{
    mdlPtr = mdlPtr_;
    initialScale = mdlPtr->getScale();
    ui->setupUi(this);
    this->ui->widthSpin->setValue(initialScale.x());
    this->ui->heightSpin->setValue(initialScale.y());
    this->ui->depthSpin->setValue(initialScale.z());
}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}

void ScaleDialog::on_applyButton_clicked()
{
    QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
    mdlPtr->setScale(scl);
    this->close();
    delete this;
}

void ScaleDialog::on_cancelButton_clicked()
{
    mdlPtr->setScale(initialScale);
    this->close();
    delete this;
}

void ScaleDialog::on_widthSpin_valueChanged(double arg1)
{
    if(ui->keepScaleCheck->isChecked()){
        ui->heightSpin->setValue(ui->widthSpin->value());
        ui->depthSpin->setValue(ui->widthSpin->value());
    }
    QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
    mdlPtr->setScale(scl);
}

void ScaleDialog::on_heightSpin_valueChanged(double arg1)
{
    if(ui->keepScaleCheck->isChecked()){
        ui->widthSpin->setValue(ui->heightSpin->value());
        ui->depthSpin->setValue(ui->heightSpin->value());
    }
    QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
    mdlPtr->setScale(scl);
}

void ScaleDialog::on_depthSpin_valueChanged(double arg1)
{
    if(ui->keepScaleCheck->isChecked()){
        ui->widthSpin->setValue(ui->depthSpin->value());
        ui->heightSpin->setValue(ui->depthSpin->value());
    }
    QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
    mdlPtr->setScale(scl);
}




