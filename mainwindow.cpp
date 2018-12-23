#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scaledialog.h"
#include "rotdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::browseFile(){
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Object"), "", tr("3D Files (*.stl)"));
    processFile(filePath);
}

void MainWindow::processFile(QString filePath){
    ui->progressLabel->setText("processing File...");
    QFileInfo fi(filePath);
    QString ext = fi.suffix();
    if(ext == "stl"){
        processSTLFile(filePath);
    }else {
        ui->progressLabel->setText("Invalide file extension !");
        ui->progressBar->setValue(0);
    }
}


void MainWindow::processSTLFile(QString filePath){
    mesh::Model model;
    ui->progressLabel->setText("Checking STL File...");
    ui->progressBar->setValue(5);
    mesh::FILE_FORMAT format = mesh::getFileFormat(filePath);
    if(format == mesh::INVALID){
        ui->progressBar->setValue(0);
        ui->progressLabel->setText("Corrupted file !");
        return;
    }else{
        ui->progressBar->setValue(10);
        if(format == mesh::ASCII ){
            ui->progressLabel->setText("Processing ASCII vertex...");
            model = mesh::parseAscii(filePath, *ui->progressBar);
            if(model.facets.size() != 0){
                ui->progressLabel->setText("Rendering...");
                ui->progressBar->setValue(0);
                ui->openGLWidget->loadModel(&model);
                ui->listWidget->addItem( "  " + QString::number(model.id) + "  :  " + model._name + " " + QString::number(model.bbox.width) + "x" + QString::number(model.bbox.heigth)+ "x" + QString::number(model.bbox.depth) + "mm");
                ui->openGLWidget->updateGL();
                ui->progressBar->setValue(100);
                ui->progressLabel->setText("Done.");
            }
        }
        else{
            ui->progressLabel->setText("Processing Binary vertex...");
            model = mesh::parseBinary(filePath.toStdString(), *ui->progressBar);
            if(model.facets.size() != 0){
                ui->progressLabel->setText("Rendering...");
                ui->progressBar->setValue(0);
                ui->openGLWidget->loadModel(&model);
                ui->openGLWidget->updateGL();
                ui->progressBar->setValue(100);
                ui->progressLabel->setText("Done.");
            }
        }
    }
}


void MainWindow::on_actionImporter_triggered()
{
   browseFile();
}


void MainWindow::on_scaleButton_clicked()
{
    if(this->ui->listWidget->count() > 0){
        int id = this->ui->listWidget->currentRow();
        mesh::Model* mdlPtr = this->ui->openGLWidget->get(id);
        ScaleDialog *dialog = new ScaleDialog(mdlPtr);
         dialog->show();
    }
}


void MainWindow::on_rotateButton_clicked()
{
    if(this->ui->listWidget->count() > 0){
        int id = this->ui->listWidget->currentRow();
        mesh::Model* mdlPtr = this->ui->openGLWidget->get(id);
        RotDialog *dialog = new RotDialog(mdlPtr);
        dialog->show();
    }
}
