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

    int pass = settings.value("pass_count", "").toInt();
    if (ui->InkFlowSpinBox) ui->passSpinBox->setValue(pass);

    float buildX = settings.value("build_x","").toFloat();
    if (ui->doubleSpinBox) ui->doubleSpinBox->setValue(buildX);

    float buildY = settings.value("build_y","").toFloat();
    if (ui->doubleSpinBox) ui->doubleSpinBox_3->setValue(buildY);

    float buildZ = settings.value("build_z","").toFloat();
    if (ui->doubleSpinBox) ui->doubleSpinBox_2->setValue(buildZ);


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

    sText = (ui->SGcode) ? ui->SGcode->toPlainText() : ";Gcode generated by Powd3r";
    settings.setValue("start_gcode", sText);
    if (ui->SGcode) ui->SGcode->setPlainText(sText);

    float slayerHeight = (ui->layerHeightSpinBox) ? ui->layerHeightSpinBox->value() : 0.2f;
    settings.setValue("layer_height", slayerHeight);
    if (ui->layerHeightSpinBox) ui->layerHeightSpinBox->setValue(slayerHeight);

    float sSpread_rate = (ui->SpreadRateSpinBox) ? ui->SpreadRateSpinBox->value() : 0.0f;
    settings.setValue("spread_rate", sSpread_rate);
    if (ui->SpreadRateSpinBox) ui->SpreadRateSpinBox->setValue(sSpread_rate);

    int sfirstNozzle = (ui->firstNozzleSpinBox) ? ui->firstNozzleSpinBox->value() : 1.0f;
    settings.setValue("first_nozzle", sfirstNozzle);
    if (ui->firstNozzleSpinBox) ui->firstNozzleSpinBox->setValue(sfirstNozzle);

    int slastNozzle = (ui->lastNozzleSpinBox) ? ui->lastNozzleSpinBox->value() : 12.0f;
    settings.setValue("last_nozzle", slastNozzle);
    if (ui->lastNozzleSpinBox) ui->lastNozzleSpinBox->setValue(slastNozzle);

    float sInkflow = (ui->InkFlowSpinBox) ? ui->InkFlowSpinBox->value() : 100.0f;
    settings.setValue("ink_flow", sInkflow);
    if (ui->InkFlowSpinBox) ui->InkFlowSpinBox->setValue(sInkflow);

    int pass_count = (ui->InkFlowSpinBox) ? ui->passSpinBox->value() : 1;
    settings.setValue("pass_count", pass_count);
    if (ui->InkFlowSpinBox) ui->InkFlowSpinBox->setValue(pass_count);

    float buildX = (ui->doubleSpinBox) ? ui->doubleSpinBox->value() : 200.0f;
    settings.setValue("build_x", buildX);
    if (ui->doubleSpinBox) ui->doubleSpinBox->setValue(buildX);

    float buildY = (ui->doubleSpinBox_3) ? ui->doubleSpinBox_3->value() : 200.0f;
    settings.setValue("build_y", buildY);
    if (ui->doubleSpinBox_3) ui->doubleSpinBox_3->setValue(buildY);

    float buildZ = (ui->doubleSpinBox_2) ? ui->doubleSpinBox_2->value() : 200.0f;
    settings.setValue("build_z", buildZ);
    if (ui->doubleSpinBox_2) ui->doubleSpinBox_2->setValue(buildZ);

    float offbuildX = (ui->X_OffsetSpinBox) ? ui->X_OffsetSpinBox->value() : 0.0f;
    settings.setValue("offbuild_x", offbuildX);
    if (ui->X_OffsetSpinBox) ui->X_OffsetSpinBox->setValue(offbuildX);

    float offbuildY = (ui->Y_OffsetSpinBox) ? ui->Y_OffsetSpinBox->value() : 0.0f;
    settings.setValue("offbuild_y", offbuildY);
    if (ui->Y_OffsetSpinBox) ui->Y_OffsetSpinBox->setValue(offbuildY);

    float offbuildZ = (ui->Z_OffsetSpinBox) ? ui->Z_OffsetSpinBox->value() : 0.0f;
    settings.setValue("offbuild_z", offbuildZ);
    if (ui->Z_OffsetSpinBox) ui->Z_OffsetSpinBox->setValue(offbuildZ);
}


