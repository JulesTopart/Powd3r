#include "rotdialog.h"
#include "ui_rotdialog.h"

RotDialog::RotDialog(QWidget *parent, MainWindow *mainWindow) :
    QDialog(parent),
    ui(new Ui::RotDialog)
{
    mw = mainWindow;
    ui->setupUi(this);
}

RotDialog::RotDialog( Mesh *ptr, MainWindow *_mw) :
    QDialog(nullptr),
    ui(new Ui::RotDialog)
{
    mdlPtr = ptr;
    mw = _mw;
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

    if(ui->stickOnPlate->isChecked()){
        mdlPtr->updateBB();
        Vec3 newPos(mdlPtr->getPosition());
        newPos.z = (mdlPtr->getBBSize().z/2);
        mdlPtr->setPosition(newPos);
    }

    mdlPtr->applyChange();

    //QMessageBox msgBox;
    //msgBox.setText("z : " + QString::number(mdlPtr->getBoundingBox().getBottomLeft().z).toUtf8());
    //msgBox.exec();

    mw->updateList();
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

void RotDialog::on_RotDialog_destroyed()
{
    mdlPtr->setRotation(initialValue);
}
