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

    //Scrollbar synchronization
    connect(ui->gcode->verticalScrollBar(), SIGNAL(valueChanged(int)),
            ui->lineNumber->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->lineNumber->verticalScrollBar(), SIGNAL(valueChanged(int)),
            ui->gcode->verticalScrollBar(), SLOT(setValue(int)));
}

MainWindow::~MainWindow(){ delete ui; }

void MainWindow::on_actionClose_triggered(){ this->close(); }


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

void MainWindow::loadDefault(){

    if (ui->BLGcode) ui->BLGcode->setPlainText("");
    if (ui->EGcode) ui->EGcode->setPlainText("");

    QString macro;
    macro = "";
    macro += ";Gcode generated with Powd3r slicer\n";
    macro += ";Start MACRO\n";
    macro += "G28 X Y;Homing\n";
    macro += "G92 Z0\n";
    macro += "G90;absolute mode\n";
    macro += "T0\n";
    macro += "M83;Extruder relative mode\n";
    macro += ";END\n";

    if (ui->SGcode) ui->SGcode->setPlainText(macro);

    macro = "";

    macro += ";ALC MACRO\n";
    macro += "G28 X;Homming X\n";
    macro += "T1\n";
    macro += "G92 E0\n";
    macro += "M82;Extruder absolute mode\n";
    macro += "G1 E5300 F50000\n";
    macro += "M400\n";
    macro += "G1 E0 F50000\n";
    macro += "M400\n";
    macro += "T1\n";
    macro += "G92 E0\n";
    macro += "T0\n";
    macro += "M83;Extruder relative mode\n";
    macro += ";END\n";

    if (ui->ALGcode) ui->ALGcode->setPlainText(macro);

    if (ui->layerHeightSpinBox) ui->layerHeightSpinBox->setValue(0.3);

    if (ui->SpreadRateSpinBox) ui->SpreadRateSpinBox->setValue(1);

    if (ui->firstNozzleSpinBox) ui->firstNozzleSpinBox->setValue(1);

    if (ui->lastNozzleSpinBox) ui->lastNozzleSpinBox->setValue(12);

    if (ui->InkFlowSpinBox) ui->InkFlowSpinBox->setValue(300);

    if (ui->InkFlowSpinBox) ui->passSpinBox->setValue(3);

    if (ui->widthSpinBox) ui->widthSpinBox->setValue(90);

    if (ui->depthSpinBox) ui->depthSpinBox->setValue(80);

    if (ui->heightSpinBox) ui->heightSpinBox->setValue(80);

    if (ui->X_OffsetSpinBox) ui->X_OffsetSpinBox->setValue(0);

    if (ui->Y_OffsetSpinBox) ui->Y_OffsetSpinBox->setValue(-215);

    if (ui->Z_OffsetSpinBox) ui->Z_OffsetSpinBox->setValue(0);

    ui->openGLModel->originOffset.setX(0);
    ui->openGLModel->originOffset.setY(-215);
    ui->openGLModel->originOffset.setZ(0);
    ui->openGLModel->plateDim.setX(90);
    ui->openGLModel->plateDim.setY(80);
    ui->openGLModel->plateDim.setZ(80);
    ui->openGLModel->toggleBoundaries(ui->actionBoundaries->isChecked());

    ui->openGLSlice->originOffset.setX(0);
    ui->openGLSlice->originOffset.setY(-215);
    ui->openGLSlice->plateDim.setX(90);
    ui->openGLSlice->plateDim.setY(80);

    ui->openGLToolpath->originOffset.setX(0);
    ui->openGLToolpath->originOffset.setY(-215);
    ui->openGLToolpath->plateDim.setX(90);
    ui->openGLToolpath->plateDim.setY(80);

    ui->openGLViewer->originOffset.setX(0);
    ui->openGLViewer->originOffset.setY(-215);
    ui->openGLViewer->originOffset.setZ(0);
    ui->openGLViewer->plateDim.setX(90);
    ui->openGLViewer->plateDim.setY(80);
    ui->openGLViewer->plateDim.setZ(80);

    ui->checkBoxGenerateLineNumber->setCheckState(Qt::Unchecked);
    ui->actionBoundaries->setCheckState(Qt::Checked);
}