void MainWindow::browseFile(){
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Object"), "", tr("3D Files (*.stl)"));
    processFile(filePath);
}

void MainWindow::processFile(QString filePath){
    ui->progressLabel->setText("Traitement du fichier...");
    QFileInfo fi(filePath);
    QString ext = fi.suffix();
    if(ext == "stl"){
        processSTLFile(filePath);
    }else {
        ui->progressLabel->setText(" Fichier invalide");
        ui->progressBar->setValue(0);
    }
}

void MainWindow::processSTLFile(QString filePath){
    this->setCursor(QCursor(Qt::WaitCursor));
    Mesh model;
    ui->progressLabel->setText("Vérification du fichier STL...");
    ui->progressBar->setValue(5);
    FILE_FORMAT format = getFileFormat(filePath);
    if(format == INVALID){
        ui->progressBar->setValue(0);
        ui->progressLabel->setText("Fichier  corrompu !");
        return;
    }else{
        ui->progressBar->setValue(10);
        if(format == ASCII ){
            ui->progressLabel->setText("Traitement des points 3D ASCII...");
            model = parseAscii(filePath, *ui->progressBar);
            if(model.size() != 0){
                ui->progressLabel->setText("Rendu OpenGL...");
                addModel(&model);
                ui->progressBar->setValue(100);
                ui->progressLabel->setText("Fait.");
            }
        }
        else{
            ui->progressLabel->setText("Traitement des points 3D Binaire...");
            model = parseBinary(filePath.toStdString(), *ui->progressBar);
            if(model.size() != 0){
                ui->progressLabel->setText("Rendu OpenGL...");
                addModel(&model);
                ui->openGLWidget->updateGL();
                ui->progressBar->setValue(100);
                ui->progressLabel->setText("Fait.");
            }
        }
    }
    this->setCursor(QCursor(Qt::ArrowCursor));
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
    this->setCursor(QCursor(Qt::WaitCursor));
    //check if one ore more model are loaded
    if(this->ui->listWidget->count() > 0 &&  ui->listWidget->currentRow() != -1 && ui->layerHeightSpinBox->value() > 0.001){
        //generate slice polygon (result of plane intersecting with mesh)
        QVector<Lines2D> *lines;
        lines = new  QVector<Lines2D>();

        ui->progressLabel->setText("Traitements des polygon...");

        triMeshSlicer(ui->openGLWidget->get(ui->listWidget->currentRow()), *lines, ui->layerHeightSpinBox->value(), ui->progressBar);

        //generate slice from lines
        ui->progressLabel->setText("Géneration des tranches...");

        ui->openGLWidget_2->getSlice()->clear();
        int progress = 0;
        for(QVector<Lines2D>::Iterator l = lines->begin(); l < lines->end(); l++, progress++){
            Slice sliceBuf(*l);
            ui->openGLWidget_2->getSlice()->push_back(sliceBuf);
            qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,10);
            ui->progressBar->setValue(float(float(progress) / float(lines->size())) * 100);
        }
        ui->progressLabel->setText("Fait.");
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
        ui->progressLabel->setText("Géneration des sous-stanches...");

        QVector<Slice> slices = *ui->openGLWidget_2->getSlice();
        for(int i(0); i < nSlice; i++){
            this->ui->progressBar->setValue(float(float(i) / float(nSlice) * 100));
            Lines2D subLines = slices[i].subSlice(96);
            ui->openGLWidget_3->push(subLines);
        }
        this->ui->progressBar->setValue(100);
        ui->progressLabel->setText("Fait.");
        ui->tabWidget->setCurrentIndex(1);
    }else{
        if(!(ui->layerHeightSpinBox->value() > 0.001)){
            ui->progressLabel->setText("Hauteur de couche invalide");
        }else{
            ui->progressLabel->setText("Sélectionnez un modèle !");
        }
    }
    this->setCursor(QCursor(Qt::ArrowCursor));
}


