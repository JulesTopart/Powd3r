#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "stlreader.h"

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


private slots:
    void on_actionSTL_triggered();

    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H