void MainWindow::loadSettings()
{
    QSettings settings("LaMachinerie", "Powd3r");

    QString sText = settings.value("is_setting", "").toString();
    if (sText != "true") loadDefault();

    sText = settings.value("after_layer_gcode", "").toString();
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
    if (ui->passSpinBox) ui->passSpinBox->setValue(pass);

    float buildX = settings.value("build_x","").toFloat();
    if (ui->widthSpinBox) ui->widthSpinBox->setValue(buildX);

    float buildY = settings.value("build_y","").toFloat();
    if (ui->depthSpinBox) ui->depthSpinBox->setValue(buildY);

    float buildZ = settings.value("build_z","").toFloat();
    if (ui->heightSpinBox) ui->heightSpinBox->setValue(buildZ);

    float offbuildX = settings.value("offbuild_x", "").toFloat();
    if (ui->X_OffsetSpinBox) ui->X_OffsetSpinBox->setValue(offbuildX);

    float offbuildY = settings.value("offbuild_y", "").toFloat();
    if (ui->Y_OffsetSpinBox) ui->Y_OffsetSpinBox->setValue(offbuildY);

    float offbuildZ = settings.value("offbuild_z", "").toFloat();
    if (ui->Z_OffsetSpinBox) ui->Z_OffsetSpinBox->setValue(offbuildZ);

    bool showBound = settings.value("showBound", "").toBool();
    if(showBound >= 1)
        ui->actionBoundaries->setCheckState(Qt::Checked);
    else
        ui->actionBoundaries->setCheckState(Qt::Unchecked);
    ui->openGLModel->toggleBoundaries(ui->actionBoundaries->checkState() > 1);

    int genLine = settings.value("genLine", "").toInt();
    if(genLine >= 1)
        ui->checkBoxGenerateLineNumber->setCheckState(Qt::Checked);
    else
        ui->checkBoxGenerateLineNumber->setCheckState(Qt::Unchecked);

    ui->openGLModel->originOffset.setX(offbuildX);
    ui->openGLModel->originOffset.setY(offbuildY);
    ui->openGLModel->originOffset.setZ(offbuildZ);
    ui->openGLModel->plateDim.setX(buildX);
    ui->openGLModel->plateDim.setY(buildY);
    ui->openGLModel->plateDim.setZ(buildZ);

    ui->openGLSlice->originOffset.setX(offbuildX);
    ui->openGLSlice->originOffset.setY(offbuildY);
    ui->openGLSlice->plateDim.setX(buildX);
    ui->openGLSlice->plateDim.setY(buildY);

    ui->openGLToolpath->originOffset.setX(offbuildX);
    ui->openGLToolpath->originOffset.setY(offbuildY);
    ui->openGLToolpath->plateDim.setX(buildX);
    ui->openGLToolpath->plateDim.setY(buildY);

    ui->openGLViewer->originOffset.setX(offbuildX);
    ui->openGLViewer->originOffset.setY(offbuildY);
    ui->openGLViewer->originOffset.setZ(offbuildZ);
    ui->openGLViewer->plateDim.setX(buildX);
    ui->openGLViewer->plateDim.setY(buildY);
    ui->openGLViewer->plateDim.setZ(buildZ);
}