void MainWindow::generateGcode(){

    //GUI
    this->setCursor(QCursor(Qt::WaitCursor));
    ui->progressLabel->setText("Géneration du Gcode...");


    //initialisation
    this->ui->gcode->setPlainText(this->ui->SGcode->toPlainText());               //Start Gcode
    QVector2D offset(ui->X_OffsetSpinBox->value(), ui->Y_OffsetSpinBox->value()); //Store origin offset
    int id = this->ui->listWidget->currentRow();
    Mesh* mdlPtr = this->ui->openGLWidget->get(id);
    //offset += QVector2D(mdlPtr->getBBSize().x, mdlPtr->getBBSize().y) / 2;        //Move to model bottom left corner
    offset += QVector2D(mdlPtr->getPosition().x(), mdlPtr->getPosition().y());
    offset += QVector2D(ui->openGLWidget->plateDim.x()/2, ui->openGLWidget->plateDim.y()/2);
    int nSlice = ui->openGLWidget_2->sliceCount();                                //Store sliceCount
    QVector<Lines2D> subLines = *ui->openGLWidget_3->getLines();                  //Store nozzleLines
    int pass = ui->passSpinBox->value();
    int progress(0);                                                              //TODO modify this progress handling


    long max(0);

    for (QVector<Lines2D>::Iterator lines = subLines.begin(); lines != subLines.end(); lines++) { //For each slice
        max += lines->size();
    }
    ui->gcodePBar->setMaximum(max);

    for (QVector<Lines2D>::Iterator lines = subLines.begin(); lines != subLines.end(); lines++, progress++) { //For each slice
        //GUI Update
        qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,10);
        this->ui->gcodePBar->setValue(progress);

        SweepCollection sweeps = SweepCollection::generateSweeps(lines->toStdVector(), ui->firstNozzleSpinBox->value() - 1, ui->lastNozzleSpinBox->value() - 1, 96); //Generate slice sweeps
        std::string out = sweeps.toGcode(pass, offset); //number of pass
        QString cast = QString::fromStdString(out);

        this->ui->gcode->append(cast);

        this->ui->gcode->append(this->ui->BLGcode->toPlainText());
        this->ui->gcode->append(";Switching to layer " + QString::number(progress + 1));
        this->ui->gcode->append("G1 Y" +  QString::number(ui->layerHeightSpinBox->value())); //Move to next layer
        this->ui->gcode->append(this->ui->ALGcode->toPlainText());
    }
     ui->gcode->append(this->ui->EGcode->toPlainText());
     ui->progressLabel->setText("Fait.");
     ui->gcodePBar->setMaximum(100);
     ui->gcodePBar->setValue(100);
     this->setCursor(QCursor(Qt::ArrowCursor));
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

void MainWindow::on_firstNozzleSpinBox_valueChanged(int)
{
    ui->openGLWidget_3->nozzleCount = ui->lastNozzleSpinBox->value() + 1 - ui->firstNozzleSpinBox->value();
}

void MainWindow::on_lastNozzleSpinBox_valueChanged(int)
{
    ui->openGLWidget_3->nozzleCount = ui->lastNozzleSpinBox->value() + 1 - ui->firstNozzleSpinBox->value();
}



//Build plate
void MainWindow::on_doubleSpinBox_valueChanged(double arg)
{
    ui->openGLWidget->plateDim.setX(arg);
    ui->openGLWidget_2->plateDim.setX(arg);
    ui->openGLWidget_3->plateDim.setX(arg);
}

void MainWindow::on_doubleSpinBox_3_valueChanged(double arg)
{
    ui->openGLWidget->plateDim.setY(arg);
    ui->openGLWidget_2->plateDim.setY(arg);
    ui->openGLWidget_3->plateDim.setY(arg);
}

void MainWindow::on_doubleSpinBox_2_valueChanged(double arg)
{
    ui->openGLWidget->plateDim.setZ(arg);
}


//Origin offset
void MainWindow::on_X_OffsetSpinBox_valueChanged(double arg)
{
    ui->openGLWidget->originOffset.setX(arg);
    ui->openGLWidget_2->originOffset.setX(arg);
    ui->openGLWidget_3->originOffset.setX(arg);
}

void MainWindow::on_Y_OffsetSpinBox_valueChanged(double arg)
{
    ui->openGLWidget->originOffset.setY(arg);
    ui->openGLWidget_2->originOffset.setY(arg);
    ui->openGLWidget_3->originOffset.setY(arg);
}

void MainWindow::on_Z_OffsetSpinBox_valueChanged(double arg)
{
    ui->openGLWidget->originOffset.setZ(arg);
}
