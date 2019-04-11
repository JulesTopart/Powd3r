#include "scaledialog.h"
#include "ui_scaledialog.h"

ScaleDialog::ScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScaleDialog)
{
    ui->setupUi(this);
}

ScaleDialog::ScaleDialog(Mesh *mdlPtr_, MainWindow *mainWindow) :
    QDialog(nullptr),
    ui(new Ui::ScaleDialog)
{
    mdlPtr = mdlPtr_;
    mw = mainWindow;
    ui->setupUi(this);

    initialScale = mdlPtr->getScale();
    this->intialDim = mdlPtr->getBBSize();
    this->ui->widthSpin->setValue( initialScale.x);
    this->ui->heightSpin->setValue(initialScale.y);
    this->ui->depthSpin->setValue( initialScale.z);
}

ScaleDialog::~ScaleDialog()
{
    delete ui;
}

void ScaleDialog::on_applyButton_clicked()
{
    if(!ui->fromBBCheckBox->isChecked()){
        QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        mdlPtr->setScale(scl);
    }else{
        QVector3D scl3D(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        scl3D /= intialDim;
        mdlPtr->setScale(scl3D);
    }
    mw->updateList();
    mdlPtr->applyTransform();
    this->close();
    delete this;
}

void ScaleDialog::on_cancelButton_clicked()
{
    mdlPtr->setScale(initialScale);
    mdlPtr->applyTransform();
    mw->updateList();
    this->close();
    delete this;
}

void ScaleDialog::on_widthSpin_valueChanged(double)
{
    if(running) return;
    running = true;
    if(!ui->fromBBCheckBox->isChecked()){
        if(ui->keepScaleCheck->isChecked()){
            ui->heightSpin->setValue(ui->widthSpin->value());
            ui->depthSpin->setValue(ui->widthSpin->value());
        }
        QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        mdlPtr->setScale(scl);
    }else{
        double scl = ui->widthSpin->value() / intialDim.x;
        if(ui->keepScaleCheck->isChecked()){
            ui->heightSpin->setValue(scl * intialDim.y);
            ui->depthSpin->setValue( scl * intialDim.z);
        }
        QVector3D scl3D(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        scl3D /= intialDim;
        mdlPtr->setScale(scl3D);
    }
    running = false;
    mdlPtr->applyTransform();
}

void ScaleDialog::on_heightSpin_valueChanged(double)
{
    if(running) return;
    running = true;
    if(!ui->fromBBCheckBox->isChecked()){
        if(ui->keepScaleCheck->isChecked()){
            ui->widthSpin->setValue(ui->heightSpin->value());
            ui->depthSpin->setValue(ui->heightSpin->value());
        }
        QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        mdlPtr->setScale(scl);
    }else{
        double scl = ui->heightSpin->value() / intialDim.y;
        if(ui->keepScaleCheck->isChecked()){
            ui->widthSpin->setValue(scl * intialDim.x);
            ui->depthSpin->setValue(scl * intialDim.z);
        }
        QVector3D scl3D(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        scl3D /= intialDim;
        mdlPtr->setScale(scl3D);
    }
    running = false;
    mdlPtr->applyTransform();
}

void ScaleDialog::on_depthSpin_valueChanged(double)
{
    if(running) return;
    running = true;
    if(!ui->fromBBCheckBox->isChecked()){
        if(ui->keepScaleCheck->isChecked()){
            ui->widthSpin->setValue(ui->depthSpin->value());
            ui->heightSpin->setValue(ui->depthSpin->value());
        }
        QVector3D scl(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        mdlPtr->setScale(scl);
    }else{
        double scl = ui->depthSpin->value() / intialDim.z;
        if(ui->keepScaleCheck->isChecked()){
            ui->widthSpin->setValue(scl * intialDim.x);
            ui->heightSpin->setValue(scl * intialDim.y);
        }
        QVector3D scl3D(ui->widthSpin->value(), ui->heightSpin->value(), ui->depthSpin->value());
        scl3D /= intialDim;
        mdlPtr->setScale(scl3D);
    }
    running = false;
    mdlPtr->applyTransform();
}





void ScaleDialog::on_fromBBCheckBox_stateChanged(int)
{
    if(ui->fromBBCheckBox->isChecked()){
        ui->widthSpin->setValue( intialDim.x);
        ui->heightSpin->setValue(intialDim.y);
        ui->depthSpin->setValue( intialDim.z);
        ui->widthSpin->setSuffix( "mm");
        ui->heightSpin->setSuffix("mm");
        ui->depthSpin->setSuffix( "mm");
    }else{
        this->ui->widthSpin->setValue( initialScale.x);
        this->ui->heightSpin->setValue(initialScale.y);
        this->ui->depthSpin->setValue( initialScale.z);
        ui->widthSpin->setSuffix("");
        ui->heightSpin->setSuffix("");
        ui->depthSpin->setSuffix("");
    }

}

void ScaleDialog::on_ScaleDialog_destroyed()
{
    mdlPtr->setScale(initialScale);
    mdlPtr->applyTransform();
}