void MainWindow::saveSettings()
{
    QSettings settings("LaMachinerie", "Powd3r");

    QString sText = "true";
    settings.setValue("is_setting", sText);

    sText = (ui->ALGcode) ? ui->ALGcode->toPlainText() : "";
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

    double slayerHeight = (ui->layerHeightSpinBox) ? ui->layerHeightSpinBox->value() : 0.2f;
    settings.setValue("layer_height", slayerHeight);
    if (ui->layerHeightSpinBox) ui->layerHeightSpinBox->setValue(slayerHeight);

    double sSpread_rate = (ui->SpreadRateSpinBox) ? ui->SpreadRateSpinBox->value() : 0.0f;
    settings.setValue("spread_rate", sSpread_rate);
    if (ui->SpreadRateSpinBox) ui->SpreadRateSpinBox->setValue(sSpread_rate);

    int sfirstNozzle = (ui->firstNozzleSpinBox) ? ui->firstNozzleSpinBox->value() : 1.0f;
    settings.setValue("first_nozzle", sfirstNozzle);
    if (ui->firstNozzleSpinBox) ui->firstNozzleSpinBox->setValue(sfirstNozzle);

    int slastNozzle = (ui->lastNozzleSpinBox) ? ui->lastNozzleSpinBox->value() : 12.0f;
    settings.setValue("last_nozzle", slastNozzle);
    if (ui->lastNozzleSpinBox) ui->lastNozzleSpinBox->setValue(slastNozzle);

    double sInkflow = (ui->InkFlowSpinBox) ? ui->InkFlowSpinBox->value() : 100.0;
    settings.setValue("ink_flow", sInkflow);
    //if (ui->InkFlowSpinBox) ui->InkFlowSpinBox->setValue(sInkflow);
    //qDebug() << sInkflow << "\n";

    int pass_count = (ui->passSpinBox) ? ui->passSpinBox->value() : 1;
    settings.setValue("pass_count", pass_count);
    if (ui->passSpinBox) ui->passSpinBox->setValue(pass_count);

    double buildX = (ui->widthSpinBox) ? ui->widthSpinBox->value() : 100.0f;
    settings.setValue("build_x", buildX);
    if (ui->widthSpinBox) ui->widthSpinBox->setValue(buildX);

    double buildY = (ui->depthSpinBox) ? ui->depthSpinBox->value() : 100.0f;
    settings.setValue("build_y", buildY);
    if (ui->depthSpinBox) ui->depthSpinBox->setValue(buildY);

    double buildZ = (ui->heightSpinBox) ? ui->heightSpinBox->value() : 150.0f;
    settings.setValue("build_z", buildZ);
    if (ui->heightSpinBox) ui->heightSpinBox->setValue(buildZ);

    double offbuildX = (ui->X_OffsetSpinBox) ? ui->X_OffsetSpinBox->value() : -110.0f;
    settings.setValue("offbuild_x", offbuildX);
    if (ui->X_OffsetSpinBox) ui->X_OffsetSpinBox->setValue(offbuildX);

    double offbuildY = (ui->Y_OffsetSpinBox) ? ui->Y_OffsetSpinBox->value() : 10.0f;
    settings.setValue("offbuild_y", offbuildY);
    if (ui->Y_OffsetSpinBox) ui->Y_OffsetSpinBox->setValue(offbuildY);

    double offbuildZ = (ui->Z_OffsetSpinBox) ? ui->Z_OffsetSpinBox->value() : 0.0f;
    settings.setValue("offbuild_z", offbuildZ);
    if (ui->Z_OffsetSpinBox) ui->Z_OffsetSpinBox->setValue(offbuildZ);

    int showBound = (ui->actionBoundaries) ? ui->actionBoundaries->isChecked() : 2;
    settings.setValue("showBound", showBound);

    int genLine = (ui->checkBoxGenerateLineNumber) ? ui->checkBoxGenerateLineNumber->isChecked() : 0;
    settings.setValue("genLine", genLine);
}


void MainWindow::resetSettings(){
    QSettings settings("LaMachinerie", "Powd3r");
    settings.clear();
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
    if(ext == "stl" || ext == "STL"){
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
                ui->openGLModel->updateGL();
                ui->progressBar->setValue(100);
                ui->progressLabel->setText("Fait.");
            }
        }
    }
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::addModel(Mesh* mdlPtr){
    ui->openGLModel->loadModel(mdlPtr);
    ui->openGLModel->updateGL();
    updateList();
}

void MainWindow::updateList(){
   int size = ui->openGLModel->modelCount();
    ui->listWidget->clear();
    Mesh* mdlPtr;
    for(int i(0); i < size; i++){
        mdlPtr = this->ui->openGLModel->get(i);
        ui->listWidget->addItem( "  " + QString::number(mdlPtr->getId()) + "  :  " + mdlPtr->getName() + " " + QString::number(mdlPtr->getBBSize().x) + " x " + QString::number(mdlPtr->getBBSize().y)+ " x " + QString::number(mdlPtr->getBBSize().z) + "mm");
    }
    selectModel();
}

