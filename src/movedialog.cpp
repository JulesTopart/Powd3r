#include "movedialog.h"
#include "ui_movedialog.h"

moveDialog::moveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::moveDialog)
{
    ui->setupUi(this);
}

moveDialog::moveDialog(Mesh *ptr) :
    QDialog(nullptr),
    ui(new Ui::moveDialog)
{
    ui->setupUi(this);
    mdlPtr = ptr;
    initialPos = mdlPtr->getPosition();
    ui->XSpinBox->setValue(initialPos.x);
    ui->YSpinBox->setValue(initialPos.y);
    ui->ZSpinBox->setValue(initialPos.z);
}

moveDialog::~moveDialog()
{
    delete ui;
}

void moveDialog::on_validateButton_clicked()
{
    QVector3D pos(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setPosition(pos);
    mdlPtr->applyTransform();
    this->close();
    delete this;
}

void moveDialog::on_cancelButton_clicked()
{
    mdlPtr->setPosition(initialPos);
    mdlPtr->applyTransform();
    this->close();
    delete this;
}


void moveDialog::on_XSpinBox_valueChanged(double)
{
    if(active) return;
    active = true;
    QVector3D pos(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setPosition(pos);
    mdlPtr->applyTransform();
    active = false;
}

void moveDialog::on_YSpinBox_valueChanged(double)
{
    if(active) return;
    active = true;
    QVector3D pos(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setPosition(pos);
    mdlPtr->applyTransform();
    active = false;
}

void moveDialog::on_ZSpinBox_valueChanged(double)
{
    if(active) return;
    active = true;
    QVector3D pos(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setPosition(pos);
    mdlPtr->applyTransform();
    active = false;
}
