#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scaledialog.h"
#include "rotdialog.h"
#include "movedialog.h"

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
    Mesh model;
    ui->progressLabel->setText("Checking STL File...");
    ui->progressBar->setValue(5);
    FILE_FORMAT format = getFileFormat(filePath);
    if(format == INVALID){
        ui->progressBar->setValue(0);
        ui->progressLabel->setText("Corrupted file !");
        return;
    }else{
        ui->progressBar->setValue(10);
        if(format == ASCII ){
            ui->progressLabel->setText("Processing ASCII vertex...");
            model = parseAscii(filePath, *ui->progressBar);
            if(model.size() != 0){
                ui->progressLabel->setText("Rendering...");
                addModel(&model);
                ui->progressBar->setValue(100);
                ui->progressLabel->setText("Done.");
            }
        }
        else{
            ui->progressLabel->setText("Processing Binary vertex...");
            model = parseBinary(filePath.toStdString(), *ui->progressBar);
            if(model.size() != 0){
                ui->progressLabel->setText("Rendering...");
                addModel(&model);
                ui->openGLWidget->updateGL();
                ui->progressBar->setValue(100);
                ui->progressLabel->setText("Done.");
            }
        }
    }
}

void MainWindow::addModel(Mesh* mdlPtr){
    ui->openGLWidget->loadModel(mdlPtr);
    ui->openGLWidget->updateGL();
    updateList();
}
void MainWindow::updateList(){
   int size = ui->openGLWidget->modelCount();
    ui->listWidget->clear();
    Mesh* mdlPtr;
    for(int i(0); i < size; i++){
        mdlPtr = this->ui->openGLWidget->get(i);
        ui->listWidget->addItem( "  " + QString::number(mdlPtr->getId()) + "  :  " + mdlPtr->getName() + " " + QString::number(mdlPtr->getBBSize().x) + " x " + QString::number(mdlPtr->getBBSize().y)+ " x " + QString::number(mdlPtr->getBBSize().z) + "mm");
    }
}


void MainWindow::on_actionImporter_triggered()
{
   browseFile();
}

void MainWindow::on_scaleButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1){
        int id = this->ui->listWidget->currentRow();
        Mesh* mdlPtr = this->ui->openGLWidget->get(id);
        ScaleDialog *dialog = new ScaleDialog(mdlPtr, this);
         dialog->show();
    }
}


void MainWindow::on_rotateButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1){
        int id = this->ui->listWidget->currentRow();
        Mesh* mdlPtr = this->ui->openGLWidget->get(id);
        RotDialog *dialog = new RotDialog(mdlPtr);
        dialog->show();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1){
        this->ui->openGLWidget->unloadModel(this->ui->listWidget->currentRow());
        updateList();
    }
}

void MainWindow::on_duplicateButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1){
        int id = this->ui->listWidget->currentRow();
        Mesh* mdlPtr = this->ui->openGLWidget->get(id);
        addModel(mdlPtr);
    }
}

void MainWindow::on_listWidget_itemPressed(QListWidgetItem*)
{
    int id = ui->listWidget->currentRow();
    ui->openGLWidget->select(id);
    ui->openGLWidget->updateGL();
}


//Reset View Button
void MainWindow::on_pushButton_clicked()
{
    ui->openGLWidget->FrontView();
}

//Add button
void MainWindow::on_pushButton_2_clicked()
{
    browseFile();
}

void MainWindow::on_moveButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1){
        int id = this->ui->listWidget->currentRow();
        Mesh* mdlPtr = this->ui->openGLWidget->get(id);
        moveDialog *dialog = new moveDialog(mdlPtr);
        dialog->show();
    }
}

void MainWindow::on_sliceButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1){
        triMeshSlicer(ui->openGLWidget->get(ui->listWidget->currentRow()),*ui->openGLWidget_2->getLines(), ui->layerHeightSpinBox->value());
        int nSlice = ui->openGLWidget_2->getLines()->size();
        ui->verticalSlider->setMaximum(nSlice);
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    ui->openGLWidget_2->selectSlice(value);
}
