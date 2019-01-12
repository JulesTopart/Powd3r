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
    void addModel               ( Mesh*               );
    void browseFile             ();
    void updateList             ();
    void generateGcode          ();

//-------------Slots-----------

private slots:
    void on_actionClose_triggered       ();
    void on_actionImporter_triggered    ();
    void on_scaleButton_clicked         ();
    void on_rotateButton_clicked        ();
    void on_deleteButton_clicked        ();
    void on_listWidget_itemPressed      ( QListWidgetItem*  );
    void on_duplicateButton_clicked     ();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_moveButton_clicked();
    void on_sliceButton_clicked();
    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider2_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
