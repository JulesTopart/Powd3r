#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "mesh.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//-----------Constructor---------

    explicit MainWindow         ( QWidget *parent = nullptr );
    ~MainWindow                 ();

//-------------Methods-----------

    void processFile            ( QString filePath          );
    void processSTLFile         ( QString filePath          );
    void processOBJFile         ( QString filePath          );
    void addModel               ( Mesh*    );
    void selectModel            (int i = -1);
    void browseFile             ();
    void updateList             ();
    void generateGcode          ();
    void upddateLineNumber      ();

    void loadDefault  ();
    void loadSettings ();
    void saveSettings ();
    void resetSettings();

    void closeEvent             (QCloseEvent *event);
    void reject();

//-------------Slots-----------

private slots:
    void on_actionClose_triggered       ();
    void on_actionImporter_triggered    ();
    void on_actionOption_triggered      ();

    void on_scaleButton_clicked         ();
    void on_rotateButton_clicked        ();
    void on_deleteButton_clicked        ();
    void on_listWidget_itemPressed      ( QListWidgetItem*  );
    void on_duplicateButton_clicked     ();


    void on_centerButton_clicked();
    void on_addButton_clicked();

    void on_moveButton_clicked();
    void on_sliceButton_clicked();
    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider2_valueChanged(int value);

    void on_gcodeExportButton_clicked();
    void on_gcodeGenButton_clicked();


    void on_firstNozzleSpinBox_valueChanged(int arg1);

    void on_lastNozzleSpinBox_valueChanged(int arg1);


    void on_widthSpinBox_valueChanged(double arg1);

    void on_depthSpinBox_valueChanged(double arg1);

    void on_heightSpinBox_valueChanged(double arg1);


    void on_X_OffsetSpinBox_valueChanged(double arg1);

    void on_Y_OffsetSpinBox_valueChanged(double arg1);

    void on_Z_OffsetSpinBox_valueChanged(double arg1);

    void on_actionLoadDefaultSetting_triggered();

    void on_actionCleanSettings_triggered();

private:
    Ui::MainWindow *ui;
    QString m_sSettingsFile;

};

#endif // MAINWINDOW_H