void MainWindow::selectModel(int i){
    if(i >= 0 && i < ui->openGLModel->modelCount()){
        ui->openGLModel->select(i);
        ui->listWidget->setCurrentRow(i);
    }
    else{
        i = ui->openGLModel->modelCount() - 1;
        ui->openGLModel->select(i);
        ui->listWidget->setCurrentRow(i);
    }
}

void MainWindow::on_actionImporter_triggered()
{
   browseFile();
}


void MainWindow::on_actionOption_triggered(){
    ui->tabWidget->setCurrentIndex(4);
}


void MainWindow::on_scaleButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->openGLModel->getSelected() != -1){
        int id = ui->openGLModel->getSelected();
        Mesh* mdlPtr = this->ui->openGLModel->get(id);
        ScaleDialog *dialog = new ScaleDialog(mdlPtr, this);
         dialog->show();
    }
}


void MainWindow::on_rotateButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->openGLModel->getSelected() != -1){
        int id = ui->openGLModel->getSelected();
        Mesh* mdlPtr = this->ui->openGLModel->get(id);
        RotDialog *dialog = new RotDialog(mdlPtr, this);
        dialog->show();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->openGLModel->getSelected() != -1){
        this->ui->openGLModel->unloadModel(ui->openGLModel->getSelected());
        ui->openGLModel->select(-1);
        updateList();
    }
}

void MainWindow::on_duplicateButton_clicked()
{
    if(this->ui->listWidget->count() > 0 &&  ui->openGLModel->getSelected() != -1){
        int id = ui->openGLModel->getSelected();
        Mesh* mdlPtr = this->ui->openGLModel->get(id);
        addModel(mdlPtr);
    }
}

void MainWindow::on_listWidget_itemPressed(QListWidgetItem*)
{
    int id = ui->listWidget->currentRow();
    ui->openGLModel->select(id);
    ui->openGLModel->updateGL();
}


//Reset View Button
void MainWindow::on_centerButton_clicked()
{
    ui->openGLModel->FrontView();
}

//Add button
void MainWindow::on_addButton_clicked()
{
    browseFile();
}

void MainWindow::on_moveButton_clicked()
{
    if(this->ui->listWidget->count() > 0 && ui->openGLModel->getSelected() != -1){
        int id = ui->openGLModel->getSelected();
        Mesh* mdlPtr = this->ui->openGLModel->get(id);
        moveDialog *dialog = new moveDialog(mdlPtr);
        dialog->show();
    }
}

void MainWindow::on_sliceButton_clicked()
{
    this->setCursor(QCursor(Qt::WaitCursor));
    //check if one ore more model are loaded
    if(this->ui->openGLModel->modelCount() > 0 &&  ui->openGLModel->getSelected() > -1 && ui->layerHeightSpinBox->value() > 0.001){
        //generate slice polygon (result of plane intersecting with mesh)
        QVector<Lines2D> *lines;
        lines = new  QVector<Lines2D>();

        ui->progressLabel->setText("Traitements des polygon...");

        triMeshSlicer(ui->openGLModel->getModels(), *lines, ui->layerHeightSpinBox->value(), ui->progressBar);

        //generate slice from lines
        ui->progressLabel->setText("Géneration des tranches...");

        ui->openGLSlice->getSlice()->clear();
        int progress = 0;
        for(QVector<Lines2D>::Iterator l = lines->begin(); l < lines->end(); l++, progress++){
            Slice sliceBuf(*l);
            ui->openGLSlice->getSlice()->push_back(sliceBuf);
            qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,10);
            ui->progressBar->setValue(float(float(progress) / float(lines->size())) * 100);
        }
        ui->progressLabel->setText("Fait.");
        ui->progressBar->setValue(100);
        ui->openGLSlice->getSlice()->remove(0);
        //ui->openGLSlice->getSlice()->remove(ui->openGLSlice->sliceCount() - 1);
        int nSlice = ui->openGLSlice->sliceCount();
        ui->verticalSlider->setMaximum(nSlice - 1);
        ui->viewerVerticalSlider->setMaximum(nSlice - 1);
        ui->sliceCount->setText(QString::number(nSlice));

        //update Gui
        ui->verticalSlider2->setMaximum(nSlice - 1);
        ui->sliceCount2->setText(QString::number(nSlice));
        ui->openGLToolpath->clear();


        //SubSlicing
        ui->progressLabel->setText("Géneration des sous-stanches...");

        QVector<Slice> slices = *ui->openGLSlice->getSlice();
        for(int i(0); i < nSlice; i++){
            this->ui->progressBar->setValue(float(float(i) / float(nSlice) * 100));
            Lines2D subLines = slices[i].subSlice(96);
            ui->openGLToolpath->push(subLines);
        }
        this->ui->progressBar->setValue(100);
        ui->progressLabel->setText("Fait.");
        ui->tabWidget->setCurrentIndex(1);
    }else{
        if(!(ui->layerHeightSpinBox->value() > 0.001)){
            ui->progressLabel->setText("Hauteur de couche invalide");
        }else{
            ui->progressLabel->setText("Sélectionnez un modèle valide");
        }
    }
    this->setCursor(QCursor(Qt::ArrowCursor));
}


