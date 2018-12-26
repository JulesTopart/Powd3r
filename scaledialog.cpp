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
    QVector3D scale = mdlPtr->getScale();
    ui->setupUi(this);
    this->ui->widthSpin->setValue(scale.x());
    this->ui->heightSpin->setValue(scale.y());
    this->ui->depthSpin->setValue(scale.z());

}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}
