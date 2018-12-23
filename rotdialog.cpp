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
    ui->setupUi(this);
    rotPtr = ptr;
}

RotDialog::~RotDialog()
{
    delete ui;
}

void RotDialog::on_applyButton_clicked()
{
    rotPtr->rotate(QVector3D(ui->XSpinBox->value(), ui->YSpinBox->value(), ui->ZSpinBox->value()));
    this->close();
}

void RotDialog::on_cancelButton_clicked()
{
    this->close();
}