void MainWindow::generateGcode(){

    ui->gcode->clear();
    //GUI
    this->setCursor(QCursor(Qt::WaitCursor));

    if(this->ui->openGLModel->modelCount() > 0 && ui->openGLSlice->sliceCount() > 0){

        ui->progressLabel->setText("Géneration du Gcode...");

        //initialisation
        this->ui->gcode->setPlainText(this->ui->SGcode->toPlainText());                 //Start Gcode
        QVector2D offset(-ui->X_OffsetSpinBox->value(), -ui->Y_OffsetSpinBox->value()); //Store origin offset
        int id = this->ui->openGLModel->getSelected();
        Mesh* mdlPtr = this->ui->openGLModel->get(id);
        //offset += QVector2D(mdlPtr->getBBSize().x, mdlPtr->getBBSize().y) / 2;        //Move to model bottom left corner
        offset += QVector2D(mdlPtr->getPosition().x(), mdlPtr->getPosition().y());
        offset += QVector2D(ui->openGLModel->plateDim.x()/2, ui->openGLModel->plateDim.y()/2);
        int nSlice = ui->openGLSlice->sliceCount();                                     //Store sliceCount
        QVector<Lines2D> subLines = *ui->openGLToolpath->getLines();                    //Store nozzleLines
        int pass = ui->passSpinBox->value();
        int progress(0);                                                              //TODO modify this progress handling


        long max(subLines.size());

        ui->gcodePBar->setMaximum(max);

        for (QVector<Lines2D>::Iterator lines = subLines.begin(); lines != subLines.end(); lines++, progress++) { //For each slice
            //GUI Update
            qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,10);
            this->ui->gcodePBar->setValue(progress);

            this->ui->gcode->append(this->ui->BLGcode->toPlainText());
            this->ui->gcode->append(";Switching to layer " + QString::number(progress + 1));
            this->ui->gcode->append("G1 Z" +  QString::number(ui->layerHeightSpinBox->value()) + " F" + QString::number(ui->verticalSpeedSpinBox->value())); //Move to next layer
            //this->ui->gcode->append("G1 F" +  QString::number(ui->horizontalSpeedSpinBox->value())); //Move to next layer
            this->ui->gcode->append(this->ui->ALGcode->toPlainText());

            SweepCollection sweeps = SweepCollection::generateSweeps(lines->toStdVector(), ui->firstNozzleSpinBox->value() - 1, ui->lastNozzleSpinBox->value() - 1, 96); //Generate slice sweeps
            std::string out = sweeps.toGcode(pass, offset,ui->sweepSpeedSpinBox->value(), ui->InkFlowSpinBox->value() / 100.0); //number of pass
            QString cast = QString::fromStdString(out);

            this->ui->gcode->append(cast);
        }
         ui->gcode->append(this->ui->EGcode->toPlainText());
         ui->progressLabel->setText("Fait.");
         ui->gcodePBar->setMaximum(100);
         ui->gcodePBar->setValue(100);
         upddateLineNumber();
         ui->openGLViewer->generatePath(ui->gcode->toPlainText());
    }else{
        ui->progressLabel->setText("Sélectionnez un modèle valide");
    }
    this->setCursor(QCursor(Qt::ArrowCursor));
 }



