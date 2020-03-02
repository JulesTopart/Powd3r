#include "movedialog.h"
#include "ui_movedialog.h"

#include "mesh.h"

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

    if(ui->stickOnPlate->isChecked()){
        mdlPtr->updateBB();
        Vec3 newPos(mdlPtr->getPosition());
        newPos.z = (mdlPtr->getBBSize().z/2);
        mdlPtr->setPosition(newPos);
    }
    mdlPtr->applyChange();
    this->close();
    delete this;
}

void moveDialog::on_cancelButton_clicked()
{
    mdlPtr->setPosition(initialPos);
    this->close();
    delete this;
}


void moveDialog::on_XSpinBox_valueChanged(double)
{
    QVector3D pos(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setPosition(pos);
}

void moveDialog::on_YSpinBox_valueChanged(double)
{
    QVector3D pos(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setPosition(pos);
}

void moveDialog::on_ZSpinBox_valueChanged(double)
{
    QVector3D pos(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value());
    mdlPtr->setPosition(pos);
}
