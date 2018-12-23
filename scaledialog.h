#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include "mesh.h"

namespace Ui {
class ScaleDialog;
}

class ScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleDialog(QWidget *parent = nullptr);
    explicit ScaleDialog(mesh::Model *parent = nullptr);
    ~ScaleDialog();

private:
    Ui::ScaleDialog *ui;
};

#endif // SCALEDIALOG_H
