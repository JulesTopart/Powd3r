#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_actionSTL_triggered()
{
    browseFile();
}


void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::browseFile(){
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Object"), "", tr("3D Files (*.stl *.obj)"));
    processFile(filePath);
}

void MainWindow::processFile(QString filePath){
    ui->progressLabel->setText("processing File...");
    QFileInfo fi(filePath);
    QString ext = fi.suffix();
    if(ext == "stl"){
        processSTLFile(filePath);
    }else if(ext == ".obj"){
        processOBJFile(filePath);
    }
}




void MainWindow::processSTLFile(QString filePath){
    ui->progressLabel->setText("Checking STL File...");
    ui->progressBar->setValue(5);
    stl::STL_STATUS status = stl::checkStlFileFormat(filePath);
    if(status == stl::STL_INVALID){
        ui->progressBar->setValue(0);
        ui->progressLabel->setText("Corrupted file !");
    }else{
        ui->progressBar->setValue(10);
        ui->progressLabel->setText("Processing vertex...");
    }
}

void MainWindow::processOBJFile(QString filePath){
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
}





