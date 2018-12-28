#include "rotdialog.h"
#include "ui_rotdialog.h"

RotDialog::RotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RotDialog)
{
    ui->setupUi(this);
}

RotDialog::RotDialog( Mesh *ptr) :
    QDialog(nullptr),
    ui(new Ui::RotDialog)
{
    mdlPtr = ptr;
    initialValue = mdlPtr->getRotation();
    ui->setupUi(this);
    ui->XSpinBox->setValue(initialValue.x());
    ui->YSpinBox->setValue(initialValue.y());
    ui->ZSpinBox->setValue(initialValue.z());

}

RotDialog::~RotDialog()
{
    delete ui;
}

void RotDialog::on_applyButton_clicked()
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setRotation(rot);
    this->close();
    delete this;
}

void RotDialog::on_cancelButton_clicked()
{
    mdlPtr->setRotation(initialValue);
    this->close();
    delete this;
}

void RotDialog::on_ZSpinBox_valueChanged(double arg1)
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setRotation(rot);
}

void RotDialog::on_YSpinBox_valueChanged(double arg1)
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setRotation(rot);
}

void RotDialog::on_XSpinBox_valueChanged(double arg1)
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setRotation(rot);
}
