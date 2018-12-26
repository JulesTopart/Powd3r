#include "rotdialog.h"
#include "ui_rotdialog.h"

RotDialog::RotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RotDialog)
{
    ui->setupUi(this);
}

RotDialog::RotDialog( mesh::Model *ptr) :
    QDialog(nullptr),
    ui(new Ui::RotDialog)
{
    rotPtr = ptr;
    QVector3D rot = rotPtr->getRotation();
    ui->setupUi(this);
    ui->XSpinBox->setValue(rot.x());
    ui->YSpinBox->setValue(rot.y());
    ui->ZSpinBox->setValue(rot.z());

}

RotDialog::~RotDialog()
{
    delete ui;
}

void RotDialog::on_applyButton_clicked()
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    rot -= rotPtr->getRotation();
    rotPtr->rotate(rot);
    this->close();
}

void RotDialog::on_cancelButton_clicked()
{
    this->close();
}

void RotDialog::on_ZSpinBox_valueChanged(double arg1)
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    rot -= rotPtr->getRotation();
    rotPtr->rotate(rot);
}

void RotDialog::on_YSpinBox_valueChanged(double arg1)
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    rot -= rotPtr->getRotation();
    rotPtr->rotate(rot);
}

void RotDialog::on_XSpinBox_valueChanged(double arg1)
{
    QVector3D rot(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    rot -= rotPtr->getRotation();
    rotPtr->rotate(rot);
}
