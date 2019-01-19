#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scaledialog.h"
#include "rotdialog.h"
#include "movedialog.h"
#include "sweep.h"
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_sSettingsFile = QApplication::applicationDirPath() + "/settings.ini";
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}


void MainWindow::closeEvent (QCloseEvent *event)
{
    saveSettings();
    /*
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;
    resBtn = QMessageBox::question( this, "Powd3r",
                                    tr("Voulez vous garder les paramètres ?\n"),
                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Cancel);
    if (resBtn == QMessageBox::Cancel) {
        event->ignore();
    } else if(resBtn == QMessageBox::Yes){
        saveSettings();
        event->accept();

    }else */event->accept();
}


void MainWindow::loadSettings()
{
    QSettings settings("Machinerie", "Powd3r");

    QString sText = settings.value("after_layer_gcode", "").toString();
    if (ui->ALGcode) ui->ALGcode->setPlainText(sText);

    sText = settings.value("before_layer_gcode", "").toString();
    if (ui->BLGcode) ui->BLGcode->setPlainText(sText);

    sText = settings.value("end_gcode", "").toString();
    if (ui->EGcode) ui->EGcode->setPlainText(sText);

    sText = settings.value("start_gcode", "").toString();
    if (ui->SGcode) ui->SGcode->setPlainText(sText);

    float slayerHeight = settings.value("layer_height", "").toFloat();
    if (ui->layerHeightSpinBox) ui->layerHeightSpinBox->setValue(slayerHeight);

    float sSpread_rate = settings.value("spread_rate", "").toFloat();
    if (ui->SpreadRateSpinBox) ui->SpreadRateSpinBox->setValue(sSpread_rate);

    int sfirstNozzle = settings.value("first_nozzle", "").toInt();
    if (ui->firstNozzleSpinBox) ui->firstNozzleSpinBox->setValue(sfirstNozzle);

    int slastNozzle = settings.value("last_nozzle", "").toInt();
    if (ui->lastNozzleSpinBox) ui->lastNozzleSpinBox->setValue(slastNozzle);

    float sInkflow = settings.value("ink_flow", "").toFloat();
    if (ui->InkFlowSpinBox) ui->InkFlowSpinBox->setValue(sInkflow);
}