void MainWindow::on_verticalSlider_valueChanged(int value)
{
    ui->openGLSlice->selectSlice(value);
    ui->zeroCount->setText(QString::number(value));
    ui->verticalSlider2->setValue(value);
}

void MainWindow::on_verticalSlider2_valueChanged(int value)
{
    ui->openGLToolpath->selectSlice(value);
    ui->zeroCount3->setText(QString::number(value));
    ui->verticalSlider->setValue(value);
}


void MainWindow::on_gcodeGenButton_clicked()
{
    generateGcode();
}

void MainWindow::on_firstNozzleSpinBox_valueChanged(int)
{
    ui->openGLToolpath->nozzleCount = ui->lastNozzleSpinBox->value() + 1 - ui->firstNozzleSpinBox->value();
}

void MainWindow::on_lastNozzleSpinBox_valueChanged(int)
{
    ui->openGLToolpath->nozzleCount = ui->lastNozzleSpinBox->value() + 1 - ui->firstNozzleSpinBox->value();
}



//Build plate
void MainWindow::on_widthSpinBox_valueChanged(double arg)
{
    ui->openGLModel->plateDim.setX(arg);
    ui->openGLSlice->plateDim.setX(arg);
    ui->openGLToolpath->plateDim.setX(arg);
}

void MainWindow::on_depthSpinBox_valueChanged(double arg)
{
    ui->openGLModel->plateDim.setY(arg);
    ui->openGLSlice->plateDim.setY(arg);
    ui->openGLToolpath->plateDim.setY(arg);
}

void MainWindow::on_heightSpinBox_valueChanged(double arg)
{
    ui->openGLModel->plateDim.setZ(arg);
}


//Origin offset
void MainWindow::on_X_OffsetSpinBox_valueChanged(double arg)
{
    ui->openGLModel->originOffset.setX(arg);
    ui->openGLSlice->originOffset.setX(arg);
    ui->openGLToolpath->originOffset.setX(arg);
}

void MainWindow::on_Y_OffsetSpinBox_valueChanged(double arg)
{
    ui->openGLModel->originOffset.setY(arg);
    ui->openGLSlice->originOffset.setY(arg);
    ui->openGLToolpath->originOffset.setY(arg);
}

void MainWindow::on_Z_OffsetSpinBox_valueChanged(double arg)
{
    ui->openGLModel->originOffset.setZ(arg);
}


void MainWindow::upddateLineNumber(){
    ui->lineNumber->clear();

    if(ui->checkBoxGenerateLineNumber->isChecked()){
        ui->gcodePBar->setMaximum(100);
        ui->gcodePBar->setValue(0);

        int nLine = ui->gcode->document()->blockCount();
        QString numbers = "";
        for(int i = 0; i < nLine; i++){
            qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,10);
            numbers += QString::number(i);
            numbers += '\n';
            ui->gcodePBar->setValue(float(float(i) / float(nLine)) * 100);
        }

        ui->gcodePBar->setValue(100);
        ui->lineNumber->append(numbers);
    }
}


void MainWindow::on_actionLoadDefaultSetting_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Attention", "Vous allez perdre vos paramètres machine. Voulez vous continuez ?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
        loadDefault();
        saveSettings();
        loadSettings();
}

void MainWindow::on_actionCleanSettings_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Attention", "Vous allez perdre vos paramètres machine. Voulez vous continuez ?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
        resetSettings();
        saveSettings();
        loadSettings();
}

void MainWindow::on_saveSetting_pressed()
{
    saveSettings();
}

void MainWindow::on_defaultSetting_pressed()
{
    on_actionLoadDefaultSetting_triggered();
}

void MainWindow::on_actionBoundaries_stateChanged(int i)
{
    bool state = false;
    if(i == 2) state = true;
    ui->openGLModel->toggleBoundaries(state);
    if(state)ui->actionBoundaries->setCheckState(Qt::Checked);
    else ui->actionBoundaries->setCheckState(Qt::Unchecked);
}

void MainWindow::on_gcodeExportButton_clicked()
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

void MainWindow::on_viewerVerticalSlider_valueChanged(int value)
{
    ui->openGLViewer->selectSlice(value);
    ui->zeroCount->setText(QString::number(value));
    ui->viewerVerticalSlider->setValue(value);
}
