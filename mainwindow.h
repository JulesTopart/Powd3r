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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void browseFile();
    void processFile(QString filePath);
    void processSTLFile(QString filePath);
    void processOBJFile(QString filePath);
    void addModel(mesh::Model*);
    void updateList();

private slots:

    void on_actionClose_triggered();
    void on_actionImporter_triggered();
    void on_scaleButton_clicked();
    void on_rotateButton_clicked();
    void on_deleteButton_clicked();
    void on_listWidget_itemPressed(QListWidgetItem *item);
    void on_duplicateButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