void MainWindow::saveSettings()
{
    QSettings settings("Machinerie", "Powd3r");

    QString sText = (ui->ALGcode) ? ui->ALGcode->toPlainText() : "";
    settings.setValue("after_layer_gcode", sText);
    if (ui->ALGcode) ui->ALGcode->setPlainText(sText);

    sText = (ui->BLGcode) ? ui->BLGcode->toPlainText() : "";
    settings.setValue("before_layer_gcode", sText);
    if (ui->BLGcode) ui->BLGcode->setPlainText(sText);

    sText = (ui->EGcode) ? ui->EGcode->toPlainText() : "";
    settings.setValue("end_gcode", sText);
    if (ui->EGcode) ui->EGcode->setPlainText(sText);

    sText = (ui->SGcode) ? ui->SGcode->toPlainText() : "";
    settings.setValue("start_gcode", sText);
    if (ui->SGcode) ui->SGcode->setPlainText(sText);

    float slayerHeight = (ui->layerHeightSpinBox) ? ui->layerHeightSpinBox->value() : 0.2f;
    settings.setValue("layer_height", slayerHeight);
    if (ui->layerHeightSpinBox) ui->layerHeightSpinBox->setValue(slayerHeight);

    float sSpread_rate = (ui->SpreadRateSpinBox) ? ui->SpreadRateSpinBox->value() : 0.0f;
    settings.setValue("spread_rate", sSpread_rate);
    if (ui->SpreadRateSpinBox) ui->SpreadRateSpinBox->setValue(sSpread_rate);

    int sfirstNozzle = (ui->firstNozzleSpinBox) ? ui->firstNozzleSpinBox->value() : 0.0f;
    settings.setValue("first_nozzle", sfirstNozzle);
    if (ui->firstNozzleSpinBox) ui->firstNozzleSpinBox->setValue(sfirstNozzle);

    int slastNozzle = (ui->lastNozzleSpinBox) ? ui->lastNozzleSpinBox->value() : 0.0f;
    settings.setValue("last_nozzle", slastNozzle);
    if (ui->lastNozzleSpinBox) ui->lastNozzleSpinBox->setValue(slastNozzle);

    float sInkflow = (ui->InkFlowSpinBox) ? ui->InkFlowSpinBox->value() : 100.0f;
    settings.setValue("ink_flow", sInkflow);
    if (ui->InkFlowSpinBox) ui->InkFlowSpinBox->setValue(sInkflow);
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

        ui->progressLabel->setText("Slicing polygon...");

        triMeshSlicer(ui->openGLWidget->get(ui->listWidget->currentRow()), *lines, ui->layerHeightSpinBox->value(), ui->progressBar);

        //generate slice from lines
        ui->progressLabel->setText("Generating slice...");

        ui->openGLWidget_2->getSlice()->clear();
        int progress = 0;
        for(QVector<LineSegment2Ds>::Iterator l = lines->begin(); l < lines->end(); l++, progress++){
            Slice sliceBuf(*l);
            ui->openGLWidget_2->getSlice()->push_back(sliceBuf);
            ui->progressBar->setValue(float(float(progress) / float(lines->size())) * 100);
        }
        ui->progressLabel->setText("Done.");
        ui->progressBar->setValue(100);
        ui->openGLWidget_2->getSlice()->remove(0);
        //ui->openGLWidget_2->getSlice()->remove(ui->openGLWidget_2->sliceCount() - 1);
        int nSlice = ui->openGLWidget_2->sliceCount();
        ui->verticalSlider->setMaximum(nSlice - 1);
        ui->sliceCount->setText(QString::number(nSlice));

        //update Gui
        ui->verticalSlider2->setMaximum(nSlice - 1);
        ui->sliceCount2->setText(QString::number(nSlice));
        ui->openGLWidget_3->clear();


        //SubSlicing
        ui->progressLabel->setText("Generating Sub-slice...");

        QVector<Slice> slices = *ui->openGLWidget_2->getSlice();
        for(int i(0); i < nSlice; i++){
            this->ui->progressBar->setValue(float(float(i) / float(nSlice) * 100));
            LineSegment2Ds subLines = slices[i].subSlice(96);
            ui->openGLWidget_3->push(subLines);
        }
        this->ui->progressBar->setValue(100);
        ui->progressLabel->setText("Done.");
    }
}


void MainWindow::generateGcode(){
    this->ui->gcode->setPlainText(this->ui->SGcode->toPlainText()); //Start Gcode

    //update Gui
    int nSlice = ui->openGLWidget_2->sliceCount();
    ui->gcodePBar->setMaximum(100);
    ui->progressLabel->setText("Generating Gcode...");
    QVector<LineSegment2Ds> subLines = *ui->openGLWidget_3->getLines();
    int i(0);
    for (QVector<LineSegment2Ds>::Iterator lines = subLines.begin(); lines != subLines.end(); lines++, i++) {

        this->ui->gcodePBar->setValue(float(float(i) / float(lines->size() * 2) * 100));

        std::vector<Line> Slines;
        Point A, B;
        for (LineSegment2Ds::Iterator line = lines->begin(); line != lines->end(); line++, i++){
            A = Point(line->A().x(), line->A().y());
            B = Point(line->B().x(), line->B().y());
            Slines.push_back(Line(A,B));
        }

        i++;
        this->ui->gcodePBar->setValue(float(float(i) / float(lines->size() * 2) * 100));

        SweepCollection sweeps = SweepCollection::generateSweeps(Slines, ui->firstNozzleSpinBox->value(), ui->lastNozzleSpinBox->value() - 1, 96);
        std::string out = sweeps.toGcode(1);
        QString cast = QString::fromStdString(out);

        this->ui->gcode->append(cast);
        this->ui->gcode->append(this->ui->BLGcode->toPlainText());
        this->ui->gcode->append("G1 Y" +  QString::number(ui->layerHeightSpinBox->value()));
        this->ui->gcode->append(this->ui->ALGcode->toPlainText());
    }
     ui->gcode->append(this->ui->EGcode->toPlainText());
     ui->progressLabel->setText("Done.");
     ui->gcodePBar->setValue(100);
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

void MainWindow::on_pushButton_4_clicked()
{
    generateGcode();
}
