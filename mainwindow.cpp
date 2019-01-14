#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scaledialog.h"
#include "rotdialog.h"
#include "movedialog.h"
#include "sweep.h"

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
    //check if one ore more model are loaded
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1){
        //generate slice polygon (result of plane intersecting with mesh)
        QVector<LineSegment2Ds> *lines;
        lines = new  QVector<LineSegment2Ds>();
        triMeshSlicer(ui->openGLWidget->get(ui->listWidget->currentRow()), *lines, ui->layerHeightSpinBox->value());

        //generate slice from lines
        ui->openGLWidget_2->getSlice()->clear();
        int progress = 0;
        for(QVector<LineSegment2Ds>::Iterator l = lines->begin(); l < lines->end(); l++, progress++){
            Slice sliceBuf(*l);
            ui->openGLWidget_2->getSlice()->push_back(sliceBuf);
            this->ui->progressBar->setValue(float(float(progress) / float(lines->size())) * 20);
        }


        //update Gui
        int nSlice = ui->openGLWidget_2->sliceCount();
        ui->verticalSlider->setMaximum(nSlice);
        ui->sliceCount->setText(QString::number(nSlice));
        ui->verticalSlider2->setMaximum(nSlice);
        ui->sliceCount2->setText(QString::number(nSlice));

        ui->openGLWidget_3->clear();
        QVector<Slice> slices = *ui->openGLWidget_2->getSlice();
        for(int i(0); i < nSlice; i++){
            LineSegment2Ds subLines = slices[i].subSlice();
            ui->openGLWidget_3->push(subLines);
        }


        QVector<LineSegment2Ds> subLines = *ui->openGLWidget_3->getLines();
        for (QVector<LineSegment2Ds>::Iterator lines = subLines.begin(); lines != subLines.end(); lines++) {
            std::vector<Line> Slines;
            Point A, B;
            for (LineSegment2Ds::Iterator line = lines->begin(); line != lines->end(); line++){
                A = Point(line->A().x(), line->A().y());
                A = Point(line->B().x(), line->B().y());
                Slines.push_back(Line(A,B));
            }
            SweepCollection sweeps = SweepCollection::generateSweeps(Slines, 1, 11, 96);
            std::string out = sweeps.toGcode(1);
            QString cast = QString::fromStdString(out);
#ifdef DEBUG_SWEEP
            std::cout << "out :" << out << std::endl;
#endif
            this->ui->gcode->setPlainText(cast);
        }
     }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    ui->openGLWidget_2->selectSlice(value);
    ui->zeroCount->setText(QString::number(value));
    ui->verticalSlider2->setValue(value);
}

void MainWindow::on_verticalSlider2_valueChanged(int value)
{
    ui->openGLWidget_3->selectSlice(value);
    ui->zeroCount3->setText(QString::number(value));
    ui->verticalSlider->setValue(value);
}

void MainWindow::generateGcode(){

}

void MainWindow::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Export Gcode"), "",
            tr("GCode (*.gcode);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }


        QTextStream out(&file);
        out << this->ui->gcode->toPlainText();


        file.close();
    }
}
