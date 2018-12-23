#include "scaledialog.h"
#include "ui_scaledialog.h"

ScaleDialog::ScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleDialog)
{
    ui->setupUi(this);
}

ScaleDialog::ScaleDialog(mesh::Model *mdlPtr) :
    QDialog(nullptr),
    ui(new Ui::ScaleDialog)
{
    ui->setupUi(this);
}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}